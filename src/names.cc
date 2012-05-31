#include "names.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

/* Name storage and retrieval routines */

names::names(void) //Initializes nametable with reserved keywords
{
  nametable.clear();
  nametable.push_back("DEVICES");
  nametable.push_back("CONNECTIONS");
  nametable.push_back("MONITORS");
  nametable.push_back("CLOCK");
  nametable.push_back("SWITCH");
  nametable.push_back("AND");
  nametable.push_back("NAND");
  nametable.push_back("OR");
  nametable.push_back("NOR");
  nametable.push_back("");
  nametable.push_back("DTYPE");
  nametable.push_back("XOR");
  nametable.push_back("I1");
  nametable.push_back("I2");
  nametable.push_back("I3");
  nametable.push_back("I4");
  nametable.push_back("I5");
  nametable.push_back("I6");
  nametable.push_back("I7");
  nametable.push_back("I8");
  nametable.push_back("I9");
  nametable.push_back("I10");
  nametable.push_back("I11");
  nametable.push_back("I12");
  nametable.push_back("I13");
  nametable.push_back("I14");
  nametable.push_back("I15");
  nametable.push_back("I16");
  nametable.push_back("DATA");
  nametable.push_back("CLK");
  nametable.push_back("SET");
  nametable.push_back("CLR");
  nametable.push_back("Q");
  nametable.push_back("QBAR");
}

name names::cvtname (namestring str) 
{
  name id = 0;
  bool found = false; 
  while (found == false && id < nametable.size()) {
    if ( nametable[id] == str ) {
      found = true;
      return id;          // Name found, id returned, search stopped
    } else {
      id++;
    }
  }
  if (found == false){
    return blankname;     // Name not found, blankname returned
  }
}

name names::lookup (namestring str) 
{
  name id = cvtname(str);
  if (id == blankname) {
    id = newname(str);
    return id;
  } else {
    return id;
  }
}

name names::newname (namestring str)
{
  if (str.length() > maxlength) {
    str = str.substr(0,maxlength);
  }
  nametable.push_back(str);
  name id = cvtname(str);
  return id;
}

namestring names::getname(name id)
 {
   if (blankname < id < nametable.size()) {
     namestring namestr = nametable[id];
     return namestr;
   } else return "Invalid Name ID";
 }

void names::writename (name id)
{
  cout << getname(id) ;
}

length names::namelength (name id)
{
  if (blankname < id < nametable.size()) {
    length strlength = nametable[id].length();
    return strlength;
  } else return blankname;
}















