/*******************************TODO***************************************************
Error handling --> line 55/56
go over it and reformat code --> yep
saying if message is invalid or too long --> unneeded?
create thing (cloud variable)
-Use link: https://create.arduino.cc/iot/things
-create basic integer type variable with updates every 3600s called distance(or whatever)
-use code structure below with board plugged in
-thingProperties.h is created with thing, network info, and defines initProperties()
-sample code: https://forum.arduino.cc/t/initialize-properties-on-arduino-power-up/664152/2
figure error correction of ultrasonic reading to make sure its accurate/consistent
eventually adapt it for longer use and potentially automating it to database or csv
***************************************************************************************/

#include <string>
#include <MKRGSM.h>
#include <iostream>
#include "thingProperties.h" //library for arduino cloud variables


//function reminders
//available (): checks sms messages are on sim card, returns quantity of characters as an integer - SMS.available()
//remoteNumber(): retrieves phone # from incoming message and stores it in array - SMS.remoteNumber(number, size)
//read(): reads byte from message - SMS.read()
//write(): write a character(val) to message - SMS.write(val)
//print(): writes char array as message - SMS.print(message)
//flush(): clears modem memory of sent messages - SMS.flush()

#define echoPin 13
#define trigPin 14
#define smsCharMax 160 //max message size

GSM gsmAccess;
GSM_SMS sms;

char PINNUMBER[10] = secretpinnumber; //public mobile sim card pin

void setup(){
    Serial.begin(9600); //check if required (its in sample code)
    
    //  connection  state
    bool  connected  =  false;

    while  (!connected)  {
        if  (gsmAccess.begin(PINNUMBER)  ==  GSM_READY)  {
            connected  =  true;
        }  else  {
            delay(10);
        }
    }
    initProperties(); //defined in thingProperties.h
    ArduinoCloud.begin(ArduinoIoTPreferredConnection); //connects to Arduino Cloud
    ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, onIoTSync); //Initialize cloud variables
    //debugging function, not required, set level up to 4 for "more granular information"
    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();
}

void loop() {
    ArduinoCould.update();

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  char remoteNumber[20]; 
  char incomingArray[smsCharMax];

  //check correct boot/start sequence
  //check if a message has been received
  if(sms.available()){

    sms.remoteNumber(remoteNumber,  20);
    
    int arrayCount = 0;
    char c;

    while(arrayCount < smsCharMax){
      c = sms.read();
      if (std::isalnum(c)) {
        incomingArray[arrayCount] = c;
        arrayCount++;
      } else {
        break;
      }
      incomingArray[arrayCount] = '\0';
    }
    //if incoming message matches "IWANTWATER", sends gauge reading message.
    if(strcmp(incomingArray,"IWANTWATER") == 0){

      sms.beginSMS(remoteNumber);

      int distance = distanceMeasure();
      sms.print("Gauge is reading: ");
      sms.print(distance);
      sms.print("\n");

      Serial.println("REACHED Valid SMS");

    } else {    //prints error message and exits loop if arrays do not match
    sms.beginSMS(remoteNumber);
      sms.print("INVALID MESSAGE RECEIVED");
    }
    sms.print("Reading has Finished");
    sms.endSMS();
  }
}

int distanceMeasure (){
  long  duration;
  int  distance;
  int  summed_distance  =  0;
  int  avg_distance;
  int  count  =  0;

  while  (count  <  50)  {
          digitalWrite(trigPin,  LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin,  HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin,  LOW);

          duration  =  pulseIn(echoPin,  HIGH);
          distance  =  duration  *  0.034  /  2;
          summed_distance  +=  distance;
      
          count++;
      }
      avg_distance  =  summed_distance  /  count;
      return  avg_distance;
}
void onIoTSync(){
//initialize distance variable (same name as thing)
nameThing = distanceMeasure(); or //nameThing.distanceMeasure(); but not 100% sure
Serial.println("Arduino synced to IoT Cloud"); //debug test
}
