#include "Differentiator.h"

int get_eq_string(Tree* const tree_ptr)
{
    int error_code = get_size(tree_ptr);
    if(error_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, error_code)
        return error_code;
    }

    error_code = get_into_buff(tree_ptr);
    if(error_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, error_code)
        return error_code;
    }

    return RETURN_OK;
}

Node* rule_G(Tree* const tree_ptr, FILE* log_ptr)
{
    Node* root_node = rule_E(tree_ptr, log_ptr);

    if(STRING(POSITION) != '\0')
    {
        ERROR_MESSAGE(stderr, ERR_NO_END_OF_LINE)
        PRINT_PARSE_LOG(log_ptr, FUNC_NAME, RULE_G_WAIT, RULE_OK)
        return nullptr;
    }

    PRINT_PARSE_LOG(log_ptr, FUNC_NAME, RULE_G_WAIT, RULE_OK)

    return root_node;
}

Node* rule_E(Tree* const tree_ptr, FILE* log_ptr)
{
    Node*  right_child = nullptr;
    Node*  comb_node   = nullptr;
    Node*  left_child  = rule_N(tree_ptr, log_ptr);

    if(left_child == nullptr)
    {
        ERROR_MESSAGE(stderr, INVALID_TOK)
        PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_E_ERR)
        return nullptr;
    }

    while(STRING(POSITION) == '-' || STRING(POSITION) == '+')
    {
        if(STRING(POSITION) == '-')
        {
            POSITION++;
            comb_node = SUB_NODE(left_child, nullptr);
        }
        else
        {
            POSITION++;
            comb_node = ADD_NODE(left_child, nullptr);
        }
        right_child = rule_N(tree_ptr, log_ptr);
        comb_node->right_child = right_child;
        left_child = comb_node;
        PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_OK)
    }
    return left_child; 
}

Node* rule_T(Tree* const tree_ptr, FILE* log_ptr)
{
    // Node*  right_child = nullptr;
    // Node*  comb_node   = nullptr;
    // Node*  left_child  = rule_N(tree_ptr, log_ptr);

    // if(left_child == nullptr)
    // {
    //     ERROR_MESSAGE(stderr, INVALID_TOK)
    //     PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_E_ERR)
    //     return nullptr;
    // }

    // while(STRING(POSITION) == '-' || STRING(POSITION) == '+')
    // {
    //     if(STRING(POSITION) == '-')
    //     {
    //         POSITION++;
    //         comb_node = SUB_NODE(left_child, nullptr);
    //     }
    //     else
    //     {
    //         POSITION++;
    //         comb_node = ADD_NODE(left_child, nullptr);
    //     }
    //     right_child = rule_N(tree_ptr, log_ptr);
    //     comb_node->right_child = right_child;
    //     left_child = comb_node;
    //     PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_OK)
    // }
    // return comb_node; 
}

Node* rule_P(Tree* const tree_ptr, FILE* log_ptr)
{


}

Node* rule_N(Tree* const tree_ptr, FILE* log_ptr)
{
    if((('0' <= (STRING(POSITION))) &&  ((STRING(POSITION)) <= '9')) || ((STRING(POSITION)) == '.') || ((STRING(POSITION)) == '-'))
    {   
        double value = atof(&(STRING(POSITION)));

        char arr_of_val[25];
        sprintf(arr_of_val, "%f", value);
        length_double(arr_of_val);
        PRINT_PARSE_LOG(log_ptr, RULE_N, RULE_N_WAIT, RULE_OK)

        printf("pos bef: %ld\n", POSITION);
        POSITION += strlen(arr_of_val);
        printf("pos aft: %ld\n", POSITION);

        printf("\n1\n");

        return NUM_NODE(value);
    }
    else 
    {
        PRINT_PARSE_LOG(log_ptr, FUNC_NAME, RULE_N_WAIT, RULE_N_ERR)
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

    Node* root_node = rule_G(tree_ptr, log_ptr);

    if(fclose(log_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_PARSER_LOG)
        return nullptr;
    }

    return root_node;
}

size_t length_double(char* str_double)
{
    size_t old_length = strlen(str_double) - 1;
    size_t num_of_zeros = 0;
    for(size_t right_end_id = old_length; right_end_id >= 0; right_end_id--)
    {
        if(str_double[right_end_id] != '0')
        {
            break;
        }
        num_of_zeros++;
    }
    str_double[old_length + 1 - num_of_zeros] = '\0'; //shorts the atof value to normal 1.230000 -> 1.23'\0'
}
