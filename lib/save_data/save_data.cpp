#include "save_data.hpp"

bool SaveData::begin()
{
  SaveDataGuard guard(*this);

  if (!guard.is_data_opened())
  {
    return false;
  }

  // check sentinal value
  auto sentinal = preferences.getInt(SENTINAL_KEY);
  if (sentinal != SENTINAL_VAL)
  {
    // set to default values
    set_defaults();
  }

  return true;
}

void SaveData::set_defaults()
{

  Serial.println("Setting saved data to default values");

  preferences.clear();
  delay(100);

  preferences.putFloat(LATITUDE_KEY, 27.867867048971497);
  preferences.putFloat(LONGITUDE_KEY, -82.72259499531707);

  preferences.putString(WIFI_SSID_KEY, WIFI_SSID_KEY);
  preferences.putString(WIFI_PASS_KEY, WIFI_PASS_KEY);

  preferences.putInt(SENTINAL_KEY, SENTINAL_VAL);
}

bool SaveData::open_namespace()
{
  return preferences.begin(DATA_NAMESPACE, false);
}

void SaveData::close_namespace()
{
  preferences.end();
}

void SaveData::clear_namespace()
{
  preferences.clear();
}

String SaveData::get_wifi_ssid()
{
  return preferences.getString(WIFI_SSID_KEY);
}

String SaveData::get_wifi_password()
{
  return preferences.getString(WIFI_PASS_KEY);
}

void SaveData::set_wifi_ssid(const String &ssid)
{
  preferences.putString(WIFI_SSID_KEY, ssid);
}

void SaveData::set_wifi_password(const String &pass)
{
  preferences.putString(WIFI_PASS_KEY, pass);
}

float SaveData::get_latitude()
{
  return preferences.getFloat(LATITUDE_KEY);
}

float SaveData::get_longitude()
{
  return preferences.getFloat(LONGITUDE_KEY);
}

void SaveData::set_latitude(float lat)
{
  preferences.putFloat(LATITUDE_KEY, lat);
}

void SaveData::set_longitude(float lon)
{
  preferences.putFloat(LONGITUDE_KEY, lon);
}

SaveDataGuard::SaveDataGuard(SaveData &data)
    : data_(data)
{
  data_opened = data.open_namespace();
}

SaveDataGuard::~SaveDataGuard()
{
  data_.close_namespace();
}

bool SaveDataGuard::is_data_opened() const
{
  return data_opened;
}
