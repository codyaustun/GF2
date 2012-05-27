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

class parser {
public:
    bool readline();
    parser(scanner s);
private:
    symbol curSym; // Current symbol
    name curName; // Current name if curSym is a namesym
    int curInt; // Current integer value if curSym is a numsym
    int errorCount; // Number of errors
    symbol stopSym;
    scanner scan; // Scanner for getting symbols
    
   
    void error(string message, symbol stop); 
    void buildDeviceList();
    void buildConnectionList();
    void buildMonitorList();
    void device();
    void connection();
    void monitor();
    
};


#endif
