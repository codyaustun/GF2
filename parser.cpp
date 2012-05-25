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
    scan.getSymbol(curSym, curName, curInt);
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
    
}

void parser::buildConnectionList()
{
    
}

void parser::buildMonitorList()
{
    
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
