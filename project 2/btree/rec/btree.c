/***********************************/
/*  Author: Marek Čupr (xcuprm01)  */
/*  Subject: IAL - project 2       */
/*  Date: 26. 10. 2023             */
/*  Points: 2.5/2.5                */
/***********************************/

/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
  (*tree) = NULL; // Set pointer to null
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL || value == NULL) // Value or root node pointer is invalid
    return false; // Searching was NOT successful

  /* The matching key was NOT found yet */   
  if (tree->key > key){
    return bst_search(tree->left, key, value); // The node being searched is on the left side of the current root node
  } else if (tree->key < key){
    return bst_search(tree->right, key, value); // The node being searched is on the right side of the current root node
  }

  /* The matching key was found */
  else {
    *value = tree->value; // Save the value of the searched node
    return true; // Searching was successful
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
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

  /* The already existing node with the matching key was NOT found yet */   
  if ((*tree)->key > key){
    bst_insert(&((*tree)->left), key, value); // The node being inserted is on the left side of the current root node
  } else if ((*tree)->key < key){ 
    bst_insert(&((*tree)->right), key, value); // The node being inserted is on the right side of the current root node
  } 

  /* The already existing node with the matching key was found*/
  else {
    (*tree)->value = value; // Change the value of the already existing node 
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (*tree == NULL || target == NULL) // Target or root node pointer is invalid
    return; // Replacing with the most right node was NOT successful

  // The node at the most right position was NOT found yet
  if ((*tree)->right != NULL){
    bst_replace_by_rightmost(target, &((*tree)->right)); // Move to the next right node
  }

  // The node at the most right position was found
  else { 
    // Replace the target value and key
    target->key = (*tree)->key;
    target->value = (*tree)->value;

    // The node being deleted doesn't have a left subtree
    if ((*tree)->left == NULL){
      // Delete the node at the most right position
      free(*tree);
      *tree = NULL;
      return;
    }

    // The node being deleted has a left subtree
    bst_node_t *deleted_node = *tree; // Save the adress of the node being deleted for later
    (*tree) = (*tree)->left; // Move to the left subtree

    // Delete the node at the most right position
    free(deleted_node);
    deleted_node = NULL;
  }
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (*tree == NULL) // Root node pointer is invalid
    return;

  /* The matching key was NOT found yet */   
  if ((*tree)->key > key){
    bst_delete(&((*tree)->left), key); // The node being searched for is on the left side of the current root node
  } else if ((*tree)->key < key){
    bst_delete(&((*tree)->right), key); // The node being searched for is on the right side of the current root node
  }

  /* The matching key was found */
  else {
    // The node being deleted has both left and right subtrees
    if ((*tree)->left != NULL && (*tree)->right != NULL){
      bst_replace_by_rightmost(*tree, &((*tree)->left)); // Replace the node being deleted with the node at the most right positon of its' left subtree
    } 

    // The node being deleted has only the right subtree
    else if ((*tree)->left == NULL && (*tree)->right != NULL) {
      bst_node_t *deleted_r_node = *tree; // Save the adress of the node being deleted for later
      (*tree) = (*tree)->right; // Move to the right subtree

      // Delete the correct node
      free(deleted_r_node);
      deleted_r_node = NULL;
    } 

    // The node being deleted has only the left subtree
    else if ((*tree)->left != NULL && (*tree)->right == NULL){
      bst_node_t *deleted_l_node = *tree;// Save the adress of the node being deleted for later
      (*tree) = (*tree)->left; // Move to the left subtree

      // Delete the correct node
      free(deleted_l_node);
      deleted_l_node = NULL;
    } 
    
    // The node being deleted has NO subtrees
    else {
      // Delete the correct node
      free(*tree);
      *tree = NULL;
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if (*tree != NULL){ // Root node pointer is invalid
    bst_dispose(&((*tree)->left)); // Delete the left subtree
    bst_dispose(&((*tree)->right)); // Delete the right subtree

    // Delete the correct node
    free(*tree);
    *tree = NULL;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){ // Root node pointer is NOT invalid
    // Preorder: Root | Left | Right
    bst_add_node_to_items(tree, items); // Add root node to the array of nodes
    bst_preorder(tree->left, items); // Move to the left node
    bst_preorder(tree->right, items); // Move to the right node
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){ // Root node pointer is NOT invalid
    // Inorder: Left | Root | Right
    bst_inorder(tree->left, items); // Move to the left node
    bst_add_node_to_items(tree, items); // Add root node to the array of nodes
    bst_inorder(tree->right, items); // Move to the right node
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){
    // Postorder: Left | Right | Root
    bst_postorder(tree->left, items); // Move to the left node
    bst_postorder(tree->right, items); // Move to the right node
    bst_add_node_to_items(tree, items); // Add root node to the array of nodes
  }
}
