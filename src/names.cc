#include "names.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

/* Name storage and retrieval routines */

names::names(void)
{
  nametable [0] = "DEVICES"; nametable [1] = "CONNECTIONS";
  nametable [2] = "MONITORS"; nametable [3] = "FIN";
  nametable [4] = "CLOCK"; nametable [5] = "SWITCH";
  nametable [6] = "AND"; nametable [7] = "NAND";
  nametable [8] = "OR"; nametable [9] = "NOR";
  nametable [10] = "DTYPE"; nametable [11] = "XOR";
  nametable [12] = "0"; nametable [13] = "I1"; nametable [14] = "I2";
  nametable [15] = "I3"; nametable [16] = "I4"; nametable [17] = "I5";
  nametable [18] = "I6"; nametable [19] = "I7"; nametable [20] = "I8";
  nametable [21] = "I9"; nametable [22] = "I10"; nametable [23] = "I11";
  nametable [24] = "I12"; nametable [25] = "I13";
  nametable [26] = "I14"; nametable [27] = "I15";
  nametable [28] = "I16"; nametable [29] = "DATA";
  nametable [30] = "CLK"; nametable [31] = "SET";
  nametable [32] = "CLR"; nametable [33] = "Q";
  nametable [34] = "QBAR";
}

name names::lookup (namestring str)
{
  int i = 0;
  bool found = false; 
  while (found == false && i < nametable.size()) {
    if ( nametable[i] == str ) {
      found = true;
      return i;
      exit;
    }else {
      i++;
    }
  }
  if (found == false){
    nametable.push_back(str);
    return i;
  }   
}

name names::cvtname (namestring str)
{
  int i = 0;
  bool found = false; 
  while (found == false && i < nametable.size()) {
    if ( nametable[i] == str ) {
      found = true;
      return i;
      exit;
    } else {
      i++;
    }
  }
  if (found == false){
    return blankname;
  }
}

void names::writename (name id)
{
   cout << nametable[id] ;
}

length names::namelength (name id)
{
  length strlength = nametable[id].length();
  return strlength;
}
































