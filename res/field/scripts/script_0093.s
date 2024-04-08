    .include "macros/scrcmd.inc"

    .data

    .long _0082-.-4
    .long _0084-.-4
    .long _0097-.-4
    .long _00AA-.-4
    .long _00BD-.-4
    .long _00D0-.-4
    .long _00E3-.-4
    .long _00F6-.-4
    .long _0109-.-4
    .long _011C-.-4
    .long _013B-.-4
    .long _02E4-.-4
    .long _02F7-.-4
    .long _030A-.-4
    .long _031B-.-4
    .long _0984-.-4
    .long _0709-.-4
    .long _071C-.-4
    .long _072D-.-4
    .long _0740-.-4
    .long _0753-.-4
    .long _08A4-.-4
    .long _08BB-.-4
    .long _08D0-.-4
    .long _08E7-.-4
    .long _08FE-.-4
    .long _0915-.-4
    .long _092C-.-4
    .long _0943-.-4
    .long _095A-.-4
    .long _0971-.-4
    .long _0997-.-4
    .short 0xFD13

_0082:
    ScrCmd_002

_0084:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0097:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 7
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00AA:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 23
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00BD:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00D0:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00E3:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 11
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_00F6:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 13
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0109:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 21
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_011C:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04B 0x5DC
    ScrCmd_04C 0x1A1, 0
    ScrCmd_02C 22
    ScrCmd_04D
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_013B:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_020 1
    ScrCmd_01C 1, _01A7
    ScrCmd_02C 14
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _01B2
    ScrCmd_02C 15
    ScrCmd_034
    ScrCmd_003 15, 0x800C
    ScrCmd_1BD 0x8004
    ScrCmd_011 0x8004, 0
    ScrCmd_01C 1, _01BD
    ScrCmd_011 0x8004, 1
    ScrCmd_01C 1, _01D7
    ScrCmd_011 0x8004, 2
    ScrCmd_01C 1, _01F1
    ScrCmd_011 0x8004, 3
    ScrCmd_01C 1, _020B
    ScrCmd_002

_01A7:
    ScrCmd_02C 17
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_01B2:
    ScrCmd_02C 18
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_01BD:
    ScrCmd_05E 0, _0240
    ScrCmd_05E 0xFF, _027C
    ScrCmd_05F
    ScrCmd_016 _0225
    ScrCmd_002

_01D7:
    ScrCmd_05E 0, _0240
    ScrCmd_05E 0xFF, _0294
    ScrCmd_05F
    ScrCmd_016 _0225
    ScrCmd_002

_01F1:
    ScrCmd_05E 0, _0240
    ScrCmd_05E 0xFF, _02AC
    ScrCmd_05F
    ScrCmd_016 _0225
    ScrCmd_002

_020B:
    ScrCmd_05E 0, _0258
    ScrCmd_05E 0xFF, _02C4
    ScrCmd_05F
    ScrCmd_016 _0225
    ScrCmd_002

_0225:
    ScrCmd_01E 1
    ScrCmd_02C 16
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

    .byte 75
    .byte 0
    .byte 1
    .byte 0
    .byte 65
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_0240:
    MoveAction_00E 20
    MoveAction_00C 13
    MoveAction_00F 12
    MoveAction_00C 4
    MoveAction_021
    EndMovement

    .balign 4, 0
_0258:
    MoveAction_00C
    MoveAction_00E 2
    MoveAction_00D
    MoveAction_00E 18
    MoveAction_00C 13
    MoveAction_00F 12
    MoveAction_00C 4
    MoveAction_021
    EndMovement

    .balign 4, 0
_027C:
    MoveAction_00C
    MoveAction_00E 20
    MoveAction_00C 13
    MoveAction_00F 12
    MoveAction_00C 3
    EndMovement

    .balign 4, 0
_0294:
    MoveAction_00D
    MoveAction_00E 20
    MoveAction_00C 13
    MoveAction_00F 12
    MoveAction_00C 3
    EndMovement

    .balign 4, 0
_02AC:
    MoveAction_00E
    MoveAction_00E 20
    MoveAction_00C 13
    MoveAction_00F 12
    MoveAction_00C 3
    EndMovement

    .balign 4, 0
_02C4:
    MoveAction_03F 2
    MoveAction_03E
    MoveAction_022
    MoveAction_00E 19
    MoveAction_00C 13
    MoveAction_00F 12
    MoveAction_00C 3
    EndMovement

_02E4:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 19
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_02F7:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 12
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_030A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_02C 20
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_031B:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 24
    ScrCmd_02C 25
    ScrCmd_020 0x14A
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x14B
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x14C
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x14D
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x14E
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x14F
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x150
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x151
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x152
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x153
    ScrCmd_01C 0, _03AC
    ScrCmd_020 0x154
    ScrCmd_01C 0, _03AC
    ScrCmd_01E 0x15B
    ScrCmd_016 _06E8

_03AC:
    ScrCmd_020 0x14A
    ScrCmd_01C 1, _03F4
    ScrCmd_020 0x98C
    ScrCmd_01C 0, _03F4
    ScrCmd_02C 33
    ScrCmd_028 0x8004, 46
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x14A
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_03F4:
    ScrCmd_020 0x14B
    ScrCmd_01C 1, _0442
    ScrCmd_15D 0x800C
    ScrCmd_011 0x800C, 8
    ScrCmd_01C 0, _0442
    ScrCmd_02C 34
    ScrCmd_028 0x8004, 47
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x14B
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0442:
    ScrCmd_020 0x14C
    ScrCmd_01C 1, _048A
    ScrCmd_020 0x96A
    ScrCmd_01C 0, _048A
    ScrCmd_02C 35
    ScrCmd_028 0x8004, 72
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x14C
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_048A:
    ScrCmd_020 0x14D
    ScrCmd_01C 1, _04D2
    ScrCmd_020 0x968
    ScrCmd_01C 0, _04D2
    ScrCmd_02C 36
    ScrCmd_028 0x8004, 73
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x14D
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_04D2:
    ScrCmd_020 0x14E
    ScrCmd_01C 1, _051A
    ScrCmd_020 0x969
    ScrCmd_01C 0, _051A
    ScrCmd_02C 37
    ScrCmd_028 0x8004, 74
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x14E
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_051A:
    ScrCmd_020 0x14F
    ScrCmd_01C 1, _0562
    ScrCmd_020 0x96C
    ScrCmd_01C 0, _0562
    ScrCmd_02C 38
    ScrCmd_028 0x8004, 75
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x14F
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0562:
    ScrCmd_020 0x150
    ScrCmd_01C 1, _05AA
    ScrCmd_020 0x96B
    ScrCmd_01C 0, _05AA
    ScrCmd_02C 39
    ScrCmd_028 0x8004, 76
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x150
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_05AA:
    ScrCmd_020 0x151
    ScrCmd_01C 1, _05F8
    ScrCmd_293 0x800C
    ScrCmd_011 0x800C, 100
    ScrCmd_01C 0, _05F8
    ScrCmd_02C 40
    ScrCmd_028 0x8004, 77
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x151
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_05F8:
    ScrCmd_020 0x152
    ScrCmd_01C 1, _0646
    ScrCmd_286 0x800C
    ScrCmd_011 0x800C, 100
    ScrCmd_01C 0, _0646
    ScrCmd_02C 41
    ScrCmd_028 0x8004, 78
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x152
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0646:
    ScrCmd_020 0x153
    ScrCmd_01C 1, _0694
    ScrCmd_287 0x800C
    ScrCmd_011 0x800C, 100
    ScrCmd_01C 0, _0694
    ScrCmd_02C 42
    ScrCmd_028 0x8004, 79
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x153
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0694:
    ScrCmd_020 0x154
    ScrCmd_01C 1, _06E2
    ScrCmd_288 0x800C
    ScrCmd_011 0x800C, 100
    ScrCmd_01C 0, _06E2
    ScrCmd_02C 43
    ScrCmd_028 0x8004, 80
    ScrCmd_028 0x8005, 1
    ScrCmd_085 0x8004, 0x8005, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _06F3
    ScrCmd_014 0x7DC
    ScrCmd_01E 0x154
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_06E2:
    ScrCmd_016 _06FE

_06E8:
    ScrCmd_02C 27
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_06F3:
    ScrCmd_02C 30
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_06FE:
    ScrCmd_02C 32
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0709:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 47
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_071C:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_02C 48
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_072D:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 49
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0740:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 50
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0753:
    ScrCmd_060
    ScrCmd_05E 0xFF, _087C
    ScrCmd_05F
    ScrCmd_069 0x8004, 0x8005
    ScrCmd_011 0x8005, 0x2D5
    ScrCmd_01C 1, _07A8
    ScrCmd_011 0x8005, 0x2D6
    ScrCmd_01C 1, _07C0
    ScrCmd_011 0x8005, 0x2D7
    ScrCmd_01C 1, _07D8
    ScrCmd_011 0x8005, 0x2D8
    ScrCmd_01C 1, _07F0
    ScrCmd_011 0x8005, 0x2D9
    ScrCmd_01C 1, _0808
    ScrCmd_002

_07A8:
    ScrCmd_186 32, 0x1D7, 0x2D5
    ScrCmd_186 31, 0x1D7, 0x2D5
    ScrCmd_016 _0820
    ScrCmd_002

_07C0:
    ScrCmd_186 32, 0x1D7, 0x2D6
    ScrCmd_186 31, 0x1D7, 0x2D6
    ScrCmd_016 _0820
    ScrCmd_002

_07D8:
    ScrCmd_186 32, 0x1D7, 0x2D7
    ScrCmd_186 31, 0x1D7, 0x2D7
    ScrCmd_016 _0820
    ScrCmd_002

_07F0:
    ScrCmd_186 32, 0x1D7, 0x2D8
    ScrCmd_186 31, 0x1D7, 0x2D8
    ScrCmd_016 _0820
    ScrCmd_002

_0808:
    ScrCmd_186 32, 0x1D7, 0x2D9
    ScrCmd_186 31, 0x1D7, 0x2D9
    ScrCmd_016 _0820
    ScrCmd_002

_0820:
    ScrCmd_01F 0x23C
    ScrCmd_01F 0x23D
    ScrCmd_064 32
    ScrCmd_064 31
    ScrCmd_05E 31, _0884
    ScrCmd_05F
    ScrCmd_04C 0x1AB, 0
    ScrCmd_04D
    ScrCmd_05E 32, _0894
    ScrCmd_05F
    ScrCmd_02C 0
    ScrCmd_02C 1
    ScrCmd_04C 0x1AB, 0
    ScrCmd_04D
    ScrCmd_065 31
    ScrCmd_02C 2
    ScrCmd_034
    ScrCmd_05E 32, _089C
    ScrCmd_05F
    ScrCmd_065 32
    ScrCmd_028 0x40F6, 1
    ScrCmd_061
    ScrCmd_002

    .balign 4, 0
_087C:
    MoveAction_04B
    EndMovement

    .balign 4, 0
_0884:
    MoveAction_012 9
    EndMovement

    .byte 19
    .byte 0
    .byte 8
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_0894:
    MoveAction_012 8
    EndMovement

    .balign 4, 0
_089C:
    MoveAction_00F 8
    EndMovement

_08A4:
    ScrCmd_036 52, 0, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_08BB:
    ScrCmd_037 3, 0
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03A 53, 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_08D0:
    ScrCmd_036 54, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_08E7:
    ScrCmd_036 55, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_08FE:
    ScrCmd_036 56, 1, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_0915:
    ScrCmd_036 57, 1, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_092C:
    ScrCmd_036 58, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_0943:
    ScrCmd_036 59, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_095A:
    ScrCmd_036 60, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    ScrCmd_002

_0971:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 3
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0984:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 10
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0997:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 51
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

    .byte 0
    .byte 0