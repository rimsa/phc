/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Generic utilities for which the need is ubiquitous.
 */

#include "hir_to_mir/Shredder.h"
#include "pass_manager/Pass_manager.h"
#include "parsing/parse.h"

extern Pass_manager* pm;

List<HIR::Statement*>* lower_to_hir (const char* name, HIR::Statement* in)
{
        HIR::PHP_script* script = new HIR::PHP_script (
                new List<HIR::Statement*> (in));

        assert (pm->has_pass_named (new String (name)));

        IR* new_script = pm->run_from_until (new String ("hir"), new String (name), script);

        // TODO the typing is here is quite poor
        return dynamic_cast<HIR::PHP_script*>(new_script)->statements;
}
