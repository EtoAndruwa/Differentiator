#include "Differentiator.h"

int get_eq_string(Tree* const tree_ptr)
{
    int error_code = get_size(tree_ptr);
    if(error_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, error_code)
        return error_code;
    }
    printf("Here1\n");

    error_code = get_into_buff(tree_ptr);
    if(error_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, error_code)
        return error_code;
    }
     printf("Here2\n");

    return RETURN_OK;
}

Node* rule_G(Tree* const tree_ptr, FILE* log_ptr)
{

}

Node* rule_E(Tree* const tree_ptr, FILE* log_ptr)
{


}

Node* rule_T(Tree* const tree_ptr, FILE* log_ptr)
{


}

Node* rule_P(Tree* const tree_ptr, FILE* log_ptr)
{


}


Node* rule_N(Tree* const tree_ptr, FILE* log_ptr)
{
    
    PRINT_PARSE_LOG(log_ptr, FUNC_NAME, RULE_N_WAIT)
    
    if((('0' <= (STRING(POSITION))) &&  ((STRING(POSITION)) <= 9)) || ((STRING(POSITION)) == '.') || ((STRING(POSITION)) == '-'))
    {   
        double value = atof(&(STRING(POSITION)));

        char arr_of_val[25];
        sprintf(arr_of_val, "%f", value);
        size_t legth_val = strlen(arr_of_val);
        printf("lenthg of val: %ld\n", legth_val);
        printf("val: %lf\n", value);

        printf("pos bef: %ld\n", POSITION);
        POSITION += legth_val;
        printf("pos aft: %ld\n", POSITION);

        return NUM_NODE(value);
    }
    else 
    {
        ERROR_MESSAGE(stderr, INVALID_TOK)
        return nullptr;
    }
}

// int rule_FL(Tree* const tree_ptr, FILE* log_ptr)
// {


// }

Node* get_recur_tree(Tree* const tree_ptr)
{
    FILE* log_ptr = fopen("parser_log.txt","w");
    if(log_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_PARSER_LOG)
        return nullptr;
    }

    Node* root_node = rule_N(tree_ptr, log_ptr);

    if(fclose(log_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_PARSER_LOG)
        return nullptr;
    }

    return root_node;
}