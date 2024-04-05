/*
found  at  https://www.arduino.cc/en/Tutorial/MKRGSMExamplesSendSMS  on  nov  21  2023
*/

/*
  SMS  sender
  This  sketch,  for  the  MKR  GSM  1400  board,sends  an  SMS  message
  you  enter  in  the  serial  monitor.  Connect  your  Arduino  with  the
  GSM  shield  and  SIM  card,  open  the  serial  monitor,  and  wait  for
  the  "READY"  message  to  appear  in  the  monitor.  Next,  type  a
  message  to  send  and  press  "return".  Make  sure  the  serial
  monitor  is  set  to  send  a  newline  when  you  press  return.
  Circuit:
  *  MKR  GSM  1400  board
  *  Antenna
  *  SIM  card  that  can  send  SMS
  created  25  Feb  2012
  by  Tom  Igoe
*/
//  Include  the  GSM  library
#include <MKRGSM.h>
#include "arduino_secrets.h"
#include <string.h>
#include <GSM.h>
#include <iostream>    
#include "functions.ino"

//  Please  enter  your  sensitive  data  in  the  Secret  tab  or  arduino_secrets.h
GSM  gsmAccess;
GSM_SMS  sms;
//  Array  to  hold  the  number  a  SMS  is  retrieved  from


//  PIN  Number
const char  PINNUMBER[]  =  secret_pin_number;
const int  trigPin  =  14;
const int  echoPin  =  13;
int counter = 0;


void  setup()  {

    pinMode(trigPin,  OUTPUT);  //  Sets  the  trigPin  as  an  Output
    pinMode(echoPin,  INPUT);  //  Sets  the  echoPin  as  an  Input

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

void  loop()  {
    char  c;
    char  remoteNumber[20];
    char  textMSG[100];

    //if  in  service,  gets  the  number
    if  (sms.available())  {
        //  Get  remote  number
        sms.remoteNumber(remoteNumber,  20);
        Serial.println("Received Number: ");
        Serial.println(remoteNumber);


        sms.beginSMS(remoteNumber);

        if  (strcmp(remoteNumber,  "+1**********")  ==  0)  { //compare remote num to num given (replace ********** with actual number)
            int  average_distance;
            //  Ultrasonic  sensor  distance  measurement  loop
            Serial.println("Measuring  distance...");

            average_distance  =  record_ultrasonic_values();

            Serial.print("avg  value:  ");
            Serial.println(average_distance);

            sms.print("Distance  measurement  complete:");
            sms.print(average_distance);

        }  else  {
            //  Send  the  regular  message
            sms.print("Couldn't  Calculate  distance");
        }
        sms.endSMS();
    }
}
