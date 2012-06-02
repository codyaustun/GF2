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
typedef enum {	DEVSYM, CONSYM, MONSYM, FINSYM, EOFSYM,
				NAMESYM, NUMSYM, SIGSYM, TYPESYM, BADSYM,
				EQUALS, SEMICOL, COMMA, DOLLAR,
				DASH, PERIOD, COLON	} symbol;

class scanner {

 private:
	ifstream inf; 		// Input definition file
	char curch; 		// Current character
	char prevch; 		// Previous character
	string currentLine; // Current Line
	bool eofile; 		// True when end of file is reached
	names* dfnames; 	// Names table
	bool lineEnd;		// True when we pass an end of line
	symbol cursym;		// Current symbol
	int cursymLen;		// Current symbol length

	void getch(); 
	/* Reads next character, updates curch and currentLine string */
	void getnumber(int &number); 
	/* Reads number from defFile */
	void getname(name& id); 
	/* Reads names (alphanumeric) from defFile */	
	
	void incrChar(); // Gets next character
	void skipspaces(); // Skips white spaces
	void skipcomments(); // Skips commments
	string getLine(); // Gets current line
	
	void displayError(string errorMessage);	
	/* Outputs scanner error messages */

 public:
	symbol s;
	name id;
	int num;

	scanner(names* namesMod, const char* defFile); // Opens file
	~scanner();	// Closes definition file
	
	/* The following are called by the parser: */
	void getSymbol(symbol& s, name& id, int& num);
	void getCurrentLine();

};

#endif /* scanner_h */

