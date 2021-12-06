#ifndef __SAVE_DATA_H__
#define __SAVE_DATA_H__

#include <Arduino.h>
#include <Preferences.h>

class SaveData
{
public:
  static constexpr const char *DATA_NAMESPACE = "cloud";
  static constexpr int SENTINAL_VAL = 0x4040;

  static constexpr const char *SENTINAL_KEY = "sentinal";

  static constexpr const char *WIFI_SSID_KEY = "ssid";
  static constexpr const char *WIFI_PASS_KEY = "password";
  static constexpr const char *LATITUDE_KEY = "latitude";
  static constexpr const char *LONGITUDE_KEY = "longitude";

  bool begin();

  bool open_namespace();

  void close_namespace();

  void clear_namespace();

  String get_wifi_ssid();
  String get_wifi_password();

  void set_wifi_ssid(const String &ssid);
  void set_wifi_password(const String &pass);

  float get_latitude();
  float get_longitude();

  void set_latitude(float lat);
  void set_longitude(float lon);

private:
  Preferences preferences;

  void set_defaults();
};

class SaveDataGuard
{
public:
  SaveDataGuard(SaveData &data);

  ~SaveDataGuard();

  bool is_data_opened() const;

private:
  SaveData &data_;
  bool data_opened = false;
};

#endif // __SAVE_DATA_H__