#ifndef names_h
#define names_h

#include <string>
#include <vector>

using namespace std;

const int maxnames  = 200;  /* max number of distinct names */
const int maxlength = 16;    /* max chars in a name string   */
const int blankname = -1;   /* special name                 */

typedef int name;
typedef string namestring;
typedef unsigned int length;

class names{
	
 private:
	vector<namestring> nametable; //Data structure used to store names

 public:

name lookup (namestring str);
/* Returns the internal representation of the name given in character 
form.  If the name is not already in the name table, it is        
automatically inserted.                                             */

name newname (namestring str); //Inserts new name into table, returns id

name cvtname (namestring str);
/* Returns the internal representation of the name given in character 
form.  If the name is not in the name table then 'blankname' is   
returned.                                                           */

namestring getname(name id); //Returns name in character form, from id

void writename (name id);
/* Prints out the given name on the console                         */

length namelength (name id);
/* Returns length ie number of characters in given name             */

names (void);
/* names initialises the name table.  This procedure is called at    
system initialisation before any of the above procedures/functions 
are used.                                                           */
};

#endif /* names_h */
