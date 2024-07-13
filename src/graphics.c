#include "global.h"

//Main Menu
const u16 sMainMenuTextPal[] = {RGB(0, 0, 0), RGB(30, 30, 30), RGB(17, 17, 17), RGB(7, 20, 31), RGB(7, 13, 20)};
const u16 sMainMenuTextFemalePal[] = {RGB(31, 9, 11), RGB(21, 8, 8)};
const u16 gPalMainMenuBG[] = INCBIN_U16("graphics/main_menu/bg.gbapal");
const u16 gPalMainMenuNoSel[] = INCBIN_U16("graphics/main_menu/no_sel.gbapal");
const u16 gPalMainMenuSel[] = INCBIN_U16("graphics/main_menu/sel.gbapal");
const u32 gTilesMainMenuBG1[] = INCBIN_U32("graphics/main_menu/tiles_bg1.4bpp.lz");
const u32 gTilesMainMenuBG2[] = INCBIN_U32("graphics/main_menu/tiles_bg2.4bpp.lz");
const u32 gMapMainMenuBG2[] = INCBIN_U32("graphics/main_menu/bg.bin.lz");
const u32 gMapMainMenuNewGame[] = INCBIN_U32("graphics/main_menu/new_game.bin.lz");
const u32 gMapMainMenuContinue[] = INCBIN_U32("graphics/main_menu/continue.bin.lz");
