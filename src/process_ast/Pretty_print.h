/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Pretty print if the --pretty-print
 * option is given
 */



#ifndef PHC_PRETTY_PRINT_H
#define PHC_PRETTY_PRINT_H

#include "process_ast/PHP_unparser.h"
#include "process_ast/Pass_manager.h"


class Pretty_print : public Pass
{
public:
	Pretty_print ()
	{
		this->name = new String ("pretty-print");
	}
	void run (AST_php_script* in, Pass_manager* pm)
	{
		if (pm->args_info->pretty_print_flag)
			in->visit (new PHP_unparser ());
	}
};


#endif // PHC_PRETTY_PRINT_H
