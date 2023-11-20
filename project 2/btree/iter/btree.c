/***********************************/
/*  Author: Marek Čupr (xcuprm01)  */
/*  Subject: IAL - project 2       */
/*  Date: 26. 10. 2023             */
/*  Points: 2,5/2.5                */
/***********************************/

/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL; // Set pointer to null
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL || value == NULL) // Value or root node pointer is invalid
    return false; // Searching was NOT successful
  
  while(tree != NULL){
    /* The matching key was NOT found yet */ 
    if (tree->key > key){
      tree = tree->left; // The node being searched is on the left side of the current root node
    } else if (tree->key < key){
      tree = tree->right; // The node being searched is on the right side of the current root node
    } 
    
    /* The matching key was found */
    else {
      *value = tree->value; // Save the value of the searched node
      return true; // Searching was successful
    }
  }

  return false; // Searching was NOT successful
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if ((*tree) == NULL){ // Root node pointer is invalid

    // Allocate memory for the node being inserted  
    *tree = (bst_node_t *) malloc(sizeof(struct bst_node));
    if (*tree == NULL) // Malloc failed
      return;

    // Initialize all the tree struct values
    (*tree)->value = value;
    (*tree)->key = key;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return; // Node was inserted successfuly
  }

  // Helping variables
  bst_node_t *inserted_node = *tree;
  bst_node_t *inserted_node_root;
  
  while(inserted_node != NULL){
    inserted_node_root = inserted_node; // Save the root of the next node

    /* The already existing node with the matching key was NOT found yet */
    if (inserted_node->key > key){
      inserted_node = inserted_node->left; // The node being inserted is on the left side of the current root node
    } else if (inserted_node->key < key){
      inserted_node = inserted_node->right; // The node being inserted is on the right side of the current root node
    } 

    /* The already existing node with the matching key was found*/
    else {
      inserted_node->value = value; // Change the value of the already existing node 
      return; // Searching was successful
    }
  }

  // Allocate memory for the node being inserted  
  inserted_node = (bst_node_t *) malloc(sizeof(struct bst_node));
  if (inserted_node == NULL) // Malloc failed
    return;

    // Initialize all the tree struct values
  inserted_node->value = value;
  inserted_node->key = key;
  inserted_node->left = NULL;
  inserted_node->right = NULL;

  // Connect the new inserted node to the its' root node
  if (inserted_node_root->key > key){
    inserted_node_root->left = inserted_node; // The node that was inserted is a left subtree 
  } else if (inserted_node_root->key < key){
    inserted_node_root->right = inserted_node; // The node that was inserted is a right subtree 
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (*tree == NULL || target == NULL) // Target or root node pointer is invalid
    return; // Replacing with the most right node was NOT successful

  // Helping variables
  bst_node_t *replaced_node = *tree;
  bst_node_t *replaced_node_root = NULL;

 // The node at the most right position was NOT found yet
  while(replaced_node->right != NULL){ 
    replaced_node_root = replaced_node; // Save the root of the next node
    replaced_node = replaced_node->right; // Move to the next right node
  }

  // Replace the target value and key
  target->key = replaced_node->key;
  target->value = replaced_node->value;

  // The root node already is a rightmost node
  if (replaced_node_root == NULL){
    // clean the correct node
    free(*tree);
    *tree = NULL;
    return;
  }

  // Connect the left subtree of the deleted node to its' new root 
  replaced_node_root->right = replaced_node->left;

  // Delete the node at the most right position
  free(replaced_node);
  replaced_node = NULL;
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (*tree == NULL) // Root node pointer is invalid
    return;

  // Helping variables
  bst_node_t **deleted_node = tree;
  bst_node_t *deleted_node_root;

  while(*deleted_node != NULL){
    /* The matching key was NOT found yet */
    if ((*deleted_node)->key > key){
      deleted_node = &(*deleted_node)->left; // The node being searched for is on the left side of the current root node
    } else if ((*deleted_node)->key < key){
      deleted_node = &(*deleted_node)->right; // The node being searched for is on the right side of the current root node
    }
    
    /* The matching key was found */
     else {
      // The node being deleted has both left and right subtrees
      if ((*deleted_node)->left != NULL && (*deleted_node)->right != NULL){
        bst_replace_by_rightmost((*deleted_node), &(*deleted_node)->left); // Replace the node being deleted with the node at the most right positon of its' left subtree
        return;
      }

      // The node being deleted has only the right subtree
      else if ((*deleted_node)->left == NULL && (*deleted_node)->right != NULL){
        deleted_node_root = *deleted_node; // Save the adress of the node being deleted for later
        *deleted_node = (*deleted_node)->right; // Move to the right subtree

        // Delete the correct node
        free(deleted_node_root);
        deleted_node_root = NULL;
        return;
      } 
      
      // The node being deleted has only the left subtree
      else if ((*deleted_node)->left != NULL && (*deleted_node)->right == NULL){
        deleted_node_root = *deleted_node; // Save the adress of the node being deleted for later
        *deleted_node = (*deleted_node)->left; // Move to the left subtree

        // Delete the correct node
        free(deleted_node_root);
        deleted_node_root = NULL;
        return;
      } 
      
      // The node being deleted has NO subtrees
      else {
        // Delete the correct node
        free(*deleted_node);
        *deleted_node = NULL;
        return;
      }
    }
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  // Initialize the stack
  stack_bst_t bst_stack;
  stack_bst_init(&bst_stack);

  do {
    if (*tree != NULL){ // Root node pointer is NOT invalid
      if ((*tree)->right != NULL){ // Root node has a right subtree
        stack_bst_push(&bst_stack, (*tree)->right); // Push the right subtree to the stack
      }

      // Helping variable
      bst_node_t *tmp_node = *tree; 

      *tree = (*tree)->left; // Move to the left subtree

      // Delete the correct node
      free(tmp_node); 
      tmp_node = NULL;
    } else { // Root node pointer is invalid
      if (!stack_bst_empty(&bst_stack)){ // The stack is not empty 
        *tree = stack_bst_pop(&bst_stack); // Pop the node from top of the stack
      }
    }
  } while ((*tree != NULL) || (!stack_bst_empty(&bst_stack))); // Iterate until the stack gets emptied or the root pointer gets invalid 
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  while (tree != NULL){ // Root node pointer is NOT invalid
    stack_bst_push(to_visit, tree); // Push the root node on top of the stack
    bst_add_node_to_items(tree, items); // Save the node to the node array
    tree = tree->left; // Move to the left subtree
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  // Initialize the stack
  stack_bst_t bst_stack;
  stack_bst_init(&bst_stack);

  // Call the helping function
  bst_leftmost_preorder(tree, &bst_stack, items);

  while (!stack_bst_empty(&bst_stack)){ // The stack is not empty
    tree = stack_bst_pop(&bst_stack); // Pop the node from top of the stack
    // Call the helping function with the right subtree
    bst_leftmost_preorder(tree->right, &bst_stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL){ // Root node pointer is NOT invalid
    stack_bst_push(to_visit, tree); // Push the root node on top of the stack
    tree = tree->left; // Move to the left subtree
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  // Initialize the stack
  stack_bst_t bst_stack;
  stack_bst_init(&bst_stack);

  // Call the helping function
  bst_leftmost_inorder(tree, &bst_stack);

  while (!stack_bst_empty(&bst_stack)){ // The stack is not empty
    tree = stack_bst_pop(&bst_stack); // Pop the node from top of the stack
    bst_add_node_to_items(tree, items); // Save the node to the node array
    // Call the helping function with the right subtree
    bst_leftmost_inorder(tree->right, &bst_stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  while (tree != NULL){ // Root node pointer is NOT invalid
    stack_bst_push(to_visit, tree); // Push the root node on top of the stack
    stack_bool_push(first_visit, true); // Push the bool on top of the bool stack
    tree = tree->left; // Move to the left subtree
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  // Initialize the bool stack
  stack_bool_t bst_stack_bool;
  stack_bool_init(&bst_stack_bool);

  // Initialize the stack
  stack_bst_t bst_stack;
  stack_bst_init(&bst_stack);

  bool fromLeft; // Helping variable to save the values from the top of the bool stack

  // Call the helping function
  bst_leftmost_postorder(tree, &bst_stack, &bst_stack_bool);

  while (!stack_bst_empty(&bst_stack)){ // The stack is not empty
    tree = stack_bst_top(&bst_stack); // Get the node from top of the stack
    fromLeft = stack_bool_pop(&bst_stack_bool); // Pop the bool from top of the bool stack

    if (fromLeft) { // The popped bool is true
      stack_bool_push(&bst_stack_bool, false); // Push false on top of the bool stack
      // Call the helping function with the right subtree
      bst_leftmost_postorder(tree->right, &bst_stack, &bst_stack_bool);
    } else { // The popped bool is false
      stack_bst_pop(&bst_stack); // Pop the node from top of the stack
      bst_add_node_to_items(tree, items); // Save the node to the node array
    }
  }
}
