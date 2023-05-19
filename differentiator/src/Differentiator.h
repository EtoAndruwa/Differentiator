#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

/*####################################################################################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/*####################################################################################################################################################################*/

#define EPS 1e-7        // Used in comparisson of doubles
#define PI  3.14159265  // Used in calculations
static const size_t MAX_LEN_VARIB  = 21; // The maximum length of the variable text = 20 + 1 terminating symbol
static const char* LATEX_FILE_NAME = "Tree.tex";
static const char* LATEX_DIR_NAME  = "latex/";

/*####################################################################################################################################################################*/

#include "tree.h"                         // The tree header with struct
#include "DSL.h"                          // The DSL header with macros
#include "../../graphviz/src/graphviz.h"  // The graphviz lib's header with funcs

/*####################################################################################################################################################################*/

/**
 * @brief This enum contains the codes of returns of the functions
 */
enum return_codes
{
    NULL_PTR_NODE  = 1,
    NOT_ALL_DIGITS = 2,
    IS_INT         = 3,
    IS_FLOAT       = 4,
    VALID_TOK      = 5,
    INVALID_TOK    = 6,
    EXISTING_FUNC  = 7,
    NON_EXIST_FUNC = 8,
    IS_POSITIVE    = 9,
    IS_NEGATIVE    = 10,
    IS_ZERO        = 11,
    EQ_FOR_LATEX   = 12,
    EQ_FOR_PLOT    = 13,
};

/**
 * @brief This enum contains all error codes of the differetiator's part of project
 */
enum error_codes_diff
{
    ERR_CANNOT_OPEN_OUTPUT  = -1,
    ERR_CANNOT_CLOSE_OUTPUT = -2, 
    ERR_CANNOT_OPEN_INPUT   = -3,
    ERR_CANNOT_CLOSE_INPUT  = -4,
    ERR_INVALID_VAR_NUM     = -5,
    ERR_TO_CALLOC_VARS      = -6,
    ERR_INVALID_VAR_TEXT    = -7,
    ERR_TO_CALLOC_TOKS      = -8,
    ERR_INVALID_TOKEN       = -9,
    ERR_OPEN_PARSER_LOG     = -10,
    ERR_CLOSE_PARSER_LOG    = -11,
    ERR_NO_END_OF_LINE      = -12,
    ERR_NO_CLOSING_BRACKETS = -12,
    ERR_UNKNOWN_FUNC        = -13,
    ERR_NULL_PTR_NODE       = -14,
    ERR_EMPTY_INP_FILE      = -15,
    ERR_TO_REALLOC_TOKS     = -16,
    ERR_INVALID_ARGUMENT    = -17,
    ERR_NOT_A_SPACE         = -18,
    ERR_FOUND_DIFF_NUM_VARS = -19,
    ERR_UNKNOWN_OPERATOR    = -20,
    ERR_DIV_TO_ZERO         = -21,
    ERR_UNCERTAINTY         = -22,
    ERR_OPEN_LATEX_FILE     = -23,
    ERR_CLOSE_LATEX_FILE    = -24,
    ERR_PRINT_LATEX         = -25,
    ERR_INVALID_TYPE_NODE   = -26,
};

/*####################################################################################################################################################################*/

// MAIN FUNCS

/**
 * @brief Prints recursively tree in full brackets form into the given file
 * 
 * @param node_ptr The pointer to the root node or to the root of the subtree
 * @param file_ptr The pointer to the file
 */
void print_recur_tree(const Node* const node_ptr, FILE* file_ptr); 

/**
 * @brief Opens the file and call the print_recur_tree
 * 
 * @param root_node_ptr The pointer to the root node or to the root of the subtree
 * @param file_name The name of the file
 * @return The error code or RETURN_OK
 */
int output_tree(const Node* const root_node_ptr, char* file_name);

/**
 * @brief Evaluates the tree 
 * 
 * @param tree_ptr The pointer to the tree struct 
 * @param node_ptr The pointer to the root node of the tree
 * @return The result of evaluation
 */
double eval(const Tree* const tree_ptr, const Node* const node_ptr); 

/**
 * @brief Gets the name of the function from its enum code (Sin -> sin) [CANNOT BE REPLACED BE ##]
 * 
 * @param func_code The enum code of the function
 * @return The pointer to the string with the name of the function
 */
char* get_string_func(size_t func_code); 

/**
 * @brief Creates the full differentiated tree by all variables
 * 
 * @param tree_ptr The resultant new root of the tree
 * @return int The error code or RETURN_OK
 */
int full_diff(Tree* tree_ptr); 

/**
 * @brief Copies the subtree's info
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param node_ptr The pointer to the node which copy must be created
 * @return The pointer to the new copy of the node
 */
Node* copy_subtree(Tree* tree_ptr, Node* node_ptr);

/**
 * @brief Creates the differentiate of the tree by variable
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param node_ptr The pointer to the root of the tree that must be differentiated
 * @param varib_text The pointer to the string with the name of the variable
 * @return The pointer to the new differentiated root of the tree
 */
Node* diff_tree(Tree* tree_ptr, Node* node_ptr, char* varib_text);

/**
 * @brief (OLD) Creates the tree using strtok (OUT OF DATE)
 * 
 * @param tree_ptr The pointer to the tree struct
 * @return The pointer to the root of the tree
 */
Node* input_tree_old(Tree* tree_ptr); 

/**
 * @brief Checks the text for being float
 * 
 * @param num_text The pointer to the text of the value that must be checked
 * @return Returns the enum value 
 */
size_t check_is_float(char* num_text); 

/**
 * @brief Get the text from the input file into the inner buffer
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param file_name The pointer to the string with the name of input file
 * @return The error code or RETURN_OK 
 */
int get_into_buff(Tree* tree_ptr, char* file_name); 

/**
 * @brief Gets the size of the input file in order to calloc the buffer
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param file_name The pointer to the string with the name of input file
 * @return The error code or RETURN_OK 
 */
int get_size(Tree* tree_ptr, char* file_name); 

/**
 * @brief (OLD) Gets the tokens of the tree using strtok (OUT OF DATE)
 * 
 * @param tree_ptr The pointer to the tree struct
 * @return The error code or RETURN_OK 
 */
int get_tokens(Tree* tree_ptr); 

/**
 * @brief (OLD) Reallocs the array of tokens (OUT OF DATE)
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param i Current number of toks
 * @return The error code or RETURN_OK
 */
int realloc_toks(Tree* tree_ptr, size_t i);

/**
 * @brief (OLD) Differentiates the tree using tokens (OUT OF DATE)
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param varib_text The pointer to the string with the variable's name on which differentiation must be done
 * @return The pointer to the differentiated node
 */
Node* diff_tree_old(Tree* tree_ptr, char* varib_text);

/**
 * @brief Gets the vars' text and their value, stores in the array of structs
 * 
 * @param tree_ptr The pointer to the tree struct
 * @return The error code or RETURN_OK 
 */
int get_vars(Tree* tree_ptr);

/**
 * @brief Makes the differentiated equation shorter based on some rules
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param node_ptr The pointer to the node that must be shortened
 * @return The pointer to the new shortened node
 */
Node* shortener(Tree* tree_ptr, Node* node_ptr);

/*####################################################################################################################################################################*/

// EVAL

/**
 * @brief Calculates the sum of two values
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must be used
 * @return The result of function 
 */
double func_Add(double value_1, double value_2);

/**
 * @brief Calculates the difference of two values
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must be used
 * @return The result of function 
 */
double func_Sub(double value_1, double value_2);

/**
 * @brief Calculates the product of two values
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must be used
 * @return The result of function 
 */
double func_Mul(double value_1, double value_2);

/**
 * @brief Calculates the division of two values
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must be used
 * @return The result of function 
 */
double func_Div(double value_1, double value_2);

/**
 * @brief Calculates the cos of value
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Cos(double value_1, double value_2 = 0); 

/**
 * @brief Calculates the sin of value
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Sin(double value_1, double value_2 = 0); 

/**
 * @brief Calculates the tan of value
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Tan(double value_1, double value_2 = 0); 

/**
 * @brief Calculates the asin of value
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Asin(double value_1, double value_2 = 0); 

/**
 * @brief Calculates the acos of value
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Acos(double value_1, double value_2 = 0); 

/**
 * @brief Calculates the sqrt of value
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Sqrt(double value_1, double value_2 = 0);

/**
 * @brief Calculates the exponent of value
 * 
 * @param value_1 Value must be used - the power of exp
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Exp(double value_1, double value_2 = 0); 

/**
 * @brief Calculates the natural log of value
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Log(double value_1, double value_2 = 0); 

/**
 * @brief Calculates the cot of value
 * 
 * @param value_1 Value must be used
 * @param value_2 Value must NOT be used
 * @return The result of function 
 */
double func_Cot(double value_1, double value_2 = 0); 

/**
 * @brief Calculates the power of values
 * 
 * @param value_1 Value must be used - The base of exp
 * @param value_2 Value must NOT be used - The power of exp
 * @return The result of function
 */
double func_Pow(double value_1, double value_2); 

/**
 * @brief Checks the value for beeing equal to the zero, beeing positive of negative
 * 
 * @param value The float value that must be checked
 * @return The enum result of check
 */
int check_is_positive(double value);

/*####################################################################################################################################################################*/

// RECUR DESC

/**
 * @brief This rule gets the float values
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param log_ptr The pointer to the string with the parser log's name
 * @return The pointer to the created by the rule node
 */
Node* rule_N(Tree* const tree_ptr, FILE* log_ptr);

/**
 * @brief This rule gets the root of the tree  
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param log_ptr The pointer to the string with the parser log's name
 * @return The pointer to the created by the rule node 
 */
Node* rule_G(Tree* const tree_ptr, FILE* log_ptr);

/**
 * @brief This rule gets the sum/sub nodes
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param log_ptr The pointer to the string with the parser log's name
 * @return The pointer to the created by the rule node
 */
Node* rule_E(Tree* const tree_ptr, FILE* log_ptr);

/**
 * @brief This rule gets the mul/div nodes
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param log_ptr The pointer to the string with the parser log's name
 * @return The pointer to the created by the rule node 
 */
Node* rule_T(Tree* const tree_ptr, FILE* log_ptr);

/**
 * @brief This rule gets the equation in the brackets or rules N
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param log_ptr The pointer to the string with the parser log's name
 * @return The pointer to the created by the rule node
 */
Node* rule_P(Tree* const tree_ptr, FILE* log_ptr);

/**
 * @brief This rule gets the variable node
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param log_ptr The pointer to the string with the parser log's name
 * @return The pointer to the created by the rule node 
 */
Node* rule_V(Tree* const tree_ptr, FILE* log_ptr);

/**
 * @brief This rule gets the function nodes
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param log_ptr The pointer to the string with the parser log's name
 * @return The pointer to the created by the rule node 
 */
Node* rule_F(Tree* const tree_ptr, FILE* log_ptr);

/**
 * @brief This rule gets the power nodes
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param log_ptr The pointer to the string with the parser log's name
 * @return The pointer to the created by the rule node 
 */
Node* rule_Pow(Tree* const tree_ptr, FILE* log_ptr);

/**
 * @brief Get the eq string object
 * 
 * @param tree_ptr The pointer to the tree struct
 * @param file_name The pointer to the string with the parser log's name
 * @return The error code or RETURN_OK 
 */
int get_eq_string(Tree* const tree_ptr, char* file_name);

/**
 * @brief Skips the spaces between expressions
 * 
 * @param tree_ptr The pointer to the tree struct
 * @return Returns the current position in the string (FOR SOME FUNCTIONS REQUIRED RETURN)
 */
size_t skip_spaces(Tree* tree_ptr);

/**
 * @brief Calculates the length of the float's tail in order to print into latex short equations
 * 
 * @param str_double The pointer to the string with float value
 * @return The length of the float's tail 
 */
size_t length_double(char* str_double);

/**
 * @brief Gets the tree from the file using recursive descent
 * 
 * @param tree_ptr The pointer to the tree struct
 * @return The pointer to the root of the tree
 */
Node* get_recur_tree(Tree* const tree_ptr);

/*####################################################################################################################################################################*/

// LATEX

/**
 * @brief Prints the header of the latex document
 * 
 * @return The error code or RETURN_OK 
 */
int print_header_latex();

/**
 * @brief Prints the footer of the latex file and calls add_final_diff_latex, convert_tex_pdf
 * 
 * @param node_ptr The pointer to the subtree that must be printed as final result of diff
 * @return The error code or RETURN_OK 
 */
int print_footer_latex(Node* node_ptr);

/**
 * @brief Prints already differentiated tree to the latex file
 * 
 * @param node_ptr The pointer to the node 
 * @return The error code or RETURN_OK 
 */
int add_equation_diff_latex(Node* node_ptr);

/**
 * @brief Prints the subtree in form of equation in latex
 * 
 * @param node_ptr The pointer to the subtree that must be displayed in latex
 * @param tex_file_ptr The pointer to the file
 * @param key The control key which is needed in order to switch between the print modes
 * @param var_name The name of var which is needed in order to print equation for pgfplots' plot
 * @param tree_ptr The pointer to the tree struct
 * @return The error code or RETURN_OK 
 */
int print_latex_eq(Node* node_ptr, FILE* tex_file_ptr, int key, char* var_name, Tree* tree_ptr);

/**
 * @brief Converts the tex file to the pdf format
 * 
 * @param file_dir_name The path to the tex file
 * @return The error code or RETURN_OK 
 */
int convert_tex_pdf(char* file_dir_name);

/**
 * @brief Creates pretty preamble for the equation before the differentiation
 * 
 * @param node_ptr The pointer to the equation
 * @return The error code or RETURN_OK 
 */
int add_preamble_latex(Node* node_ptr);

/**
 * @brief Prints the final result of the diff 
 * 
 * @param node_ptr The pointer to the already differentiated root of the tree
 * @return The error code or RETURN_OK 
 */
int add_final_diff_latex(Node* node_ptr);

/**
 * @brief Creates the plot using pgfplots in latex right from the subtree's equation
 * 
 * @param node_ptr The pointer to the subtree that must be displayed in form of plot
 * @param var_name The name of the variable on which plot will be based
 * @param tree_ptr The pointer to the tree struct
 * @return The error code or RETURN_OK 
 */
int print_plot_latex(Node* node_ptr, char* var_name, Tree* tree_ptr);

/*####################################################################################################################################################################*/

#endif
