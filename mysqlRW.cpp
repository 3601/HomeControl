/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mysqlRW.cpp
 * Author: Henrik
 * 
 * Created on 4. september 2016, 13:39
 */

//#include <my_global.h>  //important that this is included as the first header
//#include <mysql.h>

#ifdef WINDOWS_COMPILE
//#include <mysql/my_global.h>  //important that this is included as the first header
#include <mysql/mysql.h>
#endif


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "mysqlRW.h"

#include "tableEntry.h"
#include "configRW.h"

#ifndef WINDOWS_COMPILE
#include <mysql/my_global.h>  //important that this is included as the first header
#endif

void MysqlRW::printError(const std::string& message)
{
    std::cout << std::endl << "Error message: " << std::endl << message << std::endl;
    if (conn != NULL)
    {
        std::cout << "Error code: " << mysql_errno(conn) << ", " 
                  << mysql_sqlstate(conn) << ", " << mysql_error(conn) 
                  << std::endl;
    }
}

MysqlRW::MysqlRW(const std::string& host_name, const std::string& user_name, 
                 const std::string& db_name, const std::string& password,
                 const unsigned int port_num, const std::string& socket_name,
                 const unsigned int flags)  
                 : host_name(host_name), user_name(user_name),
                   db_name(db_name), password(password), port_num(port_num),
                   socket_name(socket_name), flags(flags)
{
    conn = mysql_init(NULL); 
    
    if (conn == NULL)
    {
        printError("Error initializing mySQL connection");
        throw -1;
    }
    
    if (!mysql_real_connect(conn, host_name.c_str(), user_name.c_str(), 
                                  password.c_str(), db_name.c_str(), 
                                  port_num, socket_name.c_str(), flags))
    {
        printError("Could not connect to mySQL database");
        throw -1;
    } else std::cout << "mySQL server connection successfully established" << std::endl;
}

MysqlRW::MysqlRW(ConfigRW& cfg)
{
    const boost::property_tree::ptree& cTree = cfg.getDbConfig();
      
    host_name   = cTree.get<std::string>(cfg.host_name);
    user_name   = cTree.get<std::string>(cfg.user_name);
    db_name     = cTree.get<std::string>(cfg.db_name);
    password    = cTree.get<std::string>(cfg.password);
    port_num    = cTree.get<unsigned int>(cfg.port_num);
    socket_name = cTree.get<std::string>(cfg.socket_name);
    flags       = cTree.get<unsigned int>(cfg.flags);

    conn = mysql_init(NULL);

    if (conn == NULL)
    {
        printError("Error initializing mySQL connection");
        throw -1;
    } 

    if (!mysql_real_connect(conn, host_name.c_str(), user_name.c_str(), 
                                  password.c_str(), db_name.c_str(), 
                                  port_num, socket_name.c_str(), flags))
    {
        printError("Could not connect to mySQL database");
        throw -1;
    } else std::cout << "mySQL server connection successfully established" << std::endl;
}

template<typename T1, typename T2, typename T3>
bool MysqlRW::uploadTableEntry(TableEntry<T1,T2,T3>& tEntry, bool flushTable)
{
    if (tEntry.getSize() > 0)
    {
        // compose query statement into stringstream (queryStr). Table entries
        // with empty ValueStr (getValueStr) are skipped
        std::stringstream queryStr { };
        
        queryStr << "INSERT INTO " << db_name << "." << tEntry.getTitle() << " ("; 
        // queryStr << "INSERT INTO " << tEntry.getTitle() << " (";
        
        bool first = true;
        for (int i = 0; i < tEntry.getSize(); ++i)
            if (first & tEntry[i].getValueStr() != "")
            {
                queryStr << tEntry[i].getTitle();
                first = false;
            } else
                if (tEntry[i].getValueStr() != "")
                    queryStr << "," << tEntry[i].getTitle();


        queryStr << ")" << " VALUES (";
        
        first = true;
        for (int i = 0; i < tEntry.getSize(); ++i)
            if (first & tEntry[i].getValueStr() != "")
            {
                queryStr << "\'" << tEntry[i].getValueStr() << "\'";
                first = false;
            } else
                if (tEntry[i].getValueStr() != "")
                    queryStr << "," << "\'" << tEntry[i].getValueStr() << "\'";
        queryStr << ")";

        // std::cout << "Composed query for table: " << tEntry.getTitle() << std::endl;
        std::cout << "mySQL query: " << queryStr.str() << std::endl;
        
        // upload query string to mySQL
        if (mysql_query(conn, queryStr.str().c_str()) != 0)
        {
            std::string tmp { };
            tmp = "Error uploading query: " + queryStr.str() + " to mySQL.";
            printError(tmp);
            return false;
        } else
        {
            if (flushTable) tEntry.flush();
            return true;
        }
    } else 
        return false;
}

bool MysqlRW::clearTable(const std::string& tableName)
{
    std::string tmpStr { "TRUNCATE TABLE " + db_name + "." + tableName };
    return !mysql_query(conn, tmpStr.c_str());  // returns false upon error
}


MysqlRW::~MysqlRW()
{
    if (conn != NULL)
    {
        mysql_close(conn);
        std::cout << "mySQL server connection successfully closed" << std::endl;
    }
}


