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
    scanner s1(2,2,2);
    
    parser p1(s1);
    
    bool output = p1.readline();
    
    // For testing
    cout << "Was the file correct " << output << endl;
    cout << "test over" << endl;
    
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
        
        
        // Check syntax rule that Connection list should be second
        // Create connection list
        if (curSym == CONSYM) {
            scan.getSymbol(curSym, curName, curInt);
            buildConnectionList();
        }else{
            stopSym = COLON;
            error("The connections list should follow the devices list", stopSym);
            buildConnectionList();
        }
        
        
        
        // Check syntax rule that Monitor list should be third
        // Create Monitor List
        if (curSym == MONSYM) {
            scan.getSymbol(curSym, curName, curInt);
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
        cout << "Detected an invalid symbol. " << endl;
    }
    cout << message << endl;
    
    // Advance to the next instance of the stop symbol
    while ((curSym != stop) && (curSym != EOFSYM)) {
        scan.getSymbol(curSym, curName, curInt);
    }
    
    
    // TO DO remove this line. For debugging only
    cout << "Exiting error function" << endl;

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
        cout << "Detected an invalid symbol. ";
    }
    cout << message << endl;
    
    // Advance to the next instance of the stop symbol
    // For best performance make less likely stop symbol stop2
    while ((curSym != stop1) && (curSym != stop2) && (curSym != EOFSYM)) {
        scan.getSymbol(curSym, curName, curInt);
    }
    
    // TO DO remove this line. For debugging only
    cout << "Exiting error function" << endl;
    
    // Throw an exception in the case curSym == EOFSYM
    if (curSym == EOFSYM) {
        throw runtime_error("Got to the end of the file");
    }
}



void parser::buildDeviceList()
{
    // EBNF: “DEVICES:” {device’,’} device’;’ 
    
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
    
    // check for SEMICOLON
    if (curSym != SEMICOL) {
        stopSym = SEMICOL;
        error("There should be a ';' at the end of the devices section.",
              stopSym);
    }
    scan.getSymbol(curSym, curName, curInt);

}



void parser::buildConnectionList()
{
    // EBNF: “CONNECTIONS:” {connection’,’} connection’;’ 
    
    // check for colon
    if (curSym == COLON) {
        scan.getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'CONNECTIONS'.",
              stopSym);
    }

    
    connection();
    while (curSym == COMMA) {
        scan.getSymbol(curSym, curName, curInt);
        connection();
    }
    
    // check for SEMICOLON
    if (curSym != SEMICOL) {
        stopSym = SEMICOL;
        error("There should be a ';' at the end of the connections section.",
              stopSym);
    }
    scan.getSymbol(curSym, curName, curInt);
}

void parser::buildMonitorList()
{
    // EBNF: “MONITORS:” {monitor’,’} monitor’;’
    
    // check for colon
    if (curSym == COLON) {
        scan.getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'MONITORS'.",
              stopSym);
    }
    
    monitor();
    
    while (curSym == COMMA) {
        scan.getSymbol(curSym, curName, curInt);
        monitor();
    }
    
    // check for SEMICOLON
    if (curSym != SEMICOL) {
        stopSym = SEMICOL;
        error("There should be a ';' at the end of the monitors section.",
              stopSym);
    }
    scan.getSymbol(curSym, curName, curInt);
}

void parser::device() throw (runtime_error)
{   
    // EBNF: device = name "=" type ['$' option] (',' or ';')
    /* OLD WAY without exceptions
    
    nameCheck();
    
    // Outer if statement prevents double error reporting
    // TO DO Try to find a better way of doing this.
    // Maybe do exceptions... See if there is any memory I need to release.
    // Possibly need to release information that is saved for actually creating the 
    // device.
    if (curSym != COMMA && curSym != SEMICOL){
        if (curSym == EQUALS){
            scan.getSymbol(curSym, curName, curInt);
            name devType = type();
            // TO ASK will devType be changed when getSymbol is called?
            // Outer if statement prevents double error reporting
            if (curSym != COMMA && curSym != SEMICOL){
                scan.getSymbol(curSym, curName, curInt);
                if (curSym == DOLLAR) {
                    scan.getSymbol(curSym, curName, curInt);
                    option(devType);
                    
                    // TO DO build device
                    
                    scan.getSymbol(curSym, curName, curInt);
                    
                }else{
                    stopSym = COMMA;
                    stopSym2 = SEMICOL;
                    error("Expected a dollar sign.", stopSym,
                          stopSym2);  
                }
            }
        }else{
            stopSym = COMMA;
            stopSym2 = SEMICOL;
            error("Expected an equals sign.", stopSym, stopSym2);
        }
    }
    */
    
    try {
        
        // check for name
        nameCheck();
        
        // check for equals sign
        if (curSym == EQUALS){
            scan.getSymbol(curSym, curName, curInt);
            
            // check type
            name devType = type();
            
            // check for dollar sign
            if (curSym == DOLLAR) {
                scan.getSymbol(curSym, curName, curInt);
                
                // check for device option
                option(devType);
                
                // TO DO build device
                
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
    } catch (runtime_error e) {
        if (curSym != COMMA && curSym != SEMICOL){
            // rethrow in case of EOFSYM error
            throw;
        }
        
        // TO DO remove this line. Only need for Debugging
        cout << e.what() << endl;
        // TO DO add cleaning
    }
    
}

void parser::connection() throw (runtime_error)
{
    // EBNF: connection = name ‘.’ signal ‘-’ name ‘.’ signal (',' | ‘;’)
    
    try {
        // check for first name
        nameCheck();
        
        // check for first period
        if(curSym == PERIOD){
            scan.getSymbol(curSym, curName, curInt);
            
            // check signal
            signalCheck();
            
            // check for dash
            if (curSym == DASH) {
                scan.getSymbol(curSym, curName, curInt);
                
                // check for second name
                nameCheck();
                
                // check for second period
                if(curSym == PERIOD){
                    scan.getSymbol(curSym, curName, curInt);
                    
                    // check for second signal
                    signalCheck();
                    
                    // TO DO make connection
                    
                    
                }else{
                    stopSym = COMMA;
                    stopSym2 = SEMICOL;
                    error("Expected a period.", stopSym, stopSym2);
                }
            }else{
                stopSym = COMMA;
                stopSym2 = SEMICOL;
                error("Expected a dash.", stopSym, stopSym2); 
            }
        }else{
            stopSym = COMMA;
            stopSym2 = SEMICOL;
            error("Expected a period.", stopSym, stopSym2);
        }
    } catch (runtime_error e) {
        if (curSym != COMMA && curSym != SEMICOL){
            // rethrow in case of EOFSYM error
            throw;
        }
        
        // TO DO remove this line. Only need for Debugging
        cout << e.what() << endl;
        // TO DO add cleaning
    }
}

void parser::monitor() throw (runtime_error)
{
    // EBNF: monitor = [name  ‘=’] name ‘.’ signal (',' | ‘;’)
    
    try {
        // TO DO
        // check for name
        nameCheck();
        
        if(curSym == EQUALS){
            scan.getSymbol(curSym, curName, curInt);
            // check for second name
            nameCheck();
        }
        
        if (curSym == PERIOD) {
            scan.getSymbol(curSym, curName, curInt);
            signalCheck();
            
            // TO DO make monitor
            
            
        }else{
            stopSym = COMMA;
            stopSym2 = SEMICOL;
            error("Expected a period or equals sign", stopSym,
                  stopSym2); 
        }
        
        
    } catch (runtime_error e) {
        if (curSym != COMMA && curSym != SEMICOL){
            // rethrow in case of EOFSYM error
            throw;
        }
        
        // TO DO remove this line. Only need for Debugging
        cout << e.what() << endl;
        // TO DO add cleaning
    }
    
}

void parser::nameCheck() throw (runtime_error)
{
    // TO DO figure out a way to tell between monitors and devices
    // EBNF: letter {letter|digit}
    
    if(curSym == NAMESYM){
        name newName = curName;
        
        // TO DO check semantically if name is okay.
        
        scan.getSymbol(curSym, curName, curInt);
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a name", stopSym,
              stopSym2);
        throw runtime_error("Skipping to next line");
    }
}

void parser::signalCheck() throw (runtime_error)
{
    // EBNF: (“O”|”I”(numberlt17not0)|”DATA”|”CLK”|”SET”|”CLEAR”|’Q’|”QBAR”)
    
    if(curSym == SIGSYM){
        name devSignal = curName;
        
        // TO DO check semantically if name is okay.
        
        scan.getSymbol(curSym, curName, curInt);
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected an input or output", stopSym,
              stopSym2);
        throw runtime_error("Skipping to next line");
    }
    
    
}

name parser::type() throw (runtime_error)
{
    // EBNF: type = (“CLOCK”|”SWITCH”|”AND”|”NAND”|”OR”|”NOR”|”DTYPE”|”XOR”)
    
    if (curSym == TYPESYM) {
        name devType = curName;
        
        // TO DO check semantically if type is okay
        
        // TO ASK will devType be changed when getSymbol is called?
        scan.getSymbol(curSym, curName, curInt);
        return devType;
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a device type.", stopSym, stopSym2);
        throw runtime_error("Skipping to next line");
    }
}

void parser::option(name devType) throw (runtime_error)
{
    //EBNF: digit {digit}
    
    // type is the device type. Use for semantic check.
    
    if (curSym == NUMSYM) {
        int option = curInt;
        
        // TO DO check semantically if option is okay
        
        scan.getSymbol(curSym, curName, curInt);
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a device option.", stopSym, stopSym2); 
        throw runtime_error("Skipping to next line");
    }
}