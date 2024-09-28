#include "struct_defs/trainer_data.h"

#include <nitro.h>
#include <string.h>

#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/trainer.h"
#include "consts/pokemon.h"

#include "struct_decls/struct_02006C24_decl.h"

#include "data/trainer_class_genders.h"
#include "overlay006/battle_params.h"

#include "heap.h"
#include "message.h"
#include "narc.h"
#include "party.h"
#include "pokemon.h"
#include "savedata.h"
#include "savedata_misc.h"
#include "strbuf.h"
#include "trainer_data.h"
#include "unk_020021B0.h"
#include "unk_0201D15C.h"

static void TrainerData_BuildParty(BattleParams *battleParams, int battler, int heapID);

void TrainerData_Encounter(BattleParams *battleParams, const SaveData *save, int heapID)
{
    TrainerData trdata;
    MessageLoader *msgLoader = MessageLoader_Init(MESSAGE_LOADER_NARC_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, 618, heapID);
    const charcode_t *rivalName = MiscSaveBlock_RivalName(SaveData_MiscSaveBlockConst(save));

    for (int i = 0; i < MAX_BATTLERS; i++) {
        if (!battleParams->trainerIDs[i]) {
            continue;
        }

        TrainerData_Load(battleParams->trainerIDs[i], &trdata);
        battleParams->trainerData[i] = trdata;

        if (trdata.class == TRAINER_CLASS_RIVAL) {
            GF_strcpy(battleParams->trainerData[i].name, rivalName);
        } else {
            Strbuf *trainerName = MessageLoader_GetNewStrbuf(msgLoader, battleParams->trainerIDs[i]);
            Strbuf_ToChars(trainerName, battleParams->trainerData[i].name, TRAINER_NAME_LEN + 1);
            Strbuf_Free(trainerName);
        }

        TrainerData_BuildParty(battleParams, i, heapID);
    }

    battleParams->battleType |= trdata.battleType;
    MessageLoader_Free(msgLoader);
}

u32 TrainerData_LoadParam(int trainerID, enum TrainerDataParam paramID)
{
    u32 result;
    TrainerData trdata;

    TrainerData_Load(trainerID, &trdata);

    switch (paramID) {
    case TRDATA_TYPE:
        result = trdata.type;
        break;

    case TRDATA_CLASS:
        result = trdata.class;
        break;

    case TRDATA_SPRITE:
        result = trdata.sprite;
        break;

    case TRDATA_PARTY_SIZE:
        result = trdata.partySize;
        break;

    case TRDATA_ITEM_1:
    case TRDATA_ITEM_2:
    case TRDATA_ITEM_3:
    case TRDATA_ITEM_4:
        result = trdata.items[paramID - TRDATA_ITEM_1];
        break;

    case TRDATA_AI_MASK:
        result = trdata.aiMask;
        break;

    case TRDATA_BATTLE_TYPE:
        result = trdata.battleType;
        break;
    }

    return result;
}

BOOL TrainerData_HasMessageType(int trainerID, enum TrainerMessageType msgType, int heapID)
{
    NARC *narc; // must declare up here to match
    u16 offset, data[2];

    BOOL result = FALSE;
    int size = NARC_GetMemberSizeByIndexPair(NARC_INDEX_POKETOOL__TRMSG__TRTBL, 0);
    NARC_ReadFromMemberByIndexPair(&offset, NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS, 0, trainerID * 2, 2);
    narc = NARC_ctor(NARC_INDEX_POKETOOL__TRMSG__TRTBL, heapID);

    while (offset != size) {
        NARC_ReadFromMember(narc, 0, offset, 4, data);

        if (data[0] == trainerID && data[1] == msgType) {
            result = TRUE;
            break;
        }

        if (data[0] != trainerID) {
            break;
        }

        offset += 4;
    }

    NARC_dtor(narc);
    return result;
}

void TrainerData_LoadMessage(int trainerID, enum TrainerMessageType msgType, Strbuf *strbuf, int heapID)
{
    NARC *narc; // must declare up here to match
    u16 offset, data[2];

    int size = NARC_GetMemberSizeByIndexPair(NARC_INDEX_POKETOOL__TRMSG__TRTBL, 0);
    NARC_ReadFromMemberByIndexPair(&offset, NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS, 0, trainerID * 2, 2);
    narc = NARC_ctor(NARC_INDEX_POKETOOL__TRMSG__TRTBL, heapID);

    while (offset != size) {
        NARC_ReadFromMember(narc, 0, offset, 4, data);

        if (data[0] == trainerID && data[1] == msgType) {
            MessageBank_GetStrbufFromNARC(NARC_INDEX_MSGDATA__PL_MSG, 617, offset / 4, heapID, strbuf);
            break;
        }

        offset += 4;
    }

    NARC_dtor(narc);

    if (offset == size) {
        Strbuf_Clear(strbuf);
    }
}

void TrainerData_Load(int trainerID, TrainerData *trdata)
{
    NARC_ReadWholeMemberByIndexPair(trdata, NARC_INDEX_POKETOOL__TRAINER__TRDATA, trainerID);
}

void TrainerData_LoadParty(int trainerID, void *trparty)
{
    NARC_ReadWholeMemberByIndexPair(trparty, NARC_INDEX_POKETOOL__TRAINER__TRPOKE, trainerID);
}

u8 TrainerClass_Gender(int trclass)
{
    return sTrainerClassGender[trclass];
}

/* #define DEBUG_LOG */

#ifdef DEBUG_LOG
static void SWIFC(char *s)
{
    asm("swi 0xFC");
}

static int Log(const char *fmt, ...)
{
    char s[1024];

    va_list va;
    va_start(va, fmt);
    vsprintf(s, fmt, va);
    va_end(va);

    SWIFC(s);

    return 0;
}
#endif

#define CHOMP_U8(buf) \
    buf[0];           \
    buf++
#define CHOMP_U16(buf)        \
    ((buf[1] << 8) | buf[0]); \
    buf += 2

static void TrainerMon_SetFriendship(Pokemon *mon);
static void TrainerMon_CheckOverrideFlags(u16 species, u8 form, u8 flags, u32 *pidMod);

/**
 * @brief Build the party for a trainer as loaded in the BattleParams struct.
 *
 * @param battleParams  The parent BattleParams struct containing trainer data.
 * @param battler       Which battler's party is to be loaded.
 * @param heapID        Heap on which to perform any allocations.
 */
static void TrainerData_BuildParty(BattleParams *battleParams, int battler, int heapID)
{
    // must make declarations C89-style to match
    void *buf;
    int i, j;
    u32 pidMod, rnd, oldSeed;
    u8 ivs, *cursor;
    Pokemon *mon;

    oldSeed = LCRNG_GetSeed();

    // alloc enough space to support the maximum possible data size
    Party_InitWithCapacity(battleParams->parties[battler], MAX_PARTY_SIZE);
    buf = Heap_AllocFromHeap(heapID, sizeof(TrainerMonWithMovesAndItem) * MAX_PARTY_SIZE);
    mon = Pokemon_New(heapID);

    TrainerData_LoadParty(battleParams->trainerIDs[battler], buf);

    // reinterpret the loaded memory as a data buffer to be read by-byte
    cursor = (u8 *)buf;

    // determine which magic gender-specific modifier to use for the RNG function
    pidMod = TrainerClass_Gender(battleParams->trainerData[battler].class) == GENDER_FEMALE
        ? 120
        : 136;

    u8 trainerFlags = battleParams->trainerData[battler].type;
#ifdef DEBUG_LOG
    Log("Party size: %d\n", battleParams->trainerData[battler].partySize);
#endif

    for (i = 0; i < battleParams->trainerData[battler].partySize; i++) {
#ifdef DEBUG_LOG
        Log("Slot %d Data:\n", i);
#endif

        u8 dv = CHOMP_U8(cursor);
        u8 flags = CHOMP_U8(cursor);
        u16 level = CHOMP_U16(cursor);
        u16 monID = CHOMP_U16(cursor);

        u16 species = monID & 0x03FF; // least-significant 10 bits
        u8 form = monID >> 10; // most-significant 6 bits

#ifdef DEBUG_LOG
        Log("  DV:         %d\n", dv);
        Log("  Flags:      %d\n", flags);
        Log("  Level:      %d\n", level);
        Log("  Species:    %d\n", species);
        Log("  Form:       %d\n", form);
#endif

        TrainerMon_CheckOverrideFlags(species, form, flags, &pidMod);

        rnd = dv + level + species + battleParams->trainerIDs[battler];
        LCRNG_SetSeed(rnd);

        for (j = 0; j < battleParams->trainerData[battler].class; j++) {
            rnd = LCRNG_Next();
        }

        rnd = (rnd << 8) + pidMod;
        ivs = dv * MAX_IVS_SINGLE_STAT / MAX_DV;
        Pokemon_InitWith(mon, species, level, ivs, TRUE, rnd, OTID_NOT_SHINY, 0);

#ifdef DEBUG_LOG
        Log("  PID:        %d\n", rnd);
#endif

        if (trainerFlags & TRDATATYPE_WITH_ITEM) {
            u16 item = CHOMP_U16(cursor);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &item);

#ifdef DEBUG_LOG
            Log("  Item:       %d\n", item);
#endif
        }

        if (trainerFlags & TRDATATYPE_WITH_MOVES) {
            for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                u16 move = CHOMP_U16(cursor);
                Pokemon_SetMoveSlot(mon, move, j);

#ifdef DEBUG_LOG
                Log("  Move %d:     %d\n", j, move);
#endif
            }
        }

        TrainerMon_SetFriendship(mon);

        u16 cbSeal = CHOMP_U16(cursor);

#ifdef DEBUG_LOG
        Log("  Ball Seal:  %d\n", cbSeal);
#endif

        Pokemon_SetBallSeal(cbSeal, mon, heapID);
        Pokemon_SetValue(mon, MON_DATA_FORM, &form);
        Pokemon_CalcStats(mon); // BUGFIX: Recalculate stats after assigning the form
        Party_AddPokemon(battleParams->parties[battler], mon);
    }

    Heap_FreeToHeap(buf);
    Heap_FreeToHeap(mon);
    LCRNG_SetSeed(oldSeed);
}

static void TrainerMon_SetFriendship(Pokemon *mon)
{
    u8 friendship = 0xFF;
    for (int i = 0; i < LEARNED_MOVES_MAX; i++) {
        u16 move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + i, NULL);
        if (move == MOVE_FRUSTRATION) {
            friendship = 0;
        }
    }

#ifdef DEBUG_LOG
    Log("  Friendship: %d\n", friendship);
#endif
    Pokemon_SetValue(mon, MON_DATA_FRIENDSHIP, &friendship);
}

static void TrainerMon_CheckOverrideFlags(u16 species, u8 form, u8 flags, u32 *pidMod)
{
    u8 genderOverride = flags & 0x0F;
    u8 abilityOverride = flags >> 4;

    if (flags != 0) {
        if (genderOverride != 0) {
            *pidMod = PokemonPersonalData_GetFormValue(species, form, MON_DATA_PERSONAL_GENDER);
            if (genderOverride == 1) {
#ifdef DEBUG_LOG
                Log("  Override to Male\n");
#endif
                *pidMod += 2;
            } else {
#ifdef DEBUG_LOG
                Log("  Override to Female\n");
#endif
                *pidMod -= 2;
            }
        }

        if (abilityOverride == 1) {
#ifdef DEBUG_LOG
            Log("  Override to Ability 1\n");
#endif
            *pidMod &= ~1;
        } else if (abilityOverride == 2) {
#ifdef DEBUG_LOG
            Log("  Override to Ability 2\n");
#endif
            *pidMod |= 1;
        }
    }
}
