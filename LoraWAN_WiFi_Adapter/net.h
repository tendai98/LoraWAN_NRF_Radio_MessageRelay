#include <WiFi.h>

#define SSID_ID "LoraRadio-0001"
#define SSID_PS "1234567890"

void init_network(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID_ID, SSID_PS);
}
