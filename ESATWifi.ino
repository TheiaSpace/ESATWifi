#include "ESP8266WiFi.h"
#include <EEPROM.h>

int wifiStatus = 0;

struct strConfig
{
  String ssid;
  String password;
  String host;
  int port;
  int id;
} config;

WiFiClient client;

String toHex(int i, int L)
{
  String ch = String(i, HEX);
  while (ch.length() < L)
  {
    ch = "0" + ch;
  }
  return ch.substring (0, L);
}

boolean wifiConnect()
{
  WiFi.begin(config.ssid.c_str(), config.password.c_str());
  //timeout
  int t1 = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((millis() - t1) > 5000);
    {
      return false;
    }
    delay(500);
  }
  delay(100);  // ESP.reset();
  return true;
}

void WriteStringToEEPROM(int beginaddress, String string)
{
  char charBuf[string.length() + 1];
  string.toCharArray(charBuf, string.length() + 1);
  for (int t = 0; t < sizeof(charBuf); t++)
  {
    EEPROM.write(beginaddress + t, charBuf[t]);
  }
}

String ReadStringFromEEPROM(int beginaddress)
{
  byte counter = 0;
  char rChar;
  String retString = "";
  while (1)
  {
    rChar = EEPROM.read(beginaddress + counter);
    if (rChar == 0)
      break;
    if (counter > 256)
      break;
    counter++;
    retString.concat(rChar);
  }
  return retString;
}

void WriteConfig(String cfg)
{
  Serial.println("#s:0");
  for (int i = 0; i < 512; i++)
  {
    EEPROM.write(i, 0);
  }
  delay(10);
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');
  WriteStringToEEPROM(16, cfg);
  EEPROM.commit();
  //ESP.reset();
}

void processCMD(String cmd)
{
  int i1 = cmd.indexOf(":");
  String key = cmd.substring(1, i1);
  String value = cmd.substring(i1 + 1, cmd.length() + 1);
  // Serial.println("#key:" + key);
  // Serial.println("#value:" + value);
  if (key == "config")
  {
    int i1;
    String temp;
    String remainder = value;
    i1 = remainder.indexOf(",");
    config.id = remainder.substring(0, i1).toInt();
    remainder = remainder.substring(i1 + 1, remainder.length());
    i1 = remainder.indexOf(",");
    config.ssid = remainder.substring (0, i1);
    remainder = remainder.substring(i1 + 1, remainder.length());
    i1 = remainder.indexOf(",");
    config.password = remainder.substring(0, i1);
    remainder = remainder.substring(i1 + 1, remainder.length());
    i1 = remainder.indexOf(",");
    config.host = remainder.substring(0, i1);
    remainder = remainder.substring(i1 + 1, remainder.length());
    // i1 = remainder.indexOf(",");
    config.port = remainder.toInt();
    WriteConfig(cmd);
    delay(50);
    if (wifiConnect())
    {
      Serial.println ("#s:2");
    }
    else
    {
      Serial.println("#s:1");
    }  // Wifi connected
  }
  else if (key == "reset")
  {
    // ESP.reset();
  }
  while (Serial.available () > 0)
  {
    Serial.read ();
  }
  // Serial.println("#ACK:" + cmd.substring(1, cmd.length() + 1));
  Serial.flush ();
}

boolean ReadConfig()
{
  // Serial.println("#Reading Configuration");
  if (EEPROM.read(0) == 'C'
      && EEPROM.read(1) == 'F'
      && EEPROM.read(2) == 'G')
  {
    Serial.println("#s:1"); // configuration found
    String cfg = ReadStringFromEEPROM(16);
    // Serial.println(cfg);
    processCMD(cfg);
    return true;
  }
  else
  {
    Serial.println("#s:0"); // configuration not found
    return false;
  }
}


void setup()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  EEPROM.begin(512);
  Serial.begin(115200); // (9600);
  // Serial.println();
  ReadConfig();
  delay(10);
}

int value = 0;

void loop()
{
  if (WiFi.status() == 3)
  {
    wifiStatus = 1;
  }
  // else
  // {
  //   Serial.println("#server_nok");
  //   delay(5000);
  // }
  // Use WiFiClient class to create TCP connections
  if ((wifiStatus > 0) && !client.connected())
  {
    // Serial.println("#connecting_to_server:" + String(config.host) + ":" + String(config.port));
    // Serial.print(".");
    if (!client.connect(config.host.c_str(), config.port))
    {
      Serial.println("#s:2"); // connection to server failed
      delay(1000);
    }
    else
    {
      Serial.println("#s:3"); //connected to server
      client.print("{\"type\":\"login\",\"id\":\""
                   + String (config.id) +
                   "\"}");
    }
  }
  delay (10);
  if (client.available())
  {
    String cmd = "";
    while (client.available())
    {
      char c = client.read();
      cmd += String(c);
    }
    Serial.println("@" + cmd);
  }
  if (Serial.available())
  {
    String cmd = "";
    cmd = Serial.readStringUntil('\r');
    String identifier = cmd.substring(0, 1);
    if (identifier == "@")
    {
      String msg =
        "{\"type\":\"onPacket\",\"data\":\""
        + toHex (config.id, 1)
        + cmd.substring(1, cmd.length() + 1)
        + "\"}";
      client.print(msg);
    }
    else if (identifier == "&")
    {
      String msg =
        "{\"type\":\"log\",\"data\":\""
        + toHex (config.id, 1)
        + cmd.substring (1, cmd.length() + 1)
        + "\"}";
      client.print(msg);
    }
    else if (identifier == "#")
    {
      processCMD(cmd);
    }
  }
}
