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
		try{
			// Get the first Symbol
			snz->getSymbol(curSym, curName, curInt);
            // Set the next section variable to connections for error handling
            // See error function
			nextSection = CONSYM;
			// Syntax rule: check that device list should be first
			// Create device list
			if (curSym == DEVSYM) {
				
				snz->getSymbol(curSym, curName, curInt);
				buildDeviceList();
			}else{
				stopSym = COLON;
				error("The devices list must come first.", stopSym);
				buildDeviceList();
			}
        }catch( runtime_error e ){
            
            // Checks to see if this the appropriate level to handle this error
			if(curSym != nextSection){
					throw;
			}
			stopSym = curSym;
			error("Forgot a ';' at the end of the devices section", stopSym);
		}
        
        // Syntax rule: check  that Connection list should be second
        // Create connection list
        try{
		snz->getSymbol(curSym, curName, curInt);
        // Set the next section variable to monitors for error handling
		nextSection = MONSYM;
			if (curSym == CONSYM) {
				
				snz->getSymbol(curSym, curName, curInt);
				buildConnectionList();
			}else{
				stopSym = COLON;
				error("The connections list should follow the devices list", stopSym);
				buildConnectionList();
			}
        }catch( runtime_error e ){
            // Checks to see if this the appropriate level to handle this error
			if(curSym != nextSection){
					throw;
			}
			stopSym = curSym;
			error("Forgot a ';' at the end of the connections section",stopSym);

		}
        
        try{
			// Syntax rule: Check that Monitor list should be third
			// Create Monitor List
			snz->getSymbol(curSym, curName, curInt);
            // Set the next section variable to Fin for error handling
            // See error function
			nextSection = FINSYM;
				if (curSym == MONSYM) {
					snz->getSymbol(curSym, curName, curInt);
					buildMonitorList();
				}else{
					stopSym = COLON;
					error("The monitors list should follow the connections list", stopSym);
					buildMonitorList();
				}
		}catch( runtime_error e ){
            // Checks to see if this the appropriate level to handle this error
            // See error function
			if(curSym != nextSection){
					throw;
			}
			stopSym = curSym;
			error("Forgot a ';' at the end of the monitors section", stopSym);
		}
        
        // Syntax rule: check that FIN symbol should be fourth
        snz->getSymbol(curSym, curName, curInt);
        if (curSym == FINSYM) {
            
            // Determine if network was parsed properly
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
        return false;
    }
}

/* Error Handling
 *@effects: Increments 'errorCount'. Prints out current line with 
 *scanner::getCurrentLine() and the error message contained in 'message'. 
 *Also, advances 'curSym' until equals 'stop'.
 *@params: message String for error message.
 *@params: stop Symbol required to start parsing again.
 *@throws: throw a Runtime error if the function has advanced to the 
 *next section or the end of file.
 *My Reasoning: Inputing both an error message and stop symbol allows for
 *more flexible in our error handling. Error messages can be tailored
 *to the specific error. Stop symbol ensures the parser can start
 *parsing again as fast as possbile. 
 */
void parser::error(string message, symbol stop) throw (runtime_error)
{
    
    // Increment error count
    errorCount++; 
    
    // Use scanner to print out current line
    snz->getCurrentLine(); 
    
    // Print out error message
    cout << "Error (" << errorCount << "): ";
    if (curSym == BADSYM) {
        cout << "Detected an invalid symbol. " << endl;
    }
    cout << message << endl;
    
    // Advance to the next instance of the stop symbol
    while ((curSym != stop) && (curSym != EOFSYM) && (curSym != nextSection)) {
        snz->getSymbol(curSym, curName, curInt);
    }
	
    // an exception in the case curSym == EOFSYM
    if ((curSym == nextSection) && (nextSection != stopSym)){
		throw runtime_error("Going to next section");
	}

    // an exception in the case that the function has reach the next section
    if (curSym == EOFSYM) {
        throw runtime_error("Got to the end of the file");
    }
}

/* Same as the other error method, but handles the case where there 2 possible
 * stopping symbols.
 * EX: an error in a device could start again after a colon or semicolon.
 */
void parser::error(string message, symbol stop1, symbol stop2) throw (runtime_error)
{
    
    // Increment error count
    errorCount++; 
    
    // Scanner should print out current line
    snz->getCurrentLine(); 
    
    // Print error message
    cout << "Error (" << errorCount << "): ";
    if (curSym == BADSYM) {
        cout << "Detected an invalid symbol. ";
    }
    cout << message << endl;
    
    // Advance to the next instance of the stop symbol
    // For best performance make less likely stop symbol stop2
    while ((curSym != stop1) && (curSym != stop2) && 
			(curSym != EOFSYM) && (curSym != nextSection)) 
	{
        snz->getSymbol(curSym, curName, curInt);
    }
   
    // an exception in the case that the function has reach the next section
    if ((curSym == nextSection) && (nextSection != stopSym)){
		throw runtime_error("Going to next section");
	}
    
    // Throw an exception in the case curSym == EOFSYM
    if (curSym == EOFSYM) {
        throw runtime_error("Got to the end of the file");
    }
}



void parser::buildDeviceList()
{
    // EBNF: “DEVICES:” {device’,’} device’;’ 
    
    // Syntax rule: check for a colon
    if (curSym == COLON) {
        snz->getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'DEVICES'.",
              stopSym);
    }
    
    // Process the first device line
    device();
    
    // Repeatly process devices until the end of the section i.e. ';'
    while (curSym == COMMA) {
        snz->getSymbol(curSym, curName, curInt);
        device();
    } 
    
    // Syntax rule: check for SEMICOLON
    if (curSym != SEMICOL) {
        stopSym = curSym;
        error("There should be a ';' at the end of the devices section.",
              stopSym);
    }

}

void parser::buildConnectionList()
{
    // EBNF: “CONNECTIONS:” {connection’,’} connection’;’ 
    
    // Syntax rule: check for a colon
    if (curSym == COLON) {
        snz->getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'CONNECTIONS'.",
              stopSym);
    }

    // Process the first connection line
    connection();
    
    // Repeatly process connections until the of the section i.e. ';'
    while (curSym == COMMA) {
        snz->getSymbol(curSym, curName, curInt);
        connection();
    }
    
    // Syntax rule: check for SEMICOLON
    if (curSym != SEMICOL) {
        stopSym = curSym;
        error("There should be a ';' at the end of the connections section.",
              stopSym);
    }
    
    // Semantic rule: check to make sure all inputs are connected
    if(usedIns.size() != allIns.size()){
        stopSym = curSym;
        error("All inputs must be connected to an output.", stopSym);
    }
}

void parser::buildMonitorList()
{
    // EBNF: “MONITORS:” {monitor’,’} monitor’;’
    
    // Syntax rule: check for a colon
    if (curSym == COLON) {
        snz->getSymbol(curSym, curName, curInt);
    }else{
        stopSym = NAMESYM;
        error("There should be a colon following the keyword 'MONITORS'.",
              stopSym);
    }
    
    // Process first line in Monitors section
    mon();
    
    // Repeatly process monitors until the of the section i.e. ';'
    while (curSym == COMMA) {
        snz->getSymbol(curSym, curName, curInt);
        mon();
	
    }
   
    // Syntax rule: check for SEMICOLON
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
        
        // Syntax rule: check for name
        name devName = nameCheck(DEV);
        
        // Syntax rule: check for equals sign
        if (curSym == EQUALS){
            snz->getSymbol(curSym, curName, curInt);
            
            // Syntax rule: check type
            name devType = type();
            
            // create deviceTemp
            // This is used in error handling
            // Needed to check if device exists
            deviceTemp newDev;
            
            // XOR and DTYPE don't have options
            if (devType < 10){
            // Syntax rule: check for dollar sign
                if (curSym == DOLLAR) {
                    snz->getSymbol(curSym, curName, curInt);
                    
                    // Syntax rule: check for device option
                    int devOpt = option(devType);
                    newDev.option = devOpt;
                }else{
                    nextLine("Expected a dollar sign.");  
                }
            }
            
            if ((curSym == COMMA) || (curSym == SEMICOL)) {
                
                newDev.n = devName;
                // Self imposed: when creating deviceTemp subtract 4 from devType
                newDev.type = (devType - 4);
                madeD.push_back(newDev);

                // maps dev.type to devicekind enum in devices.cc
                // creates device
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
        // checks to see if this is the correct level to handle this
        if (curSym != COMMA && curSym != SEMICOL){
            // rethrow in case of EOFSYM error
            throw;
        }
    }
    
}

void parser::connection() throw (runtime_error)
{
    // EBNF: connection = name ‘.’ signal ‘-’ name ‘.’ signal (',' | ‘;’)
    
    try {
        // Syntax rule: check for first name
        name devName1 = nameCheck();
        
            
            // Syntax rule: check signal
            name sig1 = signalCheck(devName1);
            
            // Semantic rule: connections must be made output to input
            if (!((sig1 == blankname) || ((sig1 > 31)&& (sig1 < 34)))){
                nextLine("An output must come first in a connection");
            }
            
            // Syntax rule: check for dash
            if (curSym == DASH) {
                snz->getSymbol(curSym, curName, curInt);
                
                // Syntax rule: check for second name
                name devName2 = nameCheck();
                    
                    // Syntax rule: check for second signal
                    name sig2 = signalCheck(devName2);
                    
                    // Semantic rule: connections must be made output to input
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
        // checks to see if this is the correct level to handle this
        if (curSym != COMMA && curSym != SEMICOL){
            // rethrow in case of EOFSYM error
            throw;
        }
    }
}

void parser::mon() throw (runtime_error)
{
    // EBNF: monitor = name ‘.’ signal (',' | ‘;’)
    
    try {
        
        // Syntax rule: check for a valid device name
        name devName = nameCheck();
        
        // Syntax rule: check signal name
        name sig = signalCheck(devName);
        
        // Semantic rule: monitors can only be connected to outputs 
        if ((sig == blankname) || ((sig > 31)&& (sig < 34))) {
            // make monitor
            monz->makemonitor(devName, sig, ok);
        }else{
            nextLine("Monitors can only be connected outputs");
        }
    } catch (runtime_error e) {
        if (curSym != COMMA && curSym != SEMICOL){
            // rethrow in case of EOFSYM error
            throw;
        }
    }
    
}


// TO DO Fuse these two function
name parser::nameCheck() throw (runtime_error)
{
    // EBNF: letter {letter|digit}
    
    if(curSym == NAMESYM){
       
        // TO DO remove this and replace newName with curName
        name newName = curName;
        
        // Semantic rule: check to see if the device exists
        if (!nameExist(newName)){
            nextLine("The device does not exist");
        }
        
        // Go to next symbol
        snz->getSymbol(curSym, curName, curInt);
        
        // return name
        return newName;
        
    }else{
        nextLine("Expected a name");
       
    }

}

// Originally needed dom (Device or Monitor) to tell between
// device and monitor names, but monitor names were removed.
name parser::nameCheck(dom deviceOrMonitor) throw (runtime_error)
{
    // EBNF: letter {letter|digit}
    if(curSym == NAMESYM){
        
        name newName = curName; 
        
        // Semantic rule: check to see if the device exists
        if (nameExist(newName)){
            nextLine("The name has already been used");
        }
        
        // Go to next symbol
        snz->getSymbol(curSym, curName, curInt);
        
        // return name
        return newName;
        
    // Semantic rule: keywords cannot be names
    // TO DO figure out a nicer way to do this
    }else if((curSym == TYPESYM) || (curSym == SIGSYM) || (curSym == CONSYM) ||
             (curSym == DEVSYM) || (curSym == MONSYM)){
        nextLine("Keywords cannot be names");
        
    }
    else{
        nextLine("Expected a name");
    }
}

// check to see if a device name already exists
bool parser::nameExist(name n)
{
    // Initializing variable
    bool found = 0;
    
    // Scanning all made devices
    for (vector<deviceTemp>::iterator i = madeD.begin(); i != madeD.end(); ++i) {
        if (n == i->n) {
            found = 1;
            break;
        }
    }
    
    // returning result
    return found;
}


name parser::signalCheck(name deviceName) throw (runtime_error)
{
    // EBNF: (“O”|”I”(numberlt17not0)|”DATA”|”CLK”|”SET”|”CLEAR”|’Q’|”QBAR”)
    
    
    // device must exist by the time this function is called
    deviceTemp dev = getDeviceTemp(deviceName);
    
    // Syntax rule: there must be a period if the signal isn't a lone output
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
    }else{
        // Definition: a DTYPE always requires a period since it has multiple outputs.
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
        // check semantically if type is okay
        // Done by definition
        
        // Return device type
        name devType = curName;
        snz->getSymbol(curSym, curName, curInt);
        return devType;
        
    }else{
        nextLine("Expected a device type.");
    }
}

int parser::option(name devType) throw (runtime_error)
{
    //EBNF: digit {digit}
    
    // devType is the device type. Used for semantic check.
    
    if (curSym == NUMSYM) {
        int option = curInt;
        
        //  Definition: check if option is okay
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

// Return deviceTemp for a given name.
// a deviceTemp contains all important device information
deviceTemp parser::getDeviceTemp(name d)
{
    
    for (vector<deviceTemp>::iterator i = madeD.begin(); i != madeD.end(); ++i) {
        if (d == i->n) {
            deviceTemp dev = (*i);
            return dev;
        }
    }
   
}

// call error function and move to the next line of the file.
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

// This function is just used for bug testing
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

