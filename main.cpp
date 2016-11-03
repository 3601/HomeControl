/* 
 * File:   main.cpp
 * Author: Henrik
 *
 * Created on 28. august 2016, 11:54
 */


#include <iostream>
#include <string>

#include "mysqlRW.h"

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
    
    // ./config.json
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
    
    return 0; 

}

