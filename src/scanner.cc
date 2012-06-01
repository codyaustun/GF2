#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>
#include "scanner.h"
#include "names.h"
#include "parser.h"

using namespace std;

scanner::scanner(names* namesMod, const char* defFile)
{
  dfnames = namesMod;
  inf.open(defFile); // Open defFile
  if (!inf) {
    displayError("Cannot open file");
    exit;
  }
  // If file successfully opened:
  inf.clear();
  inf.seekg(0, ios::beg); // Seek beginning of file, clear any fail bits first
  currentLine.clear();
  initch();
}

scanner::~scanner()
{
  inf.close(); // Close defFile
}

void scanner::getSymbol(symbol& s, name& id, int& num)
{
  id = blankname; num = 0;
  skipspaces();
  if (eofile) s = EOFSYM;
  else {
    if (isdigit(curch)) {
      s = NUMSYM;
      getnumber(num);
    } else {
      if (isalpha(curch)) {
	getname(id);
	if (id == 0) s = DEVSYM; else
	  if (id == 1) s = CONSYM; else
	    if (id == 2) s = MONSYM; else
	      if (id == 3) s = FINSYM; else
		if (id >= 3 && id <= 11) s = TYPESYM; else    
		  if(id >=12 && id <= 33) s = SIGSYM; else
		    s = NAMESYM;
      } else {
	switch (curch) {
	case '/': skipcomments(); getSymbol(s, id, num); break;
	case '=': s = EQUALS; break;
	case ':': s = COLON; break;
	case ';': s = SEMICOL; break;
	case ',': s = COMMA; break;
	case '.': s = PERIOD; break;
	case '$': s = DOLLAR; break;
	case '-': s = DASH; break;
	default: s = BADSYM; break;
	}
	getch();
      } 
    }
  }		    
};

void scanner::skipspaces()
{
  while (!eofile && isspace(curch)) {
    getch();
   }
 }
void scanner::skipcomments()
{         
    currentLine.clear();
    eofile = (inf.get(curch) == 0);
    while (!eofile && prevch != '/') { //If '/' read, skip through until another '/' is read or eof reached 
      prevch = curch;
      eofile = (inf.get(curch) == 0);
    }
   if (eofile) {
     displayError("Comment not closed");
   }
}

string scanner::getLine()
{
  while (curch != ':' && curch != ';' && curch != ',') {
    getch();
  }
  return currentLine;
}

void scanner::getname(name &id)
{
  int i = 1;
  namestring str;
  while (isalnum(curch)) {     // Read characters, save to str
    str.push_back(curch) ;        
    if ( i == maxlength) {     // If str reaches maxlength, put in table, keep reading 
      id = dfnames->lookup(str);
      cout << dfnames->getname(id) << endl;
    } 
    i = i+1;
    getch();
  }

  if (i < maxlength) {                             // If str < maxlength, put in table
    id = dfnames->lookup(str);
  } else {                                         // If str > maxlength, inform user
    string errM = "Name "; 
    errM.append(str); 
    errM.append(" was truncated."); 
    displayError(errM);
  }   
}

void scanner::getnumber(int &number)               // Check for max possible number here?
{
  number = 0;                                      // Clear variable
  while (isdigit(curch)) {                         // Read number from file
     number = 10*number + atoi(&curch);
    getch();
   }
}

void scanner::getch()
{
  prevch = curch;
  eofile = (inf.get(curch) == 0);
  currentLine.push_back(curch);                     // Add new character to string

  if (prevch == ':' || prevch == ';' || prevch == ',') {
    currentLine.clear();                             // Clear string to start new line
    skipspaces();
  }
}

void scanner::displayError (string errorMessage)
{
   cout << "Error: " << errorMessage << endl;
}

void scanner::initch()
{
  eofile = (inf.get(curch) == 0);
}

void scanner::getCurrentLine() //called by parser, displays parser errors, location
{
  string errorMarker;
  for (int i = 0; i < currentLine.length(); i++) errorMarker.append(" ");
  errorMarker.append("^");
  
  cout << getLine() << endl;
  cout << errorMarker << endl;
}
