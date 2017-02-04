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

#ifndef AM2302_H
#define AM2302_H

#include "configRW.h"
#include <string>
#include <array>

// Remember to call wiringPiSetup() to initialize and set pin numbering
// priority of thread can be adjusted by calling piHiPri(xx) with value
// from 0 - 100

class AM2302 
{
    private:
        const int max_timings { 85 };  // total number of pulses read
        
        int bitLengthCutoff { };
        int pinNumber { };  // communication pin using wiringPi numbering
        int priority { };   // input for WiringPi function wiHiPri(x)
     
        // using BitArray = std::array<int,40>;
        
        typedef std::array<int,40> BitArray;
        
        bool readOnce(double& RH, double& T, BitArray& bitArray);
        
    public:

        AM2302(int pinNumber_ = 3, int bitLengthCutoff_ = 16, int priority_ = 55);
        
        // Fetches AM2302 config information from the config file via the 
        // ConfigRW class. Exception is thrown if error is encountered 
        // if 'priority' is not specified in the config file, if will be set
        // to 0 and the piHIPri function will not be called
        AM2302(ConfigRW& cfg, const std::string& sensor_name = "AM2302");          
        
        // if reps > 1, the median temperature and humidity is return
        // false is returned if the number of failed reads is equal to reps
        // when reps >= 5, otherwise if number of failed reads is >5
        bool read(double& RH, double& T, int reps = 3, int delay_ms = 2000);
        
        
        // measure length of bit pulses and takes median of average for each
        // of the replicate measurements
        // returns 0 if the number of failed reads is equal to reps when
        // reps >= 5, similarly if number of failed reads is 5
        int estimateBitLengthCutoff(int reps = 3, int delay_ms = 2000);
        
        void setBitLengthCutoff(int cutoff); 


};

#endif /* AM2302BASE_H */

