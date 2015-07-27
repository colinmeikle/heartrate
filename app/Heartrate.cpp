#include <../include/Heartrate.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>
Timer procTimer;
 
// Important the input must be <1V  so use a voltage divider on the input to divide by 3 (if using 3V supply to sensor)

/**
 * No pin setup is required as the adc pin is fixed
 */
Heartrate::Heartrate(){
    
}
/**
 * timer handler thats called every 2 mS */
void IRAM_ATTR Heartrate::pulseInterruptHandler() {
   cli();
  
    Signal = system_adc_read();              // read the Pulse Sensor 
    //inc by 2 as the in is every 2 ms
    sampleCounter += 2;                         // keep track of the time in mS with this variable
    int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

//  find the peak and trough of the pulse wave
    if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T){                        // T is the trough
            T = Signal;                         // keep track of lowest point in pulse wave 
         }
       }
      
    if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
        P = Signal;                             // P is the peak
       }                                        // keep track of highest point in pulse wave
    
  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
if (N > 250){                                   // avoid high frequency noise
  if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
    Pulse = true;                               // set the Pulse flag when we think there is a pulse
    IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
    lastBeatTime = sampleCounter;               // keep track of time for next pulse
         
         if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
             firstBeat = false;                 // clear firstBeat flag
             return;                            // IBI value is unreliable so discard it
            }   
         if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
            secondBeat = false;                 // clear secondBeat flag
               for(int i=0; i<=9; i++){         // seed the running total to get a realisitic BPM at startup
                    rate[i] = IBI;                      
                    }
            }
          
    // keep a running total of the last 10 IBI values
    word runningTotal = 0;                   // clear the runningTotal variable    

    for(int i=0; i<=8; i++){                // shift data in the rate array
          rate[i] = rate[i+1];              // and drop the oldest IBI value 
          runningTotal += rate[i];          // add up the 9 oldest IBI values
        }
        
    rate[9] = IBI;                          // add the latest IBI to the rate array
    runningTotal += rate[9];                // add the latest IBI to runningTotal
    runningTotal /= 10;                     // average the last 10 IBI values 
    BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
    QS = true;                              // set Quantified Self flag 
    // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
}

  if (Signal < thresh && Pulse == true){     // when the values are going down, the beat is over
      Pulse = false;                         // reset the Pulse flag so we can do it again
      amp = P - T;                           // get amplitude of the pulse wave
      thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
      P = thresh;                            // reset these for next time
      T = thresh;
     }
  
  if (N > 2500){                             // if 2.5 seconds go by without a beat
      thresh = 512;                          // set thresh default
      P = 512;                               // set P default
      T = 512;                               // set T default
      lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
      firstBeat = true;                      // set these to avoid noise
      secondBeat = true;                     // when we get the heartbeat back
     }
  
  sei(); 
}


/**
 * gets the Quantified Self flag i.e. valid heartBeat
 * THIS resets the flag!
 * @return true if valid heartRate 
 */
boolean Heartrate::getQS(){
      QS=false;
    return QS;
}

/**
 * 
 * @return the beats per minute i.e. actual heartrate 
 */
int16_t Heartrate::getBPM(){
    return BPM;
}

int16_t Heartrate::getSignal(){
    return Signal;
}

/**
 * 
 * @return the inter beat interval  
 */
int16_t Heartrate::getIBI(){
    return IBI;
}
/***
 *Starts the timer to take the measurements every 2 mS
 */
void Heartrate::start(){
     procTimer.initializeMs(2, TimerDelegate(&Heartrate::pulseInterruptHandler,this)).start(); // every 2 ms
    
}

