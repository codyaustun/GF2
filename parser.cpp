//
//  Parser.cpp
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//

#include <iostream>
#include "parser.h"


parser::parser(scanner s)
{
    outputBool = false;
    errorCount = 0;
    scan = s;
}

bool parser::readline()
{
    // Get first Symbol
    scan.getSymbol(curSym, curName, curInt);
    
    // Check syntax rule that device list should be first
    // Create device list
    if (curSym == DEVSYM) {
        buildDeviceList();
    }else{
        stopSym = DEVSYM;
        error("The devices list must come first.", stopSym);
        buildDeviceList();
    }
    
    // Check syntax rule that Connection list should be second
    // Create connection list
    if (curSym == CONSYM) {
        buildConnectionList();
    }else{
        stopSym = DEVSYM;
        error("The connections list should follow the devices list", stopSym);
        buildConnectionList();
    }
    
    // Check syntax rule that Monitor list should be third
    // Create Monitor List
    if (curSym == MONSYM) {
        buildMonitorList();
    }else{
        stopSym = MONSYM;
        error("The monitors list should follow the connections list", stopSym);
        buildMonitorList();
    }
    
    // Check syntax rule that End of file symbol should be fourth
    if (curSym == EOFSYM) {
        return (errorCount == 0);
    }else{
        stopSym = EOFSYM;
        error("'FIN' should follow the monitors list", stopSym);
        return false;
    }
}

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
void parser::error(string message, symbol stop)
{
    
}

void parser::buildDeviceList()
{
    scan.getSymbol(curSym, curName, curInt); 
    device();
    while (curSym == SEMICOL) {
        scan.getSymbol(curSym, curName, curInt);
        if (curSym == CONSYM) {
            break;
        }
        device();
    } 
}

void parser::buildConnectionList()
{
    scan.getSymbol(curSym, curName, curInt); 
    connection();
    while (curSym == SEMICOL) {
        scan.getSymbol(curSym, curName, curInt);
        if (curSym == MONSYM) {
            break;
        }
        connection();
    }
}

void parser::buildMonitorList()
{
    scan.getSymbol(curSym, curName, curInt); 
    monitor();
    while (curSym == SEMICOL) {
        scan.getSymbol(curSym, curName, curInt);
        if (curSym == EOFSYM) {
            break;
        }
        monitor();
    }
}

void parser::device()
{
    
}

void parser::connection()
{
    
}

void parser::monitor()
{
    
}
