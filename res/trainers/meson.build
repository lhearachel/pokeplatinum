trainer_data_dir = meson.current_source_dir() / 'data'

trainer_consts = fs.read(trainers_txt).splitlines()
trainer_stems = []
trainer_data_files = []
foreach trainer : trainer_consts
    # `substring` here strips the `TRAINER_` prefix
    trainer_stem = trainer.substring(8).to_lower()
    trainer_stems += trainer_stem
    trainer_data_files += files(trainer_data_dir / (trainer_stem + '.json'))
endforeach

trainer_env = environment()
trainer_env.append('TRAINERS', trainer_stems, separator: ';')

datagen_trainer_out = custom_target('datagen_trainer_out',
    output: [
        'trdata.narc',
        'trpoke.narc',
    ],
    command: [
        datagen_trainer_exe,
        meson.current_build_dir(),
        trainer_data_dir,
    ],
    env: trainer_env,
    depend_files: [
        trainer_data_files,
    ],
)

nitrofs_files += datagen_trainer_out
