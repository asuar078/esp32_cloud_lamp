#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>

#include <weather_conditions.hpp>

class LEDStrip : public Adafruit_DotStar
{
public:
  // use super constructor
  using Adafruit_DotStar::Adafruit_DotStar;

  void weather_update(WeatherConditions weather);

  void clear_cloud(void);
  void blue_sky(void);
  void white_Clouds(void);
  void overcast(void);
  void sunset(void);
  void rain(void);
  void cloudy(void);

  void error_weather(void);
  void set_color(int color);

  // todo
  void thunderstorm(void);
  void snow(void);
  void misty(void);
  void smokey(void);
  void tornado(void);
  void disco(void);
  void dusty(void);
};

#endif