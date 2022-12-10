#include <SoftwareSerial.h>
SoftwareSerial s(2,3);
volatile bool isRotate = false;
//stepper motor
#include <Stepper.h>
const int stepsPerRevolution = 480;  // change this to fit the number of steps per revolution
// for your motor
Stepper myStepper(stepsPerRevolution, 7, 9,8, 10);
int startLocation = 1;
int CW = 0;
int CCW = 0;
String sensorData;
String prev;
volatile bool IR1;
volatile bool IR2;
volatile bool IR3;
volatile float compassData;
//volatile bool isRunning;
float  temperature;
int distance;
//bool photocell;
//int side = 0;

void setup() {
  // put your setup code here, to run once:
  s.begin(57600);
  Serial.begin(115200);
  myStepper.setSpeed(45);
  Serial.println("Command List: ");
  Serial.println("1: compass | 2: Distance | 3: Temperature & speed of sound | 4: all");
  Serial.println("Motor adjustment: ");
  Serial.println("7: clockwise | 8: counter-clockwise");
  Serial.println("--------------------------------------------------------------------------");
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
  dtemp = d.substring(9, 13);
  temperature = dtemp.toFloat();
  /*
  if(d.charAt(13) == '1')
  {
isRunning = true;
  }
  else
  {
isRunning = false;    
  }  
*/  
  
  dtemp = d.substring(13);
  distance = dtemp.toInt();
  
}


void readData()
{
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
    //Serial.println();
    decode(sensorData);
    prev = sensorData;

    //Serial.println(sensorData);

    //Serial.println(sensorData);
    //bool temp = isRunning;
    //Serial.println(isRunning);
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

  }
  int input;
    if(Serial.available())
    {
      input = Serial.read()-48;
    }    
    if(input == 1)
    {
      Serial.print("Compass Heading: ");
      Serial.println(compassData);
      Serial.println();
    }else if(input == 2)
    {
      Serial.print ("Distance: ");
      Serial.println(distance);  
      Serial.println();
    }else if(input ==3)
    {
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Speed of sound: ");
      Serial.print(temperature *0.6 + 331.6);
      Serial.println(" m/s");
    Serial.println();
    }
    else if(input == 4)
    {
      Serial.print("Compass Heading: ");
      Serial.println(compassData);  
      Serial.print ("Distance: ");
      Serial.println(distance);  
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Speed of sound: ");
      Serial.print(temperature *0.6 + 331.6);
      Serial.println(" m/s");
      Serial.println();    
    }
    else if (input == 7)
    {
      CW = (50);
      myStepper.step(CW);
      delay(1000);      
    }
    else if(input ==8)
    {
      CCW = -(50);
      myStepper.step(CCW);
      delay(1000);           
    }
}



void rotate()
{
    if(IR1 || (IR1 && IR2))
    {
      motorCCW();
      readData();
      delay(100);
      Serial.print("Pointing at: ");
      Serial.println(compassData);
      Serial.print("Distance: ");
      Serial.println(distance);
      //Serial.println(prev);
      motorCW();
    }
    else if(IR3 || (IR2 && IR3))
    {
      motorCW();  
      readData();
      delay(100);
      Serial.print("Pointing at: ");
      Serial.println(compassData);
      Serial.print("Distance: ");
      Serial.println(distance);
      //Serial.println(prev);
      motorCCW();
      //side = 0; 
 
      //motorCCW(); 
    }
    else if(IR2)
    {
        //takepicture    
    }    
    else //nothing
    {
      //takepicture
    
    }  
}



void motorCW()
{
    // step one revolution  in one direction:
    //Serial.println("clockwise");
    CW = (stepsPerRevolution);
    myStepper.step(CW);
    delay(1000);
    //Serial.println("at location A");

}

void motorCCW()
{
  // step one revolution  in the other direction:
    //Serial.println("counterclockwise");
    CCW = (-stepsPerRevolution);
    myStepper.step(CCW);
    delay(1000);
    //Serial.println("at location B");
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



 
void loop() {
  // put your main code here, to run repeatedly:
    readData();
    rotate();
    

    
    
  }


