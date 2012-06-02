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
  ifstream inf;          // Input definition file
  char curch;            // Current character
  char prevch;           // Previous character
  string currentLine;    // Current Line
  bool eofile;           // True when end of file is reached
  names* dfnames;        // Names table
  bool lineEnd;          // Used to keep track of currentLine
  symbol curSym;

  void skipspaces();     // Skips white spaces

  void skipcomments();   // Skips commments

  string getLine();      // Returns string with the entire current line

  void getch();   // Reads next character, updates curch and currentLine string

  void getnumber(int &number);  //Reads number from defFile

  void getname(name& id);   //Reads names (alphanumeric) from defFile
 
  void initch();  // Gets first character

 public:

  scanner(names* namesMod, const char* defFile); // Constructor, opens definition file
  ~scanner(); // Destructor, closes file

  void getSymbol(symbol& s, name& id, int& num);
  /* Gets current symbol + nametable id (if name) or number (if int) */

  void getCurrentLine(); // Called by parser upon error, prints currentLine, error marker

};

#endif /* scanner_h */
