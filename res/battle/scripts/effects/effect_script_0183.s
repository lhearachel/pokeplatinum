#include "macros/btlcmd.inc"

    .data

_000:
    TrySetMagicCoat _010
    // {0} shrouded itself with Magic Coat!
    BufferMessage pl_msg_00000368_00571, TAG_NICKNAME, BTLSCR_ATTACKER
    UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_FLAGS_INDIRECT, MOVE_SIDE_EFFECT_ON_HIT|MOVE_SUBSCRIPT_PTR_PRINT_MESSAGE_AND_PLAY_ANIMATION
    End 

_010:
    UpdateVar OPCODE_SET, BTLVAR_MOVE_STATUS_FLAGS, MOVE_STATUS_FAILED
    End 