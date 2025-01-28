#include "global.h"
#include "malloc.h"
#include "bg.h"
#include "blit.h"
#include "dma3.h"
#include "event_data.h"
#include "field_weather.h"
#include "graphics.h"
#include "main.h"
#include "map_name_popup.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon_icon.h"
#include "region_map.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "window.h"
#include "config/map_name_popup.h"
#include "constants/songs.h"


static EWRAM_DATA u8 sStartMenuWindowId = 0;
static EWRAM_DATA u8 sMapNamePopupWindowId = 0;
#if MAP_POPUP_GENERATION == GEN_5
static EWRAM_DATA u8 sSecondaryPopupWindowId = 0;
#endif
#if MAP_POPUP_GENERATION == GEN_5
    sSecondaryPopupWindowId = WINDOW_NONE;
#endif

u8 AddMapNamePopUpWindow(void)
{
    if (sMapNamePopupWindowId == WINDOW_NONE)
    {
    #if MAP_POPUP_GENERATION == GEN_5
        sMapNamePopupWindowId = AddWindowParameterized(0, 0, 0, 30, 3, 14, 0x107);
    #else
        sMapNamePopupWindowId = AddWindowParameterized(0, 1, 1, 10, 3, 14, 0x107);
    #endif
    }
    return sMapNamePopupWindowId;
}
// BW map pop-ups
#if MAP_POPUP_GENERATION == GEN_5
u8 AddSecondaryPopUpWindow(void)
{
    if (sSecondaryPopupWindowId == WINDOW_NONE)
        sSecondaryPopupWindowId = AddWindowParameterized(0, 0, 17, 30, 3, 14, 0x161);
    return sSecondaryPopupWindowId;
}
u8 GetSecondaryPopUpWindowId(void)
{
    return sSecondaryPopupWindowId;
}
void RemoveSecondaryPopUpWindow(void)
{
    if (sSecondaryPopupWindowId != WINDOW_NONE)
    {
        RemoveWindow(sSecondaryPopupWindowId);
        sSecondaryPopupWindowId = WINDOW_NONE;
    }
}
void HBlankCB_DoublePopupWindow(void)
{
    u16 offset = gTasks[gPopupTaskId].data[2];
    u16 scanline = REG_VCOUNT;
    if (scanline < 80 || scanline > 160)
    {
        REG_BG0VOFS = offset;
        if(MAP_POPUP_BW_ALPHA_BLEND && !IsWeatherAlphaBlend())
            REG_BLDALPHA = BLDALPHA_BLEND(15, 5);
    }
    else
    {
        REG_BG0VOFS = 512 - offset;
    }
}
#endif
