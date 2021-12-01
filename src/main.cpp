#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>

#include <pull_weather.hpp>
#include <weather_conditions.hpp>

#include <wifi_credentials.hpp>

const float latitude = 27.867867048971497;
const float longitude = -82.72259499531707;

PullWeather pull_weather(latitude, longitude);

namespace
{
  bool update_in_progress = false;
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
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

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

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

  pull_weather.begin();

  pull_weather.get_weather_condition();
}

void loop()
{
  // put your main code here, to run repeatedly:
  ArduinoOTA.handle();
}