//
//  Parser.h
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//

#ifndef _Parser_h
#define _Parser_h
#include "CCScanner.h"
#include <stdexcept>;
#include <vector>;
#include "src/names.h";

using std::runtime_error;

typedef enum {
    DEV = 1,
    MON = 0
} dom;

struct deviceTemp{
    namestring name;
    int type;
    int option;
};

class parser {
public:
    bool readline();
    parser(scanner s);
    /* TO DO hook up to names.cpp
    parser(scanner s, names n);
    */
private:
    symbol curSym;    // Current symbol
    name curName;     // Current name if curSym is a namesym
    int curInt;       // Current integer value if curSym is a numsym
    int errorCount;   // Number of errors
    symbol stopSym;   // First stop symbol for error control
    symbol stopSym2;  // Second stop symbol for error control
    scanner scan;     // Scanner for getting symbols
    
    /* TO DO hook up to names.cpp
    names namesTable; // Names table with all names
    */
    
    // Needed for semantic error control
    vector<string> devNames;
    vector<string> monNames;
    vector<deviceTemp> madeD;
    

    
    // Needed for syntax error control
    void error(string message, symbol stop) throw (runtime_error); 
    void error(string message, symbol stop1, symbol stop2) throw (runtime_error);
    void buildDeviceList();
    void buildConnectionList();
    void buildMonitorList();
    void device() throw (runtime_error);
    void connection() throw (runtime_error);
    void monitor() throw (runtime_error);
    
    // TO DO see if you could compress this down
    namestring nameCheck() throw (runtime_error);
    void nameCheck(dom deviceOrMonitor) throw (runtime_error);
    namestring nameCheck(namestring d) throw (runtime_error);
    
    void signalCheck(namestring deviceName) throw (runtime_error);
    name type() throw (runtime_error);
    void option(name type) throw (runtime_error);
    bool nameExist(vector<string> names, namestring str);
    deviceTemp getDeviceTemp(namestring d);
};


#endif
