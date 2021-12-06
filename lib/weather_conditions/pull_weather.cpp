#include "pull_weather.hpp"

PullWeather::PullWeather(float latitude, float longitude)
    : latitude_(latitude),
      longitude_(longitude)
{
}

bool PullWeather::begin()
{
  owoc_.setOpenWeatherKey(const_cast<char *>(OPENWEATHER_API_KEY));

  // Here we use the Lat and Lon for Pensacola, Florida (Using the predefined values)
  owoc_.setLatLon(latitude_, longitude_);

  // Third set any EXCLUDES if required (Here we are not using any
  owoc_.setExcl(excludes_);

  // Set History if you want historical weather other wise NULL
  owoc_.setHistory(history_);

  // Set UNITS of MEASURE otherwise default is IMPERIAL
  owoc_.setUnits(weather_units_);
}

int PullWeather::set_lat_lon(float lat, float lon)
{
  return owoc_.setLatLon(lat, lon);
}

WeatherConditions PullWeather::get_weather_condition()
{
  // Now call the weather. Please notice no arguments are required in this call
  auto ret = owoc_.parseWeather();

  if (ret != 0)
  {
    return WeatherConditions::unknown;
  }

  // Now display some information, note the pointer requirement for current and alert, this is NEW for v3.0.0
  // Location info is available for ALL modes (History/Current)
  printf("\nLocation: % s, % s % s\n", owoc_.location.CITY, owoc_.location.STATE, owoc_.location.COUNTRY);

  printf("Current weather ID: %.0f\n", owoc_.current->id);
  printf("Current weather: %s\n", owoc_.current->main);
  printf("Current summary: %s\n", owoc_.current->summary);
  // printf("Current Temp : % .0f\n", owoc_.current->temperature);
  // printf("Current Humidity : % .0f\n", owoc_.current->humidity);
  printf("Current condition: %d\n", static_cast<int>(weather_id_to_condition(owoc_.current->id)));

  return weather_id_to_condition(owoc_.current->id);
}
