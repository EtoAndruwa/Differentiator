#include "Differentiator.h"

size_t print_recur_tree(const Node* const node_ptr, FILE* file_ptr) // ok
{
    if(node_ptr == nullptr)
    {
        return 0;
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

size_t output_tree(const Node* const root_node_ptr, char* file_name) // ok
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

    file_ptr = nullptr;
}

double eval(const Tree* const tree_ptr, const Node* const node_ptr) // ok
{
    if(tree_ptr->num_found_vars != tree_ptr->num_of_vars)
    {   
        ERROR_MESSAGE(stderr, ERR_FOUND_DIFF_NUM_VARS)
        return 0;
    }

    if(node_ptr == nullptr)
    {
        return 0;
    }
    if(node_ptr->type == IS_VAL)
    {
        return node_ptr->value.node_value;
    }
    if(node_ptr->type == IS_VARIB)
    {
        for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
        {
            if(node_ptr->value.text[0] == tree_ptr->vars[cur_var].var_text[0])
            {
                return tree_ptr->vars[cur_var].var_value;
            }
        }
    }

    double val1 = 0;
    double val2 = 0;

    switch(node_ptr->value.op_number)
    {

    #define DEF_OP(code, ...)                                                                                       \ 
        case code:                                                                                                  \
            val1 = eval(tree_ptr, node_ptr->left_child);                                                            \
            val2 = eval(tree_ptr, node_ptr->right_child);                                                           \
                                                                                                                    \
            if(val1 != NAN && val2 != NAN)                                                                          \
            {                                                                                                       \
                return func_ ## code(eval(tree_ptr, node_ptr->left_child), eval(tree_ptr, node_ptr->right_child));  \   
            }                                                                                                       \
            ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)                                                             \     
            return NAN;                                                                                             \
            
    #define DEF_FUNC(code, ...)                                                                                 \ 
    case code:                                                                                                  \
        val1 = eval(tree_ptr, node_ptr->left_child);                                                            \
        val2 = eval(tree_ptr, node_ptr->right_child);                                                           \
                                                                                                                \
        if(val1 != NAN && val2 != NAN)                                                                          \
        {                                                                                                       \
            return func_ ## code(eval(tree_ptr, node_ptr->left_child), eval(tree_ptr, node_ptr->right_child));  \   
        }                                                                                                       \
        ERROR_MESSAGE(stderr, ERR_INVALID_ARGUMENT)                                                             \    
        return NAN;                                                                                             \

    #include "def_cmd.h"

    #undef DEF_OP
    #undef DEF_FUNC

    default:
        ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC);
        return NAN;
    }
}

Node* input_tree(Tree* tree_ptr) // ok
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
            tree_ptr->num_found_vars++;
            return VARIB_NODE(tree_ptr->toks[cur_tok].text)
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_CNST_VAR)
        {
            GET_CUR_TOK()
            tree_ptr->num_found_vars++;
            return CNST_VARIB_NODE(tree_ptr->toks[cur_tok].text)
        }

        Node* left  = nullptr;
        Node* right = nullptr;
        switch(tree_ptr->toks[tree_ptr->cur_tok].value.int_val)
        {

        #define DEF_OP(code, ...)                                               \
            case code: tree_ptr->cur_tok++;                                     \
                left  = input_tree(tree_ptr);                                   \
                right = input_tree(tree_ptr);                                   \
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
                    left  = input_tree(tree_ptr);                                       \
                    right = input_tree(tree_ptr);                                       \
                    return create_node(tree_ptr, code, IS_FUNC, "", left, right);       \
                }                                                                       \
                left = input_tree(tree_ptr);                                            \
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
    file_inp_ptr = nullptr;
    return RETURN_OK;
}

int get_into_buff(Tree* tree_ptr, char* file_name) // ok
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

    size_t toks_num = 0; // current totatl number of tokens
    while(token_val != NULL)                        
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
                    tree_ptr->toks[toks_num].type  = IS_OP;             \
                }                                                       \                                         

            #define DEF_FUNC(name, code, str_val)                       \
                if(strcasecmp(token_val, str_val) == 0)                 \
                {                                                       \
                    invalid_tok = VALID_TOK;                            \
                    tree_ptr->toks[toks_num].value.int_val = code;      \
                    tree_ptr->toks[toks_num].type  = IS_FUNC;           \
                }                                                       \

            #include "def_cmd.h"
            #undef DEF_OP
            #undef DEF_FUNC

            if(invalid_tok == INVALID_TOK)
            {
                int is_const = IS_CNST_VAR;
                for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
                {
                    if(strcmp(tree_ptr->vars[cur_var].var_text, token_val) == 0)
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

Node* diff_tree(Tree* tree_ptr, char* varib_text) // ok
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

                left  = diff_tree(tree_ptr, varib_text);
                right = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok    = saved_frst_tok_num;   // to get first pre dif sub_tree
                Node* left_pre_diff  = input_tree(tree_ptr); // from the array of tokens
                Node* right_pre_diff = input_tree(tree_ptr); // from the array of tokens
                Node* mul1 = MUL_NODE(left, right_pre_diff)
                Node* mul2 = MUL_NODE(left_pre_diff, right)

                return ADD_NODE(mul1, mul2)
            }
        case Sub:  // ok
            {
                tree_ptr->cur_tok++;                 
                left  = diff_tree(tree_ptr, varib_text); // from the array of tokens                  
                right = diff_tree(tree_ptr, varib_text); // from the array of tokens                                             
                return SUB_NODE(left, right)
            }
        case Add: // ok
            {
                tree_ptr->cur_tok++;                 
                left  = diff_tree(tree_ptr, varib_text);                         
                right = diff_tree(tree_ptr, varib_text);                                                                   
                return ADD_NODE(left, right)
            }
        case Div: // ok
            {
                tree_ptr->cur_tok++;                 
                size_t saved_frst_tok_num = tree_ptr->cur_tok; // stores hte cur tok for creation of sub trees

                left  = diff_tree(tree_ptr, varib_text);
                right = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff = input_tree(tree_ptr);

                size_t dub_tree_bot_id = tree_ptr->cur_tok; // to get bottom sub trees
                Node* right_pre_diff_top = input_tree(tree_ptr);

                tree_ptr->cur_tok = dub_tree_bot_id;
                Node* right_pre_diff_b1 = input_tree(tree_ptr);

                tree_ptr->cur_tok = dub_tree_bot_id;
                Node* right_pre_diff_b2 = input_tree(tree_ptr);

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
                left = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num + 1; // to get first pre dif sub_tree (inner of cos)
                Node* left_pre_diff_inner = input_tree(tree_ptr); 
                Node* sin   = SIN_NODE(left_pre_diff_inner)
                Node* minus = NUM_NODE(-1)
                Node* mul   = MUL_NODE(minus, sin)
                 
                return MUL_NODE(mul, left)
            }
        case Sin: // ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num + 1; // to get first pre dif sub_tree(inner of sin)
                Node* left_pre_diff_inner = input_tree(tree_ptr); 
                Node* cos = COS_NODE(left_pre_diff_inner);
                 
                return MUL_NODE(cos, left);
            }
        case Log: // ok
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                left = diff_tree(tree_ptr, varib_text); 

                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                Node* left_pre_diff = input_tree(tree_ptr);

                return DIV_NODE(left, left_pre_diff)
            }
        case Exp: // ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff = input_tree(tree_ptr); 
                 
                return MUL_NODE(left_pre_diff, left)
            }
        case Sqrt: // ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff = input_tree(tree_ptr);
                Node* coef = NUM_NODE(2)
                Node* mul  = MUL_NODE(coef, left_pre_diff)
                 
                return DIV_NODE(left, mul)
            }
        case Tan: //ok
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                left = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree (inner of tan)
                Node* left_pre_diff_1 = input_tree(tree_ptr);
                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree (inner of tan)
                Node* left_pre_diff_2 = input_tree(tree_ptr);

                Node* cos_1 = COS_NODE(left_pre_diff_1)
                Node* cos_2 = COS_NODE(left_pre_diff_2)

                Node* bottom = MUL_NODE(cos_1, cos_2)
                 
                return DIV_NODE(left, bottom)
            }
        case Cot: // ok
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                left = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree (inner of cot)
                Node* left_pre_diff_1 = input_tree(tree_ptr);
                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree (inner of cot)
                Node* left_pre_diff_2 = input_tree(tree_ptr);

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
        //         left = diff_tree(tree_ptr); 

        //         tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree (inner of log)
        //         Node* left_pre_diff = input_tree(tree_ptr);
        //         Node* coef   = NUM_NODE(10)
        //         Node* bottom = MUL_NODE(coef, left_pre_diff)

        //         return DIV_NODE(left, bottom)
        //     }
        case Asin:
            {
                tree_ptr->cur_tok++; 
                size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                left = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree (inner of asin)
                Node* left_pre_diff_1 = input_tree(tree_ptr);
                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree (inner of asin)
                Node* left_pre_diff_2 = input_tree(tree_ptr);

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
                left = diff_tree(tree_ptr, varib_text);

                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                Node* left_pre_diff_1 = input_tree(tree_ptr);
                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                Node* left_pre_diff_2 = input_tree(tree_ptr);

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
                Node* base = input_tree(tree_ptr);

                size_t saved_exp_id = tree_ptr->cur_tok;
                Node*  exp          = input_tree(tree_ptr);
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
                    Node* pre_dif_pow = input_tree(tree_ptr);
                    Node* ln          = LN_NODE(base)

                    tree_ptr->cur_tok = saved_exp_id;
                    left = diff_tree(tree_ptr, varib_text);

                    Node* mul_1 = MUL_NODE(pre_dif_pow, ln);
                    return MUL_NODE(mul_1, left);
                }
                else if((base->type == IS_FUNC || base->type == IS_VARIB) && exp->type == IS_VAL)
                {
                    tree_ptr->cur_tok = saved_frst_tok_num;
                    Node* pre_dif_pow = input_tree(tree_ptr);

                    double new_exp_val = exp->value.node_value - 1.0; // decreasing exp val
                    Node* coef = NUM_NODE(exp->value.node_value);
                    exp->value.node_value = new_exp_val; // changing old exp val

                    Node* new_pow = create_node(tree_ptr, Pow, IS_FUNC, nullptr, base, exp);
                    Node* mul     = MUL_NODE(coef, new_pow);

                    tree_ptr->cur_tok = saved_frst_tok_num + 1;
                    left              = diff_tree(tree_ptr, varib_text);
                    tree_ptr->cur_tok = last_id;

                    return MUL_NODE(mul, left);
                }

                Node* one          = NUM_NODE(1); // 1
                Node* new_exp_base = SUB_NODE(exp, one); // v - 1
                Node* new_pow      = POW_NODE(base, new_exp_base); // u^(v-1)

                tree_ptr->cur_tok = saved_exp_id;
                Node* old_exp     = input_tree(tree_ptr); // v
                tree_ptr->cur_tok = saved_frst_tok_num + 1; 
                Node* diff_base   = diff_tree(tree_ptr, varib_text); // du/dx

                Node* mul_1 = MUL_NODE(old_exp, new_pow); // v * u^(v-1) 
                Node* mul_2 = MUL_NODE(mul_1, diff_base);  // v * u^(v-1) * (du/dx)

                tree_ptr->cur_tok = saved_frst_tok_num;
                Node* old_pow     = input_tree(tree_ptr); // u^v
                tree_ptr->cur_tok = saved_frst_tok_num + 1;
                Node* old_base    = input_tree(tree_ptr); // u
                Node* ln_base     = LN_NODE(old_base); // ln(u)

                Node* mul_3 = MUL_NODE(old_pow, ln_base); // u^v * ln(u)

                tree_ptr->cur_tok = saved_exp_id;
                Node* diff_exp    = diff_tree(tree_ptr, varib_text); // (dv/dx)
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
        printf("Invalid number of variables\n");
        ERROR_MESSAGE(stderr, ERR_INVALID_VAR_NUM)
        return ERR_INVALID_VAR_NUM;
    }

    tree_ptr->vars = (Var*)calloc(tree_ptr->num_of_vars, sizeof(Var));
    if(tree_ptr->vars == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_TO_CALLOC_VARS)
        return ERR_TO_CALLOC_VARS;
    }

    for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
    {
        printf("\n\nEnter the %ld variable:", cur_var);
        scanf(" %s", &(tree_ptr->vars[cur_var].var_text));
        tree_ptr->vars[cur_var].var_text[20] = '\0';

        if(isalpha((int)(tree_ptr->vars[cur_var].var_text[0])) == 0)
        {
            ERROR_MESSAGE(stderr, ERR_INVALID_VAR_TEXT)
            return ERR_INVALID_VAR_TEXT;
        }

        printf("\nEnter the value of variable '%s':", tree_ptr->vars[cur_var].var_text);
        scanf(" %lf", &(tree_ptr->vars[cur_var].var_value));
    }

    return RETURN_OK;
}

// Node* shortener(Tree* tree_ptr, Node* node_ptr)
// {
//     if(node_ptr == nullptr)
//     {
//         return nullptr;
//     }
//     if(node_ptr->type == IS_VAL)
//     {
//         return node_ptr;
//     }
//     if(node_ptr->type == IS_OP)
//     {
//         double result = 0;

//         switch(node_ptr->value.op_number)
//         {
//         case Add:
//             { 
//                 if(NODE_LEFT_CHILD->type == IS_VAL && is_poisitive(NODE_LEFT_CHILD->value.node_value) == IS_ZERO && 
//                     NODE_RIGHT_CHILD->type == IS_VARIB)
//                 {
//                     return VARIB_NODE(NODE_RIGHT_CHILD->value.text)
//                 }
//                 else if(NODE_RIGHT_CHILD->type == IS_VAL && is_poisitive(NODE_RIGHT_CHILD->value.node_value) == IS_ZERO && 
//                     NODE_LEFT_CHILD->type == IS_VARIB)
//                 {
//                     return VARIB_NODE(NODE_LEFT_CHILD->value.text)
//                 }
//                 else if(NODE_RIGHT_CHILD->type == IS_VAL && NODE_LEFT_CHILD->type == IS_VAL)
//                 {
//                     return NUM_NODE(NODE_RIGHT_CHILD->value.node_value + NODE_LEFT_CHILD->value.node_value)
//                 }
//                 else if(NODE_RIGHT_CHILD->type == IS_VARIB && NODE_LEFT_CHILD->type == IS_VARIB)
//                 {
//                     return SUB_NODE()
//                 }
//             }
//         // case Sub:
//         //     {
                
//         //     }
//         // case Mul:
//         //     {
                
//         //     }
//         // case Div:
//         //     {
                
//         //     }
//         default:
//             ERROR_MESSAGE(stderr, ERR_UNKNOWN_OPERATOR)
//             return nullptr;
//         }
//     }

//     switch(node_ptr->value.op_number)
//     {        
//     case Sin:
//         {
//             Node* short_arg = SHORT_CHILD(NODE_LEFT_CHILD);
//             if(short_arg->type == IS_VAL)
//             {
//                 dtor_childs(node_ptr);
//                 return NUM_NODE(func_Sin(short_arg->value.node_value))
//             }
//             else
//             {
//                 return SIN_NODE(short_arg)
//             }
//         }
//     default:
//         break;
//     }

// }

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

Node* full_diff(Tree* tree_ptr) // ok
{
    tree_ptr->cur_tok = 0;
    if(tree_ptr->num_of_vars == 0)
    {
        dtor_childs(tree_ptr->root);
        return diff_tree(tree_ptr, tree_ptr->vars[0].var_text);
    }
    else
    {
        Node* first_diff = diff_tree(tree_ptr, tree_ptr->vars[0].var_text);
        tree_ptr->cur_tok = 0; // Needs to be set to zero because diff tree increases the cor_tok value every time it diff the tree

        for(size_t cur_diff = 1; cur_diff < tree_ptr->num_of_vars; cur_diff++)
        {
            first_diff = ADD_NODE(first_diff, diff_tree(tree_ptr, tree_ptr->vars[cur_diff].var_text));
            tree_ptr->cur_tok = 0; // Needs to be set to zero because diff tree increases the cor_tok value every time it diff the tree
        }


        dtor_childs(tree_ptr->root);
        return first_diff;
    }
}

Node* copy_subtree(Tree* tree_ptr, Node* node_ptr)
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
    else
    {
        return create_node(tree_ptr, node_ptr->value.op_number, node_ptr->type, nullptr, copy_subtree(tree_ptr, node_ptr->left_child), copy_subtree(tree_ptr, node_ptr->right_child));
    }
}
