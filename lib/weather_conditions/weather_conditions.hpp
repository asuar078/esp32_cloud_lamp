#ifndef __WEATHER_CONDITIONS_H__
#define __WEATHER_CONDITIONS_H__

#include <cstdint>
#include <array>

/**
 * @brief List of weather conditions from
 * https://openweathermap.org/weather-conditions
 * Not all possible conditions are listed just main ones
 */
enum class WeatherConditions
{
  thunderstorm,
  drizzle,
  rain,
  snow,
  mist,
  smoke,
  haze,
  dust_whirls,
  fog,
  sand,
  dust,
  ash,
  squall,
  tornado,
  clear,
  clouds,

  // must be last entry
  unknown
};

struct IdRange
{
  uint32_t min;
  uint32_t max;
};

struct WeatherIdRange
{
  IdRange id_range;
  WeatherConditions weather_condition;
};

using WeatherConditionsArray = std::array<WeatherIdRange, static_cast<std::size_t>(WeatherConditions::unknown)>;

static constexpr WeatherConditionsArray weather_array = {
    WeatherIdRange{IdRange{200, 232}, WeatherConditions::thunderstorm},
    WeatherIdRange{IdRange{300, 321}, WeatherConditions::drizzle},
    WeatherIdRange{IdRange{500, 531}, WeatherConditions::rain},
    WeatherIdRange{IdRange{600, 622}, WeatherConditions::snow},
    WeatherIdRange{IdRange{701, 701}, WeatherConditions::mist},
    WeatherIdRange{IdRange{711, 711}, WeatherConditions::smoke},
    WeatherIdRange{IdRange{721, 721}, WeatherConditions::haze},
    WeatherIdRange{IdRange{731, 731}, WeatherConditions::dust_whirls},
    WeatherIdRange{IdRange{741, 741}, WeatherConditions::fog},
    WeatherIdRange{IdRange{751, 751}, WeatherConditions::sand},
    WeatherIdRange{IdRange{761, 761}, WeatherConditions::dust},
    WeatherIdRange{IdRange{762, 762}, WeatherConditions::ash},
    WeatherIdRange{IdRange{771, 771}, WeatherConditions::squall},
    WeatherIdRange{IdRange{781, 781}, WeatherConditions::tornado},
    WeatherIdRange{IdRange{800, 800}, WeatherConditions::clear},
    WeatherIdRange{IdRange{801, 804}, WeatherConditions::clouds},
};

WeatherConditions weather_id_to_condition(float id);

#endif // __WEATHER_CONDITIONS_H__