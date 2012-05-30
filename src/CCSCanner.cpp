//
//  CCScanner.cpp
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//


#include "CCScanner.h"



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

scanner::scanner(int a, int b, int c){
    symbol dev[] = {NAMESYM, EQUALS,  TYPESYM, DOLLAR, NUMSYM};
    symbol conn[] = {NAMESYM, PERIOD, SIGSYM, DASH, NAMESYM, PERIOD, SIGSYM};
    symbol mon1[] = {NAMESYM, PERIOD, SIGSYM};
    symbol mon2[] = {NAMESYM, EQUALS, NAMESYM,PERIOD, SIGSYM};

    syms.push_back(DEVSYM);
    syms.push_back(COLON);
    int len = 0;
    for (int x = 0; x < a; x++) {
        len = sizeof(dev)/sizeof(symbol);
        for(int x2 = 0; x2 < len; x2++){
            syms.push_back(dev[x2]);
        }
        
        if ((a-x) != 1) {
            syms.push_back(COMMA);
        }else{
            syms.push_back(SEMICOL);
        }
    }
    
    
    
    syms.push_back(CONSYM);
    syms.push_back(COLON);
    for (int y = 0; y < b; y++) {
        len = sizeof(conn)/sizeof(symbol);
        for(int y2 = 0; y2 < len; y2++){
            syms.push_back(conn[y2]);
        }
        
        if ((a-y) != 1) {
            syms.push_back(COMMA);
        }else{
            syms.push_back(SEMICOL);
        }
    }
    
    syms.push_back(MONSYM);
    syms.push_back(COLON);
    for (int z = 0; z < c; z++) {
        len = sizeof(mon1)/sizeof(symbol);
        for(int z2 = 0; z2 < len; z2++){
            syms.push_back(mon1[z2]);
        }
        
        if ((a-z) != 1) {
            syms.push_back(COMMA);
        }else{
            syms.push_back(SEMICOL);
        }
    }
    
    syms.push_back(FINSYM);
    syms.push_back(EOFSYM);
    
    index = 0;
    
    

    
}

scanner::scanner(){
    index = 0;
}


void scanner::getSymbol(symbol& s, name& nameid, int& num){
    
    if(s != EOFSYM){
        
        s = syms[index];
        symbolToString(s);
        cout << " " << index << endl;
        ++index;
    }
    
}

void scanner::getCurrentLine(){
    cout << "This would print out the CurrentLine " << index << endl;
}

