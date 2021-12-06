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

#include <wifi_credentials.hpp>

#include <cctype>

namespace
{

  SaveData save_data;

  WebServer server(80);

  PullWeather pull_weather;

  constexpr const auto mode_gpio = GPIO_NUM_4;

  bool update_in_progress = false;
  uint32_t previous_millis = 0;

  /* pull weather data every 15 seconds */
  constexpr const uint32_t weather_interval = 15000;
}

void setup()
{
  Serial.begin(115200);

  save_data.begin();

  pinMode(mode_gpio, INPUT_PULLUP);
  delay(100);

  if (digitalRead(mode_gpio) == LOW)
  {
    // station mode
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

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    // access point mode
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

  // start pull weather api service
  pull_weather.begin();

  // setup server routing
  server.on(UriBraces("/lat={}/lon={}"), HTTP_POST, []()
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

              server.send(200, "text/plain", "lat: " + lat + ", " + "lon: " + lon); });

  server.begin();
  Serial.println("HTTP server started");
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

  // const auto current_millis = millis();
  // if (current_millis - previous_millis >= weather_interval)
  // {
  //   pull_weather.get_weather_condition();
  //   previous_millis = current_millis;
  // }

  delay(2); // allow the cpu to switch to other tasks
}