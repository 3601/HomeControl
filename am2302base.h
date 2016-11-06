/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   am2302base.h
 * Author: henrik
 *
 * Created on November 6, 2016, 7:58 PM
 */

#ifndef AM2302BASE_H
#define AM2302BASE_H

#include<wiringPi.h>
#include<stdint.h>

class am2302base 
{
    private:
        const int max_timings { 85 };
        const int dht_pin { };  // pin to communicate with on AM2302
        const int reps { };     // number of repeated measurements
        
        uint8_t lastState  { HIGH };
        uint8_t signalCnt  { 0 };
        uint8_t arrayIndex { 0 };
        
         int byteArray[5] { 0,0,0,0,0 };

public:
    am2302base(int dht_pin = 3, int reps_ = 3) 
        : dht_pin(dht_pin), 
          reps(([&]() -> int { if (reps_ < 1) return 1; 
                               else if (reps_ > 5) return 5; })() ) { }



};

#endif /* AM2302BASE_H */

