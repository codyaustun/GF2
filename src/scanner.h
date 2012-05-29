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
 private:
  char curch;
  string currentLine; 

 public:
  scanner(names* namesMod, char* defFile);
  ~scanner();
  void getSymbol(symbol& s, name& id, int& num);
  void getCurrentLine();
  void getnumber(int& num);
  void getname(name& id);
  void getch();
  name lookup(namestring str);
  void skipspaces();
};

#endif
