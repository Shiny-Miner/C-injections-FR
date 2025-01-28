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

const u8 gText_AM[] = _("AM");
const u8 gText_PM[] = _("PM");
bool8 IsWeatherAlphaBlend(void)
{
    return gWeatherPtr->currWeather == WEATHER_FOG_HORIZONTAL || 
           gWeatherPtr->currWeather == WEATHER_FOG_DIAGONAL   ||
           gWeatherPtr->currWeather == WEATHER_UNDERWATER_BUBBLES ||
           gWeatherPtr->currWeather == WEATHER_UNDERWATER;
}
