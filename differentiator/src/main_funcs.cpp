#include "Differentiator.h"

void print_recur_tree(const Node* const node_ptr, FILE* file_ptr) // ok
{
    if(node_ptr == nullptr)
    {
        return;
    }
    fprintf(file_ptr, "(");

    if(node_ptr->type == IS_VAL)
    {
        fprintf(file_ptr, "%lf", node_ptr->value.node_value);
    }
    else if(node_ptr->type == IS_OP)
    {
        fprintf(file_ptr, "%c", node_ptr->value.op_number);
    }
    else if(node_ptr->type == IS_FUNC)
    {
        fprintf(file_ptr, "%s", get_string_func(node_ptr->value.op_number));
    }
    else
    {
        fprintf(file_ptr, "%s", node_ptr->value.text);
    }

    print_recur_tree(node_ptr->left_child, file_ptr);
    print_recur_tree(node_ptr->right_child, file_ptr);

    fprintf(file_ptr, ")");
}

int output_tree(const Node* const root_node_ptr, char* file_name) // ok
{
    FILE* file_ptr = fopen(file_name, "w");
    if(file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_CANNOT_OPEN_OUTPUT)
        return ERR_CANNOT_OPEN_OUTPUT;
    }

    print_recur_tree(root_node_ptr, file_ptr);

    if(fclose(file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CANNOT_CLOSE_OUTPUT)
        return ERR_CANNOT_CLOSE_OUTPUT;
    }
}

double eval(const Tree* const tree_ptr, const Node* const node_ptr) // ok
{
    if(node_ptr->type == IS_CNST_VAR)
    {   
        ERROR_MESSAGE(stderr, ERR_FOUND_DIFF_NUM_VARS)
        return NAN;
    }

    if(node_ptr == nullptr)
    {
        return NAN;
    }
    if(node_ptr->type == IS_VAL)
    {
        return node_ptr->value.node_value;
    }
    if(node_ptr->type == IS_VARIB)
    {
        for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
        {
            if(strcmp(node_ptr->value.text, tree_ptr->vars_arr[cur_var].var_text) == 0) // to get the value of the variable
            {
                return tree_ptr->vars_arr[cur_var].var_value;
            }
        }
    }

    double val1 = 0;
    double val2 = 0;
    double result = 0;

    switch(node_ptr->value.op_number)
    {
        #define DEF_OP(code, ...)                                                                                       \ 
            case code:                                                                                                  \
                val1 = eval(tree_ptr, node_ptr->left_child);                                                            \
                val2 = eval(tree_ptr, node_ptr->right_child);                                                           \
                                                                                                                        \
                if(val1 != NAN && val2 != NAN)                                                                          \
                {                                                                                                       \
                    return func_ ## code(val1, val2);                                                                   \   
                }                                                                                                       \
                ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)                                                             \     
                return NAN;                                                                                             \
                
        #define DEF_FUNC(code, ...)                                                                                         \ 
            case code:                                                                                                      \   
                if(code == Pow)                                                                                             \
                {                                                                                                           \
                    val1 = eval(tree_ptr, node_ptr->left_child);                                                            \
                    val2 = eval(tree_ptr, node_ptr->right_child);                                                           \
                                                                                                                            \
                    if(val1 != NAN && val2 != NAN)                                                                          \
                    {                                                                                                       \
                        return func_ ## code(val1, val2);                                                                   \   
                    }                                                                                                       \
                    ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)                                                             \    
                    return NAN;                                                                                             \
                }                                                                                                           \
                val1 = eval(tree_ptr, node_ptr->left_child);                                                                \
                if(val1 != NAN)                                                                                             \ 
                {                                                                                                           \
                    return func_ ## code(val1, 0);                                                                          \   
                }                                                                                                           \
                ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)                                                                 \    
                return NAN;                                                                                                 \

            #include "def_cmd.h"

            #undef DEF_OP
            #undef DEF_FUNC

            default:
                ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC);
                return NAN;
    }
}

Node* input_tree_old(Tree* tree_ptr) // OLD
{
    if(tree_ptr->cur_tok < tree_ptr->num_of_toks)
    {
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VAL)
        {   
            GET_CUR_TOK()
            return NUM_NODE(tree_ptr->toks[cur_tok].value.flt_val)
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VARIB)
        {
            GET_CUR_TOK()
            return VARIB_NODE(tree_ptr->toks[cur_tok].text)
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_CNST_VAR)
        {
            GET_CUR_TOK()
            return CNST_VARIB_NODE(tree_ptr->toks[cur_tok].text)
        }

        Node* left  = nullptr;
        Node* right = nullptr;
        switch(tree_ptr->toks[tree_ptr->cur_tok].value.int_val)
        {
            #define DEF_OP(code, ...)                                               \
                case code: tree_ptr->cur_tok++;                                     \
                    left  = input_tree_old(tree_ptr);                               \
                    right = input_tree_old(tree_ptr);                               \
                    return create_node(tree_ptr, code, IS_OP, "", left, right);     \

            /*
                For POW func we need to read two arguments
                base and exp therefore is statement included 
                into the macro
            */
            #define DEF_FUNC(code, ...)                                                     \
                case code: tree_ptr->cur_tok++;                                             \
                    if(code == Pow)                                                         \
                    {                                                                       \
                        left  = input_tree_old(tree_ptr);                                   \
                        right = input_tree_old(tree_ptr);                                   \
                        return create_node(tree_ptr, code, IS_FUNC, "", left, right);       \
                    }                                                                       \
                    left = input_tree_old(tree_ptr);                                        \
                    return create_node(tree_ptr, code, IS_FUNC, "", left);                  \

            #include "def_cmd.h"
            #undef DEF_OP
            #undef DEF_FUNC

            default:
                ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
                tree_ptr->error_code = ERR_UNKNOWN_FUNC;
                return nullptr;
        }
    }
}

size_t check_is_float(char* num_text) // ok
{
    size_t is_digits = IS_FLOAT; 
    size_t length_text = strlen(num_text);

    if(num_text[0] == '-' && length_text == 1) // case for '-' minus sign only
    {
        return NOT_ALL_DIGITS;
    }
    if(num_text[0] != '-' && isdigit(num_text[0]) == 0) // case for negative float
    {    
        return NOT_ALL_DIGITS;
    }
    for(size_t i  = 1; i < length_text; i++)
    {
        if(isdigit(num_text[i]) == 0 && num_text[i] != '.') // If the character is not a digit
        {
            is_digits = NOT_ALL_DIGITS;
        }
    }

    return is_digits; // Returns IS_FLOAT if the word if float
}

int get_size(Tree* tree_ptr, char* file_name) // ok
{
    FILE* file_inp_ptr = fopen(file_name, "rb");
    if(file_inp_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_CANNOT_OPEN_INPUT)
        return ERR_CANNOT_OPEN_INPUT;
    }

    fseek(file_inp_ptr, 0, SEEK_END); // Puts the pointer inside the file to the end of it
    tree_ptr->size = ftell(file_inp_ptr); // Get the size of the file with '\r'!

    if(tree_ptr->size == 0) 
    {
        tree_ptr->error_code = ERR_EMPTY_INP_FILE; 
        ERROR_MESSAGE(stderr, ERR_EMPTY_INP_FILE)
        return tree_ptr->error_code;
    } 

    if(fclose(file_inp_ptr) == EOF)
    {   
        ERROR_MESSAGE(stderr, ERR_CANNOT_CLOSE_INPUT)
        return ERR_CANNOT_CLOSE_INPUT;
    }
    return RETURN_OK;
}

int get_into_buff(Tree* tree_ptr, char* file_name) 
{
    FILE* file_inp_ptr = fopen(file_name, "rb");
    if(file_inp_ptr == nullptr)
    {  
        tree_ptr->error_code = ERR_CANNOT_OPEN_INPUT;
        ERROR_MESSAGE(stderr, ERR_CANNOT_OPEN_INPUT)
        return ERR_CANNOT_OPEN_INPUT;
    }

    tree_ptr->tree_buff = (char*)calloc(1, sizeof(char) * (tree_ptr->size + 1)); // Allocates enough memmory for the buffer of chars  

    if(tree_ptr->tree_buff == nullptr)
    {
        tree_ptr->error_code = ERR_TO_CALLOC_BUFFER;
        ERROR_MESSAGE(stderr, ERR_TO_CALLOC_BUFFER)
        return tree_ptr->error_code;
    }

    int num_read = fread(tree_ptr->tree_buff, sizeof(char), tree_ptr->size, file_inp_ptr); // Reads the file into the buffer
    
    if((num_read <= 0) && (num_read > tree_ptr->size))
    {
        tree_ptr->error_code = ERR_EMPTY_INP_FILE;
        ERROR_MESSAGE(stderr, ERR_EMPTY_INP_FILE)
        return tree_ptr->error_code;       
    }
    tree_ptr->tree_buff[tree_ptr->size] = '\0'; // Makes form the file null-terminated string

    if(fclose(file_inp_ptr) == EOF)
    {   
        tree_ptr->error_code = ERR_CANNOT_CLOSE_INPUT;
        ERROR_MESSAGE(stderr, ERR_CANNOT_CLOSE_INPUT)
        return ERR_CANNOT_CLOSE_INPUT;
    }
    file_inp_ptr = nullptr;
    return RETURN_OK;
}

int get_tokens(Tree* tree_ptr) // ok
{
    char* token_val = strtok(tree_ptr->tree_buff,"( ) \n\r");
    tree_ptr->toks  = (Tokens*)calloc(1, sizeof(Tokens));
    
    if(tree_ptr->toks == nullptr)
    {
        tree_ptr->error_code = ERR_TO_CALLOC_TOKS;
        ERROR_MESSAGE(stderr, ERR_TO_CALLOC_TOKS)
        return tree_ptr->error_code;
    }

    size_t toks_num = 0; // current total number of tokens
    while(token_val != nullptr)                        
    {  
        realloc_toks(tree_ptr, toks_num); // Reallocs the struct with tokens
        if(tree_ptr->error_code != TREE_OK)
        {
            ERROR_MESSAGE(stderr, tree_ptr->error_code)
            return tree_ptr->error_code;
        }

        if(check_is_float(token_val) == IS_FLOAT)
        {   
            tree_ptr->toks[toks_num].value.flt_val = atof(token_val);
            tree_ptr->toks[toks_num].type  = IS_VAL;
        }
        else
        {
            int invalid_tok = INVALID_TOK;

            #define DEF_OP(name, code, char_val)                        \  
                if(token_val[0] == char_val)                            \
                {                                                       \
                    invalid_tok = VALID_TOK;                            \
                    tree_ptr->toks[toks_num].value.int_val = code;      \
                    tree_ptr->toks[toks_num].type = IS_OP;              \
                }                                                       \                                         

            #define DEF_FUNC(name, code, str_val)                       \
                if(strcasecmp(token_val, str_val) == 0)                 \
                {                                                       \
                    invalid_tok = VALID_TOK;                            \
                    tree_ptr->toks[toks_num].value.int_val = code;      \
                    tree_ptr->toks[toks_num].type = IS_FUNC;            \
                }                                                       \

            #include "def_cmd.h"
            #undef DEF_OP
            #undef DEF_FUNC

            if(invalid_tok == INVALID_TOK)
            {
                int is_const = IS_CNST_VAR;
                for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
                {
                    if(strcmp(tree_ptr->vars_arr[cur_var].var_text, token_val) == 0)
                    {
                        strcpy(tree_ptr->toks[toks_num].text, token_val);
                        tree_ptr->toks[toks_num].type = IS_VARIB;
                        is_const = IS_VARIB;
                        break;   
                    }
                }
                if(is_const == IS_CNST_VAR)
                {
                    strcpy(tree_ptr->toks[toks_num].text, token_val);
                    tree_ptr->toks[toks_num].type  = IS_CNST_VAR;
                }
            }
        }

        token_val = strtok(NULL, "( ) \n\r");
        toks_num++;
    }

    free(tree_ptr->tree_buff);
    tree_ptr->tree_buff = nullptr;
    return RETURN_OK;
}

int realloc_toks(Tree* tree_ptr, size_t i) // ok
{
    if(tree_ptr->num_of_toks == i)
    {
        tree_ptr->num_of_toks++;
        tree_ptr->toks = (Tokens*)realloc(tree_ptr->toks, tree_ptr->num_of_toks * sizeof(Tokens)); // The pointer to the array of structs

        if(tree_ptr->toks == nullptr)
        {
            tree_ptr->error_code = ERR_TO_REALLOC_TOKS;
            ERROR_MESSAGE(stderr,ERR_TO_REALLOC_TOKS)
            return tree_ptr->error_code;
        }
    }

    return RETURN_OK;
}

Node* diff_tree_old(Tree* tree_ptr, char* varib_text) // ok
{
    if(tree_ptr->cur_tok < tree_ptr->num_of_toks)
    {
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VAL)
        {   
            GET_CUR_TOK()
            return NUM_NODE(0)
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VARIB && strcmp(varib_text, tree_ptr->toks[tree_ptr->cur_tok].text) == 0)
        {
            GET_CUR_TOK()
            return NUM_NODE(1)
        }
        if((tree_ptr->toks[tree_ptr->cur_tok].type == IS_CNST_VAR) || (tree_ptr->toks[tree_ptr->cur_tok].type == IS_VARIB 
                && strcmp(varib_text, tree_ptr->toks[tree_ptr->cur_tok].text) != 0))
        {
            GET_CUR_TOK()
            return NUM_NODE(0)
        }

        Node* left  = nullptr;
        Node* right = nullptr;

        switch(tree_ptr->toks[tree_ptr->cur_tok].value.int_val)
        {
            case Mul:  // ok 
                {
                    tree_ptr->cur_tok++;
                    size_t saved_frst_tok_num = tree_ptr->cur_tok; // stores hte cur tok for creation of sub trees

                    left  = diff_tree_old(tree_ptr, varib_text);
                    right = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok    = saved_frst_tok_num;   // to get first pre dif sub_tree
                    Node* left_pre_diff  = input_tree_old(tree_ptr); // from the array of tokens
                    Node* right_pre_diff = input_tree_old(tree_ptr); // from the array of tokens
                    Node* mul1 = MUL_NODE(left, right_pre_diff)
                    Node* mul2 = MUL_NODE(left_pre_diff, right)

                    return ADD_NODE(mul1, mul2)
                }
            case Sub:  // ok
                {
                    tree_ptr->cur_tok++;                 
                    left  = diff_tree_old(tree_ptr, varib_text); // from the array of tokens                  
                    right = diff_tree_old(tree_ptr, varib_text); // from the array of tokens                                             
                    return SUB_NODE(left, right)
                }
            case Add: // ok
                {
                    tree_ptr->cur_tok++;                 
                    left  = diff_tree_old(tree_ptr, varib_text);                         
                    right = diff_tree_old(tree_ptr, varib_text);                                                                   
                    return ADD_NODE(left, right)
                }
            case Div: // ok
                {
                    tree_ptr->cur_tok++;                 
                    size_t saved_frst_tok_num = tree_ptr->cur_tok; // stores hte cur tok for creation of sub trees

                    left  = diff_tree_old(tree_ptr, varib_text);
                    right = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                    Node* left_pre_diff = input_tree_old(tree_ptr);

                    size_t dub_tree_bot_id = tree_ptr->cur_tok; // to get bottom sub trees
                    Node* right_pre_diff_top = input_tree_old(tree_ptr);

                    tree_ptr->cur_tok = dub_tree_bot_id;
                    Node* right_pre_diff_b1 = input_tree_old(tree_ptr);

                    tree_ptr->cur_tok = dub_tree_bot_id;
                    Node* right_pre_diff_b2 = input_tree_old(tree_ptr);

                    Node* mul_1 = MUL_NODE(left, right_pre_diff_top)
                    Node* mul_2 = MUL_NODE(left_pre_diff, right)

                    Node* top    = SUB_NODE(mul_1, mul_2)
                    Node* bottom = MUL_NODE(right_pre_diff_b1, right_pre_diff_b2)

                    return DIV_NODE(top, bottom)
                }
            case Cos: //ok
                {
                    size_t saved_frst_tok_num = tree_ptr->cur_tok;
                    tree_ptr->cur_tok++;
                    left = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num + 1; // to get first pre dif sub_tree (inner of cos)
                    Node* left_pre_diff_inner = input_tree_old(tree_ptr); 
                    Node* sin   = SIN_NODE(left_pre_diff_inner)
                    Node* minus = NUM_NODE(-1)
                    Node* mul   = MUL_NODE(minus, sin)
                    
                    return MUL_NODE(mul, left)
                }
            case Sin: // ok
                {
                    size_t saved_frst_tok_num = tree_ptr->cur_tok;
                    tree_ptr->cur_tok++;
                    left = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num + 1; // to get first pre dif sub_tree(inner of sin)
                    Node* left_pre_diff_inner = input_tree_old(tree_ptr); 
                    Node* cos = COS_NODE(left_pre_diff_inner);
                    
                    return MUL_NODE(cos, left);
                }
            case Log: // ok
                {
                    tree_ptr->cur_tok++;
                    size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                    left = diff_tree_old(tree_ptr, varib_text); 

                    tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                    Node* left_pre_diff = input_tree_old(tree_ptr);

                    return DIV_NODE(left, left_pre_diff)
                }
            case Exp: // ok
                {
                    size_t saved_frst_tok_num = tree_ptr->cur_tok;
                    tree_ptr->cur_tok++;
                    left = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                    Node* left_pre_diff = input_tree_old(tree_ptr); 
                    
                    return MUL_NODE(left_pre_diff, left)
                }
            case Sqrt: // ok
                {
                    size_t saved_frst_tok_num = tree_ptr->cur_tok;
                    tree_ptr->cur_tok++;
                    left = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                    Node* left_pre_diff = input_tree_old(tree_ptr);
                    Node* coef = NUM_NODE(2)
                    Node* mul  = MUL_NODE(coef, left_pre_diff)
                    
                    return DIV_NODE(left, mul)
                }
            case Tan: //ok
                {
                    tree_ptr->cur_tok++;
                    size_t saved_frst_tok_num = tree_ptr->cur_tok;
                    left = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree (inner of tan)
                    Node* left_pre_diff_1 = input_tree_old(tree_ptr);
                    tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree (inner of tan)
                    Node* left_pre_diff_2 = input_tree_old(tree_ptr);

                    Node* cos_1 = COS_NODE(left_pre_diff_1)
                    Node* cos_2 = COS_NODE(left_pre_diff_2)

                    Node* bottom = MUL_NODE(cos_1, cos_2)
                    
                    return DIV_NODE(left, bottom)
                }
            case Cot: // ok
                {
                    tree_ptr->cur_tok++;
                    size_t saved_frst_tok_num = tree_ptr->cur_tok;
                    left = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree (inner of cot)
                    Node* left_pre_diff_1 = input_tree_old(tree_ptr);
                    tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree (inner of cot)
                    Node* left_pre_diff_2 = input_tree_old(tree_ptr);

                    Node* sin_1 = SIN_NODE(left_pre_diff_1)
                    Node* sin_2 = SIN_NODE(left_pre_diff_2)

                    Node* coef   = NUM_NODE(-1)
                    Node* top    = MUL_NODE(coef, left)
                    Node* bottom = MUL_NODE(sin_1, sin_2)
                    
                    return DIV_NODE(top, bottom)
                }
            // case Log10: //ok
            //     {
            //         tree_ptr->cur_tok++;
            //         size_t saved_frst_tok_num = tree_ptr->cur_tok; 
            //         left = diff_tree_old(tree_ptr); 

            //         tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree (inner of log)
            //         Node* left_pre_diff = input_tree_old(tree_ptr);
            //         Node* coef   = NUM_NODE(10)
            //         Node* bottom = MUL_NODE(coef, left_pre_diff)

            //         return DIV_NODE(left, bottom)
            //     }
            case Asin:
                {
                    tree_ptr->cur_tok++; 
                    size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                    left = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree (inner of asin)
                    Node* left_pre_diff_1 = input_tree_old(tree_ptr);
                    tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree (inner of asin)
                    Node* left_pre_diff_2 = input_tree_old(tree_ptr);

                    Node* one = NUM_NODE(1)
                    Node* mul = MUL_NODE(left_pre_diff_1, left_pre_diff_2)
                    Node* sub = SUB_NODE(one, mul)
                    Node* srt = SQRT_NODE(sub)

                    return DIV_NODE(left, srt)
                }
            case Acos:
                {
                    tree_ptr->cur_tok++; 
                    size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                    left = diff_tree_old(tree_ptr, varib_text);

                    tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                    Node* left_pre_diff_1 = input_tree_old(tree_ptr);
                    tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                    Node* left_pre_diff_2 = input_tree_old(tree_ptr);

                    Node* one = NUM_NODE(1)
                    Node* mul = MUL_NODE(left_pre_diff_1, left_pre_diff_2)
                    Node* sub = SUB_NODE(one, mul)
                    Node* srt = SQRT_NODE(sub)

                    Node* min_one = NUM_NODE(-1)
                    Node* top     = MUL_NODE(min_one, left)

                    return DIV_NODE(top, srt)
                }
            case Pow:
                {
                    size_t saved_frst_tok_num = tree_ptr->cur_tok;

                    tree_ptr->cur_tok++;
                    Node* base = input_tree_old(tree_ptr);

                    size_t saved_exp_id = tree_ptr->cur_tok;
                    Node*  exp          = input_tree_old(tree_ptr);
                    size_t last_id      = tree_ptr->cur_tok;

                    if(base->type == IS_VAL && exp->type == IS_VAL)
                    {
                        return NUM_NODE(0);
                        free(base);
                        free(exp);
                    }
                    else if(base->type == IS_VAL)
                    {
                        free(exp);
                        tree_ptr->cur_tok = saved_frst_tok_num;
                        Node* pre_dif_pow = input_tree_old(tree_ptr);
                        Node* ln          = LN_NODE(base)

                        tree_ptr->cur_tok = saved_exp_id;
                        left = diff_tree_old(tree_ptr, varib_text);

                        Node* mul_1 = MUL_NODE(pre_dif_pow, ln);
                        return MUL_NODE(mul_1, left);
                    }
                    else if((base->type == IS_FUNC || base->type == IS_VARIB) && exp->type == IS_VAL)
                    {
                        tree_ptr->cur_tok = saved_frst_tok_num;
                        Node* pre_dif_pow = input_tree_old(tree_ptr);

                        double new_exp_val = exp->value.node_value - 1.0; // decreasing exp val
                        Node* coef = NUM_NODE(exp->value.node_value);
                        exp->value.node_value = new_exp_val; // changing old exp val

                        Node* new_pow = create_node(tree_ptr, Pow, IS_FUNC, nullptr, base, exp);
                        Node* mul     = MUL_NODE(coef, new_pow);

                        tree_ptr->cur_tok = saved_frst_tok_num + 1;
                        left              = diff_tree_old(tree_ptr, varib_text);
                        tree_ptr->cur_tok = last_id;

                        return MUL_NODE(mul, left);
                    }

                    Node* one          = NUM_NODE(1); // 1
                    Node* new_exp_base = SUB_NODE(exp, one); // v - 1
                    Node* new_pow      = POW_NODE(base, new_exp_base); // u^(v-1)

                    tree_ptr->cur_tok = saved_exp_id;
                    Node* old_exp     = input_tree_old(tree_ptr); // v
                    tree_ptr->cur_tok = saved_frst_tok_num + 1; 
                    Node* diff_base   = diff_tree_old(tree_ptr, varib_text); // du/dx

                    Node* mul_1 = MUL_NODE(old_exp, new_pow); // v * u^(v-1) 
                    Node* mul_2 = MUL_NODE(mul_1, diff_base);  // v * u^(v-1) * (du/dx)

                    tree_ptr->cur_tok = saved_frst_tok_num;
                    Node* old_pow     = input_tree_old(tree_ptr); // u^v
                    tree_ptr->cur_tok = saved_frst_tok_num + 1;
                    Node* old_base    = input_tree_old(tree_ptr); // u
                    Node* ln_base     = LN_NODE(old_base); // ln(u)

                    Node* mul_3 = MUL_NODE(old_pow, ln_base); // u^v * ln(u)

                    tree_ptr->cur_tok = saved_exp_id;
                    Node* diff_exp    = diff_tree_old(tree_ptr, varib_text); // (dv/dx)
                    Node* mul_4       = MUL_NODE(mul_3, diff_exp); // u^v * ln(u) * (dv/dx)

                    return ADD_NODE(mul_2, mul_4);
                }
            default:
                ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
                return nullptr;
        }
    }
}

int get_vars(Tree* tree_ptr) // ok
{
    printf("\nEnter the number of variables:");
    scanf(" %ld", &(tree_ptr->num_of_vars));

    if(tree_ptr->num_of_vars < 0)
    {
        printf("\nInvalid number of variables\n");
        ERROR_MESSAGE(stderr, ERR_INVALID_VAR_NUM)
        return ERR_INVALID_VAR_NUM;
    }

    tree_ptr->vars_arr = (Var*)calloc(tree_ptr->num_of_vars, sizeof(Var));
    if(tree_ptr->vars_arr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_TO_CALLOC_VARS)
        return ERR_TO_CALLOC_VARS;
    }

    for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
    {
        printf("\n\nEnter the %ld variable:", cur_var);
        scanf(" %s", &(tree_ptr->vars_arr[cur_var].var_text));
        tree_ptr->vars_arr[cur_var].var_text[20] = '\0';

        if(isalpha((int)(tree_ptr->vars_arr[cur_var].var_text[0])) == 0)
        {
            ERROR_MESSAGE(stderr, ERR_INVALID_VAR_TEXT)
            return ERR_INVALID_VAR_TEXT;
        }

        printf("\nEnter the value of variable '%s':", tree_ptr->vars_arr[cur_var].var_text);
        scanf(" %lf", &(tree_ptr->vars_arr[cur_var].var_value));
    }

    return RETURN_OK;
}

Node* shortener(Tree* tree_ptr, Node* node_ptr) // ok
{
    if(node_ptr == nullptr)
    {
        return nullptr;
    }
    if(node_ptr->type == IS_VAL)
    {
        return node_ptr;
    }
    if(node_ptr->type == IS_VARIB)
    {
        return node_ptr;
    }
    if(node_ptr->type == IS_OP)
    {
        double result = 0;
        Node* short_left  = nullptr;
        Node* short_right = nullptr;

        switch(node_ptr->value.op_number)
        {
            case Add: 
                { 
                    if(NODE_LEFT_CHILD->type == IS_VAL && check_is_positive(NODE_LEFT_CHILD->value.node_value) == IS_ZERO &&  // ok
                        NODE_RIGHT_CHILD->type == IS_VARIB)
                    {
                        return VARIB_NODE(NODE_RIGHT_CHILD->value.text)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && check_is_positive(NODE_RIGHT_CHILD->value.node_value) == IS_ZERO &&  // ok
                        NODE_LEFT_CHILD->type == IS_VARIB)
                    {
                        return VARIB_NODE(NODE_LEFT_CHILD->value.text)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && NODE_LEFT_CHILD->type == IS_VAL) // ok 
                    {
                        return NUM_NODE(NODE_RIGHT_CHILD->value.node_value + NODE_LEFT_CHILD->value.node_value)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VARIB && NODE_LEFT_CHILD->type == IS_VARIB) //  ok
                    {
                        return copy_subtree(tree_ptr, node_ptr);
                    }
                    else
                    {
                        short_left  = SHORT_CHILD(NODE_LEFT_CHILD);
                        short_right = SHORT_CHILD(NODE_RIGHT_CHILD);
                        
                        if(short_left->type == IS_VAL && check_is_positive(short_left->value.node_value) == IS_ZERO &&  // ok
                            short_right->type == IS_VARIB)
                        {
                            dtor_childs(short_left);
                            return short_right;
                        }
                        else if(short_right->type == IS_VAL && check_is_positive(short_right->value.node_value) == IS_ZERO && // ok
                            short_left->type == IS_VARIB)
                        {
                            dtor_childs(short_right);
                            return short_left;
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_VAL) // ok
                        {
                            double value = short_left->value.node_value + short_right->value.node_value;
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            return NUM_NODE(value)
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_VARIB || short_right->type == IS_VARIB && short_left->type == IS_VAL) // ok
                        {
                            return ADD_NODE(short_right, short_left)
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_OP && short_left->value.op_number == Add // ok
                            && short_left->right_child->type == IS_VAL)
                        {
                            Node* left_short_l = copy_subtree(tree_ptr, short_left->left_child);
                            Node* sort_sum = NUM_NODE(short_right->value.node_value + short_left->right_child->value.node_value)
                            Node* add = ADD_NODE(left_short_l, sort_sum)
                            dtor_childs(short_right);
                            dtor_childs(short_left);
                            return SHORT_CHILD(add);
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_OP && short_left->value.op_number == Add // ok
                            && short_left->left_child->type == IS_VAL)
                        {
                            Node* left_short_r = copy_subtree(tree_ptr, short_left->right_child);
                            Node* sort_sum = NUM_NODE(short_right->value.node_value + short_left->left_child->value.node_value)
                            Node* add = ADD_NODE(left_short_r, sort_sum)
                            dtor_childs(short_right);
                            dtor_childs(short_left);
                            return SHORT_CHILD(add);
                        }
                        else if(short_left->type == IS_VAL && short_right->type == IS_OP && short_right->value.op_number == Add // ok
                            && short_right->left_child->type == IS_VAL)
                        {
                            Node* right_short_r = copy_subtree(tree_ptr, short_right->right_child);
                            Node* sort_sum = NUM_NODE(short_left->value.node_value + short_right->left_child->value.node_value)
                            Node* add = ADD_NODE(right_short_r, sort_sum)
                            dtor_childs(short_right);
                            dtor_childs(short_left);
                            return SHORT_CHILD(add);
                        }
                        else if(short_left->type == IS_VAL && short_right->type == IS_OP && short_right->value.op_number == Add  // ok
                            && short_right->right_child->type == IS_VAL)
                        {
                            Node* right_short_l = copy_subtree(tree_ptr, short_right->left_child);
                            Node* sort_sum = NUM_NODE(short_left->value.node_value + short_right->right_child->value.node_value)
                            Node* add = ADD_NODE(right_short_l, sort_sum)
                            dtor_childs(short_right);
                            dtor_childs(short_left);
                            return SHORT_CHILD(add);
                        }
                        return ADD_NODE(short_left, short_right)
                    }
                }
            case Sub: // OK ??? 
                {
                    if(NODE_LEFT_CHILD->type == IS_VAL && check_is_positive(NODE_LEFT_CHILD->value.node_value) == IS_ZERO &&  // ok
                        NODE_RIGHT_CHILD->type == IS_VARIB)
                    {
                        Node* minus = NUM_NODE(-1)
                        Node* varib = VARIB_NODE(NODE_RIGHT_CHILD->value.text)
                        return MUL_NODE(minus, varib)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && check_is_positive(NODE_RIGHT_CHILD->value.node_value) == IS_ZERO &&  // ok
                        NODE_LEFT_CHILD->type == IS_VARIB)
                    {
                        return VARIB_NODE(NODE_LEFT_CHILD->value.text)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && NODE_LEFT_CHILD->type == IS_VAL) // ok
                    {
                        return NUM_NODE(NODE_LEFT_CHILD->value.node_value - NODE_RIGHT_CHILD->value.node_value)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VARIB && NODE_LEFT_CHILD->type == IS_VARIB) // ok
                    {
                        return copy_subtree(tree_ptr, node_ptr);
                    }
                    else
                    {
                        short_left  = SHORT_CHILD(NODE_LEFT_CHILD);
                        short_right = SHORT_CHILD(NODE_RIGHT_CHILD);
                        
                        if(short_left->type == IS_VAL && check_is_positive(short_left->value.node_value) == IS_ZERO &&  //ok
                            short_right->type == IS_VARIB)
                        {
                            Node* minus = NUM_NODE(-1)
                            dtor_childs(short_left);
                            return MUL_NODE(minus, short_right)
                        }
                        else if(short_right->type == IS_VAL && check_is_positive(short_right->value.node_value) == IS_ZERO &&  // ok
                            short_left->type == IS_VARIB)
                        {
                            dtor_childs(short_right);
                            return short_left;
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_VAL) // ok
                        {
                            double value = short_left->value.node_value - short_right->value.node_value;
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            return NUM_NODE(value)
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_VARIB) // ok
                        {
                            return SUB_NODE(short_left, short_right)
                        }
                        else if(short_right->type == IS_VARIB && short_left->type == IS_VAL) // ok
                        {
                            return SUB_NODE(short_left, short_right)
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_OP && short_left->value.op_number == Sub  // ok
                            && short_left->right_child->type == IS_VAL)
                        {
                            Node* left_short_l = copy_subtree(tree_ptr, short_left->left_child);
                            Node* sort_sum = NUM_NODE(short_right->value.node_value + short_left->right_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* sub = SUB_NODE(left_short_l, sort_sum)
                            return SHORT_CHILD(sub);
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_OP && short_left->value.op_number == Sub  // ok
                            && short_left->left_child->type == IS_VAL) 
                        {
                            Node* left_short_r = copy_subtree(tree_ptr, short_left->right_child);
                            Node* sort_sum = NUM_NODE(short_left->left_child->value.node_value - short_right->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* sub = SUB_NODE(sort_sum, left_short_r)
                            return SHORT_CHILD(sub);
                        }
                        else if(short_left->type == IS_VAL && short_right->type == IS_OP && short_right->value.op_number == Sub // ok
                            && short_right->left_child->type == IS_VAL)
                        {
                            Node* right_short_r = copy_subtree(tree_ptr, short_right->right_child);
                            Node* sort_sum = NUM_NODE(short_left->value.node_value - short_right->left_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* add = ADD_NODE(right_short_r, sort_sum)
                            return SHORT_CHILD(add);
                        }
                        else if(short_left->type == IS_VAL && short_right->type == IS_OP && short_right->value.op_number == Sub  // ok
                            && short_right->right_child->type == IS_VAL)
                        {
                            Node* right_short_l = copy_subtree(tree_ptr, short_right->left_child);
                            Node* sort_sum = NUM_NODE(short_left->value.node_value + short_right->right_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* sub = SUB_NODE(sort_sum, right_short_l)
                            return SHORT_CHILD(sub);
                        }
                        return SUB_NODE(short_left, short_right)
                    }
                }
            case Mul: // OK???
                {
                    if(NODE_LEFT_CHILD->type == IS_VAL && check_is_positive(NODE_LEFT_CHILD->value.node_value) == IS_ZERO &&  // ok
                        NODE_RIGHT_CHILD->type != IS_VAL)
                    {
                        return NUM_NODE(0)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && check_is_positive(NODE_RIGHT_CHILD->value.node_value) == IS_ZERO && // ok
                        NODE_LEFT_CHILD->type != IS_VAL)
                    {
                        return NUM_NODE(0)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && NODE_LEFT_CHILD->type == IS_VAL) // ok
                    {
                        return NUM_NODE(NODE_RIGHT_CHILD->value.node_value * NODE_LEFT_CHILD->value.node_value)
                    }
                    else
                    {
                        short_left  = SHORT_CHILD(NODE_LEFT_CHILD);
                        short_right = SHORT_CHILD(NODE_RIGHT_CHILD);

                        if(short_left->type == IS_VAL && check_is_positive(short_left->value.node_value) == IS_ZERO &&  // ok
                        short_right->type != IS_VAL)
                        {
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            return NUM_NODE(0)
                        }
                        else if(short_right->type == IS_VAL && check_is_positive(short_right->value.node_value) == IS_ZERO && // ok
                        short_left->type != IS_VAL)
                        {
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            return NUM_NODE(0)
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_VAL) // ok
                        {
                            double value = short_left->value.node_value * short_right->value.node_value;
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            return NUM_NODE(value)
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_OP && short_left->value.op_number == Mul  // ok
                            && short_left->right_child->type == IS_VAL)
                        {
                            Node* left_short_l = copy_subtree(tree_ptr, short_left->left_child);
                            Node* sort_sum = NUM_NODE(short_right->value.node_value * short_left->right_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* mul = MUL_NODE(left_short_l, sort_sum)
                            return SHORT_CHILD(mul);
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_OP && short_left->value.op_number == Mul  // ok
                            && short_left->left_child->type == IS_VAL) 
                        {
                            Node* left_short_r = copy_subtree(tree_ptr, short_left->right_child);
                            Node* sort_sum = NUM_NODE(short_left->left_child->value.node_value * short_right->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* mul = MUL_NODE(sort_sum, left_short_r)
                            return SHORT_CHILD(mul);
                        }
                        else if(short_left->type == IS_VAL && short_right->type == IS_OP && short_right->value.op_number == Mul // ok
                            && short_right->left_child->type == IS_VAL)
                        {
                            Node* right_short_r = copy_subtree(tree_ptr, short_right->right_child);
                            Node* sort_sum = NUM_NODE(short_left->value.node_value * short_right->left_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* mul = MUL_NODE(right_short_r, sort_sum)
                            return SHORT_CHILD(mul);
                        }
                        else if(short_left->type == IS_VAL && short_right->type == IS_OP && short_right->value.op_number == Mul  // ok
                            && short_right->right_child->type == IS_VAL)
                        {
                            Node* right_short_l = copy_subtree(tree_ptr, short_right->left_child);
                            Node* sort_sum = NUM_NODE(short_left->value.node_value * short_right->right_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* mul = MUL_NODE(sort_sum, right_short_l)
                            return SHORT_CHILD(mul);
                        }
                        return MUL_NODE(short_left, short_right)
                    }
                }
            case Div: // OK ???
                {
                    if(NODE_RIGHT_CHILD->type == IS_VAL && fabs(NODE_RIGHT_CHILD->value.node_value -1) <= EPS)
                    {
                        return NODE_RIGHT_CHILD;
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && fabs(NODE_RIGHT_CHILD->value.node_value - 1) <= EPS && NODE_LEFT_CHILD->type != IS_OP) // ok
                    {
                        return NODE_LEFT_CHILD;
                    }
                    else if(NODE_LEFT_CHILD->type == IS_VAL && check_is_positive(NODE_LEFT_CHILD->value.node_value) == IS_ZERO && // ok
                        NODE_RIGHT_CHILD->type != IS_VAL)
                    {
                        return NUM_NODE(0)
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && NODE_LEFT_CHILD->type == IS_VAL && check_is_positive(NODE_RIGHT_CHILD->value.node_value) == IS_ZERO // ok
                            && check_is_positive(NODE_LEFT_CHILD->value.node_value) == IS_ZERO)
                    {
                        ERROR_MESSAGE(stderr, ERR_UNCERTAINTY)
                        return nullptr;
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && check_is_positive(NODE_RIGHT_CHILD->value.node_value) == IS_ZERO) // ok
                    {
                        ERROR_MESSAGE(stderr, ERR_DIV_TO_ZERO)
                        return nullptr;
                    }
                    else if(NODE_RIGHT_CHILD->type == IS_VAL && NODE_LEFT_CHILD->type == IS_VAL)
                    {
                        return NUM_NODE(NODE_LEFT_CHILD->value.node_value / NODE_RIGHT_CHILD->value.node_value)
                    }
                    else
                    {
                        short_left  = SHORT_CHILD(NODE_LEFT_CHILD);
                        short_right = SHORT_CHILD(NODE_RIGHT_CHILD);

                        if(short_right->type == IS_VAL && fabs(short_right->value.node_value - 1) <= EPS) // ok
                        {
                            dtor_childs(short_right);
                            return short_left;
                        }
                        if(short_left->type == IS_VAL && check_is_positive(short_left->value.node_value) == IS_ZERO && // ok
                        short_right->type != IS_VAL)
                        {
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            return NUM_NODE(0)
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_VAL && check_is_positive(short_right->value.node_value) == IS_ZERO // ok
                                && check_is_positive(short_left->value.node_value) == IS_ZERO)
                        {
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            ERROR_MESSAGE(stderr, ERR_UNCERTAINTY)
                            return nullptr;
                        }
                        else if(short_right->type == IS_VAL && check_is_positive(short_right->value.node_value) == IS_ZERO) // ok
                        {
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            ERROR_MESSAGE(stderr, ERR_DIV_TO_ZERO)
                            return nullptr;
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_VAL) // ok
                        {
                            double value = short_left->value.node_value / short_right->value.node_value;
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            return NUM_NODE(value)
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_OP && short_left->value.op_number == Div  // ok
                            && short_left->right_child->type == IS_VAL)
                        {
                            Node* left_short_l = copy_subtree(tree_ptr, short_left->left_child);
                            Node* sort_sum = NUM_NODE(short_right->value.node_value * short_left->right_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* div = DIV_NODE(left_short_l, sort_sum)
                            return SHORT_CHILD(div);
                        }
                        else if(short_right->type == IS_VAL && short_left->type == IS_OP && short_left->value.op_number == Div  // ok
                            && short_left->left_child->type == IS_VAL) 
                        {
                            Node* left_short_r = copy_subtree(tree_ptr, short_left->right_child);
                            Node* sort_sum = NUM_NODE(short_left->left_child->value.node_value / short_right->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* div = DIV_NODE(sort_sum, left_short_r)
                            return SHORT_CHILD(div);
                        }
                        else if(short_left->type == IS_VAL && short_right->type == IS_OP && short_right->value.op_number == Div // ok
                            && short_right->left_child->type == IS_VAL)
                        {
                            Node* right_short_r = copy_subtree(tree_ptr, short_right->right_child);
                            Node* sort_sum = NUM_NODE(short_left->value.node_value / short_right->left_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* mul = MUL_NODE(right_short_r, sort_sum)
                            return SHORT_CHILD(mul);
                        }
                        else if(short_left->type == IS_VAL && short_right->type == IS_OP && short_right->value.op_number == Div  // ok
                            && short_right->right_child->type == IS_VAL)
                        {
                            Node* right_short_l = copy_subtree(tree_ptr, short_right->left_child);
                            Node* sort_sum = NUM_NODE(short_left->value.node_value * short_right->right_child->value.node_value)
                            dtor_childs(short_left);
                            dtor_childs(short_right);
                            Node* div = DIV_NODE(sort_sum, right_short_l)
                            return SHORT_CHILD(div);
                        }
                        return DIV_NODE(short_left, short_right)
                    }
                }
            default:
                ERROR_MESSAGE(stderr, ERR_UNKNOWN_OPERATOR)
                return nullptr;
            }
        }

        Node* short_arg = 0; // argument of the function

        switch(node_ptr->value.op_number)
        {        
            case Sin: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Sin(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return SIN_NODE(short_arg)
                    }
                    ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                    return nullptr;
                }
            case Cos: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Sin(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return COS_NODE(short_arg)
                    }
                }
            case Tan: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Tan(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return TAN_NODE(short_arg);
                    }
                }
            case Sqrt: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Sqrt(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return SQRT_NODE(short_arg);
                    }
                }
            case Asin: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Asin(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return ASIN_NODE(short_arg);
                    }
                }
            case Acos: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Acos(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return ACOS_NODE(short_arg);
                    }
                }
            case Log: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Log(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return LN_NODE(short_arg);
                    }
                }
            case Cot: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Cot(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return COT_NODE(short_arg);
                    }
                }
            case Exp: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    if(short_arg->type == IS_VAL)
                    {
                        double argument = func_Exp(short_arg->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        return EXP_NODE(short_arg);
                    }
                }
            case Pow: // ok
                {
                    short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
                    Node* short_arg_r = SHORT_CHILD(NODE_RIGHT_CHILD);

                    if(short_arg->type == IS_VAL && short_arg_r->type == IS_VAL)
                    {
                        double argument = func_Pow(short_arg->value.node_value, short_arg_r->value.node_value);
                        if(argument != NAN)
                        {
                            dtor_childs(short_arg);
                            dtor_childs(short_arg_r);
                            return NUM_NODE(argument)
                        }
                        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)
                        return nullptr;
                    }
                    else
                    {
                        if(short_arg_r->type == IS_VAL)
                        {
                            if(fabs(short_arg_r->value.node_value - 1.0) <= EPS)
                            {
                                dtor_childs(short_arg_r);
                                return short_arg;
                            }
                            if(check_is_positive(short_arg_r->value.node_value) == IS_ZERO)
                            {
                                dtor_childs(short_arg_r);
                                dtor_childs(short_arg);
                                return NUM_NODE(1);
                            } 
                        }
                        return POW_NODE(short_arg, short_arg_r);
                    }
                }
            default:
                printf("TYPE: %ld, TEXT: %s, OP_VAL: %ld\n", node_ptr->type, node_ptr->value.text, node_ptr->value.op_number);
                ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
                return nullptr;
    }

}

char* get_string_func(size_t func_code) // ok
{
    switch(func_code)
    {
        #define DEF_FUNC(name, code, string)        \
            case name:                              \
                return string;                      \

        #define DEF_OP(name, code, string)
        #include "def_cmd.h"

        #undef DEF_FUNC
        #undef DEF_OP

        default:
            ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
            return "ERR_UNKNOWN_FUNC";
    }
}

Node* full_diff_old(Tree* tree_ptr) // OLD
{
    tree_ptr->cur_tok = 0;
    if(tree_ptr->num_of_vars == 0)
    {
        dtor_childs(tree_ptr->root);
        return diff_tree_old(tree_ptr, tree_ptr->vars_arr[0].var_text);
    }
    else
    {
        Node* first_diff = diff_tree_old(tree_ptr, tree_ptr->vars_arr[0].var_text);
        tree_ptr->cur_tok = 0; // Needs to be set to zero because diff tree increases the cor_tok value every time it diff the tree

        for(size_t cur_diff = 1; cur_diff < tree_ptr->num_of_vars; cur_diff++)
        {
            first_diff = ADD_NODE(first_diff, diff_tree_old(tree_ptr, tree_ptr->vars_arr[cur_diff].var_text));
            tree_ptr->cur_tok = 0; // Needs to be set to zero because diff tree increases the cor_tok value every time it diff the tree
        }

        dtor_childs(tree_ptr->root);

        return first_diff;
    }
}

Node* copy_subtree(Tree* tree_ptr, Node* node_ptr) // ok
{
    if(node_ptr == nullptr)
    {
        return nullptr;
    }
    if(node_ptr->type == IS_VAL)
    {
        return NUM_NODE(node_ptr->value.node_value)
    }
    if(node_ptr->type == IS_VARIB)
    {
        return VARIB_NODE(node_ptr->value.text)
    }
    if(node_ptr->type == IS_CNST_VAR)
    {
        return CNST_VARIB_NODE(node_ptr->value.text)
    }

    return create_node(tree_ptr, node_ptr->value.op_number, node_ptr->type, nullptr, copy_subtree(tree_ptr, node_ptr->left_child), copy_subtree(tree_ptr, node_ptr->right_child));
    
}

Node* diff_tree(Tree* tree_ptr, Node* node_ptr, char* varib_text) // ok
{
    int err_code = RETURN_OK;

    if(node_ptr->type == IS_VAL)
    {   
        err_code = add_preamble_latex(node_ptr);
        if(err_code != RETURN_OK)
        {
            ERROR_MESSAGE(stderr, err_code)
            return nullptr;
        }

        Node* return_node = NUM_NODE(0)

        err_code = add_equation_diff_latex(return_node);
        if(err_code != RETURN_OK)
        {
            ERROR_MESSAGE(stderr, err_code)
            return nullptr;
        }

        dtor_childs(node_ptr);
        return return_node;
    }
    if(node_ptr->type == IS_VARIB && strcmp(varib_text, node_ptr->value.text) == 0)
    {
        err_code = add_preamble_latex(node_ptr);
        if(err_code != RETURN_OK)
        {
            ERROR_MESSAGE(stderr, err_code)
            return nullptr;
        }

        Node* return_node = NUM_NODE(1)

        err_code = add_equation_diff_latex(return_node);
        if(err_code != RETURN_OK)
        {
            ERROR_MESSAGE(stderr, err_code)
            return nullptr;
        }

        dtor_childs(node_ptr);
        return return_node;
    }
    if((node_ptr->type == IS_CNST_VAR) || (node_ptr->type == IS_VARIB 
            && strcmp(varib_text, node_ptr->value.text) != 0))
    {
        err_code = add_preamble_latex(node_ptr);
        if(err_code != RETURN_OK)
        {
            ERROR_MESSAGE(stderr, err_code)
            return nullptr;
        }

        Node* return_node = NUM_NODE(0)

        err_code = add_equation_diff_latex(return_node);
        if(err_code != RETURN_OK)
        {
            ERROR_MESSAGE(stderr, err_code)
            return nullptr;
        }

        dtor_childs(node_ptr);
        return return_node;
    }

    switch(node_ptr->value.op_number)
    {
        case Add:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                node_ptr->left_child  = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                node_ptr->right_child = diff_tree(tree_ptr, node_ptr->right_child, varib_text);

                err_code = add_equation_diff_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return node_ptr;
            }
        case Sub:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                node_ptr->left_child  = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                node_ptr->right_child = diff_tree(tree_ptr, node_ptr->right_child, varib_text);

                err_code = add_equation_diff_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return node_ptr;
            }
        case Mul:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* pre_diff_l = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* pre_diff_r = copy_subtree(tree_ptr, node_ptr->right_child);

                Node* diff_l = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                Node* diff_r = diff_tree(tree_ptr, node_ptr->right_child, varib_text);

                free(node_ptr);
                Node* mul_l = MUL_NODE(pre_diff_l, diff_r)
                Node* mul_r = MUL_NODE(diff_l, pre_diff_r)

                Node* return_node = ADD_NODE(mul_l, mul_r)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Div:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* pre_diff_l  = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* pre_diff_r  = copy_subtree(tree_ptr, node_ptr->right_child);
                Node* bot_right_1 = copy_subtree(tree_ptr, node_ptr->right_child);
                Node* bot_right_2 = copy_subtree(tree_ptr, node_ptr->right_child);

                Node* diff_l = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                Node* diff_r = diff_tree(tree_ptr, node_ptr->right_child, varib_text);

                free(node_ptr);
                Node* mul_l   = MUL_NODE(diff_l, pre_diff_r)
                Node* mul_r   = MUL_NODE(diff_r, pre_diff_l)
                Node* sub     = SUB_NODE(mul_l, mul_r)
                Node* mul_bot = MUL_NODE(bot_right_1, bot_right_2)

                Node* return_node = DIV_NODE(sub, mul_bot)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Sin:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* pre_diff_l = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* diff_l     = diff_tree(tree_ptr, node_ptr->left_child, varib_text);

                free(node_ptr);
                Node* cos = COS_NODE(pre_diff_l)

                Node* return_node = MUL_NODE(cos, diff_l)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Cos:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* pre_diff_l = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* diff_l     = diff_tree(tree_ptr, node_ptr->left_child, varib_text);

                free(node_ptr);
                Node* minus_one = NUM_NODE(-1)
                Node* sin = SIN_NODE(pre_diff_l)
                Node* mul = MUL_NODE(minus_one, sin)

                Node* return_node = MUL_NODE(mul, diff_l)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Tan:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* pre_diff_l = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* diff_l     = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                
                free(node_ptr);
                Node* cos_1   = COS_NODE(pre_diff_l)
                Node* cos_2   = COS_NODE(copy_subtree(tree_ptr, pre_diff_l))
                Node* bot_mul = MUL_NODE(cos_1, cos_2)

                Node* return_node = DIV_NODE(diff_l, bot_mul)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Log: 
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* pre_diff_l = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* diff_l     = diff_tree(tree_ptr, node_ptr->left_child, varib_text);

                free(node_ptr);

                Node* return_node = DIV_NODE(diff_l, pre_diff_l)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }

        case Exp:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* pre_diff_l = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* diff_l     = diff_tree(tree_ptr, pre_diff_l, varib_text);


                Node* return_node = MUL_NODE(node_ptr, diff_l)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Cot:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* pre_diff_l = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* diff_l     = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                
                free(node_ptr);
                Node* sin_1   = SIN_NODE(pre_diff_l)
                Node* sin_2   = SIN_NODE(copy_subtree(tree_ptr, pre_diff_l))
                Node* bot_mul = MUL_NODE(sin_1, sin_2)
                Node* minus_one = NUM_NODE(-1)
                Node* mul_top   = MUL_NODE(minus_one, diff_l)

                Node* return_node = DIV_NODE(mul_top, bot_mul)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Sqrt:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* node_copy = copy_subtree(tree_ptr, node_ptr);
                Node* diff_l    = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                
                free(node_ptr);
                Node* two     = NUM_NODE(2)
                Node* bot_mul = MUL_NODE(two, node_copy)

                Node* return_node = DIV_NODE(diff_l, bot_mul)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Asin:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* node_copy_1 = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* node_copy_2 = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* diff_l    = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                
                free(node_ptr);
                Node* one     = NUM_NODE(1)
                Node* mul     = MUL_NODE(node_copy_1, node_copy_2)
                Node* sub     = SUB_NODE(one, mul)
                Node* sqrt    = SQRT_NODE(sub)

                Node* return_node = DIV_NODE(diff_l, sqrt)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Acos:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                Node* node_copy_1 = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* node_copy_2 = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* diff_l    = diff_tree(tree_ptr, node_ptr->left_child, varib_text);
                
                free(node_ptr);
                Node* one     = NUM_NODE(1)
                Node* mul     = MUL_NODE(node_copy_1, node_copy_2)
                Node* sub     = SUB_NODE(one, mul)
                Node* sqrt    = SQRT_NODE(sub)
                Node* minus_one = NUM_NODE(-1)
                Node* div       = DIV_NODE(diff_l, sqrt)

                Node* return_node = MUL_NODE(minus_one, div)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        case Pow:
            {
                err_code = add_preamble_latex(node_ptr);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                if(node_ptr->left_child->type == IS_VAL && node_ptr->right_child->type == IS_VAL)
                {
                    dtor_childs(node_ptr);

                    Node* return_node = NUM_NODE(0)
                    err_code = add_equation_diff_latex(return_node);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return nullptr;
                    }

                    return return_node;
                }

                Node* copy_base = copy_subtree(tree_ptr, node_ptr->left_child);
                Node* copy_exp  = copy_subtree(tree_ptr, node_ptr->right_child);
                Node* ln        = LN_NODE(copy_base)

                Node* copy_ln_1  = copy_subtree(tree_ptr, ln);
                Node* copy_ln_2  = copy_subtree(tree_ptr, ln);
                Node* copy_exp_1 = copy_subtree(tree_ptr, copy_exp);
                Node* copy_exp_2 = copy_subtree(tree_ptr, copy_exp);

                dtor_childs(node_ptr);
                Node* e_exp = MUL_NODE(copy_exp_1, ln)
                Node* exp   = EXP_NODE(e_exp)

                Node* diff_base = diff_tree(tree_ptr, copy_ln_1, varib_text);
                Node* diff_exp  = diff_tree(tree_ptr, copy_exp_2, varib_text);
                Node* mul_1     = MUL_NODE(diff_exp, copy_ln_2)
                Node* mul_2     = MUL_NODE(diff_base, copy_exp)
                Node* add       = ADD_NODE(mul_1, mul_2)

                Node* return_node = MUL_NODE(exp, add)
                err_code = add_equation_diff_latex(return_node);
                if(err_code != RETURN_OK)
                {
                    ERROR_MESSAGE(stderr, err_code)
                    return nullptr;
                }

                return return_node;
            }
        default:
            ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
            return nullptr;
    }
}

int full_diff(Tree* tree_ptr) // ok
{
    print_header_latex();
    if(tree_ptr->num_of_vars == 0)
    {
        tree_ptr->root = diff_tree(tree_ptr, tree_ptr->root, tree_ptr->vars_arr[0].var_text);

        int err_code = print_plot_latex(tree_ptr->root, tree_ptr->vars_arr[0].var_text, tree_ptr);
        if(err_code != RETURN_OK)
        {
            ERROR_MESSAGE(stderr, err_code)
            return err_code;
        }

        print_footer_latex(tree_ptr->root);
    }
    else
    {
        Node* first_diff = diff_tree(tree_ptr, copy_subtree(tree_ptr, tree_ptr->root), tree_ptr->vars_arr[0].var_text);

        int err_code = print_plot_latex(first_diff, tree_ptr->vars_arr[0].var_text, tree_ptr);
        if(err_code != RETURN_OK)
        {
            ERROR_MESSAGE(stderr, err_code)
            return err_code;
        }

        Node* diff_next_var = nullptr;
        for(size_t cur_diff = 1; cur_diff < tree_ptr->num_of_vars; cur_diff++)
        {
            diff_next_var = diff_tree(tree_ptr, copy_subtree(tree_ptr, tree_ptr->root), tree_ptr->vars_arr[cur_diff].var_text);

            int err_code = print_plot_latex(diff_next_var, tree_ptr->vars_arr[cur_diff].var_text, tree_ptr);
            if(err_code != RETURN_OK)
            {
                ERROR_MESSAGE(stderr, err_code)
                return err_code;
            }

            first_diff = ADD_NODE(first_diff, diff_next_var);
        }

        dtor_childs(tree_ptr->root);
        tree_ptr->root = first_diff;
        print_footer_latex(tree_ptr->root);
    }
}

