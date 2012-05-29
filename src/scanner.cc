#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>
#include "scanner.h"

using namespace std;

int main(int argc, char **argv){
  ifstream inf;   //input file
  char ch;        //current character
  bool eofile;    //true when end of file is reached
  int number = 0;
  namestring str;

  if (argc != 2) {
    cout << "Usage:" << argv[0] << " [filename] " << endl;
    exit(1);
  }

  inf.open(argv[1]);
  if (!inf) {
    cout << "Error: Cannot open Definition file" << argv[1] << " for reading" << endl;
    exit(1);
  }
 
}

void getSymbol(symbol& s, name& id, int& num){
  skipspaces();
  if (eofile) s = EOFSYM;
  else {
    if (isdigit(curch)) {
      s = NUMSYM;
      //getnumber(num);
    } else {
      if (isalpha(curch)) {
	//getname(id);
	if (id == 0) s = DEVSYM; else
	  if (id == 1) s = CONSYM; else
	    if (id == 2) s = MONSYM; else
	      if (id == 3) s = FINSYM; else
		if (3 < id < 12) s = TYPSYM; else
		  if(11 < id < 35) s = SIGSYM; else
		    s = NAMESYM;
      } else {
	switch (curch) {
	case '=': s = EQUALS; break;
	case ':': s = COLON; break;
	case ';': s = SEMICOL; break;
	case ',': s = COMMA; break;
	case '.': s = PERIOD; break;
	case '$': s = DOLLAR; break;
	case '-': s = DASH; break;
	default: s = BADSYM; break;
	}
	//getch();
      } 
    }
  }		    
};

void skipspaces(ifstream *infp, char &curch, bool &eofile)
{
 eofile = (infp->get(curch) == 0 ); 
 while (!eofile) {
   if (isspace(curch) == 0) {
     //cout << curch;
     eofile = (infp->get(curch) == 0);
   }
   else { eofile = (infp->get(curch) == 0); } 
 }
}

void getCurrentLine(ifstream *infp, char &curch, bool &eofile)
{
   eofile = (infp->get(curch) == 0 ); 
   while (!eofile) {
     if (isspace(curch) == 0) {
       cout << curch;
       eofile = (infp->get(curch) == 0);
     }
     else { eofile = (infp->get(curch) == 0); } 
   }
};



 name getname(ifstream *infp, char &curch, bool &eofile, namestring &str)
{
  eofile = (infp->get(curch) == 0 ); 
  int i = 1;
  while (!eofile) {

    if (isalpha(curch)) {
      while (!eofile && isalnum(curch)) {
	str.push_back(curch) ;                 
	if ( i == maxlength) {
	  cout << lookup(str) << endl;
	  nametable.push_back(str);
	  return lookup(str);
	}  
	i = i+1;
	eofile = (infp->get(curch) == 0);
      }
      if (i < maxlength) {
	cout << lookup(str) << endl;
	nametable.push_back(str);
	return lookup(str);
      } else {
	cout << "Warning: name " << str << " was truncated. " << endl;
      }

      str.clear();
      i = 1;
    }
    else {
      eofile = (infp->get(curch) == 0);
    }
  }
}

void getnumber(ifstream *infp, char &curch, bool &eofile, int &number)
{
 eofile = (infp->get(curch) == 0 ); 
 while (!eofile) {
   if (isspace(curch) == 0) {
     number = 10*number + atoi(&curch) ;
     eofile = (infp->get(curch) == 0);
   }
   else { 
     eofile = (infp->get(curch) == 0);
   } 
 }
 cout << number << endl;
}
