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
namestring names::getname(name id)
 {
   namestring namestr = nametable[id];
   return namestr;
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
































