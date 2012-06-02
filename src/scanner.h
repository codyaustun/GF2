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
NAMESYM, NUMSYM, SIGSYM, TYPESYM, BADSYM,
EQUALS, SEMICOL, COMMA, DOLLAR,
DASH, PERIOD, COLON} symbol;

class scanner {

 private:
  ifstream inf; // Input definition file
  char curch; // Current character
  char prevch; // Previous character
  string currentLine; // Current Line
  bool eofile; // True when end of file is reached
  names* dfnames; // Names table
  bool lineEnd;
  void skipspaces(); // Skips white spaces

  void skipcomments(); // Skips commments

  void displayError(string errorMessage);
  symbol cursym;
  int cursymLen;

 public:
  symbol s;
  name id;
  int num;

  scanner(names* namesMod, const char* defFile);
  ~scanner();

  void getSymbol(symbol& s, name& id, int& num);

  void getCurrentLine();

  string getLine();

  void getch(); /* Reads next character, updates curch and currentLine string */

  void getnumber(int &number); /* Reads number from defFile */

  void initch();

  void getname(name& id); /* Reads names (alphanumeric) from defFile */

};

#endif /* scanner_h */
