#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "../../graphviz/src/graphviz.h"

enum return_codes
{
    NULL_PTR_NODE = 1,
};

enum error_codes_diff
{
    ERR_CANNOT_OPEN_OUTPUT  = 1,
    ERR_CANNOT_CLOSE_OUTPUT = 2, 
};

size_t print_recur_tree(const Node* node_ptr, FILE* file_ptr);
size_t output_tree(const Node* root_node_ptr);


#endif
