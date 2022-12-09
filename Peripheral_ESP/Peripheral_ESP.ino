//MAC Address:  38:2B:78:03:83:8B
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
uint8_t broadcastAddress[] = {0x38,0x2B,0x78,0x03,0x83,0x8B};
//const int Rx = 2;
//const int Tx = 3;
const int Rx = D1;
const int Tx = D2;
SoftwareSerial s(Rx, Tx);
int count = 0;
typedef struct CameraData{
  uint8_t by[4];
}CameraData;
CameraData Ctemp;
  String sensorData;
uint8_t startByte;
void setup() {
  //Wire.begin();
  /*
  Wire.begin(0x08);
  Wire.pins(D1, D2);
  Wire.onRequest(requestEvent);
  //Wire.onReceive(receiveEvent);
  */
  s.begin(57600);
  Serial.begin(921600);
  //while (!Serial)
  //  ;
  while (!s)
    ;
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  esp_now_register_recv_cb(OnDataRecv);

}
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    //Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }

}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&startByte, incomingData, sizeof(startByte));
  //Serial.println(receive.str);
  s.write(startByte);
  Serial.println(startByte);
  Serial.println("received");
  
}
void receiveEvent(int i) {
  while(Wire.available() > 0)
  {
    byte temp = Wire.read();
    Serial.write(temp);   
  }
   
}
void loop() {
  /*
  if (Serial.available()) {
    s.write(Serial.read());
  }
  */  
  



  //original ss code-
  char temp;
  while(s.available())
  {
    
    temp = s.read();
    if(temp == '\n')
    {
      break;
    }
    else {
      sensorData += temp;
    }
    //String temp = String(s.read());
    //delay(10);
  }
  if(temp == '\n')
  {
    esp_now_send(broadcastAddress,(uint8_t*) &sensorData, sizeof(sensorData));
    //Serial.print(sensorData);
    sensorData = "";
    //Serial.println();
  }
  //Serial.println();
}

void requestEvent() {


}