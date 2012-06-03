//
//  Parser.h
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//

#ifndef _Parser_h
#define _Parser_h
#include "scanner.h"
#include <stdexcept>
#include <vector>
#include "names.h"
#include "devices.h"
#include "network.h"
#include "monitor.h"

using std::runtime_error;

// TO DO come up with a clever way of doing this
typedef enum {
    DEV = 1,
} dom;


// Structure to store created devices
struct deviceTemp{
    name n;
    int type;
    int option;
};

// Structure to store used inputs
struct inpTemp{
    name dev;
    name sig;
};

class parser {
public:
    
    bool readin();
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
    scanner* snz;     // Scanner for getting symbols
    network* netz;    // Network for making connections
    devices* devz;    // Devices for making devices
    monitor* monz;    // Monitor instance for making monitors in the network
   
    
    // Needed for semantic error control
    vector<deviceTemp> madeD;   // Vector of successfully created devices
    vector<inpTemp> usedIns;    // Vector of inputs used for connections
    vector<inpTemp> allIns;     // Vector of all inputs. 
    

    
    // Needed for error reporting
    void error(string message, symbol stop) throw (runtime_error); 
    void error(string message, symbol stop1, symbol stop2) throw (runtime_error);
    void nextLine(string message);
    symbol nextSection;
    
    // Needed for structure and syntax error handling
    void buildDeviceList();
    void buildConnectionList();
    void buildMonitorList();
    void device() throw (runtime_error);
    void connection() throw (runtime_error);
    void mon() throw (runtime_error);
    
    // Checks if symbols are semantically ok
    name nameCheck() throw (runtime_error);
    name nameCheck(dom deviceOrMonitor) throw (runtime_error);
    name signalCheck(name deviceName) throw (runtime_error);
    name type() throw (runtime_error);
    int option(name type) throw (runtime_error);
    
    // Helper functions
    bool nameExist(name n);
    deviceTemp getDeviceTemp(name d);
    bool usedInput(name d, name s);
    void symbolToString(symbol s);
};


#endif
