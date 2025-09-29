#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataproc/dataproc.h"
#include "dataproc/minijson.h"
#include "dataproc/stdutil.h"
#include "dataproc/validate.h"

#include "narc/narc.h"

// IWYU pragma: begin_keep
#define POKEPLATINUM_GENERATED_ENUM
#define POKEPLATINUM_GENERATED_LOOKUP
#define POKEPLATINUM_GENERATED_LOOKUP_IMPL
#include "constants/items.h"
#include "generated/abilities.h"
#include "generated/egg_groups.h"
#include "generated/exp_rates.h"
#include "generated/gender_ratios.h"
#include "generated/items.h"
#include "generated/pokemon_colors.h"
#include "generated/pokemon_types.h"
// IWYU pragma: end_keep

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define ALIGN_4 __attribute__((aligned(4)))

#include "struct_defs/species.h"

// TODO: Remaining bits:
//   1. flesh out the rest of the consumers below for remaining fields (learnsets :despairge:)
//   2. verify the output for some common user-mistakes
//   3. implement a writer for the library
//   4. port msgenc to C, maybe...? just to purge rapidjson entirely
static const dp_field_t FIELDS[];

static bool consume_base_stats(mj_reader_t *r, const char *key, mj_value_t *base_stats, void *user);
static bool consume_types(mj_reader_t *r, const char *key, mj_value_t *types, void *user);
static bool consume_catch_rate(mj_reader_t *r, const char *key, mj_value_t *catch_rate, void *user);
static bool consume_base_exp_reward(mj_reader_t *r, const char *key, mj_value_t *base_exp_reward, void *user);
static bool consume_ev_yields(mj_reader_t *r, const char *key, mj_value_t *ev_yields, void *user);
static bool consume_held_items(mj_reader_t *r, const char *key, mj_value_t *held_items, void *user);
static bool consume_gender_ratio(mj_reader_t *r, const char *key, mj_value_t *gender_ratio, void *user);
static bool consume_hatch_cycles(mj_reader_t *r, const char *key, mj_value_t *hatch_cycles, void *user);
static bool consume_base_friendship(mj_reader_t *r, const char *key, mj_value_t *base_friendship, void *user);
static bool consume_exp_rate(mj_reader_t *r, const char *key, mj_value_t *exp_rate, void *user);
static bool consume_egg_groups(mj_reader_t *r, const char *key, mj_value_t *egg_groups, void *user);
static bool consume_abilities(mj_reader_t *r, const char *key, mj_value_t *abilities, void *user);
static bool consume_safari_flee_rate(mj_reader_t *r, const char *key, mj_value_t *safari_flee_rate, void *user);
static bool consume_body_color(mj_reader_t *r, const char *key, mj_value_t *body_color, void *user);
static bool consume_flip_sprite(mj_reader_t *r, const char *key, mj_value_t *flip_sprite, void *user);
static bool consume_learnset(mj_reader_t *r, const char *key, mj_value_t *learnsets, void *user);

typedef struct SpeciesContainer {
    SpeciesData personal;
} SpeciesContainer;

static int proc_single(int argc, const char **argv);
static int proc_batch(int argc, const char **argv);

int main(int argc, const char **argv) {
    if (argc < 2) {
        fputs("speciesproc: missing command\n", stderr);
        exit(EXIT_FAILURE);
    }

    const char *command = argv[1];
    if      (strcmp(command, "single") == 0) exit(proc_single(argc - 1, argv + 1));
    else if (strcmp(command, "batch") == 0)  exit(proc_batch(argc - 1, argv + 1));
    else {
        fprintf(stderr, "speciesproc: unknown command %s\n", command);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

static int proc_single(int argc, const char **argv) {
    if (argc < 2) {
        fputs("speciesproc-single: missing required argument FILE\n", stderr);
        // TODO: print usage
        return EXIT_FAILURE;
    }

    const char *infile = argv[1];
    file_t      json   = slurp(infile);
    mj_reader_t reader = mj_new_reader(json.data, json.size, (char *)infile);

    SpeciesContainer container = { 0 };
    if (!process_json(&reader, FIELDS, &container)) return EXIT_FAILURE;

    printf("\n");
    printf("base_stats/hp              -> %d\n", container.personal.baseStats.hp);
    printf("base_stats/attack          -> %d\n", container.personal.baseStats.attack);
    printf("base_stats/defense         -> %d\n", container.personal.baseStats.defense);
    printf("base_stats/speed           -> %d\n", container.personal.baseStats.speed);
    printf("base_stats/special_attack  -> %d\n", container.personal.baseStats.spAttack);
    printf("base_stats/special_defense -> %d\n", container.personal.baseStats.spDefense);
    printf("\n");
    printf("types/0 -> %d\n", container.personal.types[0]);
    printf("types/1 -> %d\n", container.personal.types[1]);
    printf("\n");
    printf("ev_yields/hp              -> %d\n", container.personal.evYields.hp);
    printf("ev_yields/attack          -> %d\n", container.personal.evYields.attack);
    printf("ev_yields/defense         -> %d\n", container.personal.evYields.defense);
    printf("ev_yields/speed           -> %d\n", container.personal.evYields.speed);
    printf("ev_yields/special_attack  -> %d\n", container.personal.evYields.spAttack);
    printf("ev_yields/special_defense -> %d\n", container.personal.evYields.spDefense);
    printf("\n");
    printf("held_items/common -> %d\n", container.personal.wildHeldItems.common);
    printf("held_items/rare   -> %d\n", container.personal.wildHeldItems.rare);
    printf("\n");
    printf("egg_groups/0 -> %d\n", container.personal.eggGroups[0]);
    printf("egg_groups/1 -> %d\n", container.personal.eggGroups[1]);
    printf("\n");
    printf("abilities/0 -> %d\n", container.personal.abilities[0]);
    printf("abilities/1 -> %d\n", container.personal.abilities[1]);
    printf("\n");
    printf("base_exp_reward  -> %d\n", container.personal.baseExpReward);
    printf("base_friendship  -> %d\n", container.personal.baseFriendship);
    printf("body_color       -> %d\n", container.personal.bodyColor);
    printf("catch_rate       -> %d\n", container.personal.catchRate);
    printf("exp_rate         -> %d\n", container.personal.expRate);
    printf("flip_sprite?     -> %s\n", container.personal.flipSprite ? "yes" : "no");
    printf("gender_ratio     -> %d\n", container.personal.genderRatio);
    printf("hatch_cycles     -> %d\n", container.personal.hatchCycles);
    printf("safari_flee_rate -> %d\n", container.personal.safariFleeRate);
    printf("\n");
    printf("learnset/by_tm/0 -> 0x%08X\n", container.personal.tmLearnsetMasks[0]);
    printf("learnset/by_tm/1 -> 0x%08X\n", container.personal.tmLearnsetMasks[1]);
    printf("learnset/by_tm/2 -> 0x%08X\n", container.personal.tmLearnsetMasks[2]);
    printf("learnset/by_tm/3 -> 0x%08X\n", container.personal.tmLearnsetMasks[3]);

    free(json.data);
    return EXIT_SUCCESS;
}

static const char *ADDITIONAL_FORMS[] = {
    "deoxys/forms/attack",
    "deoxys/forms/defense",
    "deoxys/forms/speed",
    "wormadam/forms/sandy",
    "wormadam/forms/trash",
    "giratina/forms/origin",
    "shaymin/forms/sky",
    "rotom/forms/heat",
    "rotom/forms/wash",
    "rotom/forms/frost",
    "rotom/forms/fan",
    "rotom/forms/mow",
};

typedef struct narc narc;
typedef struct vfs_pack_ctx vfs_pack_ctx;

static char* make_registry(const char *registry) {
    size_t addlen = 0;
    for (size_t i = 0; i < sizeof(ADDITIONAL_FORMS) / sizeof(char*); i++) {
        addlen += strlen(ADDITIONAL_FORMS[i]) + 1;
    }

    size_t reglen = strlen(registry);
    char  *result = memcpy(malloc(reglen + addlen + 1), registry, reglen);

    result[reglen] = ';';
    char *p = result + reglen + 1;
    for (size_t i = 0; i < sizeof(ADDITIONAL_FORMS) / sizeof(char*); i++) {
        size_t len = strlen(ADDITIONAL_FORMS[i]);
        memcpy(p, ADDITIONAL_FORMS[i], len);
        p[len] = ';';
        p     += len + 1;
    }

    result[reglen + addlen] = '\0';
    return result;
}

static int proc_batch(int argc, const char **argv) {
    if (argc < 2) {
        fputs("speciesproc-batch: missing required argument SOURCE_DIR\n", stderr);
        // TODO: print usage
        return EXIT_FAILURE;
    }

    const char *p_registry = getenv("SPECIES");
    if (p_registry == NULL) {
        fputs("speciesproc-batch: missing required environment variable SPECIES\n", stderr);
        // TODO: print usage
        return EXIT_FAILURE;
    }

    const char *source_dir = argv[1];
    const char *target_dir = argc == 3 ? argv[2] : ".";
    (void)target_dir;


    vfs_pack_ctx *vfs_species_data = narc_pack_start();

    int   errc     = EXIT_FAILURE;
    char *registry = make_registry(p_registry);
    for (char *species = strtok(registry, ";"); species != NULL; species = strtok(NULL, ";")) {
        printf("walk: %s/%s\n", source_dir, species);
        char *indir  = join_path(source_dir, species);
        char *infile = join_path(indir, "data.json");
        free(indir);

        file_t      json   = slurp(infile);
        mj_reader_t reader = mj_new_reader(json.data, json.size, (char *)infile);

        SpeciesContainer container = { 0 };
        if (!process_json(&reader, FIELDS, &container)) {
            free(infile);
            free(json.data);
            goto batch_cleanup;
        }

        narc_pack_file_copy(vfs_species_data, (u8 *)&container.personal, sizeof(container.personal));
        free(infile);
        free(json.data);
    }

    narc *species_data = narc_pack(vfs_species_data);
    vfs_species_data   = NULL;

    char *outpath = join_path(target_dir, "pl_personal.narc");
    FILE *output = fopen(outpath, "wb");
    fwrite(species_data, sizeof(char), species_data->size, output);
    fclose(output);
    free(outpath);
    free(species_data);

    errc = EXIT_SUCCESS;

batch_cleanup:
    narc_pack_halt(vfs_species_data);
    free(registry);
    return errc;
}

static const dp_field_t FIELDS[] = {
    { "base_stats",       consume_base_stats       },
    { "types",            consume_types            },
    { "catch_rate",       consume_catch_rate       },
    { "base_exp_reward",  consume_base_exp_reward  },
    { "ev_yields",        consume_ev_yields        },
    { "held_items",       consume_held_items       },
    { "gender_ratio",     consume_gender_ratio     },
    { "hatch_cycles",     consume_hatch_cycles     },
    { "base_friendship",  consume_base_friendship  },
    { "exp_rate",         consume_exp_rate         },
    { "egg_groups",       consume_egg_groups       },
    { "abilities",        consume_abilities        },
    { "safari_flee_rate", consume_safari_flee_rate },
    { "body_color",       consume_body_color       },
    { "flip_sprite",      consume_flip_sprite      },
    { "icon_palette",     NULL                     },
    { "learnset",         consume_learnset         },
    { "evolutions",       NULL                     },
    { "offspring",        NULL                     },
    { "footprint",        NULL                     },
    { "pokedex_data",     NULL                     },
    { "catching_show",    NULL                     },
    { 0 },
};

static bool consume_base_stat(const char *parent, const char *key, mj_reader_t *r, mj_value_t *v, u8 *target) {
    long n;
    bool result = get_int_in_range(parent, key, r, v, 0, UINT8_MAX, &n);
    if  (result) *target = (uint8_t)n;

    return result;
}

static bool consume_base_stats(mj_reader_t *r, const char *key, mj_value_t *base_stats, void *user) {
    SpeciesBaseStats *target = &((SpeciesContainer *)user)->personal.baseStats;

    bool       ok = is_object(key, r, base_stats);
    mj_value_t k  = { 0 };
    mj_value_t v  = { 0 };

    while (ok && mj_iter_object(r, base_stats, &k, &v)) {
        if      (mj_strequ(&k, "hp"))              ok = consume_base_stat(key, "hp",              r, &v, &target->hp);
        else if (mj_strequ(&k, "attack"))          ok = consume_base_stat(key, "attack",          r, &v, &target->attack);
        else if (mj_strequ(&k, "defense"))         ok = consume_base_stat(key, "defense",         r, &v, &target->defense);
        else if (mj_strequ(&k, "speed"))           ok = consume_base_stat(key, "speed",           r, &v, &target->speed);
        else if (mj_strequ(&k, "special_attack"))  ok = consume_base_stat(key, "special_attack",  r, &v, &target->spAttack);
        else if (mj_strequ(&k, "special_defense")) ok = consume_base_stat(key, "special_defense", r, &v, &target->spDefense);
    }

    return ok && k.type == MJ_OBJEND;
}

static bool consume_type(const char *parent, size_t i, mj_reader_t *r, mj_value_t *v, u8 *target) {
    char s_i[2] = { (char)('0' + i), '\0' };
    return get_enum_by_key(PokemonType, parent, s_i, r, v, target);
}

static bool consume_types(mj_reader_t *r, const char *key, mj_value_t *types, void *user) {
    u8 *target = ((SpeciesContainer *)user)->personal.types;

    bool       ok = is_array(key, r, types);
    mj_value_t v  = { 0 };
    size_t     i  = 0;

    while (ok && mj_iter_array(r, types, &v) && i < MAX_TYPES) {
        ok = consume_type(key, i, r, &v, &target[i]);
        i++;
    }

    return ok
        && is_array_of_size(key, r, types, &v, &i, MAX_TYPES)
        && v.type == MJ_ARREND;
}

static bool consume_catch_rate(mj_reader_t *r, const char *key, mj_value_t *catch_rate, void *user) {
    u8 *target = &((SpeciesContainer *)user)->personal.catchRate;

    long n;
    bool result = get_int_in_range(NULL, key, r, catch_rate, 0, UINT8_MAX, &n);
    if  (result) *target = (u8)n;

    return result;
}

static bool consume_base_exp_reward(mj_reader_t *r, const char *key, mj_value_t *base_exp_reward, void *user) {
    u8 *target = &((SpeciesContainer *)user)->personal.baseExpReward;

    long n;
    bool result = get_int_in_range(NULL, key, r, base_exp_reward, 0, UINT8_MAX, &n);
    if  (result) *target = (u8)n;

    return result;
}

static bool consume_ev_yield(const char *parent, const char *key, mj_reader_t *r, mj_value_t *v, u8 *target) {
    long n;
    bool result = get_int_in_range(parent, key, r, v, 0, 3, &n);
    if  (result) *target = (u8)n;

    return result;
}

static bool consume_ev_yields(mj_reader_t *r, const char *key, mj_value_t *ev_yields, void *user) {
    SpeciesEVYields *target = &((SpeciesContainer *)user)->personal.evYields;

    bool       ok = is_object(key, r, ev_yields);
    mj_value_t k  = { 0 };
    mj_value_t v  = { 0 };

    u8 tmp = 0;
    while (ok && mj_iter_object(r, ev_yields, &k, &v)) {
        // NOTE: The ugly looking casts here silence warnings about data loss due to a size-conversion that is impossible / irrelevant.
        // Any case where we would lose data will result in a failure.
        if      (mj_strequ(&k, "hp"))              { ok = consume_ev_yield(key, "hp",              r, &v, &tmp); target->hp        = (u16)(tmp & 0x3); }
        else if (mj_strequ(&k, "attack"))          { ok = consume_ev_yield(key, "attack",          r, &v, &tmp); target->attack    = (u16)(tmp & 0x3); }
        else if (mj_strequ(&k, "defense"))         { ok = consume_ev_yield(key, "defense",         r, &v, &tmp); target->defense   = (u16)(tmp & 0x3); }
        else if (mj_strequ(&k, "speed"))           { ok = consume_ev_yield(key, "speed",           r, &v, &tmp); target->speed     = (u16)(tmp & 0x3); }
        else if (mj_strequ(&k, "special_attack"))  { ok = consume_ev_yield(key, "special_attack",  r, &v, &tmp); target->spAttack  = (u16)(tmp & 0x3); }
        else if (mj_strequ(&k, "special_defense")) { ok = consume_ev_yield(key, "special_defense", r, &v, &tmp); target->spDefense = (u16)(tmp & 0x3); }
    }

    return ok && k.type == MJ_OBJEND;
}

static bool consume_held_item(const char *parent, const char *key, mj_reader_t *r, mj_value_t *v, u16 *target) {
    return get_enum_by_key(Item, parent, key, r, v, target);
}

static bool consume_held_items(mj_reader_t *r, const char *key, mj_value_t *held_items, void *user) {
    SpeciesWildHeldItems *target = &((SpeciesContainer *)user)->personal.wildHeldItems;

    bool       ok = is_object(key, r, held_items);
    mj_value_t k  = { 0 };
    mj_value_t v  = { 0 };
    while (ok && mj_iter_object(r, held_items, &k, &v)) {
        if      (mj_strequ(&k, "common")) ok = consume_held_item(key, "common", r, &v, &target->common);
        else if (mj_strequ(&k, "rare"))   ok = consume_held_item(key, "rare",   r, &v, &target->rare);
    }

    return ok && k.type == MJ_OBJEND;
}

static bool consume_gender_ratio(mj_reader_t *r, const char *key, mj_value_t *gender_ratio, void *user) {
    u8 *target = &((SpeciesContainer *)user)->personal.genderRatio;
    return get_enum_by_key(GenderRatio, NULL, key, r, gender_ratio, target);
}

static bool consume_hatch_cycles(mj_reader_t *r, const char *key, mj_value_t *hatch_cycles, void *user) {
    u8 *target = &((SpeciesContainer *)user)->personal.hatchCycles;

    long n;
    bool result = get_int_in_range(NULL, key, r, hatch_cycles, 0, UINT8_MAX, &n);
    if  (result) *target = (u8)n;

    return result;
}

static bool consume_base_friendship(mj_reader_t *r, const char *key, mj_value_t *base_friendship, void *user) {
    u8 *target = &((SpeciesContainer *)user)->personal.baseFriendship;

    long n;
    bool result = get_int_in_range(NULL, key, r, base_friendship, 0, UINT8_MAX, &n);
    if  (result) *target = (u8)n;

    return result;
}

static bool consume_exp_rate(mj_reader_t *r, const char *key, mj_value_t *exp_rate, void *user) {
    u8 *target = &((SpeciesContainer *)user)->personal.expRate;
    return get_enum_by_key(ExpRate, NULL, key, r, exp_rate, target);
}

static bool consume_egg_group(const char *parent, size_t i, mj_reader_t *r, mj_value_t *v, u8 *target) {
    char s_i[2] = { (char)('0' + i), '\0' };
    return get_enum_by_key(EggGroup, parent, s_i, r, v, target);
}

static bool consume_egg_groups(mj_reader_t *r, const char *key, mj_value_t *egg_groups, void *user) {
    u8 *target = ((SpeciesContainer *)user)->personal.eggGroups;

    bool       ok = is_array(key, r, egg_groups);
    mj_value_t v  = { 0 };
    size_t     i  = 0;

    while (ok && mj_iter_array(r, egg_groups, &v) && i < MAX_EGG_GROUPS) {
        ok = consume_egg_group(key, i, r, &v, &target[i]);
        i++;
    }

    return ok
        && is_array_of_size(key, r, egg_groups, &v, &i, MAX_EGG_GROUPS)
        && v.type == MJ_ARREND;
}

static bool consume_ability(const char *parent, size_t i, mj_reader_t *r, mj_value_t *v, u8 *target) {
    char s_i[2] = { (char)('0' + i), '\0' };
    return get_enum_by_key(Ability, parent, s_i, r, v, target);
}

static bool consume_abilities(mj_reader_t *r, const char *key, mj_value_t *abilities, void *user) {
    u8 *target = ((SpeciesContainer *)user)->personal.abilities;

    bool       ok = is_array(key, r, abilities);
    mj_value_t v  = { 0 };
    size_t     i  = 0;

    while (ok && mj_iter_array(r, abilities, &v) && i < MAX_ABILITIES) {
        ok = consume_ability(key, i, r, &v, &target[i]);
        i++;
    }

    return ok
        && is_array_of_size(key, r, abilities, &v, &i, MAX_ABILITIES)
        && v.type == MJ_ARREND;
}

static bool consume_safari_flee_rate(mj_reader_t *r, const char *key, mj_value_t *safari_flee_rate, void *user) {
    u8 *target = &((SpeciesContainer *)user)->personal.safariFleeRate;

    long n;
    bool result = get_int_in_range(NULL, key, r, safari_flee_rate, 0, UINT8_MAX, &n);
    if  (result) *target = (u8)n;

    return result;
}

static bool consume_body_color(mj_reader_t *r, const char *key, mj_value_t *body_color, void *user) {
    u8   n    = 0;
    bool result = get_enum_by_key(PokemonColor, NULL, key, r, body_color, &n);
    if  (result)  ((SpeciesContainer *)user)->personal.bodyColor = (u8)(n & 0x7F); // NOTE: cannot take the address of a bit-field

    return result;
}

static bool consume_flip_sprite(mj_reader_t *r, const char *key, mj_value_t *flip_sprite, void *user) {
    bool b;
    bool result = get_boolean(NULL, key, r, flip_sprite, &b);
    if  (result)  ((SpeciesContainer *)user)->personal.flipSprite = b; // NOTE: cannot take the address of a bit-field

    return result;
}

static bool consume_tmhm(const char *parent, size_t i, mj_reader_t *r, mj_value_t *v, u32 masks[4]) {
    char s_i[4] = { 0 }; // NOTE: max 3 digits
    sprintf(s_i, "%zu", i);

    const char *beg, *end;
    size_t      len;

    char *path   = join_path(parent, s_i);
    bool  result = get_string(NULL, path, r, v, &beg, &end, &len);

    if (result) {
        result = beg[0] == 'T' || beg[0] == 'H';
        result = beg[1] == 'M';
        if (!result) goto consume_tmhm_err;

        char *endptr = NULL;
        long  n = strtol(beg + 2, &endptr, 10);
        result  = (endptr == end);
        if (!result) goto consume_tmhm_err;

        n--;
        if (beg[0] == 'T') {
            result = n < NUM_TMS;
        } else {
            n     += beg[0] == 'H' ? NUM_TMS : 0;
            result = n < NUM_TMHMS;
        }

        if (!result) goto consume_tmhm_err;

        long mask_idx    = n / 32;
        long mask_bit    = n % 32;
        masks[mask_idx] |= ((u32)1 << mask_bit);
    }

    free(path);
    return result;

consume_tmhm_err:
    report__value_err(
        r, v,
        "expected %s to be a TM or HM identifier, but got %.*s",
        path, (int)len, beg
    );

    free(path);
    return false;
}

static bool consume_by_tm_learnset(const char *parent, const char *key, mj_reader_t *r, mj_value_t *by_tm, u32 masks[4]) {
    bool       ok   = is_array(key, r, by_tm);
    mj_value_t v    = { 0 };
    size_t     i    = 0;
    char      *path = join_path(parent, key);

    while (ok && mj_iter_array(r, by_tm, &v)) {
        ok = consume_tmhm(path, i, r, &v, masks);
        i++;
    }

    free(path);
    return ok && v.type == MJ_ARREND;
}

static bool consume_learnset(mj_reader_t *r, const char *key, mj_value_t *learnsets, void *user) {
    SpeciesContainer *container = user;

    bool       ok = is_object(key, r, learnsets);
    mj_value_t k  = { 0 };
    mj_value_t v  = { 0 };

    while (ok && mj_iter_object(r, learnsets, &k, &v)) {
        if (mj_strequ(&k, "by_level"))      {}
        else if (mj_strequ(&k, "by_tm"))    ok = consume_by_tm_learnset(key, "by_tm", r, &v, container->personal.tmLearnsetMasks);
        else if (mj_strequ(&k, "by_tutor")) {}
    }

    return ok && k.type == MJ_OBJEND;
}
