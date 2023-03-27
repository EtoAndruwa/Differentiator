#include "Differentiator.h"

size_t print_recur_tree(const Node* node_ptr, FILE* file_ptr)
{
    if(node_ptr == nullptr)
    {
        return NULL_PTR_NODE;
    }

    if(file_ptr == nullptr)
    {
        return ERR_CANNOT_OPEN_OUTPUT;
    }

    fprintf(file_ptr, "(");

    if(node_ptr->type == IS_VAL)
    {
        fprintf(file_ptr, "%.3f", node_ptr->value.node_value);
    }
    else
    {
        fprintf(file_ptr, "%c", node_ptr->value.op_number);
    }

    print_recur_tree(node_ptr->left_child, file_ptr);
    print_recur_tree(node_ptr->right_child, file_ptr);


    fprintf(file_ptr, ")");
}

size_t output_tree(const Node* root_node_ptr)
{
    FILE* file_ptr = fopen("output_tree.txt", "w");
    if(file_ptr == nullptr)
    {
        return ERR_CANNOT_OPEN_OUTPUT;
    }

    print_recur_tree(root_node_ptr, file_ptr);

    if(fclose(file_ptr) == EOF)
    {
        return ERR_CANNOT_CLOSE_OUTPUT;
    }
    free(file_ptr);
    file_ptr = nullptr;
}




