#include "global.h"
#include "strings.h"
#include "constants/songs.h"
#include "constants/weather.h"
#include "constants/rgb.h"
#include "util.h"
#include "event_object_movement.h"
#include "field_weather.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "random.h"
#include "script.h"
#include "start_menu.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "trig.h"
#include "gpu_regs.h"
#include "field_camera.h"
#include "rtc.h"
#include "string_util.h"
#include "text.h"

const u8 gText_AM[] = _("AM");
const u8 gText_PM[] = _("PM");
bool8 IsWeatherAlphaBlend(void)
{
    return gWeatherPtr->currWeather == WEATHER_FOG_HORIZONTAL || 
           gWeatherPtr->currWeather == WEATHER_FOG_DIAGONAL   ||
           gWeatherPtr->currWeather == WEATHER_UNDERWATER_BUBBLES ||
           gWeatherPtr->currWeather == WEATHER_UNDERWATER;
};
void FormatDecimalTimeWithoutSeconds(u8 *txtPtr, s8 hour, s8 minute, bool8 is24Hour)
{
    if (is24Hour)
    {
        txtPtr = ConvertIntToDecimalStringN(txtPtr, hour, STR_CONV_MODE_LEADING_ZEROS, 2);
        *txtPtr++ = CHAR_COLON;
        txtPtr = ConvertIntToDecimalStringN(txtPtr, minute, STR_CONV_MODE_LEADING_ZEROS, 2);
    }
    else
    {
        if (hour == 0)
            txtPtr = ConvertIntToDecimalStringN(txtPtr, 12, STR_CONV_MODE_LEADING_ZEROS, 2);
        else if (hour < 13)
            txtPtr = ConvertIntToDecimalStringN(txtPtr, hour, STR_CONV_MODE_LEADING_ZEROS, 2);
        else
            txtPtr = ConvertIntToDecimalStringN(txtPtr, hour - 12, STR_CONV_MODE_LEADING_ZEROS, 2);
        *txtPtr++ = CHAR_COLON;
        txtPtr = ConvertIntToDecimalStringN(txtPtr, minute, STR_CONV_MODE_LEADING_ZEROS, 2);
        txtPtr = StringAppend(txtPtr, gText_Space);
        if (hour < 12)
            txtPtr = StringAppend(txtPtr, gText_AM);
        else
            txtPtr = StringAppend(txtPtr, gText_PM);
    }
    *txtPtr++ = EOS;
    *txtPtr = EOS;
};
