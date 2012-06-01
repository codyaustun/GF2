//
//  CCScanner.h
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//

#ifndef _CCScanner_h
#define _CCScanner_h
#include <string>
#include <vector>
#include <iostream>
#include "names.h"

using namespace std;


/* Constants and Types */

typedef enum{
    DEVSYM,
    CONSYM,
    MONSYM,
    NAMESYM,
    NUMSYM,
    SIGSYM,
    TYPESYM,
    EOFSYM,
    EQUALS,
    SEMICOL,
    COMMA,
    DOLLAR,
    DASH,
    PERIOD,
    COLON,
    FINSYM,
    BADSYM,
    } symbol;

class scanner {
public:
    scanner();
    scanner(int a, int b, int c);
    void getSymbol(symbol& s, name& nameid, int& num);
    void getCurrentLine();
private:
    int index;
    vector<symbol> syms; 
};



#endif