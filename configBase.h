/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConfigLayout.h
 * Author: Henrik
 *
 * Created on 27. september 2016, 21:08
 */

#ifndef CONFIGLAYOUT_H
#define CONFIGLAYOUT_H

#include <string>

class ConfigBase {


public:
    ConfigBase();

     // name of mysql_config module in JSON config file
    const std::string mysql_config { "mysql_config" }; 

    // parameter names in mysql_config module 
    const std::string host_name   { "host_name" };
    const std::string user_name   { "user_name" };
    const std::string db_name     { "db_name" };
    const std::string password    { "password" };
    const std::string port_num    { "port_num" };
    const std::string socket_name { "socket_name" };
    const std::string flags       { "flags" };

    // name of sensor module in JSON config file
    const std::string sensors { "sensors" };

    // name of table entries in sensor module
    const std::string table_entries { "table_entries" };   

};

#endif /* CONFIGLAYOUT_H */

