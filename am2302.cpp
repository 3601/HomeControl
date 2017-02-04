/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   am2302base.cpp
 * Author: henrik
 * 
 * Created on November 6, 2016, 7:58 PM
 */

#include "am2302.h"
#include "wiringPi.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

AM2302::AM2302(int pinNumber_, int bitLengthCutoff_, int priority_)
{
    //wiringPiSetup();
    
    pinNumber = pinNumber_;
    bitLengthCutoff = bitLengthCutoff_;
    if ((priority_ > 0) && (priority_ <= 0))
    {
        priority = priority_;
        //piHiPri(priority);
    }
    else
        priority = 0;       
}

AM2302::AM2302(ConfigRW& cfg, const std::string& sensor_name)
{
   
    const boost::property_tree::ptree& Ttree = cfg.getSensorConfig(sensor_name);

    //wiringPiSetup();
    
    pinNumber = Ttree.get<int>(cfg.pin_number);
    bitLengthCutoff = Ttree.get<int>(cfg.bit_length_cutoff);
      
    
    boost::optional<int> p_tmp = Ttree.get_optional<int>(cfg.priority);
    if (p_tmp)
        if ((*p_tmp > 0) && (*p_tmp <= 100))
        {
            priority = *p_tmp;
            //piHiPri(priority);
        }
        else
            priority = 0;
    else
        priority = 0;
    
    std::cout << "-- Parameters from config.jason --" << std::endl;
    std::cout << "Pin number: " << pinNumber << std::endl;
    std::cout << "Bit-length cutoff: " << bitLengthCutoff << std::endl;
    std::cout << "Priority: " << priority << std::endl;
}

bool AM2302::readOnce(double& RH, double& T, BitArray& bitArray)
{
    
    // pull pin down for 18ms and high for 40us to wake-up sensor
    pinMode(pinNumber, OUTPUT);
//    digitalWrite(pinNumber, HIGH);
//    delay(10);
    digitalWrite(pinNumber, LOW);
    delay(18);
    digitalWrite(pinNumber, HIGH);
    delayMicroseconds(40);

    // prepare to read the pin 
    pinMode(pinNumber, INPUT);

    // detect change and read data
    int lastState      { HIGH };
    int stateCount     { };
    int stateDuration  { };
    
    int byteArray[5]   { };
    int arrayIndex     { };
    
    
    for (int stateCount = 0; stateCount < max_timings; ++stateCount)
    {
        stateDuration = 0;
        while (digitalRead(pinNumber) == lastState)
        {
            ++stateDuration;
            delayMicroseconds(1);
            if (stateDuration == 255)
                break;
        }
        
        lastState = digitalRead(pinNumber);

        if (stateDuration == 255)
        {
            std::cout << "Call to ReadOnce. StateDuration: " << stateDuration  
                      << ". StateCount: " << stateCount << std::endl;
            break;
        }
        
        // Ignore first 3 transitions = 2x80us start signal and then a low
        // pulse which constitutes the first half of the first bit
        // Since each bit is constituted by a 50-us low pulse followed by
        // a high pulse whose duration signals either 0 (26-28us) or 1 (70us)
        // only every second state (i.e. the high state) is detected using
        // the modulus 2 statement
         
        if ((stateCount >= 4) && (stateCount & 2 == 0))
        {
            // duration of high bit pulse
            if (arrayIndex < 40)
                bitArray[arrayIndex] = stateDuration; 
            
            // shove each bit into the storage bytes 
            byteArray[arrayIndex / 8] <<= 1;
            
            // a stateDuration > bitLengthCutoff signals a 1 bit (i.e. a 
            // pulse > 27 ms; The bitLenthCutoff must be machine dependent)
            if (stateDuration > bitLengthCutoff)
                    byteArray[arrayIndex / 8] |= 1;
            ++arrayIndex;
        }
    }
    
    // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
    // print it out if data is good
    // see https://cdn-shop.adafruit.com/datasheets/Digital+humidity+and+temperature+sensor+AM2302.pdf
    
    if ((arrayIndex >= 40) &&
        (byteArray[4] == ( (byteArray[0] + byteArray[1] + byteArray[2] 
                                         + byteArray[3]) & 0xFF)))
    {
        RH = static_cast<double>((byteArray[0] << 8) + byteArray[1]) / 10;

        // get rid of bit 8 in the first byte
        T = static_cast<double>(((byteArray[2] & 0x7F) << 8) + byteArray[3]) / 10;

        // sign given by the state of bit 8 in the first byte
        if (byteArray[2] & 0x80)
            T = -T;
        
        std::cout << "Readings from ReadOnce -- Temperature: " << T << "C, humidty: "
                  << RH << "%" << std::endl;
        
        return true;

    } else  
        return false;
}

bool AM2302::read(double& RH, double& T, int reps, int delay_ms)
{
   
    int i             { };
    int badReads      { };

    double RH_tmp     { };
    double T_tmp      { };
    int limit         { };
 
    BitArray bitArray { };
    
    std::vector<double> T_vec  { };
    std::vector<double> RH_vec { };

    if (reps <= 0) reps = 1;
    
    if (reps <= 5) limit = 5;
    else limit = reps;
    
    if (delay_ms < 0) delay_ms = 0;
    
    while ((i < reps) && (badReads < limit))
    {
        if (readOnce(RH_tmp, T_tmp, bitArray))
        {
            RH_vec.push_back(RH_tmp);
            T_vec.push_back(T_tmp);
            
            std::cout << "Read #" << i << ". T: " << T_tmp << "C, and RH: " 
                      << RH_tmp << "%." << std::endl;
            
            RH_tmp = T_tmp = 0;
            ++i;
        }
        else
            ++badReads;  
        
        delay(delay_ms);
    }
        
    if (i == reps)
    {
        std::sort(T_vec.begin(), T_vec.end());
        std::sort(RH_vec.begin(), RH_vec.end());
        
        if (reps % 2 == 0)
        {
            T  = (T_vec[reps / 2 - 1] + T_vec[reps / 2]) / 2;
            RH = (RH_vec[reps / 2 - 1] + T_vec[reps / 2]) / 2;
            std::cout << "Average T: " << T << "C, and RH: " << RH 
                      << "%." << std::endl; 
        } 
        else 
        {
            T = T_vec[reps / 2];
            RH = RH_vec[reps / 2];
            std::cout << "Average T: " << T << "C, and RH: " << RH 
                      << "%." << std::endl; 
        }
        return true;
    } 
    else
    {
        RH = T = 0;
        return false;
    }
}

int AM2302::estimateBitLengthCutoff(int reps, int delay_ms)
{
    int T             { };
    int RH            { };
    
    int i             { };
    int badReads      { };
    int limit         { };
  
    double RH_tmp     { };
    double T_tmp      { };

    BitArray bitArray { }; 
    
    int bitcutoff_tmp { };
    
    std::vector<int> bitCutoff { };
    
    if (reps <= 0) reps = 1;
    
    if (reps <= 5) limit = 5;
    else limit = reps;
    
    if (delay_ms < 0) delay_ms = 0;
    
    while ((i < reps) && (badReads < limit))
        if (readOnce(RH_tmp, T_tmp, bitArray))
        {
            for (int &n : bitArray)
            {
                bitcutoff_tmp += n;
                n = 0;
            }
            bitcutoff_tmp /= bitArray.size();
            bitCutoff.push_back(bitcutoff_tmp);
            
            std::cout << "Read #" << i << ". Bit cutoff: " << bitcutoff_tmp << std::endl;
            
            bitcutoff_tmp = 0;

            ++i;
            delay(delay_ms);
        }
        else
            ++badReads;


    if (i == reps)
    {
        std::sort(bitArray.begin(), bitArray.end());
        
        if (reps % 2 == 0)
        {
            bitcutoff_tmp = ((bitArray[reps / 2 - 1] + bitArray[reps / 2]) / 2);;
            std::cout << "Bit cutoff (median): " << bitcutoff_tmp;
            return ((bitArray[reps / 2 - 1] + bitArray[reps / 2]) / 2);
        }
        else
        {
            bitcutoff_tmp = bitArray[reps / 2];
            std::cout << "Bit cutoff (median): " << bitcutoff_tmp;
            return bitArray[reps / 2];
        }
    } else
        return 0;
}

void AM2302::setBitLengthCutoff (int cutoff)
{
    if (cutoff > 0) bitLengthCutoff = cutoff; 
}
