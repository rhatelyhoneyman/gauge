#include <stdio.h>
#include <string.h>
#include <MKRGSM.h>
#include <iostream>

//function reminders
//available (): checks sms messages are on sim card, returns quantity of characters as an integer - SMS.available()
//remoteNumber(): retrieves phone # from incoming message and stores it in array - SMS.remoteNumber(number, size)
//read(): reads byte from message - SMS.read()
//write(): write a character(val) to message - SMS.write(val)
//print(): writes char array as message - SMS.print(message)
//flush(): clears modem memory of sent messages - SMS.flush()

#define echoPin 10
#define trigPin 11
#define smsCharNum 11 //max message size


//int dist = distanceMeasure();

GSM gsmAccess;
GSM_SMS sms;
char PINNUMBER[11] = ""; //public mobile sim card pin

//stole code from sms_numer.ino
void setup(){
    //  initialize  serial  communications  and  wait  for  port  to  open:
    Serial.begin(9600);

    while  (!Serial)  {
        delay(100);
        Serial.println("Waiting");
        ;  //  wait  for  serial  port  to  connect.  Needed  for  native  USB  port  only
    }

    Serial.println("SMS  Messages  Sender");
    //  connection  state
    bool  connected  =  false;

    while  (!connected)  {
        if  (gsmAccess.begin(PINNUMBER)  ==  GSM_READY)  {
            connected  =  true;
            Serial.println("MADEIT");
        }  else  {
            Serial.println("Not  connected");
            delay(10);
        }
    }
    Serial.println("GSM  initialized");
}

void loop() {

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  //store number in other file or something
  char incomingNumber[11]; 
  char incomingArray[smsCharNum]; //stores incoming message
   //index for array
  int verification = 0; //verifies message matches expectation
  char testArray[smsCharNum] = "IWANTWATER"; //testArray is the message to match

  //check correct boot/start sequence
  //check if a message has been received
  if(sms.available()){
    sms.remoteNumber(incomingNumber, 10);
    //stores and prints incoming message
    int arrayCount = 0;
    while(char c = sms.read() && arrayCount < smsCharNum){
      Serial.print(c);
      incomingArray[arrayCount] = c;
      arrayCount++;
    }

    incomingArray[arrayCount+1] = '\0';

    Serial.println(incomingNumber);
    Serial.println(incomingArray);

    if(strcmp(incomingArray,"IWANTWATER") == 0){
      Serial.print("MESSAGE_MATCH");
      verification = 1;
    } 

    else {
    //prints error message and exits loop if arrays do not match
      Serial.println("MESSAGE_DOESNT_MATCH");
      verification = 2;
    }

  
    sms.beginSMS(incomingNumber);
    //if messages match, return value to sender (distance/error)
    if(verification == 1){
      //send distance value
      int distance = distanceMeasure();
      sms.println(distance);
      Serial.println("REACHED Valid SMS");

    } else {
      //send reattempt prompt
      sms.println("MESSAGE_DOESNT_MATCH_TRY_AGAIN");
      Serial.println("REACHED ERROR SMS");
    } 

    sms.endSMS();
    Serial.print("\nMSG_SENT\n");
    sms.flush();
    //ends messaging and sents confirmation message while clearing cache
  }

}
int distanceMeasure (){
int distance;
long duration;
 
  //initializes and sends pulse through transmitter
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
   
  //receives pulse and time, uses values to calculate distance
  duration = pulseIn(echoPin, HIGH);
  //debug
  distance = duration * 0.034 / 2;

  return distance;
}