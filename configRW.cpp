/* 
 * File:   configIO.cpp
 * Author: Henrik
 * 
 * Created on 13. september 2016, 21:16
 */

#include <iostream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "configRW.h"


ConfigRW::ConfigRW(const std::string& filename)
{
    boost::property_tree::read_json(filename, tree);  // exception thrown upon failure
}

const boost::property_tree::ptree& ConfigRW::getDbConfig()
{
    
    std::cout << "Config path is: " << mysql_config << std::endl;
    return tree.get_child(mysql_config);  // exception thrown upon failure
}

const boost::property_tree::ptree& ConfigRW::getSensorConfig(const std::string& sensor_name)
{
 
    std::string path { sensors + '.' + sensor_name };
    std::cout << "Sensor path is: " << path << std::endl;
    return tree.get_child(path);  // exception thrown upon failure

}


