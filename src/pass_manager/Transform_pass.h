/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Wrap generic transforms for the Pass queue.
 */

#ifndef PHC_TRANSFORM_PASS_H
#define PHC_TRANSFORM_PASS_H

#include "AST_transform.h"
#include "HIR_transform.h"

class Transform_pass : public Pass
{
	AST::Transform* ast_transform;
	HIR::Transform* hir_transform;

public:

	Transform_pass (AST::Transform* v)
	{
		ast_transform = v;
		hir_transform = NULL;
	}

	Transform_pass (HIR::Transform* v)
	{
		ast_transform = NULL;
		hir_transform = v;
	}

	void run (IR* in, Pass_manager* pm)
	{
		if (in->ast)
		{
			assert (ast_transform);
			assert (hir_transform == NULL);
			in->ast->transform_children (ast_transform);
		}
		else
		{
			assert (in->hir);
			assert (ast_transform == NULL);
			assert (hir_transform);
			in->hir->transform_children (hir_transform);
		}
	}
};

#endif // PHC_TRANSFORM_PASS_H
