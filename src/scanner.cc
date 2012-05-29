#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>
#include "scanner.h"
#include "names.h"

using namespace std;

scanner::scanner(names* namesMod, char* defFile)
{
  inf.open(defFile); // Open defFile
  if (!inf) {
    cout << "Cannot open file " << defFile << endl;
    fileOpened = false;
    exit;
  }
 
  fileOpened = true;
  inf.clear();
  inf.seekg(0, ios::beg); // Seek beginning of file, clear any fail bits first
  currentLine.clear();
}

scanner::~scanner()
{
  inf.close(); // Close defFile
}


void scanner::getSymbol(symbol& s, name& id, int& num)
{
  eofile = (inf.get(curch) == 0);
  skipspaces();
  skipcomments();

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
		if (3 < id < 12) s = TYPSYM; else    //4-11
		  if(11 < id < 34) s = SIGSYM; else  //12-33
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
  if (curch == '/') {
  //currentLine.clear();
    eofile = (inf.get(curch) == 0);
    while (!eofile && curch != '/') {
      eofile = (inf.get(curch) == 0);
    }
  }
  if (eofile) {
    cout << " Comment not closed " << endl;
  }
}
string scanner::getCurrentLine()
{
  while (curch == ':' || curch == ';' || curch == ',') {
    currentLine.push_back(curch);
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
    } 
    i = i+1;
    getch();
  }
 
  // if (!isspace(curch)) cout << "Check name; alphanumeric beginning stored" << endl;

  if (i < maxlength) {         // If str < maxlength, put in table
    id = dfnames->lookup(str);
  } else {                     // If str > maxlength, inform user
    cout << "Warning: name " << str << " was truncated. " << endl;
  }   
}

void scanner::getnumber(int &number)
{
  number = 0;                 // Clear variable
  while (isdigit(curch)) {      // Read number from file
     number = 10*number + atoi(&curch);
    getch();
   }
}

void scanner::getch()
{
  eofile = (inf.get(curch) == 0);
  if (curch == ':' || curch == ';' || curch == ',') {
    currentLine.clear();
  } else {
    currentLine.push_back(curch);
  }
}
