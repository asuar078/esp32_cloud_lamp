#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>

#include <weather_conditions.hpp>

class LEDStrip
{
private:
  Adafruit_DotStar strip;

public:
  LEDStrip(Adafruit_DotStar &dotstar);
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
  void disco(void);
};

#endif