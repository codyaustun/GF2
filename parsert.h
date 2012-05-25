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

class Parser {
public:
    bool readline();
private:
    bool success = false;
    symbol curSym; // Current symbol
    name curName; // Current name if curSym is a namesym
    int curInt; // Current integer value if curSym is a numsym
    int errors = 0; // Number of errors
    symbol stopSym;
    /* Error Handling
     @effects: Increments 'errors'. prints out current line with scanner::getCurrentLine() and the error message contained in 'message'. 
     Also, advances 'curSym' until equals 'stop'.
     @params: message String for error message.
     @params: stop Symbol require to start parsing again.
     My Reasoning: Inputing both an error message and stop symbol allows for
     more flexible in our error handling. Error messages can be tailored
     to the specific error. Stop symbol ensures the parser can start
     parsing again as fast as possbile. 
     */
    void error(string message, symbol stop); 
    void buildDeviceList();
    void buildConnectionList();
    void buildMonitorList();
    void device();
    void connection();
    void monitor();
    
    
    
    /* Implemented in Scanner
     void getSymbol(symbol& s, // The sort of symbol read from the file 
     name& id,  // return the name here if it has one
     int& num); // return the value here if it's a number
     
     string getCurrentLine(); // Returns current input line
     */
    
    
};


#endif
