//MAC Address:  50:02:91:DC:E7:3E
#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t broadcastAddress[] = {0x50,0x02,0x91,0xDC,0xE7,0x3E };
#include <SoftwareSerial.h>
const int Rx = D3;
const int Tx = D4;
SoftwareSerial s (Rx, Tx);
struct sData{
  bool b1;
  bool b2;
  bool b3;
// compasee
  float f1;
  //temp
  float f2;  
  int i;
  //bool running;
};

sData data;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
}
// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(sData));
  s.print(encode(data.b1, data.b2, data.b3, data.f1, data.f2, data.i));
  //Serial.println(temp);
  //s.println(temp);
}
 
void setup() {
  Serial.begin(115200);
  s.begin(57600);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}

String encode(bool b1, bool b2, bool b3, float f, float temperature, int distance)
{
  String return_value = "";
  if(b1 == true)
  {
    return_value += '1'; 
  }
  else{
    return_value += '0';
  }

  if(b2 == true)
  {
    return_value += '1'; 
  }
  else{
    return_value += '0';
  }

  if(b3 == true)
  {
    return_value += '1'; 
  }
  else{
    return_value += '0';
  }

  String temp = String(f,2);
  return_value += temp;


  temp = String(temperature, 1);
  return_value += temp;
/*
  if(running)
  {
return_value += '1';
  }
  else
  {
    return_value += '0';
  }
*/  
  return_value += String(distance);
  return_value += '\n';
  

  Serial.println(return_value);
  return return_value;
  
}
