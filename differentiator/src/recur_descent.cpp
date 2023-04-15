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
    size_t iteration  = 0;
    Node*  left_child  = nullptr;
    Node*  right_child = nullptr;
    Node*  op_node     = nullptr;
    Node*  comb_node   = nullptr;

    left_child = rule_N(tree_ptr, log_ptr);

    printf("Here\n");

    if(left_child == nullptr)
    {
        ERROR_MESSAGE(stderr, INVALID_TOK)
        PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_E_ERR)
        return nullptr;
    }

    printf("\nchar on cur pos: %c\n", STRING(POSITION));

    while(STRING(POSITION) == '-' || STRING(POSITION) == '+')
    {
        // PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_OK)
        printf("While\n");
        if(iteration == 1)
        {
            if(STRING(POSITION) == '-')
            {
                POSITION++;
                right_child = rule_N(tree_ptr, log_ptr);
                comb_node = SUB_NODE(op_node, right_child);
                op_node = comb_node;
                PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_OK)
                continue;
            }
            else
            {
                POSITION++;
                right_child = rule_N(tree_ptr, log_ptr);
                comb_node = ADD_NODE(op_node, right_child);
                op_node = comb_node;
                PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_OK)
                continue;
            }
        }
        else
        {
            if(STRING(POSITION) == '-')
            {
                op_node = SUB_NODE(left_child, nullptr)
                POSITION++;
                PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_OK)
            }
            else
            {
                op_node = ADD_NODE(left_child, nullptr)
                POSITION++; 
                PRINT_PARSE_LOG(log_ptr, RULE_E, RULE_E_WAIT, RULE_OK)
            }

            op_node->right_child = rule_N(tree_ptr, log_ptr);
            iteration++;
        }
    }
    return op_node; 
}

Node* rule_T(Tree* const tree_ptr, FILE* log_ptr)
{


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
        sprintf(arr_of_val, "%.0f", value);
        size_t legth_val = strlen(arr_of_val);
        printf("lenthg of val: %ld\n", legth_val);
        printf("val: %lf\n", value);

        PRINT_PARSE_LOG(log_ptr, RULE_N, RULE_N_WAIT, RULE_OK)

        printf("pos bef: %ld\n", POSITION);
        POSITION += legth_val;
        printf("pos aft: %ld\n", POSITION);

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

// get
