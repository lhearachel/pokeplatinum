#include "unk_0201D670.h"

#include <nitro.h>
#include <string.h>

#include "struct_defs/struct_0201D670.h"
#include "struct_defs/struct_0205AA50.h"

#include "heap.h"
#include "render_text.h"
#include "strbuf.h"
#include "sys_task.h"
#include "sys_task_manager.h"
#include "unk_02002B7C.h"
#include "unk_02006E3C.h"
#include "unk_02018340.h"

static int sub_0201D9E8(TextPrinter *param0);
static void sub_0201D97C(SysTask *param0, void *param1);
static u8 sub_0201D67C(SysTaskFunc param0, void *param1, u32 param2);
static void sub_0201D6B0(u8 param0);
static void sub_0201DB48(TextPrinter *param0);
static void sub_0201DBD8(TextPrinter *param0);
static u8 *sub_0201DB50(void);

static const UnkStruct_0201D670 *Unk_02100840 = NULL;
static u8 Unk_021C04D8 = 0;

void sub_0201D670(const UnkStruct_0201D670 *param0)
{
    Unk_02100840 = param0;
}

static SysTask *Unk_021C04E0[8] = { 0 };

static u8 sub_0201D67C(SysTaskFunc param0, void *param1, u32 param2)
{
    int v0;

    for (v0 = 0; v0 < 8; v0++) {
        if (Unk_021C04E0[v0] == NULL) {
            Unk_021C04E0[v0] = sub_0200DA20(param0, param1, param2);

            if (Unk_021C04E0[v0] == NULL) {
                v0 = 8;
            }

            break;
        }
    }

    return v0;
}

static void sub_0201D6B0(u8 param0)
{
    GF_ASSERT(param0 < 8);
    GF_ASSERT(Unk_021C04E0[param0] != NULL);

    if ((param0 < 8) && (Unk_021C04E0[param0] != NULL)) {
        TextPrinter *v0 = SysTask_GetParam(Unk_021C04E0[param0]);

        if (v0) {
            sub_0201DBD8(v0);
            Heap_FreeToHeap(v0);
        }

        SysTask_Done(Unk_021C04E0[param0]);
        Unk_021C04E0[param0] = NULL;
    }
}

static BOOL sub_0201D6F8(u8 param0)
{
    return Unk_021C04E0[param0] != NULL;
}

void sub_0201D710(void)
{
    int v0;

    for (v0 = 0; v0 < 8; v0++) {
        Unk_021C04E0[v0] = NULL;
    }
}

u8 Message_Printing(u8 param0)
{
    return sub_0201D6F8(param0);
}

void PrintString_ForceStop(u8 param0)
{
    sub_0201D6B0(param0);
}

u8 PrintStringSimple(Window *window, u32 fontID, const Strbuf *strbuf, u32 xOffset, u32 yOffset, u32 renderDelay, TextPrinterCallback callback)
{
    TextPrinterTemplate v0;

    v0.toPrint.strbuf = strbuf;
    v0.window = window;
    v0.fontID = fontID;
    v0.x = xOffset;
    v0.y = yOffset;
    v0.currX = xOffset;
    v0.currY = yOffset;
    v0.letterSpacing = Unk_02100840[fontID].unk_02;
    v0.lineSpacing = Unk_02100840[fontID].unk_03;
    v0.dummy14 = Unk_02100840[fontID].unk_04;
    v0.fgColor = Unk_02100840[fontID].unk_05;
    v0.bgColor = Unk_02100840[fontID].unk_06;
    v0.shadowColor = Unk_02100840[fontID].unk_07;
    v0.glyphTable = 0;
    v0.dummy1A = 0;
    v0.cacheColor = 0xFF;

    return sub_0201D834(&v0, renderDelay, callback);
}

u8 sub_0201D78C(Window *param0, u32 param1, const Strbuf *param2, u32 param3, u32 param4, u32 param5, u32 param6, TextPrinterCallback param7)
{
    TextPrinterTemplate v0;

    v0.toPrint.strbuf = param2;
    v0.window = param0;
    v0.fontID = param1;
    v0.x = param3;
    v0.y = param4;
    v0.currX = param3;
    v0.currY = param4;
    v0.letterSpacing = Unk_02100840[param1].unk_02;
    v0.lineSpacing = Unk_02100840[param1].unk_03;
    v0.dummy14 = Unk_02100840[param1].unk_04;
    v0.fgColor = (((param6) >> 16) & 0xff);
    v0.shadowColor = (((param6) >> 8) & 0xff);
    v0.bgColor = (((param6) >> 0) & 0xff);
    v0.glyphTable = 0;
    v0.dummy1A = 0;
    v0.cacheColor = 0xFF;

    return sub_0201D834(&v0, param5, param7);
}

u8 PrintStringWithColorAndMargins(Window *param0, u32 param1, const Strbuf *param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, TextPrinterCallback param9)
{
    TextPrinterTemplate v0;

    v0.toPrint.strbuf = param2;
    v0.window = param0;
    v0.fontID = param1;
    v0.x = param3;
    v0.y = param4;
    v0.currX = param3;
    v0.currY = param4;
    v0.letterSpacing = param7;
    v0.lineSpacing = param8;
    v0.dummy14 = Unk_02100840[param1].unk_04;
    v0.fgColor = (((param6) >> 16) & 0xff);
    v0.shadowColor = (((param6) >> 8) & 0xff);
    v0.bgColor = (((param6) >> 0) & 0xff);
    v0.glyphTable = 0;
    v0.dummy1A = 0;
    v0.cacheColor = 0xFF;

    return sub_0201D834(&v0, param5, param9);
}

u8 sub_0201D834(const TextPrinterTemplate *param0, u32 param1, TextPrinterCallback param2)
{
    TextPrinter *v0;
    int v1;

    if (Unk_02100840 == NULL) {
        return 0xff;
    }

    v0 = Heap_AllocFromHeap(0, sizeof(TextPrinter));

    v0->active = 1;
    v0->state = 0;
    v0->textSpeedBottom = param1;
    v0->delayCounter = 0;
    v0->scrollDistance = 0;
    v0->callbackResult = 0;

    for (v1 = 0; v1 < 7; v1++) {
        v0->substruct[v1] = 0;
    }

    v0->template = *param0;
    v0->template.toPrint.raw = Strbuf_GetData(v0->template.toPrint.strbuf);
    v0->callback = param2;

    Unk_021C04D8 = 0;
    sub_0201DB48(v0);

    if ((param1 != 0xff) && (param1 != 0)) {
        v0->textSpeedBottom--;
        v0->textSpeedTop = 1;
        v0->id = sub_0201D67C(sub_0201D97C, v0, 1);
        return v0->id;
    } else {
        u32 v2;

        v0->textSpeedBottom = 0;
        v0->textSpeedTop = 0;

        v2 = 0;
        sub_0201D9FC(param0->fgColor, param0->bgColor, param0->shadowColor);

        while (v2 < 1024) {
            if (sub_0201D9E8(v0) == 1) {
                break;
            }

            v2++;
        }

        if (param1 != 0xff) {
            sub_0201A954(v0->template.window);
        }

        sub_0201DBD8(v0);
        Heap_FreeToHeap(v0);

        return 8;
    }
}

static void sub_0201D97C(SysTask *param0, void *param1)
{
    TextPrinter *v0;
    int v1;

    if (Unk_021C04D8) {
        return;
    }

    v0 = (TextPrinter *)param1;

    if (v0->callbackResult == 0) {
        v0->callbackParam = 0;
        sub_0201D9FC(v0->template.fgColor, v0->template.bgColor, v0->template.shadowColor);

        v1 = sub_0201D9E8(v0);

        switch (v1) {
        case 0:
            sub_0201A954(v0->template.window);
        case 3:
            if (v0->callback != NULL) {
                v0->callbackResult = (v0->callback)(&(v0->template), v0->callbackParam);
            }
            break;
        case 1:
            sub_0201D6B0(v0->id);
            break;
        }
    } else {
        v0->callbackResult = (v0->callback)(&(v0->template), v0->callbackParam);
    }
}

static int sub_0201D9E8(TextPrinter *param0)
{
    int v0;

    do {
        v0 = sub_02002D18(param0->template.fontID, param0);
    } while (v0 == 2);

    return v0;
}

static u16 Unk_021C0500[256];
static u16 Unk_021C04DE, Unk_021C04DA, Unk_021C04DC;

void sub_0201D9FC(u8 param0, u8 param1, u8 param2)
{
    int v0, v1, v2, v3, v4;
    u32 v5[4];

    v5[0] = 0;
    v5[1] = param0;
    v5[2] = param2;
    v5[3] = param1;

    Unk_021C04DE = param1;
    Unk_021C04DA = param0;
    Unk_021C04DC = param2;

    v4 = 0;

    for (v0 = 0; v0 < 4; v0++) {
        for (v1 = 0; v1 < 4; v1++) {
            for (v2 = 0; v2 < 4; v2++) {
                for (v3 = 0; v3 < 4; v3++) {
                    Unk_021C0500[v4++] = (v5[v3] << 12) | (v5[v2] << 8) | (v5[v1] << 4) | (v5[v0]);
                }
            }
        }
    }
}

void sub_0201DAA0(u32 param0, u32 param1)
{
    u32 v0;
    u16 *v1;
    u16 *v2;

    v1 = (u16 *)param0;
    v2 = (u16 *)param1;

    v0 = (u32)(*v1) >> 8;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)((*v1++) << 24) >> 24;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)(*v1) >> 8;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)((*v1++) << 24) >> 24;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)(*v1) >> 8;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)((*v1++) << 24) >> 24;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)(*v1) >> 8;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)((*v1++) << 24) >> 24;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)(*v1) >> 8;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)((*v1++) << 24) >> 24;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)(*v1) >> 8;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)((*v1++) << 24) >> 24;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)(*v1) >> 8;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)((*v1++) << 24) >> 24;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)(*v1) >> 8;
    *v2++ = Unk_021C0500[v0];

    v0 = (u32)((*v1) << 24) >> 24;
    *v2 = Unk_021C0500[v0];
}

static void sub_0201DB48(TextPrinter *param0)
{
    param0->iconGfx = NULL;
}

static u8 *sub_0201DB50(void)
{
    NNSG2dCharacterData *v0;
    void *v1;
    u8 *v2;

    v2 = Heap_AllocFromHeap(0, 3 * 4 * 4 * 0x20);
    v1 = sub_02006F50(14, 5, 0, &v0, 0);

    MI_CpuCopy32(v0->pRawData, v2, 3 * 4 * 4 * 0x20);
    Heap_FreeToHeap(v1);

    return v2;
}

void sub_0201DB8C(TextPrinter *param0, u16 param1, u16 param2, u16 param3)
{
    Window *v0 = param0->template.window;
    u8 *v1;

    if (param0->iconGfx == NULL) {
        param0->iconGfx = sub_0201DB50();
    }

    v1 = param0->iconGfx;
    v1 = &v1[param3 * 12 * 0x20];

    param1 = (sub_0201C294(v0) - 3) * 8;
    param2 = 0;

    sub_0201ADDC(v0, v1, 0, 0, 24, 32, param1, param2, 24, 32);
}

static void sub_0201DBD8(TextPrinter *param0)
{
    if (param0->iconGfx) {
        Heap_FreeToHeap(param0->iconGfx);
        param0->iconGfx = NULL;
    }
}
