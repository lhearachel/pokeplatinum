#ifndef POKEPLATINUM_UNK_0202DA40_H
#define POKEPLATINUM_UNK_0202DA40_H

#include "struct_decls/struct_0202DA40_decl.h"

#include "pokemon.h"
#include "savedata.h"

GlobalTrade *SaveData_GetGlobalTrade(SaveData *saveData);
int GlobalTrade_SaveSize(void);
void GlobalTrade_Init(GlobalTrade *param0);
int sub_0202DA60(GlobalTrade *param0);
void sub_0202DA68(GlobalTrade *param0, int param1);
void sub_0202DA70(GlobalTrade *param0, Pokemon *param1);
void sub_0202DA7C(GlobalTrade *param0, Pokemon *param1, int param2);
u32 sub_0202DA8C(GlobalTrade *param0);
void sub_0202DA94(GlobalTrade *param0, u32 param1);
u32 sub_0202DA9C(GlobalTrade *param0);
void sub_0202DAA4(GlobalTrade *param0, u32 param1);
int sub_0202DAAC(GlobalTrade *param0);

#endif // POKEPLATINUM_UNK_0202DA40_H
