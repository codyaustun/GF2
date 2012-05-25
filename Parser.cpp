//
//  Parser.cpp
//  
//
//  Created by Cody Coleman on 5/25/12.
//  Copyright (c) 2012 MIT. All rights reserved.
//

#include <iostream>
#include "Parser.h"


bool readline();
void error(string message, symbol stop); 
void buildDeviceList();
void buildConnectionList();
void buildMonitorList();
void device();
void connection();
void monitor();


