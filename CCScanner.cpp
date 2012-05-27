//
//  CCScanner.cpp
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//


#include "CCScanner.h"

symbol ss[] = {BADSYM, COLON, NAMESYM, EQUALS, 
    TYPESYM, DOLLAR, NUMSYM, COMMA, EOFSYM};

void symbolToString(symbol s){
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


scanner::scanner(){
    index = 0;
}


void scanner::getSymbol(symbol& s, name& nameid, int& num){
    if(s != EOFSYM){
        
        s = ss[index];
        symbolToString(s);
        cout << " " << index << endl;
        ++index;
    }
    
}

void scanner::getCurrentLine(){
    cout << "This would print out the CurrentLine " << index << endl;
}

