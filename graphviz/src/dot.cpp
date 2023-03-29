#include "graphviz.h"

static size_t number_of_images = 0; // global variable in order to count the number of jpg files

size_t graph_start(char* file_name) // writes the start of the .txt file
{
    FILE* graph_txt = fopen(file_name, "w");
    if(graph_txt == nullptr)
    {
        printf("ERROR_TO_OPEN");
        return ERR_TO_OPEN_GRAPH_TXT;
    }

    fprintf(graph_txt, "\n");
    fprintf(graph_txt, "digraph\n");
    fprintf(graph_txt, "{\n");
    fprintf(graph_txt, "\trankdir=%s;\n", RANKDIR_DOT);
    fprintf(graph_txt, "\tsplines=%s;\n", SPLINES_DOT);
    // fprintf(graph_txt, "\tlayout=\"dot\"\n");

    if(fclose(graph_txt) == EOF)
    {
        return ERR_TO_CLOSE_GRAPH_TXT;
    }
}

size_t graph_end(char* file_name) // writes the end of the .txt file
{
    FILE* graph_txt = fopen(file_name, "a+");
    if(graph_txt == nullptr)
    {
        return ERR_TO_OPEN_GRAPH_TXT;
    }

    fprintf(graph_txt, "}\n");

    if(fclose(graph_txt) == EOF)
    {
        return ERR_TO_CLOSE_GRAPH_TXT;
    }
}

size_t html_end(char* file_name) // writes the end of the html file
{
    FILE* graph_txt = fopen(file_name, "a+");
    if(graph_txt == nullptr)
    {
        ERROR_MESSAGE(stderr, 1)
    }
    fprintf(graph_txt, "</pre></html>\n");

    if(fclose(graph_txt) == EOF)
    {

    }
}

size_t hmtl_start(char* file_name) // writes the start of the html file
{
    FILE* graph_txt = fopen(file_name, "w");
    if(graph_txt == nullptr)
    {

    }
    fprintf(graph_txt, "<!DOCTYPE HTML><pre>\n");

    if(fclose(graph_txt) == EOF)
    {

    }
}

size_t print_tree_data(Tree* tree_struct, Node* node_ptr, const char* file_name)
{
    FILE* graph_txt = fopen(file_name, "a+");
    if(graph_txt == nullptr)
    {
        tree_struct->error_code = ERR_TO_OPEN_GRAPH_TXT;
        // tree_dtor(tree_struct);
        return ERR_TO_OPEN_GRAPH_TXT;
    }

    if(node_ptr != nullptr)
    {  
        if(node_ptr->type == IS_VAL)
        {
            fprintf(graph_txt, "\tnode_%p[shape = Mrecord, style=\"filled\" fillcolor=\"%s\", label =\" { <f0> left_child = %p } | {{<here> type = VALUE} | { value = %f \\n }} | { <f1> right_child = %p } \"];\n", node_ptr, RED_BG_COLOR_DOT, node_ptr->left_child, node_ptr->value.node_value, node_ptr->right_child);
        }
        else if(node_ptr->type == IS_OP)
        {
            fprintf(graph_txt, "\tnode_%p[shape = Mrecord, style=\"filled\" fillcolor=\"%s\", label =\" { <f0> left_child = %p } | {{<here> type = OPERATOR} | {value = %c \\n}} | { <f1> right_child = %p } \"];\n", node_ptr, BLUE_BG_COLOR_DOT, node_ptr->left_child, node_ptr->value.op_number, node_ptr->right_child);
        }
        else if(node_ptr->type == IS_FUNC)
        {
            char* str_value = nullptr;

            #define DEF_FUNC(name, ...) if(node_ptr->value.op_number == name){fprintf(graph_txt, "\tnode_%p[shape = Mrecord, style=\"filled\" fillcolor=\"%s\", label =\" { <f0> left_child = %p } | {{<here> type = FUNCTION} | {value = %s \\n}} | { <f1> right_child = %p } \"];\n", node_ptr, PURP_BG_COLOR_DOT, node_ptr->left_child, #name, node_ptr->right_child);}
            #define DEF_OP(...)
            #include "../../differentiator/src/def_cmd.h"
            #undef DEF_OP
            #undef DEF_FUNC

        }
        else
        {
            fprintf(graph_txt, "\tnode_%p[shape = Mrecord, style=\"filled\" fillcolor=\"%s\", label =\" { <f0> left_child = %p } | {{<here> type = VARIABLE} | {value = %s \\n}} | { <f1> right_child = %p } \"];\n", node_ptr, ORAN_BG_COLOR_DOT, node_ptr->left_child, node_ptr->value.text, node_ptr->right_child);
        }
        
        
        if(node_ptr->left_child != nullptr)
        {
            print_tree_data(tree_struct, node_ptr->left_child, file_name);
        }
        if(node_ptr->right_child != nullptr)
        {
            print_tree_data(tree_struct, node_ptr->right_child, file_name);
        }
    }

    if(fclose(graph_txt) == EOF)
    {
        tree_struct->error_code = ERR_TO_CLOSE_GRAPH_TXT;
        // tree_dtor(tree_struct);
        return ERR_TO_CLOSE_GRAPH_TXT;
    }
}

size_t print_tree_links(Tree* tree_struct, Node* node_ptr, const char* file_name)
{
    FILE* graph_txt = fopen(file_name, "a+");
    if(graph_txt == nullptr)
    {
        tree_struct->error_code = ERR_TO_OPEN_GRAPH_TXT;
        // tree_dtor(tree_struct);
        return ERR_TO_OPEN_GRAPH_TXT;
    }

    if(node_ptr->left_child != nullptr)
    {
        fprintf(graph_txt, "\tnode_%p:f0 -> node_%p:here[color=\"blue\", label = \"left_child\"];\n", node_ptr, node_ptr->left_child);
        print_tree_links(tree_struct, node_ptr->left_child, file_name);
    }
    if(node_ptr->right_child != nullptr)
    {
        fprintf(graph_txt, "\tnode_%p:f1 -> node_%p:here[color=\"red\", label = \"right_child\"];\n", node_ptr, node_ptr->right_child);
        print_tree_links(tree_struct, node_ptr->right_child, file_name);
    }

    if(fclose(graph_txt) == EOF)
    {
        tree_struct->error_code = ERR_TO_CLOSE_GRAPH_TXT;
        // tree_dtor(tree_struct);
        return ERR_TO_CLOSE_GRAPH_TXT;
    }
}

size_t create_graph_jpg(Tree* tree_struct, char* legend) // prints all data about the list into the .txt file
{
    char int_str_equivalent[11] = {};
    sprintf(int_str_equivalent, "%d", number_of_images);
    char* file_name = cat_file_directory(OUTPUT_NAME, "", int_str_equivalent);
    char* dir_file_name = cat_file_directory(file_name, TXT_FOLDER, INPUT_FORMAT);

    graph_start(dir_file_name);
    print_legend(legend, dir_file_name);
    print_tree_data(tree_struct, tree_struct->root, dir_file_name);
    print_tree_links(tree_struct, tree_struct->root, dir_file_name);
    graph_end(dir_file_name);

    system_dot(file_name);
    free(dir_file_name);
    dir_file_name = nullptr;
    free(file_name);
    file_name = nullptr;

    free(legend);
    legend = nullptr;
    number_of_images++;

    return 0;
}

char* cat_file_directory(char* file_name, char* dir, char* format) // cats the file name, dir path and extension into the one string
{
    size_t size_of_file_name = strlen(file_name) + 1;
    size_t size_of_dir_name = strlen(dir) + 1;
    size_t size_of_input_format = strlen(dir) + 1;

    char* dir_file_name = (char*)calloc(size_of_dir_name + size_of_file_name + size_of_input_format, sizeof(char));

    if(dir_file_name == nullptr)
    {

    }

    strcpy(dir_file_name, dir);
    strcat(dir_file_name, file_name);
    strcat(dir_file_name, format);

    return dir_file_name;
}

size_t create_html(char* file_name) // creates the whole html file at once
{
    char* dir_file_name = cat_file_directory(file_name, DIR_TO_DUMPS, HTML);

    hmtl_start(dir_file_name);
    add_image_to_html(dir_file_name);
    html_end(dir_file_name);

    free(dir_file_name);
    dir_file_name = nullptr;

    return 0;
}

char* system_dot(char* file_name) // creates a dot function in order to call it automatically
{
    char* dot  = " dot ";
    char* flag = " -o ";

    char* txt_file = cat_file_directory(file_name, TXT_FOLDER, INPUT_FORMAT);
    char* jpg_file = cat_file_directory(file_name, IMAGE_FOLDER, OUTPUT_FORMAT);

    size_t size_of_dot = strlen(dot) + 1;
    size_t size_of_txt_file = strlen(txt_file) + 1;
    size_t size_of_jpg_file = strlen(jpg_file) + 1;
    size_t size_of_flag = strlen(flag) + 1;
    size_t size_of_output_flag = strlen(OUTPUT_FORMAT_FLAG) + 1;

    size_t total_lenght = size_of_dot + size_of_txt_file + size_of_jpg_file + size_of_flag + size_of_output_flag;

    char* system_cmd = (char*)calloc(total_lenght , sizeof(char));

    strcpy(system_cmd, dot);


    strcat(system_cmd, txt_file);
    strcat(system_cmd, OUTPUT_FORMAT_FLAG);
    strcat(system_cmd, flag);
    strcat(system_cmd, jpg_file);

    system(system_cmd);

    free(txt_file);
    free(jpg_file);
    free(system_cmd);


    char* file_with_ext = cat_file_directory(file_name, "", OUTPUT_FORMAT);

    add_to_image_list(file_with_ext);

    free(file_with_ext);
    file_with_ext = nullptr;
    txt_file = nullptr;
    jpg_file = nullptr;
    system_cmd = nullptr;
}

size_t add_to_image_list(char* file_name) // adds the path to the image into the image_list.file
{
    char* dir_file_name = cat_file_directory("image_list.txt", DIR_TO_DUMPS, "");
    char* path_to_write = cat_file_directory(file_name, "../graph_dumps/images/", "");

    FILE* graph_txt = fopen(dir_file_name, "a+");
    if(graph_txt == nullptr)
    {
        ERROR_MESSAGE(stderr, 1);
    }

    fprintf(graph_txt, "%s\n", path_to_write);

    if(fclose(graph_txt) == EOF)
    {
        ERROR_MESSAGE(stderr, 1);
    }

    free(dir_file_name);
    dir_file_name = nullptr;
    free(path_to_write);
    path_to_write = nullptr;
}

size_t add_image_to_html(char* dir_file_name) // adds all images to the html at once
{
    FILE* graph_txt = fopen(dir_file_name, "a+");
    if(graph_txt == nullptr)
    {
        ERROR_MESSAGE(stderr, 1);
        return 1;
    }

    get_size_file();
    char* buffer = get_tokens_into_buf();
    get_tokens(buffer, graph_txt);


    if(fclose(graph_txt) == EOF)
    {
        ERROR_MESSAGE(stderr, 1);
        return 1;
    }
}

char* get_tokens_into_buf() // reads all tokens into the buffer
{
    char* dir_file_name = cat_file_directory("image_list.txt", DIR_TO_DUMPS, "");
    FILE* image_list = fopen(dir_file_name, "r");
    if(image_list == nullptr)
    {
        ERROR_MESSAGE(stderr, 1);
    }
    size_t size_of_file = get_size_file();

    char* buffer_ptr = (char*)calloc(1, sizeof(char) * (size_of_file + 1));

    fread(buffer_ptr, sizeof(char), size_of_file, image_list);

    buffer_ptr[size_of_file] = '\0'; // Makes form the file_struct_ptr null-terminated string

    if(fclose(image_list) == EOF)
    {
        ERROR_MESSAGE(stderr, 1);
    }

    free(dir_file_name);
    dir_file_name = nullptr;

    return buffer_ptr;
}

size_t get_size_file() // gets the size of the file
{
    char* dir_file_name = cat_file_directory("image_list.txt", DIR_TO_DUMPS, "");
    FILE* image_list = fopen(dir_file_name, "r");
    if(image_list == nullptr)
    {
        ERROR_MESSAGE(stderr, 1);
    }

    fseek(image_list, 0, SEEK_END); // Puts the pointer inside the file_struct_ptr to the end of it
    size_t size = ftell(image_list); // Get the size of the file_struct_ptr with '\r'!

    if(fclose(image_list) == EOF)
    {
        ERROR_MESSAGE(stderr, 1);
    }

    free(dir_file_name);
    dir_file_name = nullptr;

    return size;
}

size_t get_tokens(char* buffer, FILE* file_tpr) // prints all the tokens into the file (the path to the image)
{
    char* token = strtok(buffer," \n\r");

    while (token != NULL)
    {
        if(strstr(token, "logical") == nullptr)
        {
            fprintf(file_tpr, "<div style =\"text-align: center;\"><h1>The image below %s</h1>\n", strstr(token, OUTPUT_NAME));
            fprintf(file_tpr, "<img src=\"%s\" alt=\"%s \"></div>\n", token, token);
        }
        token = strtok(NULL, " \n\r");
    }

    free(buffer);
    buffer = nullptr;
}

size_t print_legend(char* legend_text, char* dir_file_name)
{
    FILE* graph_txt = fopen(dir_file_name, "a+");
    if(graph_txt == nullptr)
    {
        return ERR_TO_OPEN_GRAPH_TXT;
    }

    if(strcmp(legend_text, DEFAULT_LEGEND_TEXT) == 0)
    {
        fprintf(graph_txt, "\tnode_legend[shape = Mrecord, label = \"%s\"];\n", DEFAULT_LEGEND_TEXT);
    }
    else
    {
        fprintf(graph_txt, "\tnode_legend[shape=Mrecord , label = \"%s\"];\n", legend_text);
    }


    if(fclose(graph_txt) == EOF)
    {
        return ERR_TO_CLOSE_GRAPH_TXT;
    }
}

char* create_legend(const char* func_name, int first_node_id, int first_node_val, int second_node_id, int second_node_val)
{
    char* legend = (char*)calloc(100, sizeof(char));;

    if(strcmp(func_name, "push_after") == 0)
    {
        sprintf(legend, "Node_%d with value = %d was pushed after the Node_%d with value %d", first_node_id, first_node_val, second_node_id, second_node_val);
    }
    else if(strcmp(func_name, "delete_node") == 0)
    {
        sprintf(legend, "Node_%d with value = %d was deleted from the list", first_node_id, first_node_val);
    }
    else if(strcmp(func_name, "push_before") == 0)
    {
        sprintf(legend, "Node_%d with value = %d was pushed before the Node_%d with value %d", first_node_id, first_node_val, second_node_id, second_node_val);
    }
    else if(strcmp(func_name, "exchange_neighbor") == 0)
    {
        sprintf(legend, "Node_%d with value = %d was exchanged with the NEIGHBOR Node_%d with value %d", first_node_id, first_node_val, second_node_id, second_node_val);
    }
    else if(strcmp(func_name, "exchange_stranger") == 0)
    {
        sprintf(legend, "Node_%d with value = %d was exchanged with the STRANGER Node_%d with value %d", first_node_id, first_node_val, second_node_id, second_node_val);
    }
    else
    {
        sprintf(legend, "NEW COMMAND WAS PERFORMED");
    }

    return legend;
}

