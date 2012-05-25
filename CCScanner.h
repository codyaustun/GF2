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

using namespace std;


/* Constants and Types */
typedef int name;
vector<string> names;

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
    COLON
    } symbol;

class scanner {
public:
    void getSymbol(symbol& s, name& id, int& num);
    void getCurrentLine();
};



#endif
