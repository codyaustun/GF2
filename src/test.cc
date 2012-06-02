#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>
#include "scanner.h"
#include "names.h"

using namespace std;

int main()
{
  names* namesMod = new names();
  scanner scan(namesMod, (char*) "defFile1.txt");
  symbol s = BADSYM;
  name id = blankname;
  int num = 0;
  
   //while (s != EOFSYM) {
     //scan.getSymbol(s, id, num);
     //cout << "Symbol: " <<  s << " Name: " <<  namesMod->getname(id) << " Number: " << num << endl;
   //}
   //cout << endl;
   int i = 0;
   while (i<5) {
   scan.getSymbol(s, id, num);
   cout << "Symbol: " <<  s << " Name: " <<  namesMod->getname(id) << " Number: " << num << endl;
   i++;
   }
   cout << scan.getLine() << endl;
  //namesMod->writename(34);

  //scan.getSymbol(s, id, num);
  // cout << s << endl;

  // scan.getch();
  // for (int i = 0; i< 1; i++){
    // scan.skipspaces();
    //scan.skipcomments();
  //  scan.getch();
  //}
  // cout << scan.getname(id) << endl;
   //scan.getLine();

  /* Testing Names  */

   //names initNames;
   //cout << initNames.lookup("Hello72@") << endl;
   //initNames.writename(34);
   //cout << initNames.getname(37) << endl;
   //cout << initNames.namelength(5) << endl;
}
