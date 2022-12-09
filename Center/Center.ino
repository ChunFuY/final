#include <SoftwareSerial.h>
SoftwareSerial s(2,3);
volatile bool isRotate = false;
//stepper motor
#include <Stepper.h>
const int stepsPerRevolution = 400;  // change this to fit the number of steps per revolution
// for your motor
Stepper myStepper(stepsPerRevolution, 7, 9,8, 10);
int startLocation = 1;
int CW = 0;
int CCW = 0;
String sensorData;
volatile bool IR1;
volatile bool IR2;
volatile bool IR3;
volatile float compassData;
bool photocell;

void setup() {
  // put your setup code here, to run once:
  s.begin(57600);
  Serial.begin(115200);
  myStepper.setSpeed(35);

}



void loop() {
  // put your main code here, to run repeatedly:
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
  }
  if(temp == '\n')
  {
    Serial.println();
    decode(sensorData);
    sensorData = "";
    /*
    sensorData = "";   
    Serial.print("IR Left: ");
    IR_conversion(IR1);
    Serial.print("IR Middle: ");
    IR_conversion(IR2);
    Serial.print("IR Right: ");
    IR_conversion(IR3);
    Serial.print("Compass Reading: ");
    Serial.println(compassData);
    Serial.print("Photocell Reading: ");
    photocell_conversion(photocell);
    */
    rotate();
  }



}

void rotate()
{
  Serial.print("rotate");
    if(IR1 || (IR1 && IR2))
    {
      Serial.println("test");
      motorCCW();
      motorCW();
    }
    else if(IR3 || (IR2 && IR3))
    {
            Serial.println("test1");
      motorCW();   
      motorCCW(); 
    }
    else if(IR2)
    {
            Serial.println("test2");
        //takepicture    
    }
    else //nothing
    {
            Serial.println("test3");
      //takepicture
    }
  

}

void motorCW()
{
    // step one revolution  in one direction:
    Serial.println("clockwise");
    CW = (stepsPerRevolution);
    myStepper.step(CW);
    delay(500);
    Serial.println("at location A");

}

void motorCCW()
{
  // step one revolution  in the other direction:
    Serial.println("counterclockwise");
    CCW = (-stepsPerRevolution);
    myStepper.step(CCW);
    delay(500);
    Serial.println("at location B");
}

void photocell_conversion(bool b)
{
  if(true)
  {
    //Serial.println("It's bright enough");
  }else{
    //Serial.println("Not bright enough");
  }
}

void IR_conversion(bool b)
{
  if(b == true)
  {
    //Serial.println("Detected");
  }
  else {
   //Serial.println("Not Detected");
  }
}

  void decode(String d)
{
  char temp = d.charAt(0);
  if(temp == '1')
  {
    IR1 = true;
  }
  else
  {
    IR1 = false;    
  }

  temp = d.charAt(1);
  if(temp == '1')
  {
    IR2 = true;
  }
  else
  {
    IR2 = false;    
  }

  temp = d.charAt(2);
  if(temp == '1')
  {
    IR3 = true;
  }
  else
  {
    IR3 = false;    
  }

  String dtemp = d.substring(3,9);
  compassData = dtemp.toFloat();

  temp = d.charAt(9);
  if(temp == '1')
  {
    photocell = true;
  }
  else
  {
    photocell = false;    
  }
}

