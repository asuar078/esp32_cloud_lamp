#ifndef __PULL_WEATHER_H__
#define __PULL_WEATHER_H__

#include <Arduino.h>
#include <WiFi.h>
#include <OpenWeatherOneCall.h>

#include <weather_conditions.hpp>
#include <openweather_api_key.hpp>

class PullWeather
{
public:
  PullWeather() = default;

  PullWeather(float latitude, float longitude);

  bool begin();

  int set_lat_lon(float lat, float lon);

  WeatherConditions get_weather_condition();

private:
  // For Latitude and Longitude Location setting if used
  float latitude_ = 27.867867048971497;
  float longitude_ = -82.72259499531707;

  OpenWeatherOneCall owoc_;

  const int weather_units_ = IMPERIAL; //<-----METRIC, IMPERIAL, KELVIN (IMPERIAL is default)

  // Can't get current and historical at the same time
  const int history_ = NULL; //<-----Only required for historical data up to 5 days

  // See manual for excludes, only CURRENT Data allows 1,000,000 calls a month
  const int excludes_ = EXCL_D + EXCL_H + EXCL_M + EXCL_A; //<-----0 Excludes is default
};

#endif // __PULL_WEATHER_H__