/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Class to represent ints (for passing around as synthesized attributes) 
 */
 
#ifndef PHC_INTEGER
#define PHC_INTEGER

// Hack in the C++ language to distinguish between pre and post ops
#define POST_INCREMENT int 

#include "lib/Object.h"

class Integer : virtual public Object
{
private:
	long _n;
	
public:
	Integer();
	Integer(long n);
	long value();

public:
	Integer* clone();

public:
	void operator++(POST_INCREMENT); 
};

#endif
