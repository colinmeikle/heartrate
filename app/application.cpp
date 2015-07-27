#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "../include/Heartrate.h"

/**
 * Important the input must be <1V  so use a voltage divider on the input to divide by 3 (if using 3V supply to sensor)
 *  ADC pin is fixed so it can't be set
 * Very basic example of the the Heartrate sensor, timer simply writes the values out to serial every second
 * positioning of the senor (in my experience ) can be trial and error, light pressure seems to work best
 */
Heartrate pulse=Heartrate();
Timer printTimer;
void printHeartrate(){
    int16_t bpm=pulse.getBPM();
    int16_t ibi=pulse.getIBI();
    Serial.printf("BPM %d IBI %d\n",bpm,ibi);
    //int16_t sig=pulse.getSignal();
   // Serial.printf(" sig %d\n",sig);
    
}


void init()
{
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.systemDebugOutput(false); // Do not allow debug print to serial
	Serial.println("Measuing Heartrate");
        pulse.start();
        printTimer.initializeMs(1000, printHeartrate).start(); // every second

}
