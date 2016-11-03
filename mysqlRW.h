/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mysqlRW.h
 * Author: Henrik
 *
 * Created on 4. september 2016, 13:38
 */

#ifndef MYSQLRW_H
#define MYSQLRW_H

#ifdef WINDOWS_COMPILE
//#include <mysql/my_global.h>  //important that this is included as the first header
#include <mysql/mysql.h>
#endif


#include <iosfwd>             //contains all forward declarations of iostream
#include <cstdlib>

#include <string>
#include <cstring>

#include <boost/property_tree/ptree.hpp>

#ifndef WINDOWS_COMPILE
//#include <mysql/my_global.h>  //important that this is included as the first header
#include <mysql/mysql.h>
#endif



#include "tableEntry.h"
#include "configRW.h"

class MysqlRW {
private:

    std::string host_name { };
    std::string user_name { };
    std::string password { };
    std::string db_name { };
    unsigned int port_num { };
    std::string  socket_name { };
    unsigned int flags { };

    MYSQL *conn;

    void printError (const std::string& message);

public:
    // Exception is thrown (-1) if error is encountered during initialization
    // or establishment of mySQL connection
    MysqlRW(const std::string& host_name, const std::string& user_name,
            const std::string& db_name, const std::string& password,
            const unsigned int port_num, const std::string& socket_name,
            const unsigned int flags);

    // Fetches mySQL config information from the config file via the ConfigRW
    // class
    // Exception is thrown if error is encountered during initialization
    // or establishment of mySQL connection
    MysqlRW(ConfigRW& cfg);
    

    // If upload is successful and flushTable = true, the passed table is
    // flushed
    // Returns false if upload is unsuccessful   
    template<typename T1, typename T2, typename T3>
    bool uploadTableEntry(TableEntry<T1,T2,T3>& tEntry, bool flushTable);
    
    // Deletes all entries in table (name: tableName) in mySQL database 
    // (name: db_name). Returns false upon error 
    bool clearTable(const std::string& tableName);
    
    ~MysqlRW();
};

// since the argument TableEntry in uploadTableEntry is a template class 
// inclusion of the definition .cpp  file is needed and therefore should 
// not be included in the project
#include "mysqlRW.cpp"

#endif /* MYSQLRW_H */

