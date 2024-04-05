#include <MKRGSM.h>
#include "arduino_secrets.h"
#include <string.h>
#include <GSM.h>
#include <iostream>   

int  record_ultrasonic_values(num_count)  {
    long  duration;
    int  distance;
    int  summed_distance  =  0;
    int  avg_distance;
    int  count  =  0;

    while  (count  <  num_count)  {
            digitalWrite(trigPin,  LOW);
            delayMicroseconds(2);
            digitalWrite(trigPin,  HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin,  LOW);

            duration  =  pulseIn(echoPin,  HIGH);
            distance  =  duration  *  0.034  /  2;
            summed_distance  +=  distance;

            Serial.print(count);
            Serial.print("  Distance:  ");
            Serial.print(distance);
            Serial.print("  Summed  distance:  ");
            Serial.print(summed_distance);
            Serial.print("\n");
            count++;
        }
        avg_distance  =  summed_distance  /  count;
        return  avg_distance;
}