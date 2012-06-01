//
//  Parser.cpp
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//

#include "parser.h"



parser::parser(network* net, devices* dev, monitor* mon, scanner* s)
{
    errorCount = 0;
    snz = s;
    netz = net;
    devz = dev;
    monz = mon;
}






bool parser::readin()
{
    try {
        // Get first Symbol
        snz->getSymbol(curSym, curName, curInt);
        
        // Check syntax rule that device list should be first
        // Create device list
        if (curSym == DEVSYM) {
            snz->getSymbol(curSym, curName, curInt);
            buildDeviceList();
        }else{
            stopSym = COLON;
            error("The devices list must come first.", stopSym);
            buildDeviceList();
        }
        
        
        // Check syntax rule that Connection list should be second
        // Create connection list
	snz->getSymbol(curSym, curName, curInt);
        if (curSym == CONSYM) {
            snz->getSymbol(curSym, curName, curInt);
            buildConnectionList();
        }else{
            stopSym = COLON;
            error("The connections list should follow the devices list", stopSym);
            buildConnectionList();
        }
        
        // Check syntax rule that Monitor list should be third
        // Create Monitor List
	symbolToString(curSym);
	cout << endl;
	snz->getSymbol(curSym, curName, curInt);
	symbolToString(curSym);
	cout << endl;
        if (curSym == MONSYM) {
            snz->getSymbol(curSym, curName, curInt);
            buildMonitorList();
        }else{
            stopSym = COLON;
            error("The monitors list should follow the connections list", stopSym);
            buildMonitorList();
        }
        
        // Check syntax rule that End of file symbol should be fourth
	snz->getSymbol(curSym, curName, curInt);
        if (curSym == FINSYM) {
            
            // TO DO decide if it should be both
            netz->checknetwork(ok);
            return ((errorCount == 0) && ok);
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
    
    // TO DO Add check network
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
    snz->getCurrentLine(); 
    
    // Print error message
    // TO DO make this sounds better
    cout << "Error (" << errorCount << "): ";
    if (curSym == BADSYM) {
        cout << "Detected an invalid symbol. " << endl;
    }
    cout << message << endl;
    symbolToString(curSym);
    cout << endl;
    
    // Advance to the next instance of the stop symbol
    while ((curSym != stop) && (curSym != EOFSYM)) {
        snz->getSymbol(curSym, curName, curInt);
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
    snz->getCurrentLine(); 
    
    
    
    // Print error message
    // TO DO make this sounds better
    cout << "Error (" << errorCount << "): ";
    if (curSym == BADSYM) {
        cout << "Detected an invalid symbol. ";
    }
    cout << message << endl;
    symbolToString(curSym);
    cout << endl;
    
    // Advance to the next instance of the stop symbol
    // For best performance make less likely stop symbol stop2
    while ((curSym != stop1) && (curSym != stop2) && (curSym != EOFSYM)) {
        snz->getSymbol(curSym, curName, curInt);
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
        snz->getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'DEVICES'.",
              stopSym);
    }
    device();
    while (curSym == COMMA) {
        snz->getSymbol(curSym, curName, curInt);
        device();
    } 
    
    // check for SEMICOLON
    if (curSym != SEMICOL) {
        stopSym = curSym;
        error("There should be a ';' at the end of the devices section.",
              stopSym);
    }

}

void parser::buildConnectionList()
{
    // EBNF: “CONNECTIONS:” {connection’,’} connection’;’ 
    
    // check for colon
    if (curSym == COLON) {
        snz->getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'CONNECTIONS'.",
              stopSym);
    }

    
    connection();
    while (curSym == COMMA) {
        snz->getSymbol(curSym, curName, curInt);
        connection();
    }
    
    // check for SEMICOLON
    if (curSym != SEMICOL) {
        stopSym = curSym;
        error("There should be a ';' at the end of the connections section.",
              stopSym);
    }
    
    if(usedIns.size() != allIns.size()){
        stopSym = curSym;
        error("All inputs must be connected to an output.", stopSym);
    }
    
    // TO DO check to make sure all inputs are connected
}

void parser::buildMonitorList()
{
    // EBNF: “MONITORS:” {monitor’,’} monitor’;’
    
    // check for colon
    if (curSym == COLON) {
        snz->getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'MONITORS'.",
              stopSym);
    }
    
    mon();
    
    while (curSym == COMMA) {
        snz->getSymbol(curSym, curName, curInt);
        mon();
    }
    
    // check for SEMICOLON
    if (curSym != SEMICOL) {
        stopSym = curSym;
        error("There should be a ';' at the end of the monitors section.",
              stopSym);
    }
}

void parser::device() throw (runtime_error)
{   
    // EBNF: device = name "=" type ['$' option] (',' or ';')
    
    try {
        
        // check for name
        name devName = nameCheck(DEV);
        
        // check for equals sign
        if (curSym == EQUALS){
            snz->getSymbol(curSym, curName, curInt);
            
            // check type
            name devType = type();
            
            // create deviceTemp
            deviceTemp newDev;
            // XOR and DTYPE don't have options
            if (devType < 10){
            // check for dollar sign
                if (curSym == DOLLAR) {
                    snz->getSymbol(curSym, curName, curInt);
                    
                    // check for device option
                    int devOpt = option(devType);
                    newDev.option = devOpt;
                }else{
                    nextLine("Expected a dollar sign.");  
                }
            }
            
            if ((curSym == COMMA) || (curSym == SEMICOL)) {
                
                // add name to devNames
                newDev.n = devName;
                // when creating deviceTemp subtract 4 from devType
                newDev.type = (devType - 4);
                madeD.push_back(newDev);
		// TO DO combine madeD devNames
		devNames.push_back(devName);
                switch (newDev.type) {
                    case 0:
                        devz->makedevice(aclock,newDev.n,newDev.option, ok);
                        break;
                    case 1:
                        devz->makedevice(aswitch,newDev.n,newDev.option, ok);
                        break;
                    case 2:
                        devz->makedevice(andgate,newDev.n,newDev.option, ok);
                        for(int x =0; x < newDev.option; x++){
                            inpTemp newSig;
                            newSig.dev = devName;
                            // Plus 12 to get right place in names table.
                            newSig.sig = (x+12);
                            allIns.push_back(newSig);
                        }
                        break;
                    case 3:
                        devz->makedevice(nandgate,newDev.n,newDev.option, ok);
                        for(int x =0; x < newDev.option; x++){
                            inpTemp newSig;
                            newSig.dev = devName;
                            // Plus 12 to get right place in names table.
                            newSig.sig = (x+12);
                            allIns.push_back(newSig);
                        }
                        break;
                    case 4:
                        devz->makedevice(orgate,newDev.n,newDev.option, ok);
                        for(int x =0; x < newDev.option; x++){
                            inpTemp newSig;
                            newSig.dev = devName;
                            // Plus 12 to get right place in names table.
                            newSig.sig = (x+12);
                            allIns.push_back(newSig);
                        }
                        break;
                    case 5:
                        devz->makedevice(norgate,newDev.n,newDev.option, ok);
                        for(int x =0; x < newDev.option; x++){
                            inpTemp newSig;
                            newSig.dev = devName;
                            // Plus 12 to get right place in names table.
                            newSig.sig = (x+12);
                            allIns.push_back(newSig);
                        }
                        break;
                    case 6:
                        devz->makedevice(dtype,newDev.n,newDev.option, ok);
                        for(int x =0; x < 4; x++){
                            inpTemp newSig;
                            newSig.dev = devName;
                            // Plus 12 to get right place in names table.
                            newSig.sig = (x+28);
                            allIns.push_back(newSig);
                        }
                        break;
                    case 7:
                        devz->makedevice(xorgate,newDev.n,newDev.option, ok);
                        for(int x =0; x < 2; x++){
                            inpTemp newSig;
                            newSig.dev = devName;
                            // Plus 12 to get right place in names table.
                            newSig.sig = (x+12);
                            allIns.push_back(newSig);
                        }
                        break;
                        
                    default:
                        break;
                        
                        
                }
                    
            }else{
                nextLine("Expected a ',' or ';'");
            }
            
        }else{
            nextLine("Expected an equals sign.");
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
        name devName1 = nameCheck();
        
            
            // check signal
            name sig1 = signalCheck(devName1);
            
            if (!((sig1 == blankname) || ((sig1 > 31)&& (sig1 < 34)))){
                nextLine("An output must come first in a connection");
            }
            
            // check for dash
            if (curSym == DASH) {
                snz->getSymbol(curSym, curName, curInt);
                
                // check for second name
                name devName2 = nameCheck();
                    
                    // check for second signal
                    name sig2 = signalCheck(devName2);
                    
                    if (((sig2 == blankname) || ((sig2 > 31)&& (sig2 < 34)))){
                        nextLine("An input must be second in a connection");
                    }
                    
                    // Add input to list of used inputs for future error handling
                    inpTemp usedIn;
                    usedIn.dev = devName2;
                    usedIn.sig = sig2;
		    usedIns.push_back(usedIn);
                    
                    // Make connection
                    netz->makeconnection(devName2, sig2, devName1, sig1,ok);
                    
                    
                
            }else{
                nextLine("Expected a dash."); 
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

void parser::mon() throw (runtime_error)
{
    // EBNF: monitor = name ‘.’ signal (',' | ‘;’)
    
    try {
            
        name devName = nameCheck();
        name sig = signalCheck(devName);
        
        if ((sig == blankname) || ((sig > 31)&& (sig < 34))) {
            monz->makemonitor(devName, sig, ok);
        }else{
            nextLine("Monitors can only be connected outputs");
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


// TO DO Fuse these two function
name parser::nameCheck() throw (runtime_error)
{
    // EBNF: letter {letter|digit}
    
    if(curSym == NAMESYM){
        
        name newName = curName;
        
        if (!nameExist(devNames, newName)){
            stopSym = COMMA;
            stopSym2 = SEMICOL;
            error("The device does not exist", stopSym,
                  stopSym2);
            // TO DO Enable This
            /*
            error("The device "+namestable.getName(newName)+" does not exist", stopSym,
                  stopSym2);
            */
            throw runtime_error("Skipping to next line");
        }
        
        snz->getSymbol(curSym, curName, curInt);
        
        return newName;
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a name", stopSym,
              stopSym2);
        throw runtime_error("Skipping to next line");
    }

}

name parser::nameCheck(dom deviceOrMonitor) throw (runtime_error)
{
    // TO DO figure out a way to tell between monitors and devices
    // EBNF: letter {letter|digit}
    
    if(curSym == NAMESYM){
        
        name newName = curName; 
        
        // TO DO Test this
        // check semantically if name is okay.
        if (nameExist(devNames, newName)){
            stopSym = COMMA;
            stopSym2 = SEMICOL;
            error("The name has already been used",
                  stopSym,stopSym2);
            
            // TO DO Enable this
            /*
            error("The name "+namestable.getName(newName)+" has already been used", stopSym,
                  stopSym2);
            throw runtime_error("Skipping to next line");
            */
        }
        
        snz->getSymbol(curSym, curName, curInt);
        return newName;
     
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

bool parser::nameExist(vector<name> names, name n)
{
    bool found = 0;
    
    for (vector<name>::iterator i = names.begin(); i != names.end(); ++i) {
        if (n == (*i)) {
            found = 1;
        }
    }
    return found;
}


name parser::signalCheck(name deviceName) throw (runtime_error)
{
    // EBNF: (“O”|”I”(numberlt17not0)|”DATA”|”CLK”|”SET”|”CLEAR”|’Q’|”QBAR”)
    
    
    // device must exist by the time this function is called
    deviceTemp dev = getDeviceTemp(deviceName);
    if (curSym == PERIOD) {
        snz->getSymbol(curSym, curName, curInt);
        if(curSym == SIGSYM){
            name devSignal = curName;
            
            
            // TO DO check semantically if name is okay.
            // ONLY need to check if data type is right
            deviceTemp dev = getDeviceTemp(deviceName);
            switch (dev.type) {
                case 0:
                    nextLine("A clock's output is just specified by the clock's name.");
                    break;
                case 1:
                    nextLine("A switch's output is just specified by the clock's name.");
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    if ((devSignal > 11) && (devSignal < 28)) {
                        if (!(dev.option >= (devSignal-11))){
                            nextLine("This gate doesn't have that many inputs.");
                        }else{
                            if (usedInput(dev.n, devSignal)){
                                nextLine("This input has already been used"); 
                            }
                            
                        }
                    }else{
                        nextLine("A gate only has standard inputs and an output.");
                    }
                    break;
                case 6:
                    if (!((devSignal > 27) && (devSignal < 34))) {
                        nextLine("A DTYPE only has signals Q, QBAR, DATA, CLK, SET and CLEAR");
                    }
                    break;
                case 7:
                    if (!((devSignal > 11) && (devSignal < 14))){
                        nextLine("A XOR only 2 inputs.");
                    }
                    break;
                    
                default:
                    nextLine("Unexpected error");
                    break;
            }
            
            snz->getSymbol(curSym, curName, curInt);
	    return devSignal;
            
        }else{
            nextLine("Expected an input or output");
        }

    // TO DO check for NAMSYM since we have the semantic rule that connections must
    // be output to input. UPDATE think about that
    }else{
        
        if (dev.type == 6){
            nextLine("You must specify either an input or output of a DTYPE");
        }else{
            return blankname;
        }
        
        
    }
    
}

name parser::type() throw (runtime_error)
{
    // EBNF: type = (“CLOCK”|”SWITCH”|”AND”|”NAND”|”OR”|”NOR”|”DTYPE”|”XOR”)
    
    if (curSym == TYPESYM) {
        // PBUG could change with getSymbol
        name devType = curName;
        
        // TO DO check semantically if type is okay
        // Done by definition
        
        // TO ASK will devType be changed when getSymbol is called?
        snz->getSymbol(curSym, curName, curInt);
        return devType;
        
    }else{
        stopSym = COMMA;
        stopSym2 = SEMICOL;
        error("Expected a device type.", stopSym, stopSym2);
        throw runtime_error("Skipping to next line");
    }
}

int parser::option(name devType) throw (runtime_error)
{
    //EBNF: digit {digit}
    
    // type is the device type. Use for semantic check.
    
    if (curSym == NUMSYM) {
        int option = curInt;
        
        // TO DO check semantically if option is okay
        switch (devType) {
            case 4:
                // TO DO determine max clock
                if (option < 1){
                    nextLine("A clock does not have this option.");
                }
                break;
            case 5:
                // option -> initial value (1 or 0)
                if (!((option >= 0) && (option <= 1))){
                    nextLine("A switch does not have this option.");
                }
                break;
            case 6:
                // 1 <= option <= 16 -> number of inputs
                if (!((option >= 1) && (option <= 16))){
                    nextLine("An AND does not have this option.");
                }
                break;
            case 7:
               // 1 <= option <= 16 -> number of inputs
                if (!((option >= 1) && (option <= 16))){
                    nextLine("An NAND does not have this option.");
                }
                break;
            case 8:
                // 1 <= option <= 16 -> number of inputs
                if (!((option >= 1) && (option <= 16))){
                    nextLine("An OR does not have this option.");
                }
                break;
            case 9:
                // 1 <= option <= 16 -> number of inputs
                if (!((option >= 1) && (option <= 16))){
                    nextLine("An NOR does not have this option.");
                }
                break;
            case 10:
                // TO DO remove this case
                nextLine("Unexpected error");
                break;
            case 11:
                // TO DO remove this case
                nextLine("Unexpected error");
                break;
                
            default:
                nextLine("Unexpected error");
                break;
        }
        
        snz->getSymbol(curSym, curName, curInt);
        return option;
        
    }else{
        nextLine("Expected a device option.");
    }
}

deviceTemp parser::getDeviceTemp(name d)
{
    
    for (vector<deviceTemp>::iterator i = madeD.begin(); i != madeD.end(); ++i) {
        if (d == i->n) {
            deviceTemp dev = (*i);
            return dev;
        }
    }
    
    cout << "UNEXPECTED ERROR IN getDeviceTemp" << endl;
    
   
    
}


// TO DO replace old method with this
void parser::nextLine(string message)
{
    stopSym = COMMA;
    stopSym2 = SEMICOL;
    error(message, stopSym, stopSym2);
    throw runtime_error("Skipping to next line");
}
/*
 * Return true if input has already been used
 * Return false otherwise
 */
bool parser::usedInput(name d, name s)
{
    for (vector<inpTemp>::iterator i = usedIns.begin(); i != usedIns.end(); ++i) {
        if ((d == i->dev) && (s == i->sig)) {
            return true;
        }
    } 
    return false;
}

void parser::symbolToString(symbol s){
    switch (s) {
        case BADSYM:
            cout << "BADSYM";
            break;
        case COLON:
            cout << "COLON";
            break;
        case NAMESYM:
            cout << "NAMESYM";
            break;
        case EQUALS:
            cout << "EQUALS";
            break;
        case TYPESYM:
            cout << "TYPESYM";
            break;
        case DOLLAR:
            cout << "DOLLAR";
            break;
        case NUMSYM:
            cout << "NUMSYM";
            break;
        case COMMA:
            cout << "COMMA";
            break;
        case DEVSYM:
            cout << "DEVSYM";
            break;
        case CONSYM:
            cout << "CONSYM";
            break;
        case MONSYM:
            cout << "MONSYM";
            break;
        case FINSYM:
            cout << "FINSYM";
            break;
        case EOFSYM:
            cout << "EOFSYM";
            break;
        case SIGSYM:
            cout << "SIGSYM";
            break;
        case SEMICOL:
            cout << "SEMICOL";
            break;
        case DASH:
            cout << "DASH";
            break;
        case PERIOD:
            cout << "PERIOD";
            
        default:
            break;
    }
}

