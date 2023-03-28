#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tree.h"
#include "../../graphviz/src/graphviz.h"

#define EPS 1e-7

enum return_codes
{
    NULL_PTR_NODE = 1,
};

enum error_codes_diff
{
    ERR_CANNOT_OPEN_OUTPUT  = 1,
    ERR_CANNOT_CLOSE_OUTPUT = 2, 
};

size_t print_recur_tree(const Node* const node_ptr, FILE* file_ptr);
size_t output_tree(const Node* const root_node_ptr);
double eval(const Node* const node_ptr);
void print_recur_code(const Node* const node_ptr, FILE* file_ptr);
size_t generate_cpu_code(const Node* const root_node_ptr);
double func_ADD(double value_1, double value_2);
double func_SUB(double value_1, double value_2);
double func_MUL(double value_1, double value_2);
double func_DIV(double value_1, double value_2);


#endif
