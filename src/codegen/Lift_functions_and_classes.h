/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Lift functions and classes to the global scope, and introduce "__MAIN__"
 */

#ifndef PHC_LIFT_FUNCTIONS_AND_CLASSES_H
#define PHC_LIFT_FUNCTIONS_AND_CLASSES_H

#include "AST_transform.h"
#include "pass_manager/Pass_manager.h"

class Lift_functions_and_classes : public AST::AST_transform, public Pass
{
public:

	Lift_functions_and_classes () 
	{
		this->name = new String ("lfc");
	}

	// TODO this should be done on HIR, I think
	void run (IR* in, Pass_manager* pm)
	{
		if (pm->args_info->generate_c_flag
			or pm->args_info->compile_flag)
		{
			assert (in->hir == NULL);
			in->ast->transform_children (this);
		}
	}
	void children_php_script(AST::AST_php_script* in);
};

#endif // PHC_LIFT_FUNCTIONS_AND_CLASSES
