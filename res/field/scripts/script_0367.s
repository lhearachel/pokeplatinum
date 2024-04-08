    .include "macros/scrcmd.inc"

    .data

    .long _028A-.-4
    .long _0ACF-.-4
    .long _07E5-.-4
    .long _0EBF-.-4
    .long _04EC-.-4
    .long _0597-.-4
    .long _01AC-.-4
    .long _06F6-.-4
    .long _13AD-.-4
    .long _1280-.-4
    .long _129A-.-4
    .long _12B4-.-4
    .long _12CE-.-4
    .long _1328-.-4
    .long _133B-.-4
    .long _134E-.-4
    .long _1361-.-4
    .long _1374-.-4
    .long _1387-.-4
    .long _139A-.-4
    .long _13AF-.-4
    .long _0062-.-4
    .long _00F5-.-4
    .long _13E0-.-4
    .short 0xFD13

_0062:
    ScrCmd_011 0x40D8, 1
    ScrCmd_01D 1, _00F1
    ScrCmd_011 0x40D8, 3
    ScrCmd_01D 1, _00F1
    ScrCmd_011 0x40D8, 0xFF
    ScrCmd_01D 1, _00F1
    ScrCmd_238 19, 0x4000
    ScrCmd_011 0x4000, 0
    ScrCmd_01D 1, _0144
    ScrCmd_011 0x4000, 0
    ScrCmd_01D 5, _014A
    ScrCmd_1E0 0x4000
    ScrCmd_011 0x4000, 0
    ScrCmd_01C 5, _00DF
    ScrCmd_01E 0x1E9
    ScrCmd_016 _00C6
    ScrCmd_002

_00C6:
    ScrCmd_238 2, 0x4000
    ScrCmd_011 0x4000, 0
    ScrCmd_01C 5, _00EB
    ScrCmd_01E 0x20E
    ScrCmd_002

_00DF:
    ScrCmd_01F 0x1E9
    ScrCmd_016 _00C6
    ScrCmd_002

_00EB:
    ScrCmd_01F 0x20E
    ScrCmd_002

_00F1:
    ScrCmd_266
    ScrCmd_01B

_00F5:
    ScrCmd_011 0x40D8, 1
    ScrCmd_01C 5, _013A
    ScrCmd_1E0 0x4000
    ScrCmd_011 0x4000, 0
    ScrCmd_01C 5, _013C
    ScrCmd_011 0x4000, 1
    ScrCmd_01C 1, _013C
    ScrCmd_011 0x4000, 2
    ScrCmd_01C 1, _013C
    ScrCmd_011 0x4000, 3
    ScrCmd_01C 1, _013C
_013A:
    ScrCmd_002

_013C:
    ScrCmd_18C 3, 2
    ScrCmd_002

_0144:
    ScrCmd_01E 0x2C3
    ScrCmd_01B

_014A:
    ScrCmd_01F 0x2C3
    ScrCmd_01B

    .byte 219
    .byte 1
    .byte 0
    .byte 0
    .byte 1
    .byte 0
    .byte 27
    .byte 0

_0158:
    ScrCmd_028 0x40D8, 0xFF
    ScrCmd_028 0x40DB, 1
    ScrCmd_028 0x40DC, 1
    ScrCmd_028 0x40D9, 1
    ScrCmd_01B

_0172:
    ScrCmd_028 0x40D8, 0xFF
    ScrCmd_028 0x40DB, 3
    ScrCmd_028 0x40DD, 1
    ScrCmd_028 0x40DE, 1
    ScrCmd_028 0x40DF, 1
    ScrCmd_01B

_0192:
    ScrCmd_028 0x40D8, 0xFF
    ScrCmd_028 0x40DB, 1
    ScrCmd_028 0x40DC, 1
    ScrCmd_028 0x40D9, 2
    ScrCmd_01B

_01AC:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_028 0x4001, 0
    ScrCmd_1DF 0x4002
    ScrCmd_016 _01C6
    ScrCmd_002

_01C6:
    ScrCmd_029 0x8008, 0x4002
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _0239
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _024C
    ScrCmd_011 0x8008, 3
    ScrCmd_01C 1, _025F
    ScrCmd_011 0x8008, 4
    ScrCmd_01C 1, _022E
    ScrCmd_02C 15
    ScrCmd_016 _020B
    ScrCmd_002

_020B:
    ScrCmd_030
    ScrCmd_011 0x4001, 1
    ScrCmd_01C 1, _0220
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_0220:
    ScrCmd_01A _079B
    ScrCmd_016 _0789
    ScrCmd_002

_022E:
    ScrCmd_02C 89
    ScrCmd_016 _020B
    ScrCmd_002

_0239:
    ScrCmd_0D5 0, 20
    ScrCmd_028 0x8004, 85
    ScrCmd_016 _0272
    ScrCmd_002

_024C:
    ScrCmd_0D5 0, 50
    ScrCmd_028 0x8004, 86
    ScrCmd_016 _0272
    ScrCmd_002

_025F:
    ScrCmd_0D5 0, 100
    ScrCmd_028 0x8004, 87
    ScrCmd_016 _0272
    ScrCmd_002

_0272:
    ScrCmd_02C 88
    ScrCmd_028 0x8005, 1
    ScrCmd_014 0x7FE
    ScrCmd_02C 15
    ScrCmd_016 _020B
    ScrCmd_002

_028A:
    ScrCmd_060
    ScrCmd_049 0x5DC
    ScrCmd_1DA
    ScrCmd_02C 0
    ScrCmd_016 _029D
    ScrCmd_002

_029D:
    ScrCmd_02C 1
    ScrCmd_01A _02E6
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 0
    ScrCmd_01C 1, _0301
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _02DB
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _0784
    ScrCmd_016 _0784
    ScrCmd_002

_02DB:
    ScrCmd_02C 3
    ScrCmd_016 _029D
    ScrCmd_002

_02E6:
    ScrCmd_040 31, 11, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 141, 0
    ScrCmd_042 142, 1
    ScrCmd_042 143, 2
    ScrCmd_043
    ScrCmd_01B

_0301:
    ScrCmd_02C 4
    ScrCmd_040 31, 11, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 6, 0
    ScrCmd_042 7, 1
    ScrCmd_042 94, 2
    ScrCmd_043
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 0
    ScrCmd_01C 1, _0352
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _0385
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _0784
    ScrCmd_016 _0784
    ScrCmd_002

_0352:
    ScrCmd_1DB 0, 0
    ScrCmd_016 _0360
    ScrCmd_002

_0360:
    ScrCmd_1DD 1, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _03B0
    ScrCmd_1FF 91, 3, 0, 0
    ScrCmd_016 _0787
    ScrCmd_002

_0385:
    ScrCmd_1DB 0, 1
    ScrCmd_1DD 1, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _03B0
    ScrCmd_1FF 91, 4, 0, 0
    ScrCmd_016 _0787
    ScrCmd_002

_03B0:
    ScrCmd_02C 8
    ScrCmd_01A _04BE
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0784
    ScrCmd_1DD 32, 0, 0x800C
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _04A8
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _04B3
    ScrCmd_1DD 53, 0, 0x800C
    ScrCmd_31E 0x800C, 0x800C
    ScrCmd_011 0x800C, 0xFF
    ScrCmd_01C 1, _13CE
    ScrCmd_1DD 53, 1, 0x800C
    ScrCmd_31E 0x800C, 0x800C
    ScrCmd_011 0x800C, 0xFF
    ScrCmd_01C 1, _13CE
    ScrCmd_1DD 53, 2, 0x800C
    ScrCmd_31E 0x800C, 0x800C
    ScrCmd_011 0x800C, 0xFF
    ScrCmd_01C 1, _13CE
    ScrCmd_1DD 43, 0, 0x800C
    ScrCmd_011 0x800C, 4
    ScrCmd_01C 1, _09B3
    ScrCmd_016 _045C
    ScrCmd_002

_045C:
    ScrCmd_01A _0158
    ScrCmd_1DD 5, 0, 0
    ScrCmd_14E
    ScrCmd_01A _049C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _077E
    ScrCmd_1DD 57, 0, 0x800C
    ScrCmd_1DD 52, 0, 0
    ScrCmd_02C 11
    ScrCmd_030
    ScrCmd_016 _05EC
    ScrCmd_002

_049C:
    ScrCmd_014 0x7D6
    ScrCmd_029 0x800C, 0x4000
    ScrCmd_01B

_04A8:
    ScrCmd_02C 9
    ScrCmd_016 _0784
    ScrCmd_002

_04B3:
    ScrCmd_02C 10
    ScrCmd_016 _0784
    ScrCmd_002

_04BE:
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_034
    ScrCmd_1DD 30, 0, 0
    ScrCmd_0A1
    ScrCmd_1DD 31, 0, 0x800C
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_01B

_04EC:
    ScrCmd_060
    ScrCmd_028 0x40D8, 0
    ScrCmd_1DD 4, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0597
    ScrCmd_02C 16
    ScrCmd_1DB 1, 0xFFFF
    ScrCmd_1DD 43, 0, 0x800C
    ScrCmd_011 0x800C, 2
    ScrCmd_01C 1, _0548
    ScrCmd_01A _0192
    ScrCmd_1DD 5, 0, 0
    ScrCmd_01A _0734
    ScrCmd_02C 11
    ScrCmd_030
    ScrCmd_016 _05EC
    ScrCmd_002

_0548:
    ScrCmd_01A _0577
    ScrCmd_1DD 5, 0, 0
    ScrCmd_01A _0734
    ScrCmd_1DD 51, 0, 0x800C
    ScrCmd_029 0x4009, 0x800C
    ScrCmd_02C 32
    ScrCmd_030
    ScrCmd_016 _05EC
    ScrCmd_002

_0577:
    ScrCmd_028 0x40D8, 0xFF
    ScrCmd_028 0x40DB, 2
    ScrCmd_028 0x40DD, 1
    ScrCmd_028 0x40DE, 2
    ScrCmd_028 0x40DF, 0
    ScrCmd_01B

_0597:
    ScrCmd_060
    ScrCmd_02C 19
    ScrCmd_1DD 14, 0, 0x800C
    ScrCmd_011 0x800C, 4
    ScrCmd_01C 1, _05B9
    ScrCmd_016 _077E
    ScrCmd_002

_05B9:
    ScrCmd_1DD 9, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _077E
    ScrCmd_1DD 10, 0, 0x800C
    ScrCmd_029 0x8000, 0x800C
    ScrCmd_0D5 0, 0x8000
    ScrCmd_02C 65
    ScrCmd_016 _077E
    ScrCmd_002

_05EC:
    ScrCmd_034
    ScrCmd_1DD 43, 0, 0x800C
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 4
    ScrCmd_01C 1, _0652
    ScrCmd_011 0x8008, 5
    ScrCmd_01C 1, _0652
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _066C
    ScrCmd_011 0x8008, 3
    ScrCmd_01C 1, _0686
    ScrCmd_016 _0638
    ScrCmd_002

_0638:
    ScrCmd_028 0x4002, 0
    ScrCmd_028 0x4003, 15
    ScrCmd_01A _06AE
    ScrCmd_016 _0746
    ScrCmd_002

_0652:
    ScrCmd_028 0x4002, 2
    ScrCmd_028 0x4003, 19
    ScrCmd_01A _06AE
    ScrCmd_016 _0746
    ScrCmd_002

_066C:
    ScrCmd_028 0x4002, 1
    ScrCmd_028 0x4003, 11
    ScrCmd_01A _06AE
    ScrCmd_016 _06EE
    ScrCmd_002

_0686:
    ScrCmd_028 0x4002, 1
    ScrCmd_028 0x4003, 11
    ScrCmd_01A _06AE
    ScrCmd_016 _06A0
    ScrCmd_002

_06A0:
    ScrCmd_028 0x40DB, 2
    ScrCmd_016 _0746
    ScrCmd_002

_06AE:
    ScrCmd_05E 0x4002, _1248
    ScrCmd_05E 0xFF, _1260
    ScrCmd_05F
    ScrCmd_168 0, 0, 0x4003, 2, 77
    ScrCmd_16B 77
    ScrCmd_169 77
    ScrCmd_05E 0x4002, _1254
    ScrCmd_05E 0xFF, _126C
    ScrCmd_05F
    ScrCmd_16C 77
    ScrCmd_169 77
    ScrCmd_16A 77
    ScrCmd_01B

_06EE:
    ScrCmd_016 _0746
    ScrCmd_002

_06F6:
    ScrCmd_060
    ScrCmd_01A _0724
    ScrCmd_1DD 100, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0784
    ScrCmd_02C 56
    ScrCmd_01A _0734
    ScrCmd_016 _077E
    ScrCmd_002

_0724:
    ScrCmd_01A _07B9
    ScrCmd_1DD 6, 0, 0
    ScrCmd_01B

_0734:
    ScrCmd_18D
    ScrCmd_12D 0x800C
    ScrCmd_18E
    ScrCmd_049 0x61B
    ScrCmd_04B 0x61B
    ScrCmd_01B

_0746:
    ScrCmd_1E5 58
    ScrCmd_1CD 36, 0, 0, 0, 0
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_0BE 0x147, 0, 3, 6, 0
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_061
    ScrCmd_002

_077E:
    ScrCmd_01A _079B
_0784:
    ScrCmd_02C 2
_0787:
    ScrCmd_031
_0789:
    ScrCmd_034
    ScrCmd_01A _07B5
    ScrCmd_01A _07B9
    ScrCmd_061
    ScrCmd_002

_079B:
    ScrCmd_1DD 6, 0, 0
    ScrCmd_01A _07AB
    ScrCmd_01B

_07AB:
    ScrCmd_1DD 3, 0, 0
    ScrCmd_01B

_07B5:
    ScrCmd_1DC
    ScrCmd_01B

_07B9:
    ScrCmd_028 0x40D8, 0
    ScrCmd_028 0x40DB, 0
    ScrCmd_028 0x40DC, 0
    ScrCmd_028 0x40DD, 0
    ScrCmd_028 0x40D9, 0
    ScrCmd_028 0x40DE, 0
    ScrCmd_028 0x40DF, 0
    ScrCmd_01B

_07E5:
    ScrCmd_060
    ScrCmd_049 0x5DC
    ScrCmd_1DA
    ScrCmd_1DD 12, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0922
    ScrCmd_02C 36
    ScrCmd_016 _080D
    ScrCmd_002

_080D:
    ScrCmd_016 _0815
    ScrCmd_002

_0815:
    ScrCmd_1DD 15, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _08A7
    ScrCmd_016 _0832
    ScrCmd_002

_0832:
    ScrCmd_02C 37
    ScrCmd_01A _087D
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 0
    ScrCmd_01C 1, _0AA7
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _0A5A
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _089C
    ScrCmd_011 0x8008, 3
    ScrCmd_01C 1, _0784
    ScrCmd_016 _0784
    ScrCmd_002

_087D:
    ScrCmd_040 31, 9, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 141, 0
    ScrCmd_042 151, 1
    ScrCmd_042 142, 2
    ScrCmd_042 143, 3
    ScrCmd_043
    ScrCmd_01B

_089C:
    ScrCmd_02C 38
    ScrCmd_016 _0815
    ScrCmd_002

_08A7:
    ScrCmd_02C 37
    ScrCmd_01A _08FF
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 0
    ScrCmd_01C 1, _0AA7
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _0A5A
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _0AB5
    ScrCmd_011 0x8008, 3
    ScrCmd_01C 1, _089C
    ScrCmd_011 0x8008, 4
    ScrCmd_01C 1, _0784
    ScrCmd_016 _0784
    ScrCmd_002

_08FF:
    ScrCmd_040 31, 7, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 141, 0
    ScrCmd_042 151, 1
    ScrCmd_042 152, 2
    ScrCmd_042 142, 3
    ScrCmd_042 143, 4
    ScrCmd_043
    ScrCmd_01B

_0922:
    ScrCmd_02C 61
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0967
    ScrCmd_01A _049C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0784
    ScrCmd_01A _096F
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 5, _0784
    ScrCmd_02C 60
    ScrCmd_016 _0784
    ScrCmd_002

_0967:
    ScrCmd_016 _080D
    ScrCmd_002

_096F:
    ScrCmd_034
    ScrCmd_028 0x4001, 1
    ScrCmd_01A _097F
    ScrCmd_01B

_097F:
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_0B3 0x4000
    ScrCmd_1DD 16, 0x4001, 0x4000
    ScrCmd_029 0x4001, 0x4000
    ScrCmd_0A1
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_029 0x800C, 0x4001
    ScrCmd_01B

_09B3:
    ScrCmd_14E
    ScrCmd_01A _049C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _077E
    ScrCmd_01A _0158
    ScrCmd_1DD 5, 0, 0
    ScrCmd_14E
    ScrCmd_034
    ScrCmd_028 0x4001, 0
    ScrCmd_01A _097F
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 5, _077E
    ScrCmd_016 _09FB
    ScrCmd_002

_09FB:
    ScrCmd_02C 47
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _05EC
    ScrCmd_02C 48
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _09FB
    ScrCmd_028 0x40D8, 2
    ScrCmd_1DD 39, 0, 0
    ScrCmd_1DC
    ScrCmd_02C 56
    ScrCmd_01A _0734
    ScrCmd_190 30
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_034
    ScrCmd_1DD 2, 0, 0
    ScrCmd_061
    ScrCmd_002

_0A5A:
    ScrCmd_01A _049C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0784
    ScrCmd_034
    ScrCmd_028 0x4001, 2
    ScrCmd_01A _097F
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 5, _0784
    ScrCmd_1E3 0x8004, 0x8005
    ScrCmd_0D5 0, 0x8004
    ScrCmd_280 1, 0x8005, 2, 3
    ScrCmd_02C 52
    ScrCmd_030
    ScrCmd_016 _0789
    ScrCmd_002

_0AA7:
    ScrCmd_1DB 0, 4
    ScrCmd_016 _0360
    ScrCmd_002

_0AB5:
    ScrCmd_1DB 0, 5
    ScrCmd_016 _0360
    ScrCmd_002

_0AC3:
    ScrCmd_05E 0xFF, _1278
    ScrCmd_05F
    ScrCmd_01B

_0ACF:
    ScrCmd_060
    ScrCmd_049 0x5DC
    ScrCmd_1DA
    ScrCmd_02C 20
    ScrCmd_016 _0AE2
    ScrCmd_002

_0AE2:
    ScrCmd_02C 21
    ScrCmd_01A _02E6
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 0
    ScrCmd_01C 1, _0B2B
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _0B20
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _0784
    ScrCmd_016 _0784
    ScrCmd_002

_0B20:
    ScrCmd_02C 23
    ScrCmd_016 _0AE2
    ScrCmd_002

_0B2B:
    ScrCmd_1DD 1, 2, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0B50
    ScrCmd_1FF 91, 2, 0, 0
    ScrCmd_016 _0787
    ScrCmd_002

_0B50:
    ScrCmd_02C 26
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0B80
    ScrCmd_016 _0B6C
    ScrCmd_002

_0B6C:
    ScrCmd_1DB 0, 2
    ScrCmd_028 0x4001, 0
    ScrCmd_016 _0B94
    ScrCmd_002

_0B80:
    ScrCmd_1DB 0, 3
    ScrCmd_028 0x4001, 1
    ScrCmd_016 _0B94
    ScrCmd_002

_0B94:
    ScrCmd_02C 8
    ScrCmd_01A _04BE
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0784
    ScrCmd_1DD 32, 0, 0x800C
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _04A8
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _04B3
    ScrCmd_1DD 53, 0, 0x800C
    ScrCmd_31E 0x800C, 0x800C
    ScrCmd_011 0x800C, 0xFF
    ScrCmd_01C 1, _13CE
    ScrCmd_1DD 53, 1, 0x800C
    ScrCmd_31E 0x800C, 0x800C
    ScrCmd_011 0x800C, 0xFF
    ScrCmd_01C 1, _13CE
    ScrCmd_011 0x4001, 1
    ScrCmd_01C 1, _0C53
    ScrCmd_01A _0172
    ScrCmd_1DD 5, 0, 0
    ScrCmd_14E
    ScrCmd_01A _049C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _077E
    ScrCmd_1DD 57, 0, 0x800C
    ScrCmd_1DD 52, 0, 0
    ScrCmd_02C 35
    ScrCmd_016 _05EC
    ScrCmd_002

_0C53:
    ScrCmd_14E
    ScrCmd_01A _049C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _077E
    ScrCmd_016 _0C70
    ScrCmd_002

_0C70:
    ScrCmd_02C 27
    ScrCmd_040 30, 1, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 13, 0
    ScrCmd_042 14, 1
    ScrCmd_042 5, 2
    ScrCmd_043
    ScrCmd_029 0x8008, 0x800C
    ScrCmd_011 0x8008, 0
    ScrCmd_01C 1, _0CB4
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _0D08
    ScrCmd_016 _077E
    ScrCmd_002

_0CB4:
    ScrCmd_02C 112
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0C70
    ScrCmd_034
    ScrCmd_0F2 16, 0, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0CF6
    ScrCmd_011 0x800C, 3
    ScrCmd_01C 1, _0CFE
    ScrCmd_016 _0D5C
    ScrCmd_002

_0CF6:
    ScrCmd_016 _0C70
    ScrCmd_002

_0CFE:
    ScrCmd_150
    ScrCmd_016 _0C70
    ScrCmd_002

_0D08:
    ScrCmd_02C 112
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0C70
    ScrCmd_034
    ScrCmd_0F3 16, 0, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0D4A
    ScrCmd_011 0x800C, 3
    ScrCmd_01C 1, _0D52
    ScrCmd_016 _0D5C
    ScrCmd_002

_0D4A:
    ScrCmd_016 _0C70
    ScrCmd_002

_0D52:
    ScrCmd_150
    ScrCmd_016 _0C70
    ScrCmd_002

_0D5C:
    ScrCmd_136
    ScrCmd_135 1
    ScrCmd_1DD 58, 0, 0
    ScrCmd_016 _0D72
    ScrCmd_002

_0D72:
    ScrCmd_1E1 0, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0D72
    ScrCmd_1E2 0, 0x4000
    ScrCmd_029 0x8008, 0x4000
    ScrCmd_011 0x8008, 1
    ScrCmd_01C 1, _0DE7
    ScrCmd_011 0x8008, 2
    ScrCmd_01C 1, _0E05
    ScrCmd_011 0x8008, 3
    ScrCmd_01C 1, _0E23
    ScrCmd_136
    ScrCmd_135 2
    ScrCmd_1DD 57, 0, 0x8004
    ScrCmd_207 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0E8A
    ScrCmd_1E2 1, 0x4000
    ScrCmd_016 _0E58
    ScrCmd_002

_0DE7:
    ScrCmd_01A _0E4E
    ScrCmd_1DD 53, 0, 0x800C
    ScrCmd_0D0 0, 0x800C
    ScrCmd_02C 33
    ScrCmd_016 _077E
    ScrCmd_002

_0E05:
    ScrCmd_01A _0E4E
    ScrCmd_1DD 53, 1, 0x800C
    ScrCmd_0D0 0, 0x800C
    ScrCmd_02C 33
    ScrCmd_016 _077E
    ScrCmd_002

_0E23:
    ScrCmd_01A _0E4E
    ScrCmd_1DD 53, 0, 0x800C
    ScrCmd_0D0 0, 0x800C
    ScrCmd_1DD 53, 1, 0x800C
    ScrCmd_0D0 1, 0x800C
    ScrCmd_02C 34
    ScrCmd_016 _077E
    ScrCmd_002

_0E4E:
    ScrCmd_136
    ScrCmd_135 8
    ScrCmd_150
    ScrCmd_01B

_0E58:
    ScrCmd_01A _0172
    ScrCmd_1DD 5, 0, 0
    ScrCmd_14E
    ScrCmd_02C 69
    ScrCmd_01A _0734
    ScrCmd_330
    ScrCmd_02C 32
    ScrCmd_003 30, 0x800C
    ScrCmd_136
    ScrCmd_135 3
    ScrCmd_016 _05EC
    ScrCmd_002

_0E8A:
    ScrCmd_1DD 52, 0, 0
    ScrCmd_1DD 58, 0, 0
    ScrCmd_016 _0EA2
    ScrCmd_002

_0EA2:
    ScrCmd_1E1 1, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _0EA2
    ScrCmd_016 _0E58
    ScrCmd_002

_0EBF:
    ScrCmd_060
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_01A _0724
    ScrCmd_1DD 100, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0784
    ScrCmd_1DD 35, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0F7E
    ScrCmd_016 _0F05
    ScrCmd_002

_0F05:
    ScrCmd_02C 53
    ScrCmd_1DD 43, 0, 0x800C
    ScrCmd_029 0x4000, 0x800C
    ScrCmd_029 0x8008, 0x4000
    ScrCmd_011 0x8008, 5
    ScrCmd_01C 1, _0F53
    ScrCmd_011 0x8008, 4
    ScrCmd_01C 1, _11BA
    ScrCmd_1DD 49, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01D 1, _0F73
    ScrCmd_016 _0F53
    ScrCmd_002

_0F53:
    ScrCmd_02C 56
    ScrCmd_01A _0734
    ScrCmd_034
    ScrCmd_011 0x4000, 0
    ScrCmd_01C 1, _118A
    ScrCmd_016 _077E
    ScrCmd_002

_0F73:
    ScrCmd_02C 73
    ScrCmd_04E 0x483
    ScrCmd_04F
    ScrCmd_01B

_0F7E:
    ScrCmd_1DD 43, 0, 0x800C
    ScrCmd_029 0x4000, 0x800C
    ScrCmd_011 0x4000, 5
    ScrCmd_01C 1, _116C
    ScrCmd_011 0x4000, 0
    ScrCmd_01C 1, _1091
    ScrCmd_011 0x4000, 4
    ScrCmd_01C 1, _0FEA
    ScrCmd_1DD 49, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 5, _0FD6
    ScrCmd_01A _1167
    ScrCmd_016 _1177
    ScrCmd_002

_0FD6:
    ScrCmd_02C 12
    ScrCmd_02C 73
    ScrCmd_04E 0x483
    ScrCmd_04F
    ScrCmd_016 _1177
    ScrCmd_002

_0FEA:
    ScrCmd_02C 12
    ScrCmd_1DD 54, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _105E
    ScrCmd_016 _100A
    ScrCmd_002

_100A:
    ScrCmd_1DD 10, 0, 0x800C
    ScrCmd_029 0x4000, 0x800C
    ScrCmd_0CD 0
    ScrCmd_0D5 1, 0x4000
    ScrCmd_02C 55
    ScrCmd_016 _102B
    ScrCmd_002

_102B:
    ScrCmd_1DD 49, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _11F5
    ScrCmd_016 _1048
    ScrCmd_002

_1048:
    ScrCmd_0D5 0, 0x4000
    ScrCmd_02C 70
    ScrCmd_04E 0x483
    ScrCmd_04F
    ScrCmd_016 _11F5
    ScrCmd_002

_105E:
    ScrCmd_1DD 49, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _11F5
    ScrCmd_016 _107B
    ScrCmd_002

_107B:
    ScrCmd_0D5 0, 0x4000
    ScrCmd_02C 132
    ScrCmd_04E 0x483
    ScrCmd_04F
    ScrCmd_016 _11F5
    ScrCmd_002

_1091:
    ScrCmd_1DD 45, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _10AE
    ScrCmd_016 _10F5
    ScrCmd_002

_10AE:
    ScrCmd_01A _1167
    ScrCmd_016 _10BC
    ScrCmd_002

_10BC:
    ScrCmd_02C 14
    ScrCmd_01A _0734
    ScrCmd_1DF 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _077E
    ScrCmd_02C 113
    ScrCmd_034
    ScrCmd_029 0x4002, 0x800C
    ScrCmd_028 0x4001, 1
    ScrCmd_01A _0AC3
    ScrCmd_016 _01C6
    ScrCmd_002

_10F5:
    ScrCmd_011 0x404F, 1
    ScrCmd_01D 1, _1135
    ScrCmd_011 0x404F, 3
    ScrCmd_01D 1, _114C
    ScrCmd_1DD 48, 0, 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _10BC
    ScrCmd_02C 84
    ScrCmd_04E 0x483
    ScrCmd_04F
    ScrCmd_016 _10BC
    ScrCmd_002

_1135:
    ScrCmd_02C 85
    ScrCmd_0CD 0
    ScrCmd_02C 87
    ScrCmd_04E 0x486
    ScrCmd_04F
    ScrCmd_028 0x404F, 2
    ScrCmd_01B

_114C:
    ScrCmd_02C 85
    ScrCmd_0CD 0
    ScrCmd_02C 86
    ScrCmd_04E 0x486
    ScrCmd_04F
    ScrCmd_028 0x404F, 4
    ScrCmd_014 0x806
    ScrCmd_01B

_1167:
    ScrCmd_02C 12
    ScrCmd_01B

_116C:
    ScrCmd_02C 54
    ScrCmd_016 _1177
    ScrCmd_002

_1177:
    ScrCmd_02C 14
    ScrCmd_01A _0734
    ScrCmd_034
    ScrCmd_016 _077E
    ScrCmd_002

_118A:
    ScrCmd_1DF 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _077E
    ScrCmd_02C 113
    ScrCmd_034
    ScrCmd_029 0x4002, 0x800C
    ScrCmd_028 0x4001, 1
    ScrCmd_01A _0AC3
    ScrCmd_016 _01C6
    ScrCmd_002

_11BA:
    ScrCmd_1DD 9, 0, 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _11D7
    ScrCmd_016 _11F5
    ScrCmd_002

_11D7:
    ScrCmd_1DD 10, 0, 0x800C
    ScrCmd_029 0x8000, 0x800C
    ScrCmd_0D5 0, 0x8000
    ScrCmd_02C 65
    ScrCmd_016 _11F5
    ScrCmd_002

_11F5:
    ScrCmd_1DD 11, 1, 0
    ScrCmd_016 _1205
    ScrCmd_002

_1205:
    ScrCmd_01A _079B
    ScrCmd_02C 56
    ScrCmd_01A _0734
    ScrCmd_02C 57
    ScrCmd_03E 0x800C
    ScrCmd_011 0x800C, 1
    ScrCmd_01C 1, _0784
    ScrCmd_01A _096F
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 5, _0784
    ScrCmd_02C 60
    ScrCmd_016 _0784

    .byte 2
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_1248:
    MoveAction_03E
    MoveAction_00C 2
    EndMovement

    .balign 4, 0
_1254:
    MoveAction_00C
    MoveAction_045
    EndMovement

    .balign 4, 0
_1260:
    MoveAction_03E
    MoveAction_00C 2
    EndMovement

    .balign 4, 0
_126C:
    MoveAction_00C 2
    MoveAction_045
    EndMovement

    .balign 4, 0
_1278:
    MoveAction_003
    EndMovement

_1280:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_028 0x8000, 0
    ScrCmd_028 0x8001, 0
    ScrCmd_01A _1304
    ScrCmd_002

_129A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_028 0x8000, 0
    ScrCmd_028 0x8001, 1
    ScrCmd_01A _1304
    ScrCmd_002

_12B4:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_028 0x8000, 0
    ScrCmd_028 0x8001, 2
    ScrCmd_01A _1304
    ScrCmd_002

_12CE:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_1E4 0x800C
    ScrCmd_011 0x800C, 0
    ScrCmd_01C 1, _12F9
    ScrCmd_028 0x8000, 1
    ScrCmd_028 0x8001, 0
    ScrCmd_01A _1304
    ScrCmd_002

_12F9:
    ScrCmd_02C 111
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_1304:
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_1D9 0x8000, 0x8001
    ScrCmd_0A1
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_061
    ScrCmd_01B

_1328:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 74
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_133B:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 75
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_134E:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 76
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_1361:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 77
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_1374:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 78
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_1387:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 79
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_139A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 80
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_13AD:
    ScrCmd_002

_13AF:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04B 0x5DC
    ScrCmd_04C 25, 0
    ScrCmd_02C 81
    ScrCmd_04D
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_13CE:
    ScrCmd_01A _07B5
    ScrCmd_01A _07B9
    ScrCmd_014 0x809
    ScrCmd_002

_13E0:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_020 0xFF
    ScrCmd_01D 0, _1492
    ScrCmd_020 0xFF
    ScrCmd_01D 1, _1497
    ScrCmd_01E 0xFF
    ScrCmd_016 _140A
    ScrCmd_002

_140A:
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_034
    ScrCmd_191
    ScrCmd_193 0x800C
    ScrCmd_0A1
    ScrCmd_029 0x8000, 0x800C
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_011 0x8000, 0xFF
    ScrCmd_01C 1, _1549
    ScrCmd_198 0x8000, 0x8004
    ScrCmd_011 0x8004, 0
    ScrCmd_01C 1, _149C
    ScrCmd_2EE 0x8000, 0x8001, 0x8002, 0x8003
    ScrCmd_011 0x8001, 90
    ScrCmd_01C 3, _14E9
    ScrCmd_011 0x8001, 120
    ScrCmd_01C 3, _14F4
    ScrCmd_011 0x8001, 150
    ScrCmd_01C 3, _14FF
    ScrCmd_011 0x8001, 151
    ScrCmd_01C 4, _150A
    ScrCmd_002

_1492:
    ScrCmd_02C 114
    ScrCmd_01B

_1497:
    ScrCmd_02C 115
    ScrCmd_01B

_149C:
    ScrCmd_02C 117
    ScrCmd_016 _140A
    ScrCmd_002

_14A7:
    ScrCmd_02D 0x8002
    ScrCmd_016 _14B3
    ScrCmd_002

_14B3:
    ScrCmd_011 0x8003, 15
    ScrCmd_01C 3, _1515
    ScrCmd_011 0x8003, 25
    ScrCmd_01C 3, _1520
    ScrCmd_011 0x8003, 30
    ScrCmd_01C 3, _152B
    ScrCmd_011 0x8003, 31
    ScrCmd_01C 4, _1536
    ScrCmd_002

_14E9:
    ScrCmd_02C 118
    ScrCmd_016 _14A7
    ScrCmd_002

_14F4:
    ScrCmd_02C 119
    ScrCmd_016 _14A7
    ScrCmd_002

_14FF:
    ScrCmd_02C 120
    ScrCmd_016 _14A7
    ScrCmd_002

_150A:
    ScrCmd_02C 121
    ScrCmd_016 _14A7
    ScrCmd_002

_1515:
    ScrCmd_02C 128
    ScrCmd_016 _1541
    ScrCmd_002

_1520:
    ScrCmd_02C 129
    ScrCmd_016 _1541
    ScrCmd_002

_152B:
    ScrCmd_02C 130
    ScrCmd_016 _1541
    ScrCmd_002

_1536:
    ScrCmd_02C 131
    ScrCmd_016 _1541
    ScrCmd_002

_1541:
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002

_1549:
    ScrCmd_02C 116
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    ScrCmd_002