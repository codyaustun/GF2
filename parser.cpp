//
//  Parser.cpp
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//

#include "parser.h"


parser::parser(scanner s)
{
    errorCount = 0;
    scan = s;
}

int main(){
    scanner s1;
    
    parser p1(s1);
    
    p1.readline();
    
}

bool parser::readline()
{
    try {
        // Get first Symbol
        scan.getSymbol(curSym, curName, curInt);
        
        // Check syntax rule that device list should be first
        // Create device list
        if (curSym == DEVSYM) {
            scan.getSymbol(curSym, curName, curInt);
            buildDeviceList();
        }else{
            stopSym = COLON;
            error("The devices list must come first.", stopSym);
            buildDeviceList();
        }
        
        // For testing
        cout << "test over" << endl;
        return false;
        
        // Check syntax rule that Connection list should be second
        // Create connection list
        if (curSym == CONSYM) {
            buildConnectionList();
        }else{
            stopSym = COLON;
            error("The connections list should follow the devices list", stopSym);
            buildConnectionList();
        }
        
        
        
        // Check syntax rule that Monitor list should be third
        // Create Monitor List
        if (curSym == MONSYM) {
            buildMonitorList();
        }else{
            stopSym = COLON;
            error("The monitors list should follow the connections list", stopSym);
            buildMonitorList();
        }
        
        // Check syntax rule that End of file symbol should be fourth
        if (curSym == FINSYM) {
            return (errorCount == 0);
        }else{
            stopSym = EOFSYM;
            error("'FIN' should follow the monitors list", stopSym);
            return false;
        }
    }
    // Handling the case where the error function gets to the end of file.
    // No point in continuing to parse at that point.
    catch ( runtime_error e ){
        cout << e.what() << endl;
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
void parser::error(string message, symbol stop) throw (runtime_error)
{
    
    // Increment error count
    errorCount++; 
    
    // Scanner should print out current line
    scan.getCurrentLine(); 
    
    // Print error message
    // TO DO make this sounds better
    cout << "Error (" << errorCount << "): ";
    if (curSym == BADSYM) {
        cout << "Detected an invalid symbol." << endl;
    }
    cout << message << endl;
    
    // Advance to the next instance of the stop symbol
    while ((curSym != stop) && (curSym != EOFSYM)) {
        scan.getSymbol(curSym, curName, curInt);
    }

    // an exception in the case curSym == EOFSYM
    if (curSym == EOFSYM) {
        throw runtime_error("Got to the end of the file");
    }
}

// Same as the other error method, but handles the case where there 2 possible
// stopping symbols.
// EX: an error in a device could start again after a colon or semicolon.
void parser::error(string message, symbol stop1, symbol stop2) throw (runtime_error)
{
    
    // Increment error count
    errorCount++; 
    
    // Scanner should print out current line
    scan.getCurrentLine(); 
    
    
    
    // Print error message
    // TO DO make this sounds better
    cout << "Error (" << errorCount << "): ";
    if (curSym == BADSYM) {
        cout << "Detected an invalid symbol.";
    }
    cout << message << endl;
    
    // Advance to the next instance of the stop symbol
    // For best performance make less likely stop symbol stop2
    while ((curSym != stop1) && (curSym != stop2) && (curSym != EOFSYM)) {
        scan.getSymbol(curSym, curName, curInt);
    }
    
    // Throw an exception in the case curSym == EOFSYM
    if (curSym == EOFSYM) {
        throw runtime_error("Got to the end of the file");
    }
}



void parser::buildDeviceList()
{
    
    // check for colon
    if (curSym == COLON) {
        scan.getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'DEVICES'.",
              stopSym);
    }
    device();
    while (curSym == COMMA) {
        scan.getSymbol(curSym, curName, curInt);
        device();
    } 
    
    // TO DO check for SEMICOLON
    // This maybe unneeded depending on how I implement device()
    if (curSym != SEMICOL) {
        stopSym = SEMICOL;
        error("There should be a ';' at the end of the devices section.", stopSym);
    }
    scan.getSymbol(curSym, curName, curInt);

}

void parser::device()
{   
    // Throw exception to prevent double error reporting
    nameCheck();
    
    // Outer if statement prevents double error reporting
    if (curSym != COMMA && curSym != SEMICOL){
        if (curSym == EQUALS){
            scan.getSymbol(curSym, curName, curInt);
            type();
            if (curSym == DOLLAR) {
                scan.getSymbol(curSym, curName, curInt);
                option();
            }else{
                stopSym = COMMA;
                stopSym2 = SEMICOL;
                error("Expected a dollar sign.", stopSym,
                      stopSym2);  
            }
        }else{
            stopSym = COMMA;
            stopSym2 = SEMICOL;
            error("Expected an equals sign.", stopSym, stopSym2);
        }
    }
    
}

void parser::buildConnectionList()
{
    
    // check for colon
    if (curSym != COLON) {
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'CONNECTIONS'.",
              stopSym);
    }

    
    connection();
    while (curSym == COMMA) {
        scan.getSymbol(curSym, curName, curInt);
        connection();
    }
    
    // TO DO check for SEMICOLON
}

void parser::buildMonitorList()
{
    // check for colon
    if (curSym != COLON) {
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'MONITORS'.",
              stopSym);
    }
    
    monitor();
    
    while (curSym == COMMA) {
        scan.getSymbol(curSym, curName, curInt);
        monitor();
    }
    
    // TO DO check for SEMICOLON
}



void parser::connection()
{
    
}

void parser::monitor()
{
    
}

void parser::nameCheck(){
    if(curSym == NAMESYM){
        name newName = curName;
        
        // TO DO checkly semantically if name is okay.
        
        scan.getSymbol(curSym, curName, curInt);
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a name", stopSym,
              stopSym2);   
    }
}

void parser::type(){
    
}

void parser::option(){
    
}