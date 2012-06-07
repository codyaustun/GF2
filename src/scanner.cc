#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>
#include "scanner.h"
#include "names.h"
#include "translate.h"

using namespace std;

scanner::scanner(names* namesMod, const char* defFile)
{
	dfnames = namesMod;
	inf.open(defFile); 		// Open defFile
	if (!inf) {
		displayError("Error: Cannot open file");
		exit;
	}
	inf.clear();			// Clear any fail bits
	inf.seekg(0, ios::beg); // Seek beginning of file
	currentLine.clear();
	lineNum = 1;
	incrChar();
	cout <<s_("File opened successfully") << endl;
	s = BADSYM;
}

scanner::~scanner()
{
	inf.close(); 			// Close defFile
}

/* Public functions: */

void scanner::getSymbol(symbol& s, name& id, int& num)
{
	s = BADSYM; id = blankname; num = 0; 
	skipspaces(); skipcomments();
	if (eofile) {
	  s = EOFSYM;
	  cursymLen=0;
	}
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
					case '=': s = EQUALS; break;
					case ':': s = COLON; break;
					case ';': s = SEMICOL;break;
					case ',': s = COMMA; break;
					case '.': s = PERIOD; break;
					case '$': s = DOLLAR; break;
					case '-': s = DASH; break;
					default: s = BADSYM; break;
					}
					cursymLen = 1;
					getch();
					if (prevch==':' || prevch==';' || prevch==','){
						lineEnd = true;
					}
			}
		}
	}
	cursym = s;
}

void scanner::getCurrentLine() 
{	
	string errorMarker;
	for (int i = 0; i < (currentLine.length()-cursymLen); i++) 
	{
	  errorMarker.append(" ");
	}
	errorMarker.append("^"); 
	cout << "Line " << lineNum << ":" << endl;	
	cout << getLine() << endl;		// Prints current line
	cout << errorMarker << endl;	// Prints position of error
}

/* Private functions: */

void scanner::incrChar()
{
	eofile = (inf.get(curch) == 0);
	if (curch == '\n') lineNum ++;
}

void scanner::getch()
{
	prevch = curch;
	incrChar();
	if (lineEnd) {
		currentLine.clear(); // Clear string to start new line
		skipspaces();
		lineEnd = false;
	}	
	if(prevch != '\n'){
		currentLine.push_back(prevch);
	}
}

void scanner::getname(name &id)
{
	cursymLen = 0;
	int i = 1;
	namestring str;
	while (isalnum(curch)) { 
		str.push_back(curch) ;
		cursymLen++;
		if ( i == maxlength) { 			// Continue reading string
			id = dfnames->lookup(str);
			cout << dfnames->getname(id) << endl;
		}
		i = i+1;
			getch();	
	}

	if (i < maxlength) { 		// If str < maxlength, put/find in table
		id = dfnames->lookup(str);
	} else { 					// If str > maxlength, inform the user
	  wxString errM = _("Warning: Name ");
	  errM.Append(wxString(str.c_str(),wxConvUTF8));
	  errM.Append(_(" was truncated."));
	  displayError(string(errM.mb_str()));
	}
}

void scanner::getnumber(int &number)
{
	cursymLen = 0;
	number = 0; 
	while (isdigit(curch)) { 				// Read number
		number = 10*number + atoi(&curch);
		getch();
		cursymLen++;
	}
	if (number > 1000) {
		number = 1000;
		cout << s_("Warning: Max parameter value = 1000") << endl;
	}
}

void scanner::skipspaces()
{
	while (!eofile && (isspace(curch) || curch == '\n')) { 
		getch();
	}
 }

void scanner::skipcomments()
{
	if (curch =='/') {
		incrChar(); 
		while (!eofile && prevch != '/') { 
			prevch = curch;
			incrChar();
		}
		if (eofile) {
		  displayError(s_("Error: Comment not closed"));
		}
		skipspaces();
	}
}

string scanner::getLine()
{	
	if(cursym != SEMICOL && cursym != COLON && cursym != COMMA){
		while (curch !=':' && curch !=';' && curch !=',' && !eofile) {
			getch(); 
		}
		currentLine.push_back(curch);
	}
	return currentLine;
}

void scanner::displayError (string errorMessage)
{
   cout << errorMessage << endl;
}
