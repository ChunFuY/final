#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SoftwareSerial.h>
Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

//ultrasonic pins
#define trigPin 6
#define echoPin 5

const int temp_input = A0;

//left=1 middle=2 right=3
const int IR1 =8;
const int IR2 =9;
const int IR3 =10;
SoftwareSerial s(2,3);

void setup() {
  s.begin(57600);
  Serial.begin(115200);
  Serial.println("Magnetometer Test");
  Serial.println("");
  if (!mag.begin()) {
  /* There was a problem detecting the LSM303 ... check your connections */
  Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  while (1)
    ;
  }
  // put your setup code here, to run once:
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);  
}

float getTemp()
{
  int input = analogRead(temp_input);
  float voltage = input * 5.0;
  voltage /= 1024.0;
  float temp = (voltage - 0.5) * 100;
  return temp;
  Serial.println(temp);
}

float getCompass()
{
  sensors_event_t event;
  mag.getEvent(&event);

  float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / Pi;

  // Normalize to 0-360
  if (heading < 0) {
    heading = 360 + heading;
  }
  //Serial.print("Compass Heading: ");
  //Serial.println(heading);
  return heading;
}

int getDistance()
{
  long duration, distance;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //speed of sound = 331.6 + 0.6* tempurature
  double speed = (getTemp() * 0.6 + 331.6) / 10000;
  Serial.println(speed, 4);
  distance = speed * duration / 2;
  if(distance > 0 && distance < 150)
  {
    return distance;
  }
  else{
    return 0;
  }
}

bool isSend = false;

void loop() {
  // put your main code here, to run repeatedly:
  int brightness = analogRead(A0);
  bool ir_array[3];
  ir_array[0] = digitalRead(IR1);
  ir_array[1] = digitalRead(IR2);
  ir_array[2] = digitalRead(IR3);
  float temp = getTemp();
  //Serial.println(brightness);
  //Serial.println(getDistance());
  int distance = getDistance();
  s.print(encode(ir_array[0], ir_array[1], ir_array[2], getCompass(),temp, distance));  
  while( !ir_array[0] || !ir_array[1] || !ir_array[2])
  {
    ir_array[0] = digitalRead(IR1);
    ir_array[1] = digitalRead(IR2);
    ir_array[2] = digitalRead(IR3);
    getCompass();
    brightness = analogRead(A0);
    temp = getTemp();
    distance = getDistance();    
  }
delay(1000);


}

//ir1 ir2 ir3 compass photocell
String encode(bool b1, bool b2, bool b3, float f, float temperature, int distance)
{
  String return_value = "";
  if(!b1 == true)
  {
    return_value += '1'; 
  }
  else{
    return_value += '0';
  }

  if(!b2 == true)
  {
    return_value += '1'; 
  }
  else{
    return_value += '0';
  }

  if(!b3 == true)
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
  return_value += String(distance);

  return_value += '\n';
  

  Serial.println(return_value);
  return return_value;
  
}