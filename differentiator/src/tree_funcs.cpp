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

void tree_dtor(Tree* tree_ptr)
{
    dtor_childs(tree_ptr->root);
    tree_ptr->error_code = TREE_OK;
}

void dtor_childs(Node* node_ptr)
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

    free(node_ptr->node_value);
    node_ptr->node_value == nullptr;
    free(node_ptr);
    node_ptr = nullptr;
}

Node* create_node(Tree* tree_ptr, Node_data node_value)
{
    Node* new_node_ptr = (Node*)calloc(1, sizeof(Node));

    if(new_node_ptr == nullptr)
    {   
        tree_ptr->error_code = ERR_TO_CALLOC_NODE;
    }

    new_node_ptr->node_value = node_value;
    new_node_ptr->left_child = nullptr;
    new_node_ptr->right_child = nullptr;

    return new_node_ptr;
}

void link_node_left(Node* parent_ptr, Node* child_ptr)
{
    parent_ptr->left_child = child_ptr;
}

void link_node_right(Node* parent_ptr, Node* child_ptr)
{
    parent_ptr->right_child = child_ptr;
}

Node* search_node(Node* node_ptr, Node_data search_value)
{
    if(node_ptr->node_value == search_value)
    {
        return node_ptr;
    }
    else if(node_ptr->node_value > search_value && node_ptr->left_child != nullptr)
    {
        search_node(node_ptr->left_child, search_value);
    }
    else if(node_ptr->node_value < search_value && node_ptr->right_child != nullptr)
    {
        search_node(node_ptr->right_child, search_value);
    }
    else    
    {   
        return nullptr;
    }
}

void print_leaves(Node* node_ptr)
{
    if(node_ptr->left_child == nullptr && node_ptr->right_child == nullptr)
    {
        printf("%s ", node_ptr->node_value);
        return;
    }
    if(node_ptr->left_child != nullptr)
    {
        print_leaves(node_ptr->left_child);
    }
    if(node_ptr->right_child != nullptr)
    {
        print_leaves(node_ptr->right_child);
    }
}

