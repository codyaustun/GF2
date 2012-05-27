#ifndef scanner_h
#define scanner_h
#include <string>
#include <vector>

using namespace std;

/* Constants and Types */

typedef int name;
typedef enum {DEVSYM, CONSYM, MONSYM, FINSYM, EOFSYM,
	      NAMESYM, NUMSYM, SIGSYM, TYPESYM, BADSYM,
	      EQUALS, SEMICOL, COMMA, DOLLAR,
	      DASH, PERIOD, COLON} symbol;

class scanner {
  char curch;                    // current input character
  names* namesMod, char* defFile

public:
    scanner(names* namesMod, char* defFile);
    ~scanner();
    void getSymbol(symbol& s, name& id, int& num);
    void getCurrentLine();
    num getnumber(ifstream *infp, char &curch, bool &eofile, int &number);
    name getname(ifstream *infp, char &curch, bool &eofile, namestring &str);
};





#endif
