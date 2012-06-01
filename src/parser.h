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
#include "names.h";
#include "devices.h";
#include "network.h";
#include "monitor.h";

using std::runtime_error;

// TO DO come up with a clever way of doing this
typedef enum {
    DEV = 1,
} dom;

struct deviceTemp{
    name n;
    int type;
    int option;
};

class parser {
public:
    bool readline();
    // Proper constructor
    parser(network* net, devices* dev, monitor* mon, scanner* s);
private:
    bool ok;          // Output bool
    symbol curSym;    // Current symbol
    name curName;     // Current name if curSym is a name, type, or sig symbol
    int curInt;       // Current integer value if curSym is a numsym
    int errorCount;   // Number of errors
    symbol stopSym;   // First stop symbol for error control
    symbol stopSym2;  // Second stop symbol for error control
    scanner scan;     // Scanner for getting symbols
    // TO DO change all references to scan to snz
    scanner* snz;     // Scanner for getting symbols
    names* nmz;       // Names table with all names
    network* netz;
    devices* devz;
    monitor* monz;
    
    // Needed for semantic error control
    vector<name> devNames;     // Vector of successfully create device names
    vector<deviceTemp> madeD;   // Vector of successfully created devices
    

    
    // Needed for error reporting
    void error(string message, symbol stop) throw (runtime_error); 
    void error(string message, symbol stop1, symbol stop2) throw (runtime_error);
    void nextLine(string message);
    
    // Needed for structure and syntax error handling
    void buildDeviceList();
    void buildConnectionList();
    void buildMonitorList();
    void device() throw (runtime_error);
    void connection() throw (runtime_error);
    void mon() throw (runtime_error);
    
    // TO DO see if you could compress this down
    name nameCheck() throw (runtime_error);
    name nameCheck(dom deviceOrMonitor) throw (runtime_error);
    
    name signalCheck(name deviceName) throw (runtime_error);
    name type() throw (runtime_error);
    int option(name type) throw (runtime_error);
    bool nameExist(vector<name> names, name n);
    deviceTemp getDeviceTemp(name d);
};


#endif
