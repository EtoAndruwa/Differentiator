#include "Differentiator.h"

int print_header(FILE* tex_file_ptr)
{
    fprintf(tex_file_ptr, "\\documentclass[a4paper, 10pt]{article}\n");
    fprintf(tex_file_ptr, "\\begin{document}\n");
    return RETURN_OK;
}

int print_footer(FILE* tex_file_ptr)
{
    fprintf(tex_file_ptr, "\\end{document}\n");
    return RETURN_OK;
}

int create_latex(Node* root_node_ptr)
{
    char* file_dir_name = cat_file_directory((char*)LATEX_FILE_NAME, (char*)LATEX_DIR_NAME, "");
    char* local_dir = cat_file_directory(file_dir_name, "./", "");


    FILE* tex_file_ptr = fopen(local_dir, "w");
    if(tex_file_ptr == nullptr)
    {
        ERROR_MESSAGE(stderr, ERR_OPEN_LATEX_FILE)
        return ERR_OPEN_LATEX_FILE;
    }

    if(print_header(tex_file_ptr) != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, ERR_PRINT_LATEX)
        return ERR_PRINT_LATEX;
    }

    add_equation(root_node_ptr, tex_file_ptr);

    if(print_footer(tex_file_ptr) != RETURN_OK)
    {
        ERROR_MESSAGE(stderr, ERR_PRINT_LATEX)
        return ERR_PRINT_LATEX; 
    }

    if(fclose(tex_file_ptr) == EOF)
    {
        ERROR_MESSAGE(stderr, ERR_CLOSE_LATEX_FILE)
        return ERR_CLOSE_LATEX_FILE;
    }

    convert_tex_pdf(file_dir_name);
    free(file_dir_name);
    free(local_dir);
}

int add_equation(Node* node_ptr, FILE* tex_file_ptr)
{
    fprintf(tex_file_ptr, "\\begin{dmath}\n");
    fprintf(tex_file_ptr, "f(x) = ");
    print_latex_eq(node_ptr, tex_file_ptr);
    fprintf(tex_file_ptr, "\\end{dmath}\n");
}

int print_latex_eq(Node* node_ptr, FILE* tex_file_ptr)
{
    if(node_ptr == nullptr)
    {
        return 0;
    }
    else if(node_ptr->type == IS_VAL) // ok
    {
        int flag_brack = is_positive(node_ptr->value.node_value);  // to print brack around the negative value

        if(flag_brack == IS_NEGATIVE)
        {
            fprintf(tex_file_ptr, "(");
        }
        int tail_length = 0;

        char double_as_char[50];
        sprintf(double_as_char, "%f", node_ptr->value.node_value);

        size_t full_length = length_double(double_as_char);
        char* dot_pos = strchr((char*)double_as_char, '.');

        if(dot_pos == nullptr)
        {
            fprintf(tex_file_ptr, "%d", atoi(double_as_char));   
        }
        else
        {
            tail_length = full_length - (dot_pos - (char*)double_as_char) - 1;
            fprintf(tex_file_ptr, "%.*f", tail_length, node_ptr->value.node_value); 
        }
        
        if(flag_brack == IS_NEGATIVE)
        {
            fprintf(tex_file_ptr, ")");
        }
    }
    else if(node_ptr->type == IS_VARIB || node_ptr->type == IS_CNST_VAR)
    {
        fprintf(tex_file_ptr, "%s", node_ptr->value.text);
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
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                if(node_ptr->left_child->type == IS_OP && (node_ptr->left_child->value.op_number == Sub || node_ptr->left_child->value.op_number == Add))
                {
                    fprintf(tex_file_ptr, ")");
                }
                fprintf(tex_file_ptr, " \\cdot ");

                if(node_ptr->right_child->type == IS_OP && (node_ptr->right_child->value.op_number == Sub || node_ptr->right_child->value.op_number == Add))
                {
                    fprintf(tex_file_ptr, "(");
                }
                print_latex_eq(node_ptr->right_child, tex_file_ptr);
                if(node_ptr->right_child->type == IS_OP && (node_ptr->right_child->value.op_number == Sub || node_ptr->right_child->value.op_number == Add))
                {
                    fprintf(tex_file_ptr, ")");
                }

                break;
            }
        case Add:
            {
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, " + ");
                print_latex_eq(node_ptr->right_child, tex_file_ptr);
                break;
            }
        case Sub:
            {
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, " - ");
                print_latex_eq(node_ptr->right_child, tex_file_ptr);
                break;
            }
        case Div:
            {
                fprintf(tex_file_ptr, "(");
                print_latex_eq(node_ptr->left_child, tex_file_ptr);
                fprintf(tex_file_ptr, ")");

                fprintf(tex_file_ptr, "\\cdot ");

                fprintf(tex_file_ptr, "(");
                print_latex_eq(node_ptr->right_child, tex_file_ptr);
                fprintf(tex_file_ptr, ")^{-1}");
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
                    fprintf(tex_file_ptr, "sin(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Cos:
                {
                    fprintf(tex_file_ptr, "cos(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Tan:
                {
                    fprintf(tex_file_ptr, "tan(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Asin:
                {
                    fprintf(tex_file_ptr, "asin(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Acos:
                {
                    fprintf(tex_file_ptr, "acos(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Sqrt:
                {
                    fprintf(tex_file_ptr, "\\sqrt{");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, "}");
                    break;
                }
            case Exp:
                {
                    fprintf(tex_file_ptr, "\\exp(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Log:
                {
                    fprintf(tex_file_ptr, "ln(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Cot:
                {
                    fprintf(tex_file_ptr, "cot(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            case Pow:
                {
                    fprintf(tex_file_ptr, "(");
                    print_latex_eq(node_ptr->left_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");

                    fprintf(tex_file_ptr, " \\textsuperscript{$\\wedge$} (");
                    print_latex_eq(node_ptr->right_child, tex_file_ptr);
                    fprintf(tex_file_ptr, ")");
                    break;
                }
            default:
                fprintf(tex_file_ptr, "ERR_UNKNOWN_FUNC");
                ERROR_MESSAGE(stderr, ERR_UNKNOWN_FUNC)
                break;
            }
    }
}

int convert_tex_pdf(char* file_dir_name)
{
    char  final_cmd[100];
    char* compile_cmd = "pdflatex -output-directory=../differentiator/latex/ ../differentiator/";
    strcpy(final_cmd, compile_cmd);
    strcat(final_cmd, file_dir_name);
    final_cmd[99] = '\0';
    printf(final_cmd);
    system(final_cmd);
}


