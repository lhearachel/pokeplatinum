#include "constants/battle/side_effects.h"
#include "constants/battle/terrain.h"

static const u32 sTerrainSideEffect[] = {
    [TERRAIN_PLAIN]       = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_ACCURACY_DOWN_1_STAGE,
    [TERRAIN_SAND]        = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_ACCURACY_DOWN_1_STAGE,
    [TERRAIN_GRASS]       = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_SLEEP,
    [TERRAIN_PUDDLE]      = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_SLEEP,
    [TERRAIN_MOUNTAIN]    = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_FLINCH,
    [TERRAIN_CAVE]        = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_FLINCH,
    [TERRAIN_SNOW]        = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_FREEZE,
    [TERRAIN_WATER]       = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_ATTACK_DOWN_1_STAGE,
    [TERRAIN_ICE]         = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_FREEZE,
    [TERRAIN_BUILDING]    = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_PARALYZE,
    [TERRAIN_GREAT_MARSH] = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_SPEED_DOWN_1_STAGE,
    [TERRAIN_BRIDGE]      = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_EVASION_DOWN_1_STAGE,
    [TERRAIN_SPECIAL]     = MOVE_SIDE_EFFECT_TO_DEFENDER | MOVE_SIDE_EFFECT_PARALYZE,
};