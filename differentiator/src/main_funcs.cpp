#include "Differentiator.h"

size_t print_recur_tree(const Node* const node_ptr, FILE* file_ptr)
{
    if(node_ptr == nullptr)
    {
        return NULL_PTR_NODE;
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

size_t output_tree(const Node* const root_node_ptr, char* file_name)
{
    FILE* file_ptr = fopen(file_name, "w");
    if(file_ptr == nullptr)
    {
        return ERR_CANNOT_OPEN_OUTPUT;
    }

    print_recur_tree(root_node_ptr, file_ptr);

    if(fclose(file_ptr) == EOF)
    {
        return ERR_CANNOT_CLOSE_OUTPUT;
    }
    file_ptr = nullptr;
}

double eval(const Tree* const tree_ptr, const Node* const node_ptr) // OK
{
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
                printf("returned: %lf \n", tree_ptr->vars[cur_var].var_value);
                return tree_ptr->vars[cur_var].var_value;
            }
        }
    }

    switch(node_ptr->value.op_number)
    {

    #define DEF_OP(code, ...) case code: return func_ ## code(eval(tree_ptr, node_ptr->left_child), eval(tree_ptr, node_ptr->right_child));
    #define DEF_FUNC(code, ...) case code: return func_ ## code(eval(tree_ptr, node_ptr->left_child), eval(tree_ptr, node_ptr->right_child));

    #include "def_cmd.h"

    #undef DEF_OP
    #undef DEF_FUNC

    default:
        printf("cmd: %s\n", node_ptr->value.text);
        printf("\n\nUNKNOWN cmd\n\n");
        break;
    }
}

size_t generate_cpu_code(const Node* const root_node_ptr)
{
    FILE* file_ptr = fopen("../CPU/ASM/asm_code.txt", "w");
    if(file_ptr == nullptr)
    {
        return ERR_CANNOT_OPEN_OUTPUT;
    }

    print_recur_code(root_node_ptr, file_ptr);
    fprintf(file_ptr, "OUT\n");
    fprintf(file_ptr, "HLT\n");

    if(fclose(file_ptr) == EOF)
    {
        return ERR_CANNOT_CLOSE_OUTPUT;
    }
    file_ptr = nullptr;
}

void print_recur_code(const Node* const node_ptr, FILE* file_ptr)
{
    if(node_ptr == nullptr)
    {
        return;
    }

    print_recur_code(node_ptr->left_child, file_ptr);
    print_recur_code(node_ptr->right_child, file_ptr);

    int op_number = node_ptr->value.op_number;

    if(node_ptr->type == IS_VAL)
    {
        fprintf(file_ptr, "PUSH %d\n", (int)node_ptr->value.node_value);
        return;
    }

    // switch(op_number)
    // {

    // // #define DEF_CMD(code, ...) case code: fprintf(file_ptr, "%s\n", #code); break;
    // // #include "def_cmd.h"
    // // #undef DEF_CMD

    // default:
    //     printf("\n\nNEW CMD\n\n");
    //     break;
    // }
}

Node* input_tree(Tree* tree_ptr)
{
    if(tree_ptr->cur_tok < tree_ptr->num_of_toks)
    {
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VAL)
        {   
            GET_CUR_TOK()
            return create_node(tree_ptr, tree_ptr->toks[cur_tok].value.flt_val);
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VARIB)
        {
            GET_CUR_TOK()
            return create_node(tree_ptr, 0, IS_VARIB, tree_ptr->toks[cur_tok].text);
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_CNST_VAR)
        {
            GET_CUR_TOK()
            return create_node(tree_ptr, 0, IS_CNST_VAR, tree_ptr->toks[cur_tok].text);
        }

        Node* left  = nullptr;
        Node* right = nullptr;
        switch(tree_ptr->toks[tree_ptr->cur_tok].value.int_val)
        {

        #define DEF_OP(code, ...)                                               \
            case code: tree_ptr->cur_tok++;                                     \
                left = input_tree(tree_ptr);                                    \
                right = input_tree(tree_ptr);                                   \
                return create_node(tree_ptr, code, IS_OP, "", left, right);     \

        #define DEF_FUNC(code, ...)                                                     \
            case code: tree_ptr->cur_tok++;                                             \
                if(code == Pow)                                                         \
                {                                                                       \
                    left  = input_tree(tree_ptr);                                        \
                    right = input_tree(tree_ptr);                                       \
                    return create_node(tree_ptr, code, IS_FUNC, "", left, right);       \
                }                                                                       \
                left = input_tree(tree_ptr);                                            \
                return create_node(tree_ptr, code, IS_FUNC, "", left);                  \

        #include "def_cmd.h"
        #undef DEF_OP
        #undef DEF_FUNC

        default:
            printf("\n\nUNKNOWN token\n\n");
            break;
        }
    }
}

size_t check_is_int(char* num_text) // OK
{
    size_t is_digits = IS_INT; 

    size_t length_text = strlen(num_text);
    if(num_text[0] == '-' && length_text == 1)
    {
        return NOT_ALL_DIGITS;
    }
    if(num_text[0] != '-' && isdigit(num_text[0]) == 0)
    {   
        // printf("\nChecking char: %c\n", num_text[0]);
        return NOT_ALL_DIGITS;
    }
    for(size_t i  = 1; i < length_text; i++)
    {
        if(isdigit(num_text[i]) == 0) // If the character is not a digit
        {
            is_digits = NOT_ALL_DIGITS;
        }
    }

    return is_digits; // Returns 1 if all characters are digits (word is an integer)
}

size_t check_is_float(char* num_text) // OK
{
    size_t is_digits = IS_FLOAT; 

    size_t length_text = strlen(num_text);
    if(num_text[0] == '-' && length_text == 1)
    {
        return NOT_ALL_DIGITS;
    }
    if(num_text[0] != '-' && isdigit(num_text[0]) == 0)
    {   
        // printf("\nChecking char: %c\n", num_text[0]);
        return NOT_ALL_DIGITS;
    }
    for(size_t i  = 1; i < length_text; i++)
    {
        if(isdigit(num_text[i]) == 0 && num_text[i] != '.') // If the character is not a digit
        {
            is_digits = NOT_ALL_DIGITS;
        }
    }

    return is_digits; // Returns 1 if all characters are digits (word is an integer)  
}

int get_size(Tree* tree_ptr, char* file_name) 
{
    FILE* file_inp_ptr = fopen(file_name, "rb");
    if(file_inp_ptr == nullptr)
    {
        return ERR_CANNOT_OPEN_INPUT;
    }

    fseek(file_inp_ptr, 0, SEEK_END); // Puts the pointer inside the file to the end of it
    tree_ptr->size = ftell(file_inp_ptr); // Get the size of the file with '\r'!

    if(tree_ptr->size == 0) 
    {
        // tree_ptr->err_code = ERR_EMPTY_ASM_FILE; 
        // dump_asm(tree_ptr, FUNC_NAME, FUNC_LINE, FUNC_FILE);
        // dtor_asm(tree_ptr);  
        // return tree_ptr->err_code;
    } 

    // printf("size: %ld\n\n", tree_ptr->size);
    if(fclose(file_inp_ptr) == EOF)
    {   
        return ERR_CANNOT_CLOSE_INPUT;
    }
    file_inp_ptr = nullptr;\
    return RETURN_OK;
}

int get_into_buff(Tree* tree_ptr, char* file_name) 
{
    FILE* file_inp_ptr = fopen(file_name, "rb");
    if(file_inp_ptr == nullptr)
    {
        return ERR_CANNOT_OPEN_INPUT;
    }

    tree_ptr->tree_buff = (char*)calloc(1, sizeof(char) * (tree_ptr->size + 1)); // Allocates enough memmory for the buffer of chars  

    if(tree_ptr->tree_buff == nullptr)
    {
        // tree_ptr->err_code = ERR_TO_CALLOC_ASM_BUF;
        // dump_asm(tree_ptr, FUNC_NAME, FUNC_LINE, FUNC_FILE);
        // dtor_asm(tree_ptr);
        // return tree_ptr->err_code;
    }

    int num_read = fread(tree_ptr->tree_buff, sizeof(char), tree_ptr->size, file_inp_ptr); // Reads the file into the buffer
    
    if((num_read <= 0) && (num_read > tree_ptr->size))
    {
        // tree_ptr->err_code = ERR_READ_TO_ASM_BUF;
        // dump_asm(tree_ptr, FUNC_NAME, FUNC_LINE, FUNC_FILE);
        // dtor_asm(tree_ptr);
        // return tree_ptr->err_code;       
    }
    tree_ptr->tree_buff[tree_ptr->size] = '\0'; // Makes form the file null-terminated string

    // printf("buff: %s\n", tree_ptr->tree_buff);
    if(fclose(file_inp_ptr) == EOF)
    {   
        return ERR_CANNOT_CLOSE_INPUT;
    }
    file_inp_ptr = nullptr;
    return RETURN_OK;
}

int get_tokens(Tree* tree_ptr) // ok
{
    char* token_val = strtok(tree_ptr->tree_buff,"( ) \n\r");
    tree_ptr->toks = (Tokens*)calloc(1, sizeof(Tokens));
    
    if(tree_ptr->toks == nullptr)
    {
        tree_ptr->error_code = ERR_TO_CALLOC_TOKS;
        ERROR_MESSAGE(stderr, ERR_TO_CALLOC_TOKS)
        return tree_ptr->error_code;
    }

    size_t toks_num = 0;
    while(token_val != NULL)                        
    {   
        realloc_toks(tree_ptr, toks_num); // Reallocs the struct with tokens

        // if(tree_ptr->err_code != STRUCT_OK)
        // {
        //     return tree_ptr->err_code;
        // }
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
            tree_ptr->error_code = ERR_TO_CALLOC_TOKS;
            ERROR_MESSAGE(stderr,ERR_TO_CALLOC_TOKS)
            return tree_ptr->error_code;
        }
    }

    return RETURN_OK;
}

void print_toks(Tree* tree_ptr) // DEBUG
{
    for(size_t tok_id = 0; tok_id < tree_ptr->num_of_toks; tok_id++)
    {
        if(tree_ptr->toks[tok_id].type == IS_VAL)
        {
            printf("index: %ld, type = %ld, val = %d\n", tok_id, tree_ptr->toks[tok_id].type, tree_ptr->toks[tok_id].value);
        }
        else if(tree_ptr->toks[tok_id].type == IS_OP)
        {
            printf("index: %ld, type = %ld, val = %c\n", tok_id, tree_ptr->toks[tok_id].type, tree_ptr->toks[tok_id].value);
        }
        else
        {
            printf("index: %ld, type = %ld, val = %s\n", tok_id, tree_ptr->toks[tok_id].type, tree_ptr->toks[tok_id].text);
        }
    }
}

Node* diff_tree(Tree* tree_ptr)
{
    if(tree_ptr->cur_tok < tree_ptr->num_of_toks)
    {
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VAL)
        {   
            GET_CUR_TOK()
            return NUM_NODE(0)
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VARIB)
        {
            GET_CUR_TOK()
            return NUM_NODE(1)
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_CNST_VAR)
        {
            GET_CUR_TOK()
            return NUM_NODE(1)
        }

        Node* left  = nullptr;
        Node* right = nullptr;

        switch(tree_ptr->toks[tree_ptr->cur_tok].value.int_val)
        {
        case Mul:  // ok 
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok; // stores hte cur tok for creation of sub trees

                left  = diff_tree(tree_ptr);
                right = diff_tree(tree_ptr);

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
                left  = diff_tree(tree_ptr); // from the array of tokens                  
                right = diff_tree(tree_ptr); // from the array of tokens                                             
                return SUB_NODE(left, right)
            }
        case Add: // ok
            {
                tree_ptr->cur_tok++;                 
                left  = diff_tree(tree_ptr);                         
                right = diff_tree(tree_ptr);                                                                   
                return ADD_NODE(left, right)
            }
        case Div: // ok
            {
                tree_ptr->cur_tok++;                 
                size_t saved_frst_tok_num = tree_ptr->cur_tok; // stores hte cur tok for creation of sub trees

                left  = diff_tree(tree_ptr);
                right = diff_tree(tree_ptr);

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
                left = diff_tree(tree_ptr);

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
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num + 1; // to get first pre dif sub_tree(inner of sin)
                Node* left_pre_diff_inner = input_tree(tree_ptr); 
                Node* cos = COS_NODE(left_pre_diff_inner);
                 
                return MUL_NODE(cos, left);
            }
        case Log: // ok
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                left = diff_tree(tree_ptr); 

                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                Node* left_pre_diff = input_tree(tree_ptr);

                return DIV_NODE(left, left_pre_diff)
            }
        case Exp: // ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff = input_tree(tree_ptr); 
                 
                return MUL_NODE(left_pre_diff, left)
            }
        case Sqrt: // ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr);

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
                left = diff_tree(tree_ptr);

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
                left = diff_tree(tree_ptr);

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
                left = diff_tree(tree_ptr);

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
                left = diff_tree(tree_ptr);

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
                    left = diff_tree(tree_ptr);

                    Node* mul_1 = MUL_NODE(pre_dif_pow, ln);
                    return MUL_NODE(mul_1, left);
                }
                else if((base->type == IS_FUNC || base->type == IS_VARIB) && exp->type == IS_VAL)
                {
                    tree_ptr->cur_tok = saved_frst_tok_num;
                    Node* pre_dif_pow = input_tree(tree_ptr);

                    double new_exp_val = exp->value.node_value - 1.0; // decreasing exp val
                    printf("\n\n%lf\n\n", new_exp_val);
                    Node* coef = NUM_NODE(exp->value.node_value);
                    exp->value.node_value = new_exp_val; // changing old exp val

                    Node* new_pow = create_node(tree_ptr, Pow, IS_FUNC, nullptr, base, exp);
                    Node* mul     = MUL_NODE(coef, new_pow);

                    tree_ptr->cur_tok = saved_frst_tok_num + 1;
                    left              = diff_tree(tree_ptr);
                    tree_ptr->cur_tok = last_id;

                    return MUL_NODE(mul, left);
                }

                Node* one          = NUM_NODE(1); // 1
                Node* new_exp_base = SUB_NODE(exp, one); // v - 1
                Node* new_pow      = POW_NODE(base, new_exp_base); // u^(v-1)

                tree_ptr->cur_tok = saved_exp_id;
                Node* old_exp     = input_tree(tree_ptr); // v
                tree_ptr->cur_tok = saved_frst_tok_num + 1; 
                Node* diff_base   = diff_tree(tree_ptr); // du/dx

                Node* mul_1 = MUL_NODE(old_exp, new_pow); // v * u^(v-1) 
                Node* mul_2 = MUL_NODE(mul_1, diff_base);  // v * u^(v-1) * (du/dx)

                tree_ptr->cur_tok = saved_frst_tok_num;
                Node* old_pow     = input_tree(tree_ptr); // u^v
                tree_ptr->cur_tok = saved_frst_tok_num + 1;
                Node* old_base    = input_tree(tree_ptr); // u
                Node* ln_base     = LN_NODE(old_base); // ln(u)

                Node* mul_3 = MUL_NODE(old_pow, ln_base); // u^v * ln(u)

                tree_ptr->cur_tok = saved_exp_id;
                Node* diff_exp    = diff_tree(tree_ptr); // (dv/dx)
                Node* mul_4       = MUL_NODE(mul_3, diff_exp); // u^v * ln(u) * (dv/dx)

                return ADD_NODE(mul_2, mul_4);
            }
        default:
            printf("\n\nUNKNOWN COMMAND\n\n");
            break;
        }
    }
}

int get_vars(Tree* tree_ptr) 
{
    printf("\nEnter the number of variables:");
    scanf(" %ld", &(tree_ptr->num_of_vars));

    if(tree_ptr->num_of_vars <= 0)
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
        printf("\nEnter the %ld variable:", cur_var);
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

    // for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
    // {
    //     printf("\nVariable '%s' = %lf\n", tree_ptr->vars[cur_var].var_text, tree_ptr->vars[cur_var].var_value);
    // }
    // printf("\nVar string: %s\n", tree_ptr->vars);

    return RETURN_OK;
}

Node* shortener(Tree* tree_ptr, Node* node_ptr)
{
    if(node_ptr == nullptr)
    {
        return nullptr;
    }
    if(node_ptr->type == IS_VAL)
    {
        return node_ptr;
    }
    if(node_ptr->type == IS_OP)
    {
        double result = 0;

        switch(node_ptr->value.op_number)
        {
        case Add:
            { 
                if((node_ptr->left_child->type == IS_VARIB || node_ptr->left_child->type == IS_CNST_VAR) 
                    && (node_ptr->right_child->type == IS_VARIB || node_ptr->right_child->type == IS_CNST_VAR))
                {
                    printf("\nadd1\n");
                    return node_ptr;
                }
                else if((node_ptr->left_child->type == IS_VARIB || node_ptr->left_child->type == IS_CNST_VAR) && 
                    (node_ptr->right_child->type == IS_OP || node_ptr->right_child->type == IS_VAL || node_ptr->right_child->type == IS_FUNC))
                {
                    printf("\nadd2\n");
                    node_ptr->right_child = shortener(tree_ptr, node_ptr->right_child);
                    return node_ptr;
                }
                else if((node_ptr->left_child->type == IS_OP || node_ptr->left_child->type == IS_FUNC) && 
                    (node_ptr->right_child->type == IS_VARIB || node_ptr->right_child->type == IS_CNST_VAR))
                {
                    printf("\nadd3\n");
                    node_ptr->left_child = shortener(tree_ptr, node_ptr->left_child);
                    return node_ptr;
                }
                else if(node_ptr->left_child->type == IS_VAL && (node_ptr->right_child->type == IS_OP || node_ptr->right_child->type == IS_VAL || 
                    node_ptr->right_child->type == IS_FUNC))
                {
                    printf("\nadd4\n");
                    Node* new_right_node = shortener(tree_ptr, node_ptr->right_child);
                    if(new_right_node->type == IS_VAL)
                    {
                        result = new_right_node->value.node_value + node_ptr->left_child->value.node_value;
                        dtor_childs(node_ptr);
                        return NUM_NODE(result);
                    }
                    else
                    {
                        node_ptr->right_child = new_right_node;
                        return node_ptr;
                    }
                }
                else if(node_ptr->right_child->type == IS_VAL && (node_ptr->left_child->type == IS_OP || node_ptr->left_child->type == IS_VAL || 
                    node_ptr->left_child->type == IS_FUNC))
                {
                    printf("\nadd5\n");
                    Node* new_left_node = shortener(tree_ptr, node_ptr->left_child);
                    if(new_left_node->type == IS_VAL)
                    {
                        result = new_left_node->value.node_value + node_ptr->right_child->value.node_value;
                        dtor_childs(node_ptr);
                        return NUM_NODE(result);
                    }
                    else
                    {
                        node_ptr->left_child = new_left_node;
                        return node_ptr;
                    }
                }
                else
                {
                    printf("\nadd6\n");
                    result = shortener(tree_ptr, node_ptr->left_child)->value.node_value + shortener(tree_ptr, node_ptr->right_child)->value.node_value;
                    dtor_childs(node_ptr);
                    return NUM_NODE(result);
                }
                return node_ptr;
            }
        case Sub:
            {
                if((node_ptr->left_child->type == IS_VARIB || node_ptr->left_child->type == IS_CNST_VAR) 
                    && (node_ptr->right_child->type == IS_VARIB || node_ptr->right_child->type == IS_CNST_VAR))
                {
                    printf("\nsub1\n");
                    return node_ptr;
                }
                else if((node_ptr->left_child->type == IS_VARIB || node_ptr->left_child->type == IS_CNST_VAR) && 
                    (node_ptr->right_child->type == IS_OP || node_ptr->right_child->type == IS_FUNC))
                {
                    printf("\nsub2\n");
                    node_ptr->right_child = shortener(tree_ptr, node_ptr->right_child);
                    return node_ptr;
                }
                else if((node_ptr->left_child->type == IS_OP || node_ptr->left_child->type == IS_FUNC) && 
                    (node_ptr->right_child->type == IS_VARIB || node_ptr->right_child->type == IS_CNST_VAR))
                {
                    printf("\nsub3\n");
                    node_ptr->left_child = shortener(tree_ptr, node_ptr->left_child);
                    return node_ptr;
                }
                else if(node_ptr->left_child->type == IS_VAL && (node_ptr->right_child->type == IS_OP || node_ptr->right_child->type == IS_VAL || 
                    node_ptr->right_child->type == IS_FUNC))
                {
                    printf("\nsub4\n");
                    Node* new_right_node = shortener(tree_ptr, node_ptr->right_child);
                    if(new_right_node->type == IS_VAL)
                    {
                        result = node_ptr->left_child->value.node_value - new_right_node->value.node_value;
                        dtor_childs(node_ptr);
                        return NUM_NODE(result);
                    }
                    else
                    {
                        node_ptr->right_child = new_right_node;
                        return node_ptr;
                    }
                }
                else if(node_ptr->right_child->type == IS_VAL && (node_ptr->left_child->type == IS_OP || node_ptr->left_child->type == IS_VAL || 
                    node_ptr->left_child->type == IS_FUNC))
                {
                    printf("\nsub5\n");
                    Node* new_left_node = shortener(tree_ptr, node_ptr->left_child);
                    if(new_left_node->type == IS_VAL)
                    {
                        result = new_left_node->value.node_value - node_ptr->right_child->value.node_value;
                        dtor_childs(node_ptr);
                        return NUM_NODE(result);
                    }
                    else
                    {
                        node_ptr->left_child = new_left_node;
                        return node_ptr;
                    }
                }
                else
                {
                    printf("\nsub6\n");
                    result = shortener(tree_ptr, node_ptr->left_child)->value.node_value - shortener(tree_ptr, node_ptr->right_child)->value.node_value;
                    dtor_childs(node_ptr);
                    return NUM_NODE(result);
                }
                return node_ptr;
            }
        case Mul:
            {
                if((node_ptr->left_child->type == IS_VAL && fabs(node_ptr->left_child->value.node_value) <= EPS) || 
                    (node_ptr->right_child->type == IS_VAL && fabs(node_ptr->right_child->value.node_value) <= EPS))
                {
                    printf("\nmul0\n");
                    dtor_childs(node_ptr);
                    return NUM_NODE(0);
                }
                else if((node_ptr->left_child->type == IS_VARIB || node_ptr->left_child->type == IS_CNST_VAR) 
                    && (node_ptr->right_child->type == IS_VARIB || node_ptr->right_child->type == IS_CNST_VAR))
                {
                    printf("\nmul1\n");
                    return node_ptr;
                }
                else if((node_ptr->left_child->type == IS_VARIB || node_ptr->left_child->type == IS_CNST_VAR) && 
                    (node_ptr->right_child->type == IS_OP || node_ptr->right_child->type == IS_FUNC || node_ptr->right_child->type == IS_VAL))
                {
                    printf("\nmul2\n");
                    Node* new_right_node = shortener(tree_ptr, node_ptr->right_child);
                    if(new_right_node->type == IS_VAL && fabs(new_right_node->value.node_value) <= EPS)
                    {
                        dtor_childs(node_ptr);
                        return NUM_NODE(0);
                    }
                    node_ptr->right_child = new_right_node;
                    return node_ptr;
                }
                else if((node_ptr->left_child->type == IS_OP || node_ptr->left_child->type == IS_VAL || node_ptr->left_child->type == IS_FUNC) && 
                    (node_ptr->right_child->type == IS_VARIB || node_ptr->right_child->type == IS_CNST_VAR))
                {
                    printf("\nmul3\n");
                    Node* new_left_node = shortener(tree_ptr, node_ptr->left_child);
                    if(new_left_node->type == IS_VAL && fabs(new_left_node->value.node_value) <= EPS)
                    {
                        dtor_childs(node_ptr);
                        return NUM_NODE(0);
                    }
                    node_ptr->left_child = new_left_node;
                    return node_ptr;
                }
                else
                {
                    printf("\nmul6\n");
                    result = shortener(tree_ptr, node_ptr->left_child)->value.node_value * shortener(tree_ptr, node_ptr->right_child)->value.node_value;
                    dtor_childs(node_ptr);
                    return NUM_NODE(result);
                }
                return node_ptr;
            }
        default:
            break;
        }
    }

    switch(node_ptr->value.op_number)
    {
    case Sin:

        break;
    
    default:
        break;
    }

}

char* get_string_func(size_t func_code)
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
        return "new_func";
    }
}

