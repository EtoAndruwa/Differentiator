#include "Differentiator.h"

Tree* tree_ctor()
{
    Tree* tree_ptr = (Tree*)calloc(1, sizeof(Tree));

    if(tree_ptr == nullptr)
    {
        tree_ptr->error_code = ERR_TO_CALLOC_TREE;
    }
    tree_ptr->error_code = TREE_OK;

    return tree_ptr;
}

void  tree_dtor(Tree* tree_ptr)
{
    dtor_childs(tree_ptr->root);

    tree_ptr->root = nullptr;
    tree_ptr->error_code = TREE_OK;
}

void  dtor_childs(Node* node_ptr)
{
    if(node_ptr->left_child != nullptr)
    {
        dtor_childs(node_ptr->left_child);
        node_ptr->left_child = nullptr;
    }
    if(node_ptr->right_child != nullptr)
    {
        dtor_childs(node_ptr->right_child);
        node_ptr->right_child = nullptr;
    }

    node_ptr->value.node_value = POISON;
    node_ptr->value.op_number  = POISON;

    free(node_ptr);
    node_ptr = nullptr;
}

Node* create_node(Tree* tree_ptr, double node_value, int node_type, Node* left_child, Node* right_child)
{
    Node* new_node_ptr = (Node*)calloc(1, sizeof(Node));

    if(new_node_ptr == nullptr)
    {   
        tree_ptr->error_code = ERR_TO_CALLOC_NODE;
    }

    if(node_type == IS_VAL)
    {
        new_node_ptr->value.node_value = node_value;
    }
    else
    {
        new_node_ptr->value.op_number = int(node_value);
    }
    
    new_node_ptr->type = node_type;
    new_node_ptr->left_child = left_child;
    new_node_ptr->right_child = right_child;

    return new_node_ptr;
}

// Node* search_node(Node* node_ptr, Node_data search_value)
// {
//     if(node_ptr->node_value == search_value)
//     {
//         return node_ptr;
//     }
//     else if(node_ptr->node_value > search_value && node_ptr->left_child != nullptr)
//     {
//         search_node(node_ptr->left_child, search_value);
//     }
//     else if(node_ptr->node_value < search_value && node_ptr->right_child != nullptr)
//     {
//         search_node(node_ptr->right_child, search_value);
//     }
//     else    
//     {   
//         return nullptr;
//     }
// }

// void  print_leaves(Node* node_ptr)
// {
//     if(node_ptr->left_child == nullptr && node_ptr->right_child == nullptr)
//     {
//         printf("%s ", node_ptr->node_value);
//         return;
//     }
//     if(node_ptr->left_child != nullptr)
//     {
//         print_leaves(node_ptr->left_child);
//     }
//     if(node_ptr->right_child != nullptr)
//     {
//         print_leaves(node_ptr->right_child);
//     }
// }

