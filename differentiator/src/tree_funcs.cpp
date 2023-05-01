#include "Differentiator.h"

Tree* tree_ctor() // ok
{
    Tree* tree_ptr = (Tree*)calloc(1, sizeof(Tree));

    if(tree_ptr == nullptr)
    {
        tree_ptr->error_code = ERR_TO_CALLOC_TREE;
        ERROR_MESSAGE(stderr, ERR_TO_CALLOC_TREE);
        return nullptr;
    }
    tree_ptr->error_code  = TREE_OK;
    tree_ptr->size        = 0;        
    tree_ptr->num_of_toks = 1;        
    tree_ptr->cur_tok     = 0;        
    tree_ptr->num_of_vars = 0;        
    tree_ptr->cur_pos_str = 0;       

    tree_ptr->root       = nullptr;
    tree_ptr->tree_buff  = nullptr;
    tree_ptr->file_ptr   = nullptr;
    tree_ptr->toks       = nullptr;
    tree_ptr->vars_arr   = nullptr;
    return tree_ptr;
}

void tree_dtor(Tree* tree_ptr) // ok
{
    dtor_childs(tree_ptr->root);

    free(tree_ptr->tree_buff);

    tree_ptr->tree_buff   = nullptr;
    tree_ptr->cur_pos_str = POISON;

    tree_ptr->file_ptr = nullptr;
    tree_ptr->size     = POISON;

    free(tree_ptr->toks);
    tree_ptr->toks        = nullptr;
    tree_ptr->num_of_toks = POISON;
    tree_ptr->cur_tok     = POISON;

    free(tree_ptr->vars_arr);
    tree_ptr->vars_arr    = nullptr;
    tree_ptr->num_of_vars = POISON;

    tree_ptr->error_code = POISON;

    free(tree_ptr);
    tree_ptr = nullptr;
}

void dtor_childs(Node* node_ptr) // ok
{
    if(node_ptr->left_child != nullptr)
    {
        dtor_childs(node_ptr->left_child);
    }
    if(node_ptr->right_child != nullptr)
    {
        dtor_childs(node_ptr->right_child);
    }

    node_ptr->value.node_value = POISON;
    node_ptr->type = POISON;

    if(node_ptr != nullptr)
    {
        free(node_ptr);
        node_ptr = nullptr;
    }
}

Node* create_node(Tree* tree_ptr, double node_value, int node_type, char* text, Node* left_child, Node* right_child) // ok
{
    if((node_type == IS_VAL || node_type == IS_VARIB || node_type == IS_CNST_VAR) && (left_child != nullptr || right_child != nullptr))
    {
        tree_ptr->error_code = ERR_VAL_VAR_HAS_CHILD;
        ERROR_MESSAGE(stderr, ERR_VAL_VAR_HAS_CHILD)
        return nullptr;
    }

    Node* new_node_ptr = (Node*)calloc(1, sizeof(Node));

    if(new_node_ptr == nullptr)
    {   
        tree_ptr->error_code = ERR_TO_CALLOC_NODE;
        ERROR_MESSAGE(stderr, ERR_TO_CALLOC_NODE)
        return nullptr;
    }
    if(node_type == IS_VAL)
    {
        new_node_ptr->value.node_value = node_value;
    }
    else if((node_type == IS_VARIB || node_type == IS_CNST_VAR) && text != nullptr)
    {
        strcpy(new_node_ptr->value.text, text); 
    }
    else
    {
        new_node_ptr->value.op_number = int(node_value);
    }
    
    new_node_ptr->type = node_type;
    new_node_ptr->left_child  = left_child;
    new_node_ptr->right_child = right_child;

    return new_node_ptr;
}

void print_leaves(Node* node_ptr) // OLD
{
    if(node_ptr->left_child == nullptr && node_ptr->right_child == nullptr && node_ptr->type == IS_VAL)
    {
        printf("%f ", node_ptr->value.node_value);
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

void print_preorder(Node* node_ptr) // OLD
{
    if(node_ptr == nullptr)
    {
        return;
    }
    printf("(");
    if(node_ptr->type == IS_VAL)
    {
        printf("%.3f", node_ptr->value.node_value);
    }
    else
    {
        printf("%c", node_ptr->value.op_number);
    }
    print_preorder(node_ptr->left_child);
    print_preorder(node_ptr->right_child);
    printf(")");
}

void print_inorder(Node* node_ptr) // OLD
{
    if(node_ptr == nullptr)
    {
        return;
    }
    printf("(");
    print_inorder(node_ptr->left_child);
    if(node_ptr->type == IS_VAL)
    {
        printf("%.3f", node_ptr->value.node_value);
    }
    else if(node_ptr->type == IS_OP)
    {
        printf("%c", node_ptr->value.op_number);
    }
    else
    {
        printf("%s", node_ptr->value.text);
    }
    print_inorder(node_ptr->right_child);
    printf(")");
}

void print_postorder(Node* node_ptr) // OLD
{
    if(node_ptr == nullptr)
    {
        return;
    }
    printf("(");
    print_postorder(node_ptr->left_child);
    print_postorder(node_ptr->right_child);
    if(node_ptr->type == IS_VAL)
    {
        printf("%.3f", node_ptr->value.node_value);
    }
    else
    {
        printf("%c", node_ptr->value.op_number);
    }
    printf(")");
}
