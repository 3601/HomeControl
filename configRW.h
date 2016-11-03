/* 
 * File:   configIO.h
 * Author: Henrik
 *
 * Created on 13. september 2016, 21:16
 */

#ifndef CONFIGIO_H
#define CONFIGIO_H

#include <iosfwd>  //contains all forward declarations in iostream
#include <string>

#include <boost/property_tree/ptree.hpp>
// #include <boost/property_tree/json_parser.hpp>

#include "configBase.h"

class ConfigRW : public ConfigBase
{
    private:
        boost::property_tree::ptree tree { };
        
    public:
        // Exception is thrown if error loading config file is encountered
        ConfigRW(const std::string& filename);
        
        // A const reference to the sub-tree containing mysql_config is returned
        // Path to mysql_config is taken from the ConfigLayout class
        // Exception is thrown if read-error is encountered
        const boost::property_tree::ptree& getDbConfig();
        
         // A const reference to the sub-tree containing sensors.sensor_name is 
         // returned. Path is taken from the ConfigLayout class
        // Path to mysql_config is taken from the configLayout class
        // Exception is thrown if read-error is encountered
        const boost::property_tree::ptree& getTableConfig(const std::string& sensor_name);
};

#endif /* CONFIGIO_H */

