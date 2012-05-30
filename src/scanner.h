#ifndef scanner_h
#define scanner_h
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include "names.h"

using namespace std;

/* Constants and Types */

typedef int name;
typedef enum {DEVSYM, CONSYM, MONSYM, FINSYM, EOFSYM,
	      NAMESYM, NUMSYM, SIGSYM, TYPSYM, BADSYM,
	      EQUALS, SEMICOL, COMMA, DOLLAR,
	      DASH, PERIOD, COLON} symbol;

class scanner {

 private:
  ifstream inf;          // Input definition file
  char curch;            // Current character
  string currentLine;    // Current Line
  bool eofile;           // True when end of file is reached
  names* dfnames;         // Names table
  void skipspaces();     // Skips white spaces
  void skipcomments();   // Skips commments

  void getnumber(int &number);  /* Reads number from defFile */

  void getname(name& id);   /* Reads names (alphanumeric) from defFile */

  void getch();   /* Reads next character, updates curch and currentLine string */

  void displayError(string errorMessage);

 public:
  scanner(names* namesMod, char* defFile);
  ~scanner();
  void getSymbol(symbol& s, name& id, int& num);
  string getCurrentLine();

 
  name lookup(namestring str);
  
};

#endif /* scanner_h */
