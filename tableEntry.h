/*
 * This template class defines a vector of ColumnEntry type and
 * a title corresponding to the Table-title in a mySQL database
 * 
 * Note: since this is a template class, the corresponding .cpp file is 
 * explicitly included at the very end of this header file
 * 
 * Examples
 * 
 * int main {
 * TableEntry<int,double,std::string> sqlTable("TableTest");
 * sqlTable.addEntry("ColumnTitle1");
 * sqlTable["ColumnTitle1"] = "string";
 * sqlTable["ColumnTitle2"] = 2;
 * (sqlTable = "ColumnTitle3") = 3.0;
 * for (auto &indx : sqlTable)
 *     std::cout << indx.getColumnTitle() << std::endl;
 * }
 */

#ifndef TABLEENTRY_H
#define TABLEENTRY_H

#include <cstdlib>
#include <vector>
#include <string>
// #include <iosfwd>  //contains all forward declarations of iostream

#include <boost/property_tree/ptree.hpp>

#include "columnEntry.h"
#include "configRW.h"

template<typename T1, typename T2, typename T3>
class TableEntry 
{
    private:
        const std::string tableTitle;
        std::vector<ColumnEntry<T1,T2,T3> > entryVec;
        
    public:
        TableEntry (const std::string& name) : tableTitle(name) { }
        
        // if entryTitle already exists, false is returned
        bool addEntry (const std::string& entryTitle);
        
        // add table entries from configuration file
        // false is returned if error (exception) is encountered
        bool addTableConfig (ConfigRW& cfg, const std::string& sensor_name);
        
        // if entryTitle already exists and is not of std::string type
        // false is returned. Otherwise the entry is created and
        // a time stamp string "YYYY-MM-DD HH:MM:SS" is added
        bool addTimeStamp (const std::string& entryTitle = "Time");
        
        // access to vector element using column title
        // if entryTitle is not found, a new entry with the name is created        
        ColumnEntry<T1,T2,T3>& operator[] (const std::string& entryTitle);
        
        // access to vector element using integer index
        // throws std::out_of_range exception if index is out of range
        ColumnEntry<T1,T2,T3>& operator[] (const int& index);
        
        // addition of tableEntry using overloaded output (<<) operator
        template <typename T4, typename T5, typename T6>
        friend TableEntry<T4,T5,T6>& operator<< (TableEntry<T4,T5,T6>& table, 
                                                 const std::string& entryTitle);
        
        // overloading of the input operator >>. Allows for addition of
        // entries to TableEntry class using this operator and an input stream
        // from basic_istream, e.g. istringstream or stringstream.
        // Declaration as friend function allows for access to private 
        // parameters if necessary
        template <typename charT, typename traits, typename T4, typename T5, 
                  typename T6>
        friend std::basic_istream<charT,traits>& operator>> 
            (std::basic_istream<charT,traits>& istr, TableEntry<T4,T5,T6>& table);
        
        
        // enable range-based for loops by delegation of methods to the 
        // underlying std::vector<...> container
        typename std::vector<ColumnEntry<T1,T2,T3> >::iterator begin () 
            { return entryVec.begin(); }
        typename std::vector<ColumnEntry<T1,T2,T3> >::iterator end () 
            { return entryVec.end(); }
        typename std::vector<ColumnEntry<T1,T2,T3> >::const_iterator begin () const
            { return entryVec.begin(); }
        typename std::vector<ColumnEntry<T1,T2,T3> >::const_iterator end () const
            { return entryVec.end(); }
        
        // deletes assigned values for each element in the table, but
        // retains column titles and consequently the vector size
        void flush();
        
        int getSize () const;
        
        std::string getTitle() const;
};

// since TableEntry is a template class inclusion of the definition .cpp 
// file is needed and therefore should not be included in the project

#include "tableEntry.cpp"

#endif /* TABLEENTRY_H */

