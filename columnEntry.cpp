/* 
 * File:   ColumnEntry.cpp
 * Author: Henrik
 * 
 * Created on 28. august 2016, 11:56
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>

#include "columnEntry.h"

template<typename T1, typename T2, typename T3>
bool ColumnEntry<T1,T2,T3>::setValue(const T1 &val)
{
    if (readoutType == TYPE_T1 || readoutType == TYPE_UNDEF)
    {
        t1_val = val;
        readoutType = TYPE_T1;
        // std::cout << "Assignment of type: " << typeid(t1_val).name() << ", with value: " 
        //           << t1_val << std::endl; 
        valueSet = true;
        return true;
    } else
        return false;
}

template<typename T1, typename T2, typename T3>
bool ColumnEntry<T1,T2,T3>::setValue(const T2 &val)
{
    if (readoutType == TYPE_T2 || readoutType == TYPE_UNDEF)
    {
        t2_val = val;
        readoutType = TYPE_T2;
        // std::cout << "Assignment of type: " << typeid(t2_val).name() << ", with value: " 
        //           << t2_val << std::endl; 
        valueSet = true;
        return true;
    } else
        return false;
}

template<typename T1, typename T2, typename T3>
bool ColumnEntry<T1,T2,T3>::setValue(const T3 &val)
{
    if (readoutType == TYPE_T3 || readoutType == TYPE_UNDEF)
    {
        t3_val = val;
        readoutType = TYPE_T3;
        // std::cout << "Assignment of type: " << typeid(t3_val).name() << ", with value: " 
        //           << t3_val << std::endl; 
        valueSet = true;
        return true;
    } else
        return false;
}

template<typename T1, typename T2, typename T3>
void ColumnEntry<T1,T2,T3>::operator=(const T1 &val) 
{ 
    if (readoutType == TYPE_T1 || readoutType == TYPE_UNDEF)
    {
        t1_val = val;
        readoutType = TYPE_T1;
        valueSet = true;
        // std::cout << "Assignment of type " << typeid(t1_val).name() << " using overloaded () operator. The value is: " 
        //           << t1_val << std::endl; 
    } else
        throw -1;
}

template<typename T1, typename T2, typename T3>
void ColumnEntry<T1,T2,T3>::operator=(const T2 &val) 
{
    if (readoutType == TYPE_T2 || readoutType == TYPE_UNDEF)
    {
        t2_val = val;
        readoutType = TYPE_T2;
        valueSet = true;
        // std::cout << "Assignment of type " << typeid(t2_val).name() << " using overloaded () operator. The value is: " 
        //           << t2_val << std::endl; 
    } else
        throw -1;
}

template<typename T1, typename T2, typename T3>
void ColumnEntry<T1,T2,T3>::operator=(const T3 &val) 
{
    if (readoutType == TYPE_T3 || readoutType == TYPE_UNDEF)
    {
        t3_val = val;
        readoutType = TYPE_T3;
        valueSet = true;
        // std::cout << "Assignment of type " << typeid(t3_val).name() << " using overloaded () operator. The value is: " 
        //           << t3_val << std::endl; 
    } else
        throw -1;
}

template<typename T1, typename T2, typename T3>
std::string ColumnEntry<T1,T2,T3>::getValueStr() const
{
    std::stringstream tmpstr;
    
    // returns empty string ("") if valueSet = false or TYPE_UNDEF
    if (valueSet)
        switch (readoutType)
        {
            case TYPE_T1: 
                tmpstr << t1_val; 
                // std::cout << "getValueStr switch call: TYPE_T1" << std::endl;
                break;
            case TYPE_T2: 
                tmpstr << t2_val; 
                // std::cout << "getValueStr switch call: TYPE_T2" << std::endl;
                break;
            case TYPE_T3: 
                tmpstr << t3_val; 
                // std::cout << "getValueStr switch call: TYPE_T3" << std::endl;
                break;
            case TYPE_UNDEF: 
                tmpstr << "";  
                break;       
        }
    else
        tmpstr << "";
    
    return tmpstr.str();
}

template<typename T1, typename T2, typename T3>
std::string ColumnEntry<T1,T2,T3>::getTitle() const
{ 
    return columnTitle; 
}

template<typename T1, typename T2, typename T3>
void ColumnEntry<T1,T2,T3>::flush()
{
    valueSet = false;
}

