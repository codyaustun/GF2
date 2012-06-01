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

  //scan.initch();
   while (s != EOFSYM) {
     scan.getSymbol(s, id, num);
     cout << "Symbol: " <<  s << " ID: " <<  id << " Number: " << num << endl;
   }

  //namesMod->writename(34);

  //scan.getSymbol(s, id, num);
  // cout << s << endl;

  // scan.getch();
 //  for (int i = 0; i< 1; i++){
    // scan.skipspaces();
    //scan.skipcomments();
 //   scan.getch();
 // }
  
  // cout << scan.getname(id) << endl;
  //  cout << scan.getCurrentLine() << endl;


  //names initNames;
   //cout << initNames.lookup("Hello72@") << endl;
   // initNames.writename(34);
   //cout << initNames.getname(37) << endl;
}
