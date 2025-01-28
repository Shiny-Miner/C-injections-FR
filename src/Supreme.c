#include "global.h"
#include "strings.h"
#include "field_weather.h"

const u8 gText_AM[] = _("AM");
const u8 gText_PM[] = _("PM");
bool8 IsWeatherAlphaBlend(void)
{
    return gWeatherPtr->currWeather == WEATHER_FOG_HORIZONTAL || 
           gWeatherPtr->currWeather == WEATHER_FOG_DIAGONAL   ||
           gWeatherPtr->currWeather == WEATHER_UNDERWATER_BUBBLES ||
           gWeatherPtr->currWeather == WEATHER_UNDERWATER;
}
