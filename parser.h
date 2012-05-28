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

using std::runtime_error;

typedef enum {
    DEV = 1,
    MON = 0
} dom;

class parser {
public:
    bool readline();
    parser(scanner s);
private:
    symbol curSym;  // Current symbol
    name curName;   // Current name if curSym is a namesym
    int curInt;     // Current integer value if curSym is a numsym
    int errorCount; // Number of errors
    symbol stopSym; // First stop symbol for error control
    symbol stopSym2;// Second stop symbol for error control
    scanner scan;   // Scanner for getting symbols
    
    // Needed for semantic error control
    vector<string> devNames;
    vector<string> monNames;
    
    struct deviceTemp{
        string name;
        int type;
        int option;
    };
    
    // Needed for syntax error control
    void error(string message, symbol stop) throw (runtime_error); 
    void error(string message, symbol stop1, symbol stop2) throw (runtime_error);
    void buildDeviceList();
    void buildConnectionList();
    void buildMonitorList();
    void device() throw (runtime_error);
    void connection() throw (runtime_error);
    void monitor() throw (runtime_error);
    void nameCheck(dom deviceOrMonitor) throw (runtime_error);
    void signalCheck() throw (runtime_error);
    name type() throw (runtime_error);
    void option(name type) throw (runtime_error);
};


#endif
