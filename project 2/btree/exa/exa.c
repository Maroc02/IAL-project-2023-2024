/***********************************/
/*  Author: Marek Čupr (xcuprm01)  */
/*  Subject: IAL - project 2       */
/*  Date: 29. 10. 2023             */
/*  Points: 1.6/2                  */
/***********************************/

/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/* Some elemental functions since I'm unable to use ctype.h or string.h according to the assignemnt */

/**
 * Converts uppercase letters to lowercase letters
 * @param input_char Character to be converted to lowercase 
 * @returns The character in lowercase or the character itself if it's not a letter 
*/
int my_tolower(int input_char){
    if (input_char >= 65 && input_char <= 90){
        return input_char + 32;
    } else {
        return input_char;
    }
}

/**
 * Checks if character is a letter (a-z, A-Z)
 * @param input_char Character to be checked 
 * @returns ~ 1 if the character is a letter 
 * @returns ~ 0 if the character is NOT a letter 
*/
int my_isalpha(int input_char){
    if ((input_char >= 65 && input_char <= 90) || (input_char >= 97 && input_char <= 122)){
        return 1; // Character is a letter
    } else {
        return 0;  // Character is NOT a letter
    }
}

/**
 * Checks the length of a string
 * @param input String to count the length of 
 * @returns The length of a string 
*/
int my_strlen(char *input){
    int str_len = 0;
    for (int i = 0; input[i] != '\0'; i++){
        str_len++;
    }
    return str_len;
}

/**
 * Initializes a new bst_items structure
 * @param bst_items Structure to be initialized 
 * @param input Input string that indicates how much memory to allocate 
*/
void bst_items_init(bst_items_t *bst_items, char *input){
    // Allocate memory for all the possible nodes
    bst_items->nodes = (bst_node_t**) malloc(sizeof(bst_node_t *) * my_strlen(input));
    if (bst_items->nodes == NULL){ // Malloc failed
        return;
    }

    // Set the values of bst_items
    bst_items->capacity = my_strlen(input); // Maximal capacity is as big as the input string
    bst_items->size = 0; // Size is 0 by default
}


/**
 * Pushes all the unique symbols from the input to seperate nodes that are kept in bst_items
 * Keeps track of the symbols' occurances. 
 * @param bst_items Structure that holds all the created nodes 
 * @param input Input string to be handled 
*/
void handle_input(bst_items_t *bst_items, char *input){
    bool matched; // Checks if there's a node for a specific symbol already 
    int index; // Indexing through the nodes
    char curr_char; // The character we're currently on
    for (int i = 0; (curr_char = my_tolower(input[i])) != '\0'; i++){
        index = 0;
        matched = false; // We've NOT found the matching symbol yet
        while (index < bst_items->size){ // Go through all the nodes
            if (curr_char != ' ' && !my_isalpha(curr_char)){ // All the characters apart from letters and spaces should be represented as '_'
                curr_char = '_'; // Character is not a letter or a space
            }

            if (curr_char == bst_items->nodes[index]->key){ // Check if the node already exists for a specific symbol 
                matched = true; // We've matched the symbol
                bst_items->nodes[index]->value++; // Increase the amount of occurances
            }
            index++;
         }
        if (!matched){ // We've NOT matched the symbol
            bst_node_t *node = (bst_node_t*) malloc(sizeof(bst_node_t)); // Create a new node
            node->value = 1; // Set the amount of occurances to 1 by default
            if (my_isalpha(input[i]) || input[i] == ' ') // The character is a letter or a space
                node->key = curr_char;
            else // The character is not a letter or a space
                node->key = '_';

            bst_add_node_to_items(node, bst_items); // Add the new created node to the nodes array
        }
    }
}

/**
 * Inserts all the nodes from nodes array to the tree 
 * @param bst_items Structure that holds all the nodes to be inserted 
 * @param tree The tree to instert the nodes to 
*/
void insert_nodes(bst_node_t **tree, bst_items_t *bst_items){
    for (int i = 0; i < bst_items->size; i++){ // Go through all the nodes
        bst_insert(tree, bst_items->nodes[i]->key, bst_items->nodes[i]->value); // Insert the node to the tree
    }
}

/**
 * Clean all the dynamically allocated nodes 
 * @param bst_items Array of nodes to be deleted 
*/
void bst_items_clean(bst_items_t *bst_items){
    for (int i = 0; i < bst_items->capacity; i++){ // Go through all the nodes allocated previously
        // Clean all the nodes
        free(bst_items->nodes[i]);
        bst_items->nodes[i] = NULL;
    }

    // Clean the nodes array itself
    free(bst_items->nodes);
    bst_items->nodes = NULL;
}

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    // Initialize the tree
    bst_init(tree);

    // Initialize bst_items to keep all the nodes inside
    bst_items_t bst_items;
    bst_items_init(&bst_items, input);

    // Create nodes for all the unique symbols and keep a track of their occurances in the string
    handle_input(&bst_items, input);

    // Insert all the nodes created to the tree
    insert_nodes(tree, &bst_items);

    // Clean all the dynamically allocated resources
    bst_items_clean(&bst_items);
}

/**
 * Creates a balanced tree from the nodes array
 * @param tmp_tree Balanced tree to be created 
 * @param bst_items Nodes array that holds all the nodes 
 * @param left Starting index of nodes array (the node on the most left position) 
 * @param left Ending index of nodes array (the node on the most right position)
*/
void balance_tree(bst_node_t **tmp_tree, bst_items_t *bst_items, int left_node, int right_node){
    if (left_node <= right_node) {
        int middle_node = (left_node + right_node) / 2; // Start at the middle node
        *tmp_tree = (bst_node_t *) malloc(sizeof(bst_node_t)); // Allocate memory for a new node
        
        // Set values of the new node to the values of the node in the middle of the array
        (*tmp_tree)->value = bst_items->nodes[middle_node]->value;
        (*tmp_tree)->key = bst_items->nodes[middle_node]->key;

        balance_tree(&(*tmp_tree)->left, bst_items, left_node, middle_node - 1); // Move to the left from the middle node 
        balance_tree(&(*tmp_tree)->right, bst_items, middle_node + 1, right_node); // Move to the right from the middle node
    } else { // The left and right node have met
        (*tmp_tree) = NULL;
    }
}

/**
 * Create a new empty bst_items
 * @param bst_items Structure to be created 
*/
void init_empty_bst_items(bst_items_t *bst_items){
    bst_items->nodes = (bst_node_t**) malloc(sizeof(bst_node_t *)); // Allocate memory
    bst_items->capacity = 0; // Set the default capacity
    bst_items->size = 0; // Set the default size
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    // Initialize bst_items to keep all the nodes inside
    bst_items_t bst_items;
    init_empty_bst_items(&bst_items);

    // Save all the sorted nodes to the nodes array
    bst_inorder(*tree, &bst_items);

    // Initialize the temporary tree
    bst_node_t **tmp_tree = (bst_node_t **) malloc(sizeof(bst_node_t*) * bst_items.capacity); 
    bst_init(tmp_tree);

    // Create a balanced tree
    balance_tree(tmp_tree, &bst_items, 0, bst_items.size - 1);

    // Delete the original tree structure
    bst_dispose(tree);

    *tree = *tmp_tree; // Set the tree to point at the balanced tree

    // Clean all the dynamically allocated memory
    free(bst_items.nodes);
    free(tmp_tree);
}
