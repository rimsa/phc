/** @file cmdline.h
 *  @brief The header file for the command line option parser
 *  generated by GNU Gengetopt version 2.21
 *  http://www.gnu.org/software/gengetopt.
 *  DO NOT modify this file, since it can be overwritten
 *  @author GNU Gengetopt by Lorenzo Bettini */

#ifndef CMDLINE_H
#define CMDLINE_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
/** @brief the program name */
#define CMDLINE_PARSER_PACKAGE PACKAGE
#endif

#ifndef CMDLINE_PARSER_VERSION
/** @brief the program version */
#define CMDLINE_PARSER_VERSION VERSION
#endif

/** @brief Where the command line options are stored */
struct gengetopt_args_info
{
  const char *help_help; /**< @brief Print help and exit help description.  */
  const char *full_help_help; /**< @brief Print help, including hidden options, and exit help description.  */
  const char *version_help; /**< @brief Print version and exit help description.  */
  int verbose_flag;	/**< @brief Verbose output (default=off).  */
  const char *verbose_help; /**< @brief Verbose output help description.  */
  int compile_flag;	/**< @brief Compile (default=off).  */
  const char *compile_help; /**< @brief Compile help description.  */
  int pretty_print_flag;	/**< @brief Pretty print input according to the Zend style guidelines (default=off).  */
  const char *pretty_print_help; /**< @brief Pretty print input according to the Zend style guidelines help description.  */
  int obfuscate_flag;	/**< @brief Obfuscate input (default=off).  */
  const char *obfuscate_help; /**< @brief Obfuscate input help description.  */
  char ** run_arg;	/**< @brief Run the specified plugin (may be specified multiple times).  */
  char ** run_orig;	/**< @brief Run the specified plugin (may be specified multiple times) original value given at command line.  */
  int run_min; /**< @brief Run the specified plugin (may be specified multiple times)'s minimum occurreces */
  int run_max; /**< @brief Run the specified plugin (may be specified multiple times)'s maximum occurreces */
  const char *run_help; /**< @brief Run the specified plugin (may be specified multiple times) help description.  */
  char ** r_option_arg;	/**< @brief Pass option to a plugin (specify multiple flags in the same order as multiple plugins - 1 option only per plugin).  */
  char ** r_option_orig;	/**< @brief Pass option to a plugin (specify multiple flags in the same order as multiple plugins - 1 option only per plugin) original value given at command line.  */
  int r_option_min; /**< @brief Pass option to a plugin (specify multiple flags in the same order as multiple plugins - 1 option only per plugin)'s minimum occurreces */
  int r_option_max; /**< @brief Pass option to a plugin (specify multiple flags in the same order as multiple plugins - 1 option only per plugin)'s maximum occurreces */
  const char *r_option_help; /**< @brief Pass option to a plugin (specify multiple flags in the same order as multiple plugins - 1 option only per plugin) help description.  */
  int read_ast_xml_flag;	/**< @brief Assume the input is a phc AST in XML format (default=off).  */
  const char *read_ast_xml_help; /**< @brief Assume the input is a phc AST in XML format help description.  */
  int no_validation_flag;	/**< @brief Toggle XML validation (default=on).  */
  const char *no_validation_help; /**< @brief Toggle XML validation help description.  */
  int include_flag;	/**< @brief Parse included or required files at compile-time (default=off).  */
  const char *include_help; /**< @brief Parse included or required files at compile-time help description.  */
  char ** c_option_arg;	/**< @brief Pass option to the C compile (e.g., -C-g; can be specified multiple times).  */
  char ** c_option_orig;	/**< @brief Pass option to the C compile (e.g., -C-g; can be specified multiple times) original value given at command line.  */
  int c_option_min; /**< @brief Pass option to the C compile (e.g., -C-g; can be specified multiple times)'s minimum occurreces */
  int c_option_max; /**< @brief Pass option to the C compile (e.g., -C-g; can be specified multiple times)'s maximum occurreces */
  const char *c_option_help; /**< @brief Pass option to the C compile (e.g., -C-g; can be specified multiple times) help description.  */
  int generate_c_flag;	/**< @brief Generate C code (default=off).  */
  const char *generate_c_help; /**< @brief Generate C code help description.  */
  char * extension_arg;	/**< @brief Generate a PHP extension called NAME instead of a standalone application.  */
  char * extension_orig;	/**< @brief Generate a PHP extension called NAME instead of a standalone application original value given at command line.  */
  const char *extension_help; /**< @brief Generate a PHP extension called NAME instead of a standalone application help description.  */
  char * with_php_arg;	/**< @brief PHP installation path.  */
  char * with_php_orig;	/**< @brief PHP installation path original value given at command line.  */
  const char *with_php_help; /**< @brief PHP installation path help description.  */
  char * optimize_arg;	/**< @brief Optimize (default='0').  */
  char * optimize_orig;	/**< @brief Optimize original value given at command line.  */
  const char *optimize_help; /**< @brief Optimize help description.  */
  char * output_arg;	/**< @brief Place executable into file FILE.  */
  char * output_orig;	/**< @brief Place executable into file FILE original value given at command line.  */
  const char *output_help; /**< @brief Place executable into file FILE help description.  */
  int next_line_curlies_flag;	/**< @brief Output the opening curly on the next line instead of on the same line (default=off).  */
  const char *next_line_curlies_help; /**< @brief Output the opening curly on the next line instead of on the same line help description.  */
  int no_leading_tab_flag;	/**< @brief Don't start every line in between <?php .. ?> with a tab (default=off).  */
  const char *no_leading_tab_help; /**< @brief Don't start every line in between <?php .. ?> with a tab help description.  */
  int no_line_numbers_flag;	/**< @brief Don't show line numbers when dumping DOT/XML (default=off).  */
  const char *no_line_numbers_help; /**< @brief Don't show line numbers when dumping DOT/XML help description.  */
  int no_nulls_flag;	/**< @brief Don't show NULLs when dumping DOT (default=off).  */
  const char *no_nulls_help; /**< @brief Don't show NULLs when dumping DOT help description.  */
  int no_empty_lists_flag;	/**< @brief Don't show empty lists when dumping DOT (default=off).  */
  const char *no_empty_lists_help; /**< @brief Don't show empty lists when dumping DOT help description.  */
  char * tab_arg;	/**< @brief String to use for tabs while unparsing (default='\t').  */
  char * tab_orig;	/**< @brief String to use for tabs while unparsing original value given at command line.  */
  const char *tab_help; /**< @brief String to use for tabs while unparsing help description.  */
  int no_hash_bang_flag;	/**< @brief Do not output any #! lines (default=off).  */
  const char *no_hash_bang_help; /**< @brief Do not output any #! lines help description.  */
  char ** dump_arg;	/**< @brief Dump input as PHP (although potentially with gotos and labels) after the pass named 'passname'.  */
  char ** dump_orig;	/**< @brief Dump input as PHP (although potentially with gotos and labels) after the pass named 'passname' original value given at command line.  */
  int dump_min; /**< @brief Dump input as PHP (although potentially with gotos and labels) after the pass named 'passname''s minimum occurreces */
  int dump_max; /**< @brief Dump input as PHP (although potentially with gotos and labels) after the pass named 'passname''s maximum occurreces */
  const char *dump_help; /**< @brief Dump input as PHP (although potentially with gotos and labels) after the pass named 'passname' help description.  */
  char ** udump_arg;	/**< @brief Dump input as runnable PHP after the pass named 'passname'.  */
  char ** udump_orig;	/**< @brief Dump input as runnable PHP after the pass named 'passname' original value given at command line.  */
  int udump_min; /**< @brief Dump input as runnable PHP after the pass named 'passname''s minimum occurreces */
  int udump_max; /**< @brief Dump input as runnable PHP after the pass named 'passname''s maximum occurreces */
  const char *udump_help; /**< @brief Dump input as runnable PHP after the pass named 'passname' help description.  */
  char ** ddump_arg;	/**< @brief Dump input as DOT after the pass named 'passname'.  */
  char ** ddump_orig;	/**< @brief Dump input as DOT after the pass named 'passname' original value given at command line.  */
  int ddump_min; /**< @brief Dump input as DOT after the pass named 'passname''s minimum occurreces */
  int ddump_max; /**< @brief Dump input as DOT after the pass named 'passname''s maximum occurreces */
  const char *ddump_help; /**< @brief Dump input as DOT after the pass named 'passname' help description.  */
  char ** xdump_arg;	/**< @brief Dump input as XML after the pass named 'passname'.  */
  char ** xdump_orig;	/**< @brief Dump input as XML after the pass named 'passname' original value given at command line.  */
  int xdump_min; /**< @brief Dump input as XML after the pass named 'passname''s minimum occurreces */
  int xdump_max; /**< @brief Dump input as XML after the pass named 'passname''s maximum occurreces */
  const char *xdump_help; /**< @brief Dump input as XML after the pass named 'passname' help description.  */
  int list_passes_flag;	/**< @brief List the passes to be run (default=off).  */
  const char *list_passes_help; /**< @brief List the passes to be run help description.  */
  int dont_fail_flag;	/**< @brief Dont fail on error (after parsing) (default=off).  */
  const char *dont_fail_help; /**< @brief Dont fail on error (after parsing) help description.  */
  
  int help_given ;	/**< @brief Whether help was given.  */
  int full_help_given ;	/**< @brief Whether full-help was given.  */
  int version_given ;	/**< @brief Whether version was given.  */
  int verbose_given ;	/**< @brief Whether verbose was given.  */
  int compile_given ;	/**< @brief Whether compile was given.  */
  int pretty_print_given ;	/**< @brief Whether pretty-print was given.  */
  int obfuscate_given ;	/**< @brief Whether obfuscate was given.  */
  unsigned int run_given ;	/**< @brief Whether run was given.  */
  unsigned int r_option_given ;	/**< @brief Whether r-option was given.  */
  int read_ast_xml_given ;	/**< @brief Whether read-ast-xml was given.  */
  int no_validation_given ;	/**< @brief Whether no-validation was given.  */
  int include_given ;	/**< @brief Whether include was given.  */
  unsigned int c_option_given ;	/**< @brief Whether c-option was given.  */
  int generate_c_given ;	/**< @brief Whether generate-c was given.  */
  int extension_given ;	/**< @brief Whether extension was given.  */
  int with_php_given ;	/**< @brief Whether with-php was given.  */
  int optimize_given ;	/**< @brief Whether optimize was given.  */
  int output_given ;	/**< @brief Whether output was given.  */
  int next_line_curlies_given ;	/**< @brief Whether next-line-curlies was given.  */
  int no_leading_tab_given ;	/**< @brief Whether no-leading-tab was given.  */
  int no_line_numbers_given ;	/**< @brief Whether no-line-numbers was given.  */
  int no_nulls_given ;	/**< @brief Whether no-nulls was given.  */
  int no_empty_lists_given ;	/**< @brief Whether no-empty-lists was given.  */
  int tab_given ;	/**< @brief Whether tab was given.  */
  int no_hash_bang_given ;	/**< @brief Whether no-hash-bang was given.  */
  unsigned int dump_given ;	/**< @brief Whether dump was given.  */
  unsigned int udump_given ;	/**< @brief Whether udump was given.  */
  unsigned int ddump_given ;	/**< @brief Whether ddump was given.  */
  unsigned int xdump_given ;	/**< @brief Whether xdump was given.  */
  int list_passes_given ;	/**< @brief Whether list-passes was given.  */
  int dont_fail_given ;	/**< @brief Whether dont-fail was given.  */

  char **inputs ; /**< @brief unamed options (options without names) */
  unsigned inputs_num ; /**< @brief unamed options number */
} ;

/** @brief The additional parameters to pass to parser functions */
struct cmdline_parser_params
{
  int override; /**< @brief whether to override possibly already present options (default 0) */
  int initialize; /**< @brief whether to initialize the option structure gengetopt_args_info (default 0) */
  int check_required; /**< @brief whether to check that all required options were provided (default 0) */
  int check_ambiguity; /**< @brief whether to check for options already specified in the option structure gengetopt_args_info (default 0) */
} ;

/** @brief the purpose string of the program */
extern const char *gengetopt_args_info_purpose;
/** @brief the usage string of the program */
extern const char *gengetopt_args_info_usage;
/** @brief all the lines making the help output */
extern const char *gengetopt_args_info_help[];
/** @brief all the lines making the full help output (including hidden options) */
extern const char *gengetopt_args_info_full_help[];

/**
 * The command line parser
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser (int argc, char * const *argv,
  struct gengetopt_args_info *args_info);

/**
 * The command line parser (version with additional parameters - deprecated)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use cmdline_parser_ext() instead
 */
int cmdline_parser2 (int argc, char * const *argv,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);

/**
 * The command line parser (version with additional parameters)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_ext (int argc, char * const *argv,
  struct gengetopt_args_info *args_info,
  struct cmdline_parser_params *params);

/**
 * Save the contents of the option struct into a (text) file.
 * This file can be read by the config file parser (if generated by gengetopt)
 * @param filename the file where to save
 * @param args_info the option struct to save
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_file_save(const char *filename,
  struct gengetopt_args_info *args_info);

/**
 * Print the help
 */
void cmdline_parser_print_help(void);
/**
 * Print the full help (including hidden options)
 */
void cmdline_parser_print_full_help(void);
/**
 * Print the version
 */
void cmdline_parser_print_version(void);

/**
 * Allocates dynamically a cmdline_parser_params structure and initializes
 * all its fields to 0
 * @return the initialized cmdline_parser_params structure
 */
struct cmdline_parser_params *cmdline_parser_params_init();

/**
 * Initializes the passed gengetopt_args_info structure's fields
 * (also set default values for options that have a default)
 * @param args_info the structure to initialize
 */
void cmdline_parser_init (struct gengetopt_args_info *args_info);
/**
 * Deallocates the string fields of the gengetopt_args_info structure
 * (but does not deallocate the structure itself)
 * @param args_info the structure to deallocate
 */
void cmdline_parser_free (struct gengetopt_args_info *args_info);

/**
 * Checks that all the required options were specified
 * @param args_info the structure to check
 * @param prog_name the name of the program that will be used to print
 *   possible errors
 * @return
 */
int cmdline_parser_required (struct gengetopt_args_info *args_info,
  const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CMDLINE_H */
