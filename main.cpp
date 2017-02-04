/* 
 * File:   main.cpp
 * Author: Henrik
 *
 * Created on 28. august 2016, 11:54
 */


#include <iostream>
#include <string>

#include "mysqlRW.h"
#include "am2302.h"
#include "configRW.h"

#include "wiringPi.h"
#include "maxdetect.h"

//#include "tableEntry.h"
//#include "configRW.h"
//include <boost/property_tree/ptree.hpp>
//#include <stdio.h>

int main() 
{

    
    TableEntry<int,double,std::string> sqlTable("book");
    
    // sqlTable.addEntry("Henrik");
    // sqlTable["Tina"] = 2;
    // sqlTable["Henrik"] = "stringtest";
    
    // sqlTable << "Rasmus" << "Mathias";
    
    // for (const auto &indx : sqlTable)
    //    std::cout << indx.getTitle() << ":  " << indx.getValueStr() << "; ";
    
    // std::cout << std::endl;
    
    // std::cout << "Call with new title: " << sqlTable["Thomas"].getTitle() << std::endl;
    
    // sqlTable.flush();
    // std::cout << "Values after flush: " << std::endl;
    // for (const auto &indx : sqlTable)
    //    std::cout << indx.getTitle() << ": " << indx.getValueStr() << "; ";
    
    
    std::cout << std::endl << "--- Load of config.json file ---" << std::endl;
    
     
    ConfigRW cfgFile("config.json");
    
    
    sqlTable.addTableConfig(cfgFile, "book");

    for (const auto &indx : sqlTable)
        std::cout << indx.getTitle() << ": " << indx.getValueStr() << std::endl;
    
    
    
    std::cout << std::endl;
    std::cout << std::endl << "--- Establish connection to the mySQL server ---" << std::endl;
    
    
    
    MysqlRW mysqlHandle(cfgFile);
    // if (mysqlHandle.clearTable(sqlTable.getTitle()))
    //    std::cout << "Table: " << sqlTable.getTitle() << " successfully cleared" << std::endl;
    
    mysqlHandle.clearTable("book");
    
    std::string title { };
    for (int i = 10; i < 21; ++i)
    {
        title = "Title" + std::to_string(i);
        sqlTable["Title"] = title;
        sqlTable["Value"] = i;
        sqlTable.addTimeStamp();
        if (!mysqlHandle.uploadTableEntry(sqlTable, true))
            break;      
    }
    
    
    
    std::cout << std::endl << "--- Read from AM2302 sensor on Raspberry Pi ---" << std::endl;
    
    double T      { };
    double RH     { };
    
    int T_g { };
    int RH_g { };
    int i_g { };
    int bitcutoff { };
    
    wiringPiSetup();
    piHiPri(55);

    std::cout << "Read from Gordon library" << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        delay(100);
        i_g = readRHT03(3, &T_g, &RH_g);
        std::cout << "#" << i_g << ". Temperature: " << T_g / 10.0 << "C, and humidity: " << RH_g / 10.0 << "%" << std::endl;
    }
    
    
    std::cout << "Read configuration from config.json" << std::endl;
    AM2302 am2302(cfgFile, "AM2302");
    
    std::cout << "Read from AM2302" << std::endl;
    if (am2302.read(RH, T, 3, 1000))
        std::cout << "Temperature: " << T << "C, and humidity: " << RH << "%" << std::endl;
    else
        std::cout << "AM2302 reading failed" << std::endl;
    
    
    //std::cout << "Estimate bit-length cutoff" << std::endl;
    
    //bitcutoff = am2302.estimateBitLengthCutoff(3, 2000);
    //if (!bitcutoff)
    //    std::cout << "Estimated bit-length cutoff: " << bitcutoff << std::endl;
    //else
    //    std::cout << "Failure estimating bit-length cutoff" << std::endl;
    
     
     
    return 0; 
}

