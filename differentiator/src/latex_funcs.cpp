#include "Differentiator.h"

static int latex_equation_num = 0; // This global variable is needed in order to count the number of equations

int print_header_latex() // ok
{
    char* file_dir_name = cat_file_directory((char*)LATEX_FILE_NAME, (char*)LATEX_DIR_NAME, ""); // creates the dir + file_name char*
    char* local_dir = cat_file_directory(file_dir_name, "./", ""); // makes if local (cats './')

    FILE* tex_file_ptr = fopen(local_dir, "a+");
    if(tex_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_LATEX_FILE)
        return ERR_OPEN_LATEX_FILE;
    }

    fprintf(tex_file_ptr, "\\documentclass[a4paper, 10pt]{article}\n");
    fprintf(tex_file_ptr, "\\usepackage{tikz}\n");
    fprintf(tex_file_ptr, "\\usepackage{pgfplots}\n");
    fprintf(tex_file_ptr, "\\pgfplotsset{compat=1.15}\n");
    fprintf(tex_file_ptr, "\\begin{document}\n");

    if(fclose(tex_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_LATEX_FILE)
        return ERR_CLOSE_LATEX_FILE;
    }

    free(file_dir_name);
    free(local_dir);

    return RETURN_OK;
}

int print_footer_latex(Node* node_ptr) // ok
{
    char* file_dir_name = cat_file_directory((char*)LATEX_FILE_NAME, (char*)LATEX_DIR_NAME, ""); // creates the dir + file_name char*
    char* local_dir = cat_file_directory(file_dir_name, "./", ""); // makes if local (cats './')
    
    FILE* tex_file_ptr = fopen(local_dir, "a+");
    if(tex_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_LATEX_FILE)
        return ERR_OPEN_LATEX_FILE;
    }

    int err_code = add_final_diff_latex(node_ptr);
    if(err_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, err_code)
        return err_code;
    }

    fprintf(tex_file_ptr, "\\end{document}\n");

    if(fclose(tex_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_LATEX_FILE)
        return ERR_CLOSE_LATEX_FILE;
    }
    convert_tex_pdf(file_dir_name);
    free(file_dir_name);
    free(local_dir);

    return RETURN_OK;
}

int add_equation_diff_latex(Node* node_ptr) // ok
{
    char* file_dir_name = cat_file_directory((char*)LATEX_FILE_NAME, (char*)LATEX_DIR_NAME, ""); // creates the dir + file_name char*
    char* local_dir = cat_file_directory(file_dir_name, "./", ""); // makes if local (cats './')

    FILE* tex_file_ptr = fopen(local_dir, "a+");
    if(tex_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_LATEX_FILE)
        return ERR_OPEN_LATEX_FILE;
    }
    fprintf(tex_file_ptr, " It was so \\textbf{easy} for me to take this differential:\n ");
    fprintf(tex_file_ptr, " \\begin{center}\n ");
    fprintf(tex_file_ptr, " y^{\\text{'}} = ");

    int err_code = print_latex_eq(node_ptr, tex_file_ptr, EQ_FOR_LATEX, nullptr, nullptr);
    if(err_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, err_code)
        return err_code;
    }

    fprintf(tex_file_ptr, "\\\\");
    fprintf(tex_file_ptr, " \\end{center}\n ");

    if(fclose(tex_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_LATEX_FILE)
        return ERR_CLOSE_LATEX_FILE;
    }

    free(file_dir_name);
    free(local_dir);

    return RETURN_OK;
}

int print_latex_eq(Node* node_ptr, FILE* tex_file_ptr, int key, char* var_name, Tree* tree_ptr) // ok
{
    int err_code = RETURN_OK;

    if(node_ptr == nullptr)
    {
        return 0;
    }
    else if(node_ptr->type == IS_VAL) // ok
    {
        int flag_brack = check_is_positive(node_ptr->value.node_value);  // to print brack around the negative value

        if(flag_brack == IS_NEGATIVE)
        {
            fprintf(tex_file_ptr, "(");
        }
        int tail_length = 0;

        char double_as_char[50];
        sprintf(double_as_char, "%f", node_ptr->value.node_value); // writes float value to the temp buffer in order to convert into string

        size_t full_length = length_double(double_as_char);
        char* dot_pos = strchr((char*)double_as_char, '.');

        if(dot_pos == nullptr)
        {
            fprintf(tex_file_ptr, "%d", atoi(double_as_char));   
        }
        else
        {
            tail_length = full_length - (dot_pos - (char*)double_as_char) - 1; // in order to print in correct precision
            fprintf(tex_file_ptr, "%.*f", tail_length, node_ptr->value.node_value); 
        }
        
        if(flag_brack == IS_NEGATIVE)
        {
            fprintf(tex_file_ptr, ")");
        }
    }
    else if(node_ptr->type == IS_VARIB || node_ptr->type == IS_CNST_VAR)
    {
        if(key == EQ_FOR_LATEX)
        {
            fprintf(tex_file_ptr, "%s", node_ptr->value.text);
        }
        else
        {
            if(strcmp(var_name, node_ptr->value.text) == 0)
            {
                fprintf(tex_file_ptr, "x");
            }
            else
            {   // The code below prints the value of const variables in order to generate plot
                for(size_t cur_var = 0; cur_var < tree_ptr->num_of_vars; cur_var++)
                {
                    if(strcmp(tree_ptr->vars_arr[cur_var].var_text, node_ptr->value.text) == 0)
                    {
                        int flag_brack = check_is_positive(tree_ptr->vars_arr[cur_var].var_value);  // to print brack around the negative value

                        if(flag_brack == IS_NEGATIVE)
                        {
                            fprintf(tex_file_ptr, "(");
                        }
                        int tail_length = 0;

                        char double_as_char[50];
                        sprintf(double_as_char, "%f", tree_ptr->vars_arr[cur_var].var_value);

                        size_t full_length = length_double(double_as_char);
                        char* dot_pos = strchr((char*)double_as_char, '.');

                        if(dot_pos == nullptr)
                        {
                            fprintf(tex_file_ptr, "%d", atoi(double_as_char));   
                        }
                        else
                        {
                            tail_length = full_length - (dot_pos - (char*)double_as_char) - 1;
                            fprintf(tex_file_ptr, "%.*f", tail_length, tree_ptr->vars_arr[cur_var].var_value); 
                        }
                        
                        if(flag_brack == IS_NEGATIVE)
                        {
                            fprintf(tex_file_ptr, ")");
                        }
                        break;
                    }
                }
            }
        }
    }
    else if(node_ptr->type == IS_OP)
    {
        switch(node_ptr->value.op_number)
        {
            case Mul:
                {
                    if(node_ptr->left_child->type == IS_OP && (node_ptr->left_child->value.op_number == Sub || node_ptr->left_child->value.op_number == Add))
                    {
                        fprintf(tex_file_ptr, "(");
                    }

                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    if(node_ptr->left_child->type == IS_OP && (node_ptr->left_child->value.op_number == Sub || node_ptr->left_child->value.op_number == Add))
                    {
                        fprintf(tex_file_ptr, ")");
                    }

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, " \\cdot ");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "*");
                    }

                    if(node_ptr->right_child->type == IS_OP && (node_ptr->right_child->value.op_number == Sub || node_ptr->right_child->value.op_number == Add))
                    {
                        fprintf(tex_file_ptr, "(");
                    }
                    
                    err_code = print_latex_eq(node_ptr->right_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    if(node_ptr->right_child->type == IS_OP && (node_ptr->right_child->value.op_number == Sub || node_ptr->right_child->value.op_number == Add))
                    {
                        fprintf(tex_file_ptr, ")");
                    }

                    break;
                }
            case Add:
                {
                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    fprintf(tex_file_ptr, " + ");

                    err_code = print_latex_eq(node_ptr->right_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }
                    break;
                }
            case Sub:
                {
                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    fprintf(tex_file_ptr, " - ");
                    err_code = print_latex_eq(node_ptr->right_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }
                    break;
                }
            case Div:
                {
                    fprintf(tex_file_ptr, "(");
                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }
                    fprintf(tex_file_ptr, ")");

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, " \\cdot ");

                        fprintf(tex_file_ptr, "(");
                        err_code = print_latex_eq(node_ptr->right_child, tex_file_ptr, key, var_name, tree_ptr);
                        if(err_code != RETURN_OK)
                        {
                            ERROR_MESSAGE(stderr, err_code)
                            return err_code;
                        }
                        fprintf(tex_file_ptr, ")^{-1}");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "*");

                        fprintf(tex_file_ptr, "(");
                        err_code = print_latex_eq(node_ptr->right_child, tex_file_ptr, key, var_name, tree_ptr);
                        if(err_code != RETURN_OK)
                        {
                            ERROR_MESSAGE(stderr, err_code)
                            return err_code;
                        }
                        fprintf(tex_file_ptr, ")^(-1)");
                    }
                    break;
                }
            default:
                fprintf(tex_file_ptr, "ERR_UNKNOWN_OPERATOR");
                ERROR_MESSAGE(stderr, ERR_UNKNOWN_OPERATOR)
                break;
        }
    }
    else 
    {
        switch(node_ptr->value.op_number)
        {
            case Sin:
                {
                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "sin(");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "sin(deg(");
                    }

                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, ")");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "))");
                    }
                    break;
                }
            case Cos:
                {
                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "cos(");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "cos(deg(");
                    }

                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, ")");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "))");
                    }
                    break;
                }
            case Tan:
                {
                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "tan(");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "tan(deg(");
                    }

                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, ")");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "))");
                    }
                    break;
                }
            case Asin:
                {
                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "asin(");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "(deg(asin(");
                    }

                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, ")");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, ")))");
                    }
                    break;
                }
            case Acos:
                {
                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "acos(");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "(deg(acos(");
                    }

                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, ")");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, ")))");
                    }
                    break;
                }
            case Sqrt:
                {
                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "\\sqrt{");
                        err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                        if(err_code != RETURN_OK)
                        {
                            ERROR_MESSAGE(stderr, err_code)
                            return err_code;
                        }
                        fprintf(tex_file_ptr, "}");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "(");
                        err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                        if(err_code != RETURN_OK)
                        {
                            ERROR_MESSAGE(stderr, err_code)
                            return err_code;
                        }
                        fprintf(tex_file_ptr, ")^(0.5)");
                    }
                    break;
                }
            case Exp:
                {
                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "\\exp(");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "e^(");
                    }

                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Log:
                {
                    fprintf(tex_file_ptr, "ln(");
                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Cot:
                {
                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "cot(");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "cot(deg(");
                    }

                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, ")");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "))");
                    }
                    break;
                }
            case Pow:
                {
                    fprintf(tex_file_ptr, "(");
                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }
                    fprintf(tex_file_ptr, ")");

                    if(key == EQ_FOR_LATEX)
                    {
                        fprintf(tex_file_ptr, "\\textsuperscript{$\\wedge$}(");
                    }
                    else
                    {
                        fprintf(tex_file_ptr, "^(");
                    }
                    err_code = print_latex_eq(node_ptr->left_child, tex_file_ptr, key, var_name, tree_ptr);
                    if(err_code != RETURN_OK)
                    {
                        ERROR_MESSAGE(stderr, err_code)
                        return err_code;
                    }
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            default:
                fprintf(tex_file_ptr, "ERR_UNKNOWN_FUNC");
                ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
                return ERR_UNKNOWN_FUNC;
            }
    }

    return RETURN_OK;
}

int convert_tex_pdf(char* file_dir_name) // ok
{
    char  final_cmd[100];
    char* compile_cmd = "pdflatex -output-directory=../differentiator/latex/ ../differentiator/";
    strcpy(final_cmd, compile_cmd);
    strcat(final_cmd, file_dir_name);
    final_cmd[99] = '\0';
    printf(final_cmd);
    system(final_cmd);

    return RETURN_OK;
}

int add_preamble_latex(Node* node_ptr) // ok
{
    char* file_dir_name = cat_file_directory((char*)LATEX_FILE_NAME, (char*)LATEX_DIR_NAME, ""); // creates the dir + file_name char*
    char* local_dir = cat_file_directory(file_dir_name, "./", ""); // makes if local (cats './')

    FILE* tex_file_ptr = fopen(local_dir, "a+");
    if(tex_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_LATEX_FILE)
        return ERR_OPEN_LATEX_FILE;
    }

    if(latex_equation_num == 0)
    {
        fprintf(tex_file_ptr, " \\section*{Our main function}\n");
        fprintf(tex_file_ptr, "Now I will try to differentiate this equation:\n ");
        latex_equation_num++;
    }
    else
    {
        fprintf(tex_file_ptr, " \\section*{Differentiation of equation №%d}\n ", latex_equation_num);
        fprintf(tex_file_ptr, " So, now we have equation: \n ");
        latex_equation_num++;
    }

    fprintf(tex_file_ptr, " \\begin{center}\n ");
    fprintf(tex_file_ptr, " y = ");

    int err_code = print_latex_eq(node_ptr, tex_file_ptr, EQ_FOR_LATEX, nullptr, nullptr);
    if(err_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, err_code)
        return err_code;
    }

    fprintf(tex_file_ptr, "\\\\");
    fprintf(tex_file_ptr, " \\end{center}\\\\\n ");

    if(fclose(tex_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_LATEX_FILE)
        return ERR_CLOSE_LATEX_FILE;
    }

    free(file_dir_name);
    free(local_dir);

    return RETURN_OK;
}

int add_final_diff_latex(Node* node_ptr) // ok
{
    char* file_dir_name = cat_file_directory((char*)LATEX_FILE_NAME, (char*)LATEX_DIR_NAME, ""); // creates the dir + file_name char*
    char* local_dir = cat_file_directory(file_dir_name, "./", ""); // makes if local (cats './')

    FILE* tex_file_ptr = fopen(local_dir, "a+");
    if(tex_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_LATEX_FILE)
        return ERR_OPEN_LATEX_FILE;
    }

    fprintf(tex_file_ptr, " \\section*{Full differential of our equation}\n ");
    fprintf(tex_file_ptr, " Finally... I took it: \n ");

    fprintf(tex_file_ptr, " \\begin{center}\n ");
    fprintf(tex_file_ptr, " y^{\\text{'}} = ");

    int err_code = print_latex_eq(node_ptr, tex_file_ptr, EQ_FOR_LATEX, nullptr, nullptr);
    if(err_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, err_code)
        return err_code;
    }

    fprintf(tex_file_ptr, "\\\\");
    fprintf(tex_file_ptr, " \\end{center}\\\\\n ");

    if(fclose(tex_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_LATEX_FILE)
        return ERR_CLOSE_LATEX_FILE;
    }

    free(file_dir_name);
    free(local_dir);

    return RETURN_OK;
}

int print_plot_latex(Node* node_ptr, char* var_name, Tree* tree_ptr) // ok
{
    char* file_dir_name = cat_file_directory((char*)LATEX_FILE_NAME, (char*)LATEX_DIR_NAME, ""); // creates the dir + file_name char*
    char* local_dir = cat_file_directory(file_dir_name, "./", ""); // makes if local (cats './')

    FILE* tex_file_ptr = fopen(local_dir, "a+");
    if(tex_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_LATEX_FILE)
        return ERR_OPEN_LATEX_FILE;
    }

    fprintf(tex_file_ptr, " \\begin{tikzpicture}\n ");
    fprintf(tex_file_ptr, " \\begin{axis}[\n ");

    fprintf(tex_file_ptr, " title={Derivative of equation by var - '%s'},\n", var_name);
    fprintf(tex_file_ptr, " ymin = -10, ymax = 10,\n ");
    fprintf(tex_file_ptr, " xmin = -10, xmax = 10,\n ");
    fprintf(tex_file_ptr, " xlabel={x},\n ");
    fprintf(tex_file_ptr, " ylabel={y},\n ");
    fprintf(tex_file_ptr, " major grid style = {lightgray},\n ");
    fprintf(tex_file_ptr, " minor grid style = {lightgray!25},\n ");
    fprintf(tex_file_ptr, " legend pos = north west,\n ");
    fprintf(tex_file_ptr, " grid= major,\n ");
    fprintf(tex_file_ptr, " scale mode = auto]\n ");
    fprintf(tex_file_ptr, " \\addplot [samples=60,red, smooth] {");

    int err_code = print_latex_eq(node_ptr, tex_file_ptr, EQ_FOR_PLOT, var_name, tree_ptr);
    if(err_code != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, err_code)
        return err_code;
    }
    fprintf(tex_file_ptr, "};\n ");

    fprintf(tex_file_ptr, " \\end{axis}\n ");
    fprintf(tex_file_ptr, " \\end{tikzpicture}\n ");

    if(fclose(tex_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_LATEX_FILE)
        return ERR_CLOSE_LATEX_FILE;
    }

    free(file_dir_name);
    free(local_dir);

    return RETURN_OK;
}
