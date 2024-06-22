#include <nitro.h>
#include <string.h>

#include "constants/moves.h"
#include "consts/moves.h"
#include "consts/pokemon.h"

#include "constants/battle.h"
#include "constants/pokemon.h"
#include "constants/trainer.h"

#include "data/trainer_class_genders.h"

#include "struct_decls/struct_02006C24_decl.h"
#include "savedata.h"
#include "struct_defs/trainer_data.h"
#include "overlay006/battle_params.h"

#include "heap.h"
#include "message.h"
#include "narc.h"
#include "party.h"
#include "pokemon.h"
#include "strbuf.h"
#include "trainer_data.h"
#include "unk_020021B0.h"
#include "unk_0201D15C.h"
#include "savedata_misc.h"

static void TrainerData_BuildParty(BattleParams *battleParams, int battler, int heapID);
static void TrainerMon_HandleOverrideFlags(u16 species, u8 form, u8 flags, u32 *pidMod);
static void TrainerMon_HandleFriendship(Pokemon *mon);

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

#define CHOMP_U8(buf)   *buf; buf++;
#define CHOMP_U16(buf)  (*(buf + 1) << 8) | *buf; buf += 2;

/**
 * @brief Build the party for a trainer as loaded in the BattleParams struct.
 * 
 * @param battleParams  The parent BattleParams struct containing trainer data.
 * @param battler       Which battler's party is to be loaded.
 * @param heapID        Heap on which to perform any allocations.
 */
static void TrainerData_BuildParty(BattleParams *battleParams, int battler, int heapID)
{
    u32 oldSeed = LCRNG_GetSeed();

    // alloc enough space to support the maximum possible data size
    Party_InitWithCapacity(battleParams->parties[battler], MAX_PARTY_SIZE);
    u8 *buf = Heap_AllocFromHeap(heapID, sizeof(TrainerMonWithMovesAndItem) * MAX_PARTY_SIZE);
    Pokemon *mon = Pokemon_New(heapID);

    TrainerData_LoadParty(battleParams->trainerIDs[battler], buf);

    // determine which magic gender-specific modifier to use for the RNG function
    u32 pidMod = TrainerClass_Gender(battleParams->trainerData[battler].class) == GENDER_FEMALE
            ? 120 : 136;

    u8 trainerType = battleParams->trainerData[battler].type;
    for (int i = 0; i < battleParams->trainerData[battler].partySize; i++) {
        u8 dvs    = CHOMP_U8(buf);
        u8 flags  = CHOMP_U8(buf);
        u16 level = CHOMP_U16(buf);
        u16 monID = CHOMP_U16(buf);

        u16 species = monID & 0x3FF;
        u8 form = (monID & 0xFC00) >> 10;

        TrainerMon_HandleOverrideFlags(species, form, flags, &pidMod);

        u32 rnd = dvs + level + species + battleParams->trainerIDs[battler];
        LCRNG_SetSeed(rnd);
        for (int j = 0; j < battleParams->trainerData[battler].class; j++) {
            rnd = LCRNG_Next();
        }

        rnd = (rnd << 8) + pidMod;
        u8 ivs = dvs * MAX_IVS_SINGLE_STAT / MAX_DV;
        Pokemon_InitWith(mon, species, level, ivs, TRUE, rnd, OTID_NOT_SHINY, 0);

        if (trainerType & TRDATATYPE_WITH_ITEM) {
            u16 item = CHOMP_U16(buf);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &item);
        }

        if (trainerType & TRDATATYPE_WITH_MOVES) {
            for (int j = 0; j < LEARNED_MOVES_MAX; j++) {
                u16 move = CHOMP_U16(buf);
                Pokemon_SetMoveSlot(mon, move, j);
            }
        }

        u16 ballSeal = CHOMP_U16(buf);
        Pokemon_SetBallSeal(ballSeal, mon, heapID);
        TrainerMon_HandleFriendship(mon);
        Pokemon_SetValue(mon, MON_DATA_FORM, &form);
        Pokemon_CalcStats(mon);
        Party_AddPokemon(battleParams->parties[battler], mon);
    }

    Heap_FreeToHeap(buf);
    Heap_FreeToHeap(mon);
    LCRNG_SetSeed(oldSeed);
}

static void TrainerMon_HandleOverrideFlags(u16 species, u8 form, u8 flags, u32 *pidMod)
{
    if (flags == 0) {
        return;
    }

    u8 overrideGender = flags & 0x0F;
    if (overrideGender != 0) {
        *pidMod = PokemonPersonalData_GetFormValue(species, form, MON_DATA_PERSONAL_GENDER);
        if (overrideGender == 1) {  // force male
            *pidMod += 2;
        } else {                    // force female
            *pidMod -= 2;
        }
    }

    u8 overrideAbility = (flags & 0xF0) >> 4;
    if (overrideAbility & 1) {      // ability slot 0
        *pidMod &= ~1;
    } else {                        // ability slot 1
        *pidMod |= 1;
    }
}

static void TrainerMon_HandleFriendship(Pokemon *mon)
{
    u8 friendship = 0xFF;
    for (int i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (Pokemon_GetValue(mon, MON_DATA_MOVE1 + i, NULL) == MOVE_FRUSTRATION) {
            friendship = 0;
        }
    }

    Pokemon_SetValue(mon, MON_DATA_FRIENDSHIP, &friendship);
}
