#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>
#include <Preferences.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>

#include <pull_weather.hpp>
#include <weather_conditions.hpp>
#include <save_data.hpp>

#include <led_strip.hpp>
#include <wifi_credentials.hpp>

#include <cctype>

namespace
{

  constexpr uint16_t NUMPIXELS = 60; // Number of LEDs in strip
  constexpr uint8_t DATAPIN = 17;
  constexpr uint8_t CLOCKPIN = 16;

  LEDStrip strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

  SaveData save_data;

  WebServer server(80);

  PullWeather pull_weather;
  WeatherConditions weather_condition = WeatherConditions::unknown;

  constexpr const auto mode_gpio = GPIO_NUM_4;

  bool update_in_progress = false;
  uint32_t previous_weather_millis = 0;
  uint32_t previous_led_millis = 0;

  /* pull weather data every 60 seconds */
  constexpr const uint32_t weather_interval = 60000;
  /* update led strip every 2 seconds */
  constexpr const uint32_t led_interval = 2000;
}

void setup()
{
  Serial.begin(115200);
  pinMode(mode_gpio, INPUT_PULLUP);
  delay(1000);

  strip.begin();

  save_data.begin();

  {
    SaveDataGuard guard(save_data);

    Serial.println();
    Serial.print("Saved latitude: ");
    Serial.println(save_data.get_latitude());
    Serial.print("Saved longitude: ");
    Serial.println(save_data.get_longitude());

    Serial.println();
    Serial.print("Saved SSID: ");
    Serial.println(save_data.get_wifi_ssid());
    Serial.print("Saved password: ");
    Serial.println(save_data.get_wifi_password());
  }

  if (digitalRead(mode_gpio) == LOW)
  {
    SaveDataGuard guard(save_data);

    // station mode
    Serial.println();
    Serial.println("WiFi Station Mode");
    Serial.print("Connecting to: ");
    // Serial.println(WIFI_SSID);
    Serial.println(save_data.get_wifi_ssid());

    // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.begin(save_data.get_wifi_ssid().c_str(), save_data.get_wifi_password().c_str());

    while (WiFi.status() != WL_CONNECTED)
    {
      strip.set_color(0xcf1f5c);
      delay(500);
      strip.set_color(0);
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    // access point mode
    Serial.println();
    Serial.println("WiFi Access point Mode");
    Serial.print("Creating access point: ");
    Serial.println(AP_WIFI_SSID);

    // WiFi.softAP(AP_WIFI_SSID, AP_WIFI_PASSWORD);
    WiFi.softAP(AP_WIFI_SSID);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    delay(100);
  }

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);

  ArduinoOTA
      .onStart([]()
               {
                  update_in_progress = true;
                  String type;
                  if (ArduinoOTA.getCommand() == U_FLASH) {
                    type = "sketch";
                  }
                  else {// U_SPIFFS
                    type = "filesystem";
                  }

                  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                  Serial.println("Start updating " + type); })
      .onEnd([]()
             {
                update_in_progress = false;
                Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
                else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
                else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
                else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
                else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();

  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp32.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("cloudlamp"))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  {
    SaveDataGuard guard(save_data);

    // start pull weather api service
    pull_weather.begin();

    pull_weather.set_lat_lon(save_data.get_latitude(), save_data.get_longitude());
  }

  // setup server routing
  server.on(F("/status/wifi"), HTTP_GET, []()
            { server.send(200, "text/plain", "mode:" + String(WiFi.getMode()) + "/status:" + String(WiFi.status())); });

  server.on(UriBraces("/lat={}/lon={}"), HTTP_POST, [&]()
            {
              String lat = server.pathArg(0);
              String lon = server.pathArg(1);
              Serial.print("Received lat: ");
              Serial.println(lat);
              Serial.print("Received lon: ");
              Serial.println(lon);

              SaveDataGuard guard(save_data);
              save_data.set_latitude(lat.toFloat());
              save_data.set_longitude(lon.toFloat());

              pull_weather.set_lat_lon(lat.toFloat(), lon.toFloat());

              server.send(200, "text/plain", "lat: " + lat + ", lon: " + lon); });

  server.on(UriBraces("/ssid={}/pass={}"), HTTP_POST, [&]()
            {
              String ssid = server.pathArg(0);
              String pass = server.pathArg(1);

              ssid.replace("%20", " ");
              pass.replace("%20", " ");

              Serial.print("Received ssid: ");
              Serial.println(ssid);
              Serial.print("Received password: ");
              Serial.println(pass);

              SaveDataGuard guard(save_data);
              save_data.set_wifi_ssid(ssid);
              save_data.set_wifi_password(pass);

              server.send(200, "text/plain", "ssid: " + ssid + ", pass: " + pass); });

  server.begin();
  Serial.println("HTTP server started");

  if (WiFi.getMode() != WIFI_MODE_STA && WiFi.status() != WL_CONNECTED)
  {
    weather_condition = pull_weather.get_weather_condition();
    return;
  }

  strip.weather_update(weather_condition);
}

void loop()
{
  server.handleClient();

  ArduinoOTA.handle();

  if (update_in_progress)
  {
    return;
  }

  /* from this point on if not in station mode just return */
  if (WiFi.getMode() != WIFI_MODE_STA)
  {
    return;
  }

  /* check for connection status */
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Not connected to network");
    Serial.println("Reconnecting to WiFi...");

    WiFi.disconnect();
    WiFi.reconnect();

    /* still not connected wait another 30 seconds to try again */
    if (WiFi.status() != WL_CONNECTED)
    {
      delay(30000);
    }

    return;
  }

  const auto current_millis = millis();

  // update led strip
  if (current_millis - previous_led_millis >= led_interval)
  {
    strip.weather_update(weather_condition);
    previous_led_millis = current_millis;
  }

  if (current_millis - previous_weather_millis >= weather_interval)
  {
    weather_condition = pull_weather.get_weather_condition();
    previous_weather_millis = current_millis;
  }

  delay(2); // allow the cpu to switch to other tasks
}