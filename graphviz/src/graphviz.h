#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

/*####################################################################################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*####################################################################################################################################################################*/

#include "../config/dot_config.h"                     // The config information
#include "../../differentiator/src/Differentiator.h"  // The differentiator header
#include "../../differentiator/src/tree.h"            // The tree header
#include "debugger.h"                                 // The debug macro

/*####################################################################################################################################################################*/


/**
 * @brief This enum contains all error codes of my graphviz lib 
 * (Enums are negative inorder to prevent funcitons' return's confusion)
 */
enum error_codes_dot
{
    RETURN_OK              =  0,
    ERR_TO_OPEN_GRAPH_TXT  = -1,
    ERR_TO_CLOSE_GRAPH_TXT = -2, 
    ERR_TO_OPEN_HTML       = -3,
    ERR_TO_CLOSE_HTML      = -4,
    ERR_TO_CALLOC_LEGEND   = -5,
    ERR_STRTOK_NULL_PTR    = -6,
    ERR_TO_OPEN_IMAGE_LST  = -7,
    ERR_TO_CLOSE_IMAGE_LST = -8,
    ERR_TO_CALLOC_BUFFER   = -9, 
    ERR_FSEEK_TO_START     = -10,
    ERR_CALLOC_DIR_FIL_NAM = -11,
    ERR_TO_READ_FILE       = -12,
    ERR_EMPTY_FILE         = -13,
    ERR_FILE_SIZE_PROBLEM  = -14,
    ERR_EMPTY_BUFFER       = -15,
    ERR_TO_FPRINTF_TO_TXT  = -16,
    ERR_TO_CAT_TXT         = -17,
    ERR_TO_CAT_JPG         = -18,
    ERR_TO_SYSTEM_CMD      = -19,
    ERR_TO_CAT_FILE_EXT    = -20,
    ERR_TO_SPRINTF_BUFF    = -21,
    ERR_MAKE_NEW_FILE_NAME = -22,
    ERR_MAKE_DIR_NEW_FILE  = -23,
    ERR_PATH_OF_IMAGE      = -24,
    ERR_PATH_TO_LST_FILE   = -25,
};

/*####################################################################################################################################################################*/

/**
 * @brief Adds the name of the image with the path to the image list
 * 
 * @param file_name The pointer to the name of the file
 * @return Returns the error code 
 */
int add_to_image_list(char* file_name);
/*####################################################################################################################################################################*/

/**
 * @brief Creates the images from the Tree 
 * 
 * @param tree_struct The pointer to the struct of the Tree
 * @param legend The pointer to the image's legend text
 * @return Returns the error code 
 */
int create_graph_jpg(Tree* tree_struct, char* legend); // prints all data about the list into the .txt file
/*####################################################################################################################################################################*/

/**
 * @brief Prints the start of the graph statement on the dot language
 * 
 * @param graph_txt_ptr The pointer to the file
 * @return Returns the error code  
 */
int graph_start(FILE* graph_txt_ptr);
/*####################################################################################################################################################################*/

/**
 * @brief Prints the end of the graph statement on the dot language
 * 
 * @param graph_txt_ptr The pointer to the file 
 * @return Returns the error code  
 */
int graph_end(FILE* graph_txt_ptr);
/*####################################################################################################################################################################*/

/**
 * @brief Concatenates the path, name and format of the file
 * 
 * @param file_name The name of the file if needed
 * @param dir The path of the file if needed
 * @param format The format of the file if needed
 * @return char* The pointer to the combined string
 */
char* cat_file_directory(char* file_name, char* dir, char* format);
/*####################################################################################################################################################################*/

/**
 * @brief Create the simple html site with the information about the tree
 * 
 * @param html_name The pointer to the html file
 * @return Returns the error code  
 */
int create_html(char* html_name);
/*####################################################################################################################################################################*/

/**
 * @brief Prints the start of the html file to the html file
 * 
 * @param html_file_ptr The pointer to the html file
 * @return Returns the error code 
 */
int html_start(FILE* html_file_ptr); 
/*####################################################################################################################################################################*/

/**
 * @brief Prints the end of the html file to the html file
 * 
 * @param html_file_ptr The pointer to the html file
 * @return Returns the error code  
 */
int html_end(FILE* html_file_ptr);
/*####################################################################################################################################################################*/

/**
 * @brief Makes the command for every image and calls the dot in order to create the image form the txt file
 * 
 * @param file_name The name of the file that must be created using dot 
 * @return Returns the error code  
 */
int system_dot(char* file_name);
/*####################################################################################################################################################################*/

/**
 * @brief Gets the size of the file with the given (in config) name
 * 
 * @return Returns the error code OR the size of the file 
 */
int get_size_file();
/*####################################################################################################################################################################*/

/**
 * @brief Reads the text from the file into the buffer
 * 
 * @return char* The pointer to the filled buffer with the text from the file
 */
char* get_tokens_into_buf();
/*####################################################################################################################################################################*/

/**
 * @brief Using strtok gets all pathes from the image list and adds them to the html file with appropriate html format
 * 
 * @param buffer The pointer to the buffer of chars
 * @param file_tpr The pointer to the file where all pathes from the image list will be written
 * @return Returns the error code
 */
int get_tokens(char* buffer, FILE* file_tpr);
/*####################################################################################################################################################################*/

/**
 * @brief Adds all pathes from the image list to the given file
 * 
 * @param html_file_ptr The pointer to the html file
 * @return Returns the error code 
 */
int add_image_to_html(FILE* html_file_ptr);
/*####################################################################################################################################################################*/

/**
 * @brief Prints the given legend into the given file
 * 
 * @param legend_text The pointer to the text with the legend
 * @param graph_txt_ptr The pointer to the file
 * @return Returns the error code 
 */
int print_legend(char* legend_text, FILE* graph_txt_ptr);
/*####################################################################################################################################################################*/

/**
 * @brief (USED FOR LISTS) Creates the legend of the image from the called function name
 * 
 * @param func_name The name of the function which called this function
 * @param new_node_index The name of new node
 * @param node_index The name of old node
 * @param value The value of the old node
 * @param node_index_value The value of the new node
 * @return char* The pointer to the legend text
 */
char* create_legend(const char* func_name, int new_node_index, int node_index, int value, int node_index_value);
/*####################################################################################################################################################################*/

/**
 * @brief Prints the links between nodes
 * 
 * @param tree_struct The pointer to the tree struct
 * @param node_ptr The pointer to the child node
 * @param graph_txt_ptr The pointer to the file
 * @return The error code
 */
int print_tree_links(Tree* tree_struct, Node* node_ptr, FILE* graph_txt_ptr);
/*####################################################################################################################################################################*/

/**
 * @brief Prints the tree's nodes' data
 * 
 * @param tree_struct The pointer to the tree struct
 * @param node_ptr The pointer to the child node
 * @param graph_txt_ptr The pointer to the file 
 * @return The error code
 */
int print_tree_data(Tree* tree_struct, Node* node_ptr, FILE* graph_txt_ptr);
/*####################################################################################################################################################################*/

#endif
