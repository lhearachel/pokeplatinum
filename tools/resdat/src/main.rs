use std::{collections::HashMap, error::Error, fs, path::PathBuf, process, str::FromStr};

use clap::Parser;
use codespan_reporting::{
    diagnostic::{Diagnostic, Label},
    files::SimpleFiles,
    term::{
        self,
        termcolor::{ColorChoice, StandardStream},
    },
};
use json_spanned_value::{self as jsv, spanned::Spanned};
use regex::Regex;
use serde::Deserialize;

type NarcConstants = HashMap<String, isize>;
type NarcMemberIndices = HashMap<String, Vec<String>>;

/// Generate serialized resource packs and template definitions
#[derive(Parser, Debug)]
#[command(about, long_about = None)]
struct Cli {
    /// Load FILE for the definitions of NARC ID constants
    #[arg(short = 'N', long, value_name = "FILE")]
    narc_h: String,

    /// Map NARC to the index FILE for resolving member-file indices
    #[arg(short = 'D', long, value_name = "<NARC>=<FILE>", value_parser = clap::value_parser!(NarcIndexMapping))]
    define: Vec<NarcIndexMapping>,

    /// Use OUTPUT as the basename for generated files (default: derived from INPUT)
    #[arg(short, long, value_name = "OUTPUT")]
    output: Option<String>,

    /// Path to the input JSON data describing resources and templates to serialize
    #[arg(value_name = "INPUT")]
    input: PathBuf,
}

fn main() -> Result<(), Box<dyn Error>> {
    let cli = Cli::parse();

    let input_path = cli.input.to_str().unwrap();
    let output_base = cli.output.unwrap_or(
        cli.input
            .file_stem()
            .map(|oss| oss.to_str().unwrap())
            .expect("INPUT filename is missing")
            .to_string(),
    );

    let narc_constants = load_constants(&cli.narc_h)?;
    let narc_members = load_defines(&cli.define, &narc_constants)?;
    let input_resdat = load_file(input_path)?;

    let mut filedb = SimpleFiles::new();
    let filedb_resdat = filedb.add(input_path, input_resdat.as_str());

    // Emit a prettier error here if the baseline JSON schema is invalidated.
    let input_data: ResourceData = jsv::from_str(&input_resdat).unwrap_or_else(|err| {
        emit(
            &filedb,
            &Diagnostic::error().with_message(format!("{}: {}", &input_path, err)),
        );
    });

    let resdat = ResdatSerializer {
        narc_constants,
        narc_members,
        filedb,
        filedb_resdat,
    };

    resdat
        .serialize(&input_data)
        .unwrap_or_else(|diagnostic| emit(&resdat.filedb, &diagnostic))
        .write(&output_base)?;

    Ok(())
}

fn emit(filedb: &SimpleFiles<&str, &str>, diagnostic: &Diagnostic<usize>) -> ! {
    term::emit_to_write_style(
        &mut StandardStream::stderr(ColorChoice::Auto),
        &term::Config::default(),
        filedb,
        diagnostic,
    )
    .unwrap();

    process::exit(1);
}

#[derive(Deserialize, Debug)]
#[allow(dead_code)]
struct BaseResource {
    id: Spanned<String>,
    from_narc: Spanned<String>,
    from_file: Spanned<String>,
    compressed: bool,
}

#[derive(Deserialize, Debug)]
#[allow(dead_code)]
struct CharacterResource {
    id: Spanned<String>,
    from_narc: Spanned<String>,
    from_file: Spanned<String>,
    compressed: bool,
    on_main_screen: bool,
    on_sub_screen: bool,
}

#[derive(Deserialize, Debug)]
#[allow(dead_code)]
struct PaletteResource {
    id: Spanned<String>,
    from_narc: Spanned<String>,
    from_file: Spanned<String>,
    load_in_slot: u32,
    compressed: bool,
    on_main_screen: bool,
    on_sub_screen: bool,
}

#[derive(Deserialize, Debug)]
#[allow(dead_code)]
struct ResourceTemplate {
    characters: Spanned<String>,
    palette: Spanned<String>,
    cells: Spanned<String>,
    animations: Spanned<String>,
    vram_transfer: bool,
    priority: u32,
}

#[derive(Deserialize, Debug)]
#[allow(dead_code)]
struct ResourceData {
    characters: Vec<CharacterResource>,
    palettes: Vec<PaletteResource>,
    cells: Vec<BaseResource>,
    animations: Vec<BaseResource>,
    templates: Vec<ResourceTemplate>,
}

#[derive(Clone, Debug)]
struct NarcIndexMapping {
    narc_id: String,
    index: String,
}

impl FromStr for NarcIndexMapping {
    type Err = &'static str;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let (narc_id, index) = s.split_once('=').ok_or("missing key-value separator '='")?;
        match (narc_id.len(), index.len()) {
            (0, _) => Err("missing left-hand side <NARC>"),
            (_, 0) => Err("missing right-hand side <FILE>"),
            (_, _) => Ok(Self {
                narc_id: narc_id.to_string(),
                index: index.to_string(),
            }),
        }
    }
}

fn load_file(path: &str) -> Result<String, String> {
    fs::read_to_string(path).map_err(|err| format!("{}: {}", err, path))
}

fn load_constants(narc_h: &str) -> Result<NarcConstants, String> {
    let narc_h_content = load_file(narc_h)?;
    let narc_h_enumpat = Regex::new(r"(?ms)enum\s+NarcID\s*\{.*\};").unwrap();
    let narc_h_defspat = Regex::new(r"\s*(\w+)\s*(=\s*(\d+))?,").unwrap();

    let narc_enum = narc_h_enumpat
        .find(&narc_h_content)
        .ok_or_else(|| format!("No match for 'enum NarcID' found in {}", narc_h))?;
    let mut counter = 0;
    let constants: NarcConstants = narc_h_defspat
        .captures_iter(narc_enum.as_str())
        .map(|c| {
            // NOTE: The capture groups enforce valid typing of these values.
            let symbol = c.get(1).unwrap();
            counter = c
                .get(3)
                .map(|m| m.as_str().parse::<isize>().unwrap())
                .unwrap_or_else(|| counter + 1);

            (symbol.as_str().to_string(), counter)
        })
        .collect();
    Ok(constants)
}

fn load_defines(
    defines: &[NarcIndexMapping],
    narc_constants: &NarcConstants,
) -> Result<NarcMemberIndices, String> {
    let mut mapped: HashMap<String, Vec<String>> = HashMap::new();
    let define_pat = Regex::new(r"#define\s+((?-u:\w)+)\s+\d+").unwrap();

    for NarcIndexMapping { narc_id, index } in defines.iter() {
        if !narc_constants.contains_key(narc_id) {
            return Err(format!(
                "expected a NarcID constant for member-mapping, but found {}",
                narc_id
            ));
        }

        let index_data = load_file(index)?;
        let index_defs = define_pat
            .captures_iter(index_data.as_str())
            .map(|c| c.extract::<1>())
            .map(|(_, [symbol])| symbol.to_string())
            .collect();

        mapped.insert(narc_id.to_string(), index_defs);
    }

    Ok(mapped)
}

struct ResdatSerializer<'a> {
    narc_constants: NarcConstants,
    narc_members: NarcMemberIndices,
    filedb: SimpleFiles<&'a str, &'a str>,
    filedb_resdat: usize,
}

struct Serialized {
    characters: Vec<u8>,
    palettes: Vec<u8>,
    cells: Vec<u8>,
    animations: Vec<u8>,
    templates: Vec<u8>,
}

const TERM_VALUE: u32 = 0xFFFFFFFE;
const TERM_RESDAT: &[u32] = &[TERM_VALUE; 6];
const TERM_TEMPLATE: &[u32] = &[TERM_VALUE; 8];

impl ResdatSerializer<'_> {
    fn serialize(&self, resdat: &ResourceData) -> Result<Serialized, Diagnostic<usize>> {
        Ok(Serialized {
            characters: self.serialize_resdat(&resdat.characters, 0)?,
            palettes: self.serialize_resdat(&resdat.palettes, 1)?,
            cells: self.serialize_resdat(&resdat.cells, 2)?,
            animations: self.serialize_resdat(&resdat.animations, 3)?,
            templates: self.serialize_templates(resdat)?,
        })
    }

    fn serialize_resdat<R: Resource>(
        &self,
        resources: &[R],
        type_id: u32,
    ) -> Result<Vec<u8>, Diagnostic<usize>> {
        let mut payload: Vec<u8> = Vec::with_capacity(4 + (24 * (resources.len() + 1)));
        payload.extend(&serialize(&[type_id]));
        for (i, resdef) in resources.iter().enumerate() {
            let from_narc_file = self.find_narc_file(resdef.get_from_narc())?;
            let from_narc_member =
                self.find_narc_member(resdef.get_from_narc(), resdef.get_from_file())?;

            payload.extend(&resdef.serialize(i as u32, from_narc_file, from_narc_member));
        }

        payload.extend(&serialize(TERM_RESDAT));
        Ok(payload)
    }

    fn find_narc_file(&self, from_narc: &Spanned<String>) -> Result<isize, Diagnostic<usize>> {
        self.narc_constants
            .get(&from_narc.to_string())
            .ok_or_else(|| {
                Diagnostic::error()
                    .with_message(format!(
                        "expected a NarcID constant, but found {}",
                        from_narc
                    ))
                    .with_labels(vec![Label::primary(self.filedb_resdat, from_narc.range())])
            })
            .copied()
    }

    fn find_narc_member(
        &self,
        from_narc: &Spanned<String>,
        from_file: &Spanned<String>,
    ) -> Result<usize, Diagnostic<usize>> {
        // NOTE: This `get` call is safe to unwrap without further questioning:
        // 1. We know from a previous call to `find_narc_file` that `from_narc` is a valid NarcID.
        // 2. We know that `narc_members` is keyed against the full span of valid NarcID values.
        self.narc_members
            .get(&from_narc.to_string())
            .unwrap()
            .iter()
            .position(|symbol| symbol == from_file.get_ref())
            .ok_or_else(|| {
                Diagnostic::error()
                    .with_message(format!(
                        "expected a member file for NarcID {}, but found {}",
                        from_narc, from_file
                    ))
                    .with_labels(vec![
                        Label::primary(self.filedb_resdat, from_file.range())
                            .with_message("no such member file exists"),
                        Label::secondary(self.filedb_resdat, from_narc.range())
                            .with_message("using this Narc ID's member index"),
                    ])
            })
    }

    fn find_resdef<R: Resource>(
        &self,
        needle: &Spanned<String>,
        haystack: &[R],
        type_key: &str,
    ) -> Result<usize, Diagnostic<usize>> {
        haystack
            .iter()
            .position(|hay| hay.get_id().get_ref() == needle.get_ref())
            .ok_or_else(|| {
                Diagnostic::error()
                    .with_message(format!(
                        "no {} resource named {}",
                        type_key,
                        needle.get_ref()
                    ))
                    .with_labels(vec![Label::primary(self.filedb_resdat, needle.range())])
            })
    }

    fn serialize_templates(&self, resdat: &ResourceData) -> Result<Vec<u8>, Diagnostic<usize>> {
        let mut payload: Vec<u8> = Vec::with_capacity(32 * (resdat.templates.len() + 1));

        for template in resdat.templates.iter() {
            let ncgr = self.find_resdef(&template.characters, &resdat.characters, "characters")?;
            let nclr = self.find_resdef(&template.palette, &resdat.palettes, "palette")?;
            let ncer = self.find_resdef(&template.cells, &resdat.cells, "cells")?;
            let nanr = self.find_resdef(&template.animations, &resdat.animations, "animations")?;
            let mcer: u32 = 0xFFFFFFFF;
            let manr: u32 = 0xFFFFFFFF;

            payload.extend(&serialize(&[
                ncgr as u32,
                nclr as u32,
                ncer as u32,
                nanr as u32,
                mcer,
                manr,
                template.vram_transfer as u32,
                template.priority,
            ]));
        }

        payload.extend(&serialize(TERM_TEMPLATE));
        Ok(payload)
    }
}

impl Serialized {
    fn write(&self, output_base: &str) -> Result<(), String> {
        let pairs = [
            ("characters.resdat", &self.characters),
            ("palettes.resdat", &self.palettes),
            ("cells.resdat", &self.cells),
            ("animations.resdat", &self.animations),
            ("templates", &self.templates),
        ];

        for (extension, payload) in pairs {
            let output_fpath = format!("{}.{}", output_base, extension);
            fs::write(&output_fpath, payload)
                .map_err(|err| format!("failed write to file {}: {}", output_fpath, err))?;
        }

        Ok(())
    }
}

fn serialize(payload: &[u32]) -> Vec<u8> {
    payload
        .iter()
        .flat_map(|v| {
            vec![
                (v & 0xFF) as u8,
                ((v >> 8) & 0xFF) as u8,
                ((v >> 16) & 0xFF) as u8,
                ((v >> 24) & 0xFF) as u8,
            ]
        })
        .collect()
}

trait Resource {
    fn serialize(&self, i: u32, mapped_narc: isize, mapped_file: usize) -> Vec<u8>;
    fn get_id(&self) -> &Spanned<String>;
    fn get_from_narc(&self) -> &Spanned<String>;
    fn get_from_file(&self) -> &Spanned<String>;
}

impl Resource for PaletteResource {
    fn serialize(&self, i: u32, mapped_narc: isize, mapped_file: usize) -> Vec<u8> {
        let payload = &[
            mapped_narc as u32,
            mapped_file as u32,
            self.compressed as u32,
            i,
            (self.on_main_screen as u32) | ((self.on_sub_screen as u32) << 1),
            self.load_in_slot,
        ];

        serialize(payload)
    }

    fn get_id(&self) -> &Spanned<String> {
        &self.id
    }

    fn get_from_narc(&self) -> &Spanned<String> {
        &self.from_narc
    }

    fn get_from_file(&self) -> &Spanned<String> {
        &self.from_file
    }
}

impl Resource for CharacterResource {
    fn serialize(&self, i: u32, mapped_narc: isize, mapped_file: usize) -> Vec<u8> {
        let payload = &[
            mapped_narc as u32,
            mapped_file as u32,
            self.compressed as u32,
            i,
            (self.on_main_screen as u32) | ((self.on_sub_screen as u32) << 1),
            mapped_narc as u32,
        ];

        serialize(payload)
    }

    fn get_id(&self) -> &Spanned<String> {
        &self.id
    }

    fn get_from_narc(&self) -> &Spanned<String> {
        &self.from_narc
    }

    fn get_from_file(&self) -> &Spanned<String> {
        &self.from_file
    }
}

impl Resource for BaseResource {
    fn serialize(&self, i: u32, mapped_narc: isize, mapped_file: usize) -> Vec<u8> {
        let payload = &[
            mapped_narc as u32,
            mapped_file as u32,
            self.compressed as u32,
            i,
            mapped_narc as u32,
            mapped_narc as u32,
        ];

        serialize(payload)
    }

    fn get_id(&self) -> &Spanned<String> {
        &self.id
    }

    fn get_from_narc(&self) -> &Spanned<String> {
        &self.from_narc
    }

    fn get_from_file(&self) -> &Spanned<String> {
        &self.from_file
    }
}
