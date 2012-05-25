#include <iostream>
#include <scanner.h>

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

bool Parser::readline()
{
    // Get first Symbol
    getSymbol(curSym, curName, curInt);
    
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
        return (errors == 0);
    }else{
        stopSym = EOFSYM
        error("'FIN' should follow the monitors list", stopSym);
        return false;
    }
}

void buildDeviceList()
{
    getSymbol(curSym, curName, curInt); 
    device();
    while (curSym == SEMICOLON) {
        getSymbol(curSym, curName, curInt);
        if (curSym == CONSYM) {
            break;
        }
        device();
    }
}

void device()
{
    
}

void buildConnectionList()
{
    getSymbol(curSym, curName, curInt); 
    connection();
    while (curSym == SEMICOLON) {
        getSymbol(curSym, curName, curInt);
        if (curSym == MONSYM) {
            break;
        }
        connection();
    }
}

void connection()
{
    
}

void buildMonitorList()
{
    getSymbol(curSym, curName, curInt); 
    monitor();
    while (curSym == SEMICOLON) {
        getSymbol(curSym, curName, curInt);
        if (curSym == EOFSYM) {
            break;
        }
        monitor();
    }
}

void monitor()
{
  
}

void error(string message, symbol stop)
{
    
}