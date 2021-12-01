#include <weather_conditions.hpp>

WeatherConditions weather_id_to_condition(float id)
{
  for (const auto &condition : weather_array)
  {
    if (condition.id_range.min <= id && condition.id_range.max >= id)
    {
      return condition.weather_condition;
    }
  }

  return WeatherConditions::unknown;
}