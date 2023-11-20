/***********************************/
/*  Author: Marek Čupr (xcuprm01)  */
/*  Subject: IAL - project 2       */
/*  Date: 27. 10. 2023             */
/*  Points: x/3                    */
/***********************************/

/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  // Go through all the table items
  for (int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL; // Set all the pointers to NULL
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  // Get the correct hash index for indexing
  int hash_index = get_hash(key);

  // Get the correct item using the hash index 
  ht_item_t *searched_hash_item = (*table)[hash_index];

  // Go through the linked list at the hash index
  while (searched_hash_item != NULL){
    if(strcmp(searched_hash_item->key, key) == 0){ // strcmp returns: 0 if the keys (strings) are the same
      return searched_hash_item; // The searching was successful
    }
    searched_hash_item = searched_hash_item->next; // Move to the next item in the linked list
  }

  return NULL; // The searching was NOT successful
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  // Helping variables
  ht_item_t *hash_item = ht_search(table, key); // Search for the item to see if it exists already
  ht_item_t *tmp_item;

    // Get the correct hash index for indexing
  int hash_index = get_hash(key);

  if (hash_item == NULL){ // The item being inserted does NOT exist yet
    // Create a new item
    hash_item = (ht_item_t *) malloc(sizeof(struct ht_item));
    if (hash_item == NULL) // Malloc failed
      return;

    // Set the values of the new item
    hash_item->key = key;
    hash_item->value = value;
    hash_item->next = NULL;

    // Check if the new item will be colliding with other items
    if ((*table)[hash_index] != NULL){ // The new item is colliding with other items at hash index
      // Insert the new item to the front of the linked list
      tmp_item = (*table)[hash_index]; // Save the current item at the front of the linked list for later
      (*table)[hash_index] = hash_item; // Make the new item be the first item in the linked list
      hash_item->next = tmp_item; // Connect the new first item to the previous first item (second item now) 
      return;
    }
    (*table)[hash_index] = hash_item; // The new item is NOT colliding with other items
  } else { // The item does exist already
    hash_item->value = value; // Change the value of the already existing item
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  // search for the correct item
  ht_item_t *hash_item = ht_search(table, key);

  if (hash_item != NULL){ // The searched item does exist
    return &(hash_item->value); // Return the adress of the value
  }

  return NULL; // The searched item does NOT exist
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  // Get the correct hash index for later indexing
  int hash_index = get_hash(key);

  // Helping variables
  ht_item_t *hash_item = (*table)[hash_index];
  ht_item_t *prev_item = NULL;

  // The item does exist
  while (hash_item != NULL){
    if (strcmp(hash_item->key, key) == 0){ // We've found the matching key
      if (prev_item == NULL){ // The item being deleted is the first item in the linked list
        prev_item = hash_item; // Save the adress of the item being deleted
        hash_item = hash_item->next; // Move to the next item
        free(prev_item); // Free the correct item
        return; 
      } else { // The item being deleted is not the first item in the linked list
        hash_item = hash_item->next; // Move to the next item
        free(prev_item->next); // Free the correct item
        prev_item->next = hash_item; // Connect the item before the deleted item to its' new next item
        return;
      }
    }

    // We've NOT found the matching key
    prev_item = hash_item; // Previous item of the item being deleted
    hash_item = hash_item->next; // Move to the next item until we find the correct item be deleted
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  // Helping variable
  ht_item_t *tmp_item;

  for(int i = 0; i < HT_SIZE; i++){ // Go through the whole hashtable
    while ((*table)[i] != NULL){
      int hash_index = get_hash((*table)[i]->key); // Get the correct hash index for indexing
      tmp_item = (*table)[hash_index]->next; // Save the adress to the next item in the linked list
      free((*table)[hash_index]); // Free the correct item
      (*table)[hash_index] = tmp_item; // Move to the next item in the linked list
    }
  }
}