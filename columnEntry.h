
/*
 * Henrik Østergaard, 28-Aug-2016
 * 
 * This template class defines a container holding one of 3 user-defined 
 * types and a title corresponding to the column-title in a mySQL database
 * 
 * Note: since this is a template class, the corresponding .cpp file is 
 * explicitly included at the very end of this header file
 */

#ifndef COLUMNENTRY_H
#define COLUMNENTRY_H

#include <cstdlib>
#include <string>

template<typename T1, typename T2, typename T3>
class ColumnEntry
{
    private:
        std::string columnTitle;
        enum {TYPE_T1, TYPE_T2, TYPE_T3, TYPE_UNDEF} readoutType;
        T1 t1_val { };
        T2 t2_val { };
        T3 t3_val { }; 
        bool valueSet { false };
        
    public:
        ColumnEntry(const std::string &name) 
                    : columnTitle(name), readoutType(TYPE_UNDEF) { }
                    
        bool setValue(const T1 &val);
        bool setValue(const T2 &val);
        bool setValue(const T3 &val);
        
        // Exception is thrown (value -1) if mishandled
        void operator=(const T1 &val);
        void operator=(const T2 &val);
        void operator=(const T3 &val);
        
        // returns empty string ("") if a value has not been assigned
        std::string getValueStr() const;
        
        std::string getTitle() const;
        
        // assigned value is cleared, while the type is retained
        void flush();
};

// since ColumnEntry is a template class inclusion of the definition .cpp 
// file is needed

#include "columnEntry.cpp"

#endif /* COLUMNENTRY_H */

