//
//  Parser.cpp
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//

#include "parser.h"

/* TO DO hook up to names.cpp
parser::parser(scanner s, names n)
{
    errorCount = 0;
    scan = s;
    namesTable = n;
}
 */

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
        stopSym = curSym;
        error("There should be a ';' at the end of the devices section.",
              stopSym);
    }
    
    // TO DO decide if we need this line
    //scan.getSymbol(curSym, curName, curInt);

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
        nameCheck(DEV);
        
        // check for equals sign
        if (curSym == EQUALS){
            scan.getSymbol(curSym, curName, curInt);
            
            // check type
            name devType = type();
            
            
            // XOR doesn't have options
            if (devType != 11){
            // check for dollar sign
                if (curSym == DOLLAR) {
                    scan.getSymbol(curSym, curName, curInt);
                    
                    // check for device option
                    option(devType);
            }
                
                // TO DO build device
                // add name to devNames
                // create deviceTemp
                // when creating deviceTemp subtract 4 from devType
                
                
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
        namestring devName1 = nameCheck();
        
        // check for first period
        if(curSym == PERIOD){
            scan.getSymbol(curSym, curName, curInt);
            
            // check signal
            signalCheck(devName1);
            
            // check for dash
            if (curSym == DASH) {
                scan.getSymbol(curSym, curName, curInt);
                
                // check for second name
                namestring devName2 = nameCheck();
                
                // check for second period
                if(curSym == PERIOD){
                    scan.getSymbol(curSym, curName, curInt);
                    
                    // check for second signal
                    signalCheck(devName2);
                    
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
        // TO DO fix this mess
        
        
        /*
        // TO ASK ask team about this
        // check for name
        if (!existsDevNames) {
            if(curSYm == NAMESYM){
                // TO DO hook up to names.cpp
                namestring temp = "temp"; 
                // namestring temp = namesTable.getname(curName);
                nameCheck(MON);
            }
        }
        */
        nameCheck(MON);
        
        
        // TO DO determine how to fix device vs monitor name confusion
        if(curSym == EQUALS){
            scan.getSymbol(curSym, curName, curInt);
            // check for second name
            namestring devName = nameCheck();
        }
        
        
        if (curSym == PERIOD) {
            scan.getSymbol(curSym, curName, curInt);
            
            
            // TO DO fix this
            // signalCheck(devName);
            
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

/*
namestring parser::nameCheck(namestring d) throw (runtime_error){
    
    if (!nameExist(devNames, d)){
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("The device "+d+" does not exist", stopSym,
              stopSym2);
        throw runtime_error("Skipping to next line");
    }
    
    return d;
}
*/

namestring parser::nameCheck() throw (runtime_error)
{
    // EBNF: letter {letter|digit}
    
    if(curSym == NAMESYM){
        
        // TO DO hook up to names.cpp
        namestring newName = "temp"; 
        // namestring newName = namesTable.getname(curName);
        
        if (!nameExist(devNames, newName)){
            stopSym = COMMA;
            stopSym2 = SEMICOL;
            error("The device "+newName+" does not exist", stopSym,
                  stopSym2);
            throw runtime_error("Skipping to next line");
        }
        
        scan.getSymbol(curSym, curName, curInt);
        
        return newName;
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a name", stopSym,
              stopSym2);
        throw runtime_error("Skipping to next line");
    }

}

void parser::nameCheck(dom deviceOrMonitor) throw (runtime_error)
{
    // TO DO figure out a way to tell between monitors and devices
    // EBNF: letter {letter|digit}
    
    if(curSym == NAMESYM){
        // TO DO possibly change this to used name id
        // TO DO hook up to names.cpp
        namestring newName = "temp"; 
        // namestring newName = namesTable.getname(curName);
        
        
        // TO DO Test this
        // check semantically if name is okay.
        switch (deviceOrMonitor) {
            // Don't want device to exist
            case DEV:
                // Does not add name to monNames.
                // Calling function must do that.
                if (nameExist(devNames, newName)){
                    stopSym = COMMA;
                    stopSym2 = SEMICOL;
                    error("The name "+newName+" has already been used", stopSym,
                          stopSym2);
                    throw runtime_error("Skipping to next line");
                }
                break;
                
            // Don't want Monitor to exist
            case MON:
                // Does not add name to monNames.
                // Calling function must do that.
                if (nameExist(monNames, newName)){
                    stopSym = COMMA;
                    stopSym2 = SEMICOL;
                    error("The name "+newName+" has already been used", stopSym,
                          stopSym2);
                    throw runtime_error("Skipping to next line");
                }
                break;
                
            default:
                stopSym = COMMA;
                stopSym2 = SEMICOL;
                error("Unexpected error", stopSym,
                      stopSym2);
                throw runtime_error("Skipping to next line");
                break;
        }
        
        
        scan.getSymbol(curSym, curName, curInt);
     
    // TO DO figure out a nicer way to do this
    }else if((curSym == TYPESYM) || (curSym == SIGSYM) || (curSym == CONSYM) ||
             (curSym == DEVSYM) || (curSym == MONSYM)){
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Keywords cannot be names", stopSym,
              stopSym2);
        throw runtime_error("Skipping to next line");
        
    }
    else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a name", stopSym,
              stopSym2);
        throw runtime_error("Skipping to next line");
    }
}

bool parser::nameExist(vector<string> names, namestring str)
{
    bool found = 0;
    
    for (vector<string>::iterator i = names.begin(); i != names.end(); ++i) {
        if (str == (*i)) {
            found = 1;
        }
    }
    return found;
}

// TO DO remove this
/*
bool parser::nameExist(vector<deviceTemp> devices, deviceTemp d){
    bool found = 0;
    
    for (vector<deviceTemp>::iterator i = devices.begin(); i != devices.end(); ++i) {
        if (d.name == i->name) {
            found = 1;
        }
    }
    return found;
}
*/


void parser::signalCheck(namestring deviceName) throw (runtime_error)
{
    // EBNF: (“O”|”I”(numberlt17not0)|”DATA”|”CLK”|”SET”|”CLEAR”|’Q’|”QBAR”)
    
    
    // device must exists by the time this function is called
    if(curSym == SIGSYM){
        name devSignal = curName;
        
        
        // TO DO check semantically if name is okay.
        // ONLY need to check if data type is right
        deviceTemp dev = getDeviceTemp(deviceName);
        switch (dev.type) {
            case 0:
                if (devSignal != 12){
                    nextLine("A clock only has an output.");
                }
                break;
            case 1:
                if (devSignal != 12){
                    nextLine("A switch only has an output.");
                }
                break;
            case 2:
                if ((devSignal > 11) && (devSignal < 29)) {
                    if (dev.option < devSignal-12){
                        // TO DO figure out how to do this with Niran
                    }
                }else{
                    nextLine("A device only has standard inputs and outputs.");
                }
                break;
            case 3:
                // TO DO same as case 2
                break;
            case 4:
                // TO DO same as case 2
                break;
            case 5:
                // TO DO same as case 2
                break;
            case 6:
                // TO DO
                break;
            case 7:
                if (!((devSignal > 11) && (devSignal < 15))){
                    nextLine("A XOR only has 1 output and 2 inputs.");
                }
                break;
                
            default:
                nextLine("Unexpected error");
                break;
        }
       
        
        
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
        // Done by definition
        
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
        switch (devType) {
            case 4:
                // TO DO Figure out actual condition
                if (option != 0){
                    nextLine("A clock does not have this option.");
                }
                break;
            case 5:
                // TO DO
                // TO DO Figure out actual condition
                if (option != 0){
                    nextLine("A switch does not have this option.");
                }
                break;
            case 6:
                // TO DO
                // TO DO Figure out actual condition
                if (option != 0){
                    nextLine("An AND does not have this option.");
                }
                break;
            case 7:
                // TO DO
                // TO DO Figure out actual condition
                if (option != 0){
                    nextLine("A NAND does not have this option.");
                }
                break;
            case 8:
                // TO DO
                // TO DO Figure out actual condition
                if (option != 0){
                    nextLine("A OR does not have this option.");
                }
                
                break;
            case 9:
                // TO DO
                // TO DO Figure out actual condition
                if (option != 0){
                    nextLine("A NOR does not have this option.");
                }
                break;
            case 10:
                // TO DO
                // TO DO Figure out actual condition
                if (option != 0){
                    nextLine("A DTYPE does not have this option.");
                }
                break;
            case 11:
                // TO DO
                // TO DO Figure out actual condition
                if (option != 0){
                    nextLine("A XOR does not have this option.");
                }
                break;
                
            default:
                nextLine("Unexpected error");
                break;
        }
        
        scan.getSymbol(curSym, curName, curInt);
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a device option.", stopSym, stopSym2); 
        throw runtime_error("Skipping to next line");
    }
}

deviceTemp parser::getDeviceTemp(namestring d){
    
    for (vector<deviceTemp>::iterator i = madeD.begin(); i != madeD.end(); ++i) {
        if (d == i->name) {
            deviceTemp dev = (*i);
            return dev;
        }
    }
    
    cout << "UNEXPECTED ERROR IN getDeviceTemp" << endl;
    
   
    
}


// TO DO replace old method with this
void parser::nextLine(string message){
    stopSym = COMMA;
    stopSym2 = SEMICOL;
    error(message, stopSym,
          stopSym2);
    throw runtime_error("Skipping to next line");
}