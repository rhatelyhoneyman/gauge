/*******************************TODO***************************************************
Error handling
go over it and reformat code
saying if message is invalid or too long
sms flush / board memeory/ cache wiping (or if that even needs to happen)
figure error correction of ultrasonic reading to make sure its accurate/consistent
test test
eventually adapt it for longer use and potentially automating it to database or csv
***************************************************************************************/

#include <string>
#include <MKRGSM.h>
#include <iostream>    
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

    //  connection  state
    bool  connected  =  false;

    while  (!connected)  {
        if  (gsmAccess.begin(PINNUMBER)  ==  GSM_READY)  {
            connected  =  true;
        }  else  {
            delay(10);
        }
    }
}

void loop() {

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
