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
  PullWeather(float latitude, float longitude);

  bool begin();

  WeatherConditions get_weather_condition();

private:
  // For Latitude and Longitude Location setting if used
  float latitude_;
  float longitude_;

  OpenWeatherOneCall owoc_;

  const int weather_units_ = IMPERIAL; //<-----METRIC, IMPERIAL, KELVIN (IMPERIAL is default)

  // Can't get current and historical at the same time
  const int history_ = NULL; //<-----Only required for historical data up to 5 days

  // See manual for excludes, only CURRENT Data allows 1,000,000 calls a month
  const int excludes_ = EXCL_D + EXCL_H + EXCL_M + EXCL_A; //<-----0 Excludes is default
};

#endif // __PULL_WEATHER_H__