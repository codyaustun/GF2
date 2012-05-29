#include "names.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

/* Name storage and retrieval routines */

names::names(void) //Initializes nametable with reserved keywords
{
  nametable [0] = "DEVICES"; nametable [1] = "CONNECTIONS";
  nametable [2] = "MONITORS"; nametable [3] = "FIN";
  nametable [4] = "CLOCK"; nametable [5] = "SWITCH";
  nametable [6] = "AND"; nametable [7] = "NAND";
  nametable [8] = "OR"; nametable [9] = "NOR";
  nametable [10] = "DTYPE"; nametable [11] = "XOR";
  nametable [12] = "I1"; nametable [13] = "I2";
  nametable [14] = "I3"; nametable [15] = "I4"; 
  nametable [16] = "I5"; nametable [17] = "I6"; 
  nametable [18] = "I7"; nametable [19] = "I8";
  nametable [20] = "I9"; nametable [21] = "I10"; 
  nametable [22] = "I11"; nametable [23] = "I12";
  nametable [24] = "I13"; nametable [25] = "I14"; 
  nametable [26] = "I15"; nametable [27] = "I16"; 
  nametable [28] = "DATA"; nametable [29] = "CLK";
  nametable [30] = "SET"; nametable [31] = "CLR"; 
  nametable [32] = "Q"; nametable [33] = "QBAR";
}

name names::cvtname (namestring str) 
{
  name id = 0;
  bool found = false; 
  while (found == false && id < nametable.size()) {
    if ( nametable[id] == str ) {
      found = true;
      return id;        
      exit;               // Name found, id returned, search stopped
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
































