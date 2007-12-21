#include "debug.h"
#include "Foreach.h"

#include "HIR.h"
#include "AST.h"

/* Lower IN by running all the passes between "hir" and NAME on it. */
List<HIR::Statement*>* lower_hir (const char* name, HIR::Statement* in);

/* Lower IN by running all the passes between "ast" and NAME on it. */
List<AST::Statement*>* lower_ast (const char* name, AST::Statement* in);

/* Parse CODE into a list of HIR statements, using NODE as the source for
 * filenames and line numbers. */
List<HIR::Statement*>* parse_to_hir (String* code, HIR::Node* node);

/* Parse CODE into a list of AST statements, using NODE as the source for
 * filenames and line numbers. */
List<AST::Statement*>* parse_to_ast (String* code, AST::Node* node);
