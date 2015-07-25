/* 
 * File:   Heartrate.h
 * Author: Colin Meikle
 *
 * Created on 25 July 2015, 22:26
 * code adapted from Pulse Sensor Amped 1.1 by Joel Murphy & Yury Gitman 
 * Check here for detailed code walkthrough:
 * http://pulsesensor.myshopify.com/pages/pulse-sensor-amped-arduino-v1dot1
 */

#ifndef HEARTRATE_H
#define	HEARTRATE_H
#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/Delegate.h>

class Heartrate {
public:
    void start();
    Heartrate();
    boolean getQS();
    int16_t getBPM();
    int16_t getIBI();

private:
    volatile int16_t rate[10]; // used to hold last ten IBI values
    volatile unsigned long sampleCounter = 0; // used to determine pulse timing
    volatile unsigned long lastBeatTime = 0; // used to find the inter beat interval
    volatile int16_t P = 512; // used to find peak in pulse wave
    volatile int16_t T = 512; // used to find trough in pulse wave
    volatile int16_t thresh = 512; // used to find instant moment of heart beat
    volatile int16_t amp = 100; // used to hold amplitude of pulse waveform
    volatile boolean firstBeat = true; // used to seed rate array so we startup with reasonable BPM
    volatile boolean secondBeat = true; // used to seed rate array so we startup with reasonable BPM
    volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
    volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
    volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
    volatile int16_t Signal; //raw signal data
    volatile int BPM;                   // used to hold the pulse rate
    
    void IRAM_ATTR pulseInterruptHandler();


};

#endif	/* HEARTRATE_H */

