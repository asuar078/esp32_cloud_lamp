#include "led_strip.hpp"

void LEDStrip::weather_update(WeatherConditions weather)
{
  switch (weather)
  {
  case WeatherConditions::thunderstorm:
    thunderstorm();
    break;
  case WeatherConditions::drizzle:
    rain();
    break;
  case WeatherConditions::rain:
    rain();
    break;
  case WeatherConditions::snow:
    snow();
    break;
  case WeatherConditions::mist:
    misty();
    break;
  case WeatherConditions::smoke:
    smokey();
    break;
  case WeatherConditions::haze:
    overcast();
    break;
  case WeatherConditions::dust_whirls:
    sunset();
    break;
  case WeatherConditions::fog:
    misty();
    break;
  case WeatherConditions::sand:
  case WeatherConditions::dust:
  case WeatherConditions::ash:
    sunset();
    break;
  case WeatherConditions::squall:
    tornado();
    break;
  case WeatherConditions::tornado:
    tornado();
    break;
  case WeatherConditions::clear:
    blue_sky();
    break;
  case WeatherConditions::clouds:
    cloudy();
    break;
  case WeatherConditions::unknown:
    error_weather();
    break;
  }
}

void LEDStrip::clear_cloud(void)
{
  for (int i = 0; i < numPixels(); i++)
  {                            // for all the LEDs
    setPixelColor(i, 0, 0, 0); // turn off in cloud one
  }
  show(); // show what was set in cloud one
}

void LEDStrip::blue_sky(void)
{
  for (int i = 0; i < numPixels(); i++)
  {                                 // for all of the LEDs
    setPixelColor(i, 140, 70, 255); // set LEDs a sky blue color in cloud one
  }
  show();
}

void LEDStrip::white_Clouds(void)
{
  for (int i = 0; i < numPixels(); i++)
  {                                  // for all of the LEDs
    setPixelColor(i, 200, 200, 200); // set LEDs white-ish in cloud one
  }
  show();
}

void LEDStrip::overcast(void)
{
  for (int i = 0; i < numPixels(); i++)
  {                               // for all of the LEDs
    setPixelColor(i, 80, 80, 80); // set LEDs grey-ish in cloud one
  }
  show();
}

void LEDStrip::sunset(void)
{
  int i;
  for (i = 0; i < 15; i++)
  {                                // for the first 100 LEDs
    setPixelColor(i, 10, 255, 10); // set LEDs red-ish in cloud one
  }
  for (i = 15; i < 30; i++)
  {                                // for LEDs 101 to 150
    setPixelColor(i, 100, 235, 0); // set LEDs orange-red in cloud one
  }
  for (i = 30; i < 45; i++)
  {                                // for LEDs 151 to 250
    setPixelColor(i, 100, 235, 0); // set LEDs red-orage in cloud one
  }
  for (i = 45; i < 60; i++)
  { // for LEDs 251 to 260
    // setPixelColor(i, 0, 0, 255);  //set LEDs red-orage in cloud one
    setPixelColor(i, 10, 255, 10); // set LEDs red-ish in cloud one
  }
  show(); // show all the colors that were set in cloud one
}

void LEDStrip::rain(void)
{
  int ran = random(0, 2);
  int i;
  if (ran == 0)
  {
    for (i = 0; i < 10; i++)
    {
      setPixelColor(i, 30, 30, 30);
    }
    for (i = 10; i < 20; i++)
    {
      setPixelColor(i, 10, 10, 10);
    }
    for (i = 20; i < 30; i++)
    {
      setPixelColor(i, 255, 255, 255);
    }
    for (i = 30; i < 50; i++)
    {
      setPixelColor(i, 80, 80, 80);
    }
    for (i = 50; i < 60; i++)
    {
      setPixelColor(i, 100, 100, 100);
    }
  }
  if (ran == 1)
  {
    for (i = 0; i < 10; i++)
    {
      setPixelColor(i, 30, 30, 30);
    }
    for (i = 10; i < 20; i++)
    {
      setPixelColor(i, 80, 80, 80);
    }
    for (i = 20; i < 30; i++)
    {
      setPixelColor(i, 10, 10, 10);
    }
    for (i = 30; i < 50; i++)
    {
      setPixelColor(i, 80, 80, 80);
    }
    for (i = 50; i < 60; i++)
    {
      setPixelColor(i, 255, 255, 255);
    }
  }
  if (ran == 2)
  {
    for (i = 0; i < 10; i++)
    {
      setPixelColor(i, 255, 255, 255);
    }
    for (i = 10; i < 20; i++)
    {
      setPixelColor(i, 100, 100, 100);
    }
    for (i = 20; i < 30; i++)
    {
      setPixelColor(i, 255, 255, 255);
    }
    for (i = 30; i < 50; i++)
    {
      setPixelColor(i, 80, 80, 80);
    }
    for (i = 50; i < 60; i++)
    {
      setPixelColor(i, 10, 10, 10);
    }
  }

  show();
}

void LEDStrip::cloudy(void)
{
  int i;
  for (i = 0; i < 15; i++)
  {
    setPixelColor(i, 200, 200, 200); // set LEDs white-ish in cloud one
  }
  for (i = 15; i < 30; i++)
  {
    setPixelColor(i, 140, 70, 255); // set LEDs a sky blue color in cloud one
  }
  for (i = 30; i < 45; i++)
  {
    setPixelColor(i, 200, 200, 200); // set LEDs white-ish in cloud one
  }
  for (i = 45; i < 60; i++)
  {
    setPixelColor(i, 140, 70, 255); // set LEDs a sky blue color in cloud one
  }
  show();
}

void LEDStrip::error_weather(void)
{
  int i;
  for (i = 0; i < numPixels(); i++)
  {
    setPixelColor(i, 10, 255, 10);
  }
  show(); // show all the colors that were set in cloud one
}

void LEDStrip::set_color(int color)
{
  byte red = ((color >> 16) & 0xFF);  // Extract the RR byte
  byte green = ((color >> 8) & 0xFF); // Extract the GG byte
  byte blue = ((color)&0xFF);         // Extract the BB byte

  // todo: remove
  // String colors = "red " + (String)red + " green " + (String)green + " blue " + (String)blue;
  // Serial.println(colors);

  for (int i = 0; i < numPixels(); i++)
  { // for all the LEDs
    setPixelColor(i, green, red, blue);
  }

  show();
}

void LEDStrip::thunderstorm(void)
{
  int i;
  for (i = 0; i < numPixels(); i++)
  {                               // for all of the LEDs
    setPixelColor(i, 80, 80, 80); // set LEDs grey-ish in cloud one
  }
  show();

  int ran = random(0, 6);
  int start_idx = 0;
  int stop_idx = 0;
  bool do_flash = false;

  // flash
  if (ran == 0)
  {
    start_idx = 0;
    stop_idx = 15;
    do_flash = true;
  }

  if (ran == 2)
  {
    start_idx = 15;
    stop_idx = 30;
    do_flash = true;
  }

  if (ran == 4)
  {
    start_idx = 30;
    stop_idx = 45;
    do_flash = true;
  }

  if (do_flash)
  {

    for (i = start_idx; i < stop_idx; i++)
    {
      setPixelColor(i, 0, 0, 0);
    }
    show();
    delay(800);

    for (i = start_idx; i < stop_idx; i++)
    {
      setPixelColor(i, 80, 80, 80);
    }
    show();
  }
}

void LEDStrip::snow(void)
{
  for (int i = 0; i < numPixels(); i++)
  {
    setPixelColor(i, 127, 161, 245);
  }
  show();
}

void LEDStrip::misty(void)
{
  for (int i = 0; i < numPixels(); i++)
  {
    setPixelColor(i, 219, 219, 219);
  }
  show();
}

void LEDStrip::smokey(void)
{
  for (int i = 0; i < numPixels(); i++)
  {
    setPixelColor(i, 130, 130, 130);
  }
  show();
}

void LEDStrip::tornado(void)
{
  for (int i = 0; i < numPixels(); i++)
  {
    setPixelColor(i, 130, 130, 130);
  }
  show();
}

void LEDStrip::disco(void)
{
}

void LEDStrip::dusty(void)
{
  for (int i = 0; i < numPixels(); i++)
  {
    setPixelColor(i, 74, 74, 74);
  }
  show();
}
