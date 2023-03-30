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

size_t output_tree(const Node* const root_node_ptr)
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

double eval(const Node* const node_ptr)
{
    if(node_ptr->type == IS_VAL)
    {
        return node_ptr->value.node_value;
    }

    switch(node_ptr->value.op_number)
    {

    #define DEF_OP(code, ...) case code: return func_ ## code(eval(node_ptr->left_child), eval(node_ptr->right_child));
    #define DEF_FUNC(code, ...) case code: return func_ ## code(eval(node_ptr->left_child));

    #include "def_cmd.h"

    #undef DEF_OP
    #undef DEF_FUNC

    default:
        printf("cmd: %s\n", node_ptr->value.text);
        printf("\n\nUNKNOWN cmd\n\n");
        break;
    }
}

double func_Add(double value_1, double value_2)
{
    return value_1 + value_2;
}

double func_Sub(double value_1, double value_2)
{
    return value_1 - value_2;
}

double func_Mul(double value_1, double value_2)
{
    return value_1 * value_2;
}

double func_Div(double value_1, double value_2)
{
    if(fabs(value_2) <= EPS)
    {
        return NAN;
    }
    return value_1 / value_2;
}

double func_Cos(double value_1)
{
    return cos(value_1 * PI / 180.0);
}

double func_Sin(double value_1)
{
    return sin(value_1 * PI / 180.0);
}

double func_Tan(double value_1)
{
    return tan(value_1 * PI / 180.0);
}

double func_Cot(double value_1)
{
    return 1 - tan(value_1 * PI / 180.0);
}

double func_Asin(double value_1)
{
    return (asin(value_1) * 180.0) / PI;
}

double func_Acos(double value_1)
{
    return (acos(value_1) * 180.0) / PI;
}

double func_Sqrt(double value_1)
{
    return acos(value_1);
}

double func_Exp(double value_1)
{
    return exp(value_1);
}

double func_Log(double value_1)
{
    return log(value_1);
}

double func_Log10(double value_1)
{
    return log10(value_1);
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
    free(file_ptr);
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

    switch(op_number)
    {

    // #define DEF_CMD(code, ...) case code: fprintf(file_ptr, "%s\n", #code); break;
    // #include "def_cmd.h"
    // #undef DEF_CMD

    default:
        printf("\n\nNEW CMD\n\n");
        break;
    }
}

Node* input_tree(Tree* tree_ptr)
{
    if(tree_ptr->cur_tok < tree_ptr->num_of_toks)
    {
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VAL)
        {   
            size_t cur_tok = tree_ptr->cur_tok;
            tree_ptr->cur_tok++;
            return create_node(tree_ptr, tree_ptr->toks[cur_tok].value);
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VARIB)
        {

            size_t cur_tok = tree_ptr->cur_tok;
            tree_ptr->cur_tok++;
            return create_node(tree_ptr, 0, IS_VARIB, tree_ptr->toks[cur_tok].text);
        }

        Node* left  = nullptr;
        Node* right = nullptr;
        switch(tree_ptr->toks[tree_ptr->cur_tok].value)
        {

        #define DEF_OP(code, int_val, ...) case code: tree_ptr->cur_tok++; left = input_tree(tree_ptr); right = input_tree(tree_ptr); return create_node(tree_ptr, int_val, IS_OP, "", left, right);
        #define DEF_FUNC(code, int_val, ...) case code: tree_ptr->cur_tok++; left = input_tree(tree_ptr); return create_node(tree_ptr, int_val, IS_FUNC, "", left);
        #include "def_cmd.h"
        #undef DEF_OP
        #undef DEF_FUNC

        default:
            printf("\n\nUNKNOWN token\n\n");
            break;
        }
    }
}

size_t check_is_number(char* num_text) 
{
    size_t is_digits = ALL_DIGITS; 

    size_t length_text = strlen(num_text);
    for(size_t i  = 0; i < length_text; i++)
    {
        if(isdigit(num_text[i]) == 0) // If the character is not a digit
        {
            is_digits = NOT_ALL_DIGITS;
        }
    }

    return is_digits; // Returns 1 if all characters are digits (word is an integer)
}

size_t get_size(Tree* tree_ptr) 
{
    FILE* file_inp_ptr = fopen("input_tree.txt", "rb");
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
    free(file_inp_ptr);
    file_inp_ptr = nullptr;
}

size_t get_into_buff(Tree* tree_ptr) 
{
    FILE* file_inp_ptr = fopen("input_tree.txt", "rb");
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
    free(file_inp_ptr);
    file_inp_ptr = nullptr;
}

size_t get_tokens(Tree* tree_ptr) 
{
    char* token_val = strtok(tree_ptr->tree_buff,"( ) \n\r");
    tree_ptr->toks = (tokens*)calloc(1, sizeof(tokens));
    
    // if(tree_ptr->toks == nullptr)
    // {
    //     tree_ptr->err_code = ERR_TO_CALLOC_TOKS;
    //     dump_asm(tree_ptr, FUNC_NAME, FUNC_LINE, FUNC_FILE);
    //     dtor_asm(tree_ptr);
    //     return tree_ptr->err_code;
    // }

    size_t toks_num = 0;
    while(token_val != NULL)                        
    {   
        realloc_toks(tree_ptr, toks_num); // Reallocs the struct with tokens

        // if(tree_ptr->err_code != STRUCT_OK)
        // {
        //     return tree_ptr->err_code;
        // }
        
        if(check_is_number(token_val) == ALL_DIGITS)
        {
            tree_ptr->toks[toks_num].value = atoi(token_val);
            tree_ptr->toks[toks_num].type  = IS_VAL;
        }
        else if(token_val[0] == 'x')
        {
            tree_ptr->toks[toks_num].text[0] = 'x';
            tree_ptr->toks[toks_num].text[1] = '\0';
            tree_ptr->toks[toks_num].type = IS_VARIB;
        }
        else
        {
            #define DEF_OP(name, int_val, char_val) if(token_val[0] == char_val){tree_ptr->toks[toks_num].value = int_val; tree_ptr->toks[toks_num].type  = IS_OP;}
            #define DEF_FUNC(name, int_val, str_val) if(strcmp(token_val, str_val) == 0){tree_ptr->toks[toks_num].value = int_val; tree_ptr->toks[toks_num].type  = IS_FUNC;}
            #include "def_cmd.h"
            #undef DEF_OP
            #undef DEF_FUNC

        }

        token_val = strtok(NULL, "( ) \n\r");
        toks_num++;
    }
}

size_t realloc_toks(Tree* tree_ptr, size_t i) 
{
    if(tree_ptr->num_of_toks == i)
    {
        tree_ptr->num_of_toks++;
        tree_ptr->toks = (tokens*)realloc(tree_ptr->toks, tree_ptr->num_of_toks * sizeof(tokens)); // The pointer to the array of structs

        // if(tree_ptr->toks == nullptr)
        // {
        //     tree_ptr->err_code = ERR_TO_REALLOC_TOKS;
        //     dump_asm(tree_ptr, FUNC_NAME, FUNC_LINE, FUNC_FILE);
        //     dtor_asm(tree_ptr);
        //     return tree_ptr->err_code;
        // }
    }
}

void  print_toks(Tree* tree_ptr)
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
            //printf("%d ", tree_ptr->toks[tree_ptr->cur_tok].value);
            size_t cur_tok = tree_ptr->cur_tok;
            tree_ptr->cur_tok++;
            return create_node(tree_ptr, 0.0);
        }
        if(tree_ptr->toks[tree_ptr->cur_tok].type == IS_VARIB)
        {
            size_t cur_tok = tree_ptr->cur_tok;
            tree_ptr->cur_tok++;
            return create_node(tree_ptr, 1.0);
        }

        Node* left  = nullptr;
        Node* right = nullptr;

        switch(tree_ptr->toks[tree_ptr->cur_tok].value)
        {
        case Mul:  // ok 
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok; // stores hte cur tok for creation of sub trees

                left = diff_tree(tree_ptr);
                right = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff = input_tree(tree_ptr);
                Node* right_pre_diff = input_tree(tree_ptr);
                Node* mul1 = create_node(tree_ptr, Mul, IS_OP, nullptr, left, right_pre_diff);
                Node* mul2 = create_node(tree_ptr, Mul, IS_OP, nullptr, left_pre_diff, right);

                return create_node(tree_ptr, Add, IS_OP, nullptr, mul1, mul2);
            }
        case Sub:  // ok
            {
                tree_ptr->cur_tok++;                 
                left = diff_tree(tree_ptr);                         
                right = diff_tree(tree_ptr);                                                
                return create_node(tree_ptr, Sub, IS_OP, nullptr, left, right); 
            }
        case Add: // ok
            {
                tree_ptr->cur_tok++;                 
                left = diff_tree(tree_ptr);                         
                right = diff_tree(tree_ptr);                                                                   
                return create_node(tree_ptr, Add, IS_OP, nullptr, left, right);
            }
        case Div: // ok
            {
                tree_ptr->cur_tok++;                 
                size_t saved_frst_tok_num = tree_ptr->cur_tok; // stores hte cur tok for creation of sub trees

                left = diff_tree(tree_ptr);
                right = diff_tree(tree_ptr);
                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff = input_tree(tree_ptr);
                size_t dub_tree_bot_id = tree_ptr->cur_tok; // to get bottom sub trees
                Node* right_pre_diff_top = input_tree(tree_ptr);

                tree_ptr->cur_tok = dub_tree_bot_id;
                Node* right_pre_diff_b1 = input_tree(tree_ptr);
                tree_ptr->cur_tok = dub_tree_bot_id;
                Node* right_pre_diff_b2 = input_tree(tree_ptr);

                Node* mul_1 = create_node(tree_ptr, Mul, IS_OP, nullptr, left, right_pre_diff_top);
                Node* mul_2 = create_node(tree_ptr, Mul, IS_OP, nullptr, left_pre_diff, right);

                Node* top = create_node(tree_ptr, Sub, IS_OP, nullptr, mul_1, mul_2);
                Node* bottom = create_node(tree_ptr, Mul, IS_OP, nullptr, right_pre_diff_b1, right_pre_diff_b2);

                return create_node(tree_ptr, Div, IS_OP, nullptr, top, bottom);
            }
        case Cos: //ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num + 1; // to get first pre dif sub_tree
                Node* left_pre_diff_inner = input_tree(tree_ptr); 
                Node* sin = create_node(tree_ptr, Sin, IS_FUNC, nullptr, left_pre_diff_inner);
                Node* minus = create_node(tree_ptr, -1);
                Node* mul = create_node(tree_ptr, Mul, IS_OP, nullptr, minus, sin);
                 
                return create_node(tree_ptr, Mul, IS_OP, nullptr, mul, left);
            }
        case Sin: // ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num + 1; // to get first pre dif sub_tree
                Node* left_pre_diff_inner = input_tree(tree_ptr); 
                Node* cos = create_node(tree_ptr, Cos, IS_FUNC, nullptr, left_pre_diff_inner);
                 
                return create_node(tree_ptr, Mul, IS_OP, nullptr, cos, left);
            }
        case Log: // ok
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                left = diff_tree(tree_ptr); 

                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                Node* left_pre_diff = input_tree(tree_ptr);

                return create_node(tree_ptr, Div, IS_OP, nullptr, left, left_pre_diff);
            }
        case Exp: // ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff = input_tree(tree_ptr); 
                 
                return create_node(tree_ptr, Mul, IS_OP, nullptr, left_pre_diff, left);
            }
        case Sqrt: // ok
            {
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                tree_ptr->cur_tok++;
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff = input_tree(tree_ptr);
                Node* coef = create_node(tree_ptr, 2);
                Node* mul = create_node(tree_ptr, Mul, IS_OP, nullptr, coef, left_pre_diff);
                 
                return create_node(tree_ptr, Div, IS_OP, nullptr, left, mul);
            }
        case Tan: //ok
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff_1 = input_tree(tree_ptr);
                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff_2 = input_tree(tree_ptr);
                Node* cos_1 = create_node(tree_ptr, Cos, IS_FUNC, nullptr, left_pre_diff_1);
                Node* cos_2 = create_node(tree_ptr, Cos, IS_FUNC, nullptr, left_pre_diff_2);

                Node* bottom = create_node(tree_ptr, Mul, IS_OP, nullptr, cos_1, cos_2);
                 
                return create_node(tree_ptr, Div, IS_OP, nullptr, left, bottom);
            }
        case Cot: // ok
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok;
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff_1 = input_tree(tree_ptr);
                tree_ptr->cur_tok = saved_frst_tok_num; // to get first pre dif sub_tree
                Node* left_pre_diff_2 = input_tree(tree_ptr);
                Node* sin_1 = create_node(tree_ptr, Sin, IS_FUNC, nullptr, left_pre_diff_1);
                Node* sin_2 = create_node(tree_ptr, Sin, IS_FUNC, nullptr, left_pre_diff_2);

                Node* coef = create_node(tree_ptr, -1);
                Node* top = create_node(tree_ptr, Mul, IS_OP, nullptr, coef, left);
                Node* bottom = create_node(tree_ptr, Mul, IS_OP, nullptr, sin_1, sin_2);

                 
                return create_node(tree_ptr, Div, IS_OP, nullptr, top, bottom);
            }
        case Log10: //ok
            {
                tree_ptr->cur_tok++;
                size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                left = diff_tree(tree_ptr); 

                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                Node* left_pre_diff = input_tree(tree_ptr);
                Node* coef = create_node(tree_ptr, 10);
                Node* bottom = create_node(tree_ptr, Mul, IS_OP, nullptr, coef, left_pre_diff);

                return create_node(tree_ptr, Div, IS_OP, nullptr, left, bottom);
            }
        case Asin:
            {
                tree_ptr->cur_tok++; 
                size_t saved_frst_tok_num = tree_ptr->cur_tok; 
                left = diff_tree(tree_ptr);

                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                Node* left_pre_diff_1 = input_tree(tree_ptr);
                tree_ptr->cur_tok = saved_frst_tok_num;  // to get inner sub tree
                Node* left_pre_diff_2 = input_tree(tree_ptr);

                Node* one = create_node(tree_ptr, 1);
                Node* mul = create_node(tree_ptr, Mul, IS_OP, nullptr, left_pre_diff_1, left_pre_diff_2);
                Node* sub = create_node(tree_ptr, Sub, IS_OP, nullptr, one, mul);
                Node* srt = create_node(tree_ptr, Sqrt, IS_FUNC, nullptr, sub);

                return create_node(tree_ptr, Div, IS_OP, nullptr, left, srt);
            }
        default:
            printf("\n\nUNKNOWN COMMAND\n\n");
            break;
        }
    }
}

