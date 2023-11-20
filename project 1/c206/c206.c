/***********************************/
/*  Author: Marek Čupr (xcuprm01)  */
/*  Subject: IAL - project 1       */
/*  Date: 30. 9. 2023              */
/*  Points: 4/4                    */
/***********************************/

/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	// Initialize everything to NULL by default
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	DLLElementPtr tmp_item = list->firstElement; // Temporary variable

	if (tmp_item == NULL) // List is empty
		return;

	while (tmp_item != list->lastElement){ // Go throught the list from the beginning
		tmp_item = tmp_item->nextElement;
		free(tmp_item->previousElement); // Free the correct item
	}

	free(list->lastElement); // Free the last item as well

	// Set everything to NULL
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * VLASTNÍ FUNKCE
 * Inicializuje nově vytvořenou položku seznamu
 *
 * @param item Ukazatel na nově vytvořenou položku pole
 * @param data Data k uložení do nově vytvořené položky
 */
void DLL_Init_New(struct DLLElement *item, int data ){
	item->data = data;
	item->nextElement = NULL;
	item->previousElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr new_first = malloc(sizeof(struct DLLElement)); // New first item
	DLLElementPtr tmp_item; // Temporary variable

	if (new_first == NULL){ // Failed at malloc
		DLL_Error();
		return;
	}

	// Initialize the new first item
	DLL_Init_New(new_first, data);

	if (list->firstElement == NULL){ // List is empty
		list->firstElement = list->lastElement = new_first; // Set the new first element to be both first and last
	} else { // List is not empty
		tmp_item = list->firstElement; // Temporarily save the current first item 
		list->firstElement = new_first; // Set the new first item
		// Link the new first item and the previous first item together (both ways)
		list->firstElement->nextElement = tmp_item; // Link the new first item to the previous first item
		tmp_item->previousElement = list->firstElement; // Link the previous first item to the new first item
	}
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr new_last = malloc(sizeof(struct DLLElement)); // New last item
	DLLElementPtr tmp_item; // Temporary variable

	if (new_last == NULL){  // Failed at malloc
		DLL_Error();
		return;
	}

	// Initialize the new last item
	DLL_Init_New(new_last, data);

	if (list->firstElement == NULL){ // List is empty
		list->firstElement = list->lastElement = new_last; // Set the new last element to be both first and last
	} else { // List is not empty
		tmp_item = list->lastElement; // Temporarily save the current last item 
		list->lastElement = new_last; // Set the new last item
		// Link the new last item and the previous last item (both ways)
		list->lastElement->previousElement = tmp_item; // Link the new last item to the previous last item
		tmp_item->nextElement = list->lastElement; // Link the previous last item to the new last item
	}
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement == NULL){ // List is empty
		DLL_Error(); // Error function
		return;
	}

	*dataPtr = list->firstElement->data; // Save the data of the first item
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list->firstElement == NULL){ // List is empty
		DLL_Error(); // Error function
		return;
	}

	*dataPtr = list->lastElement->data; // Save the data of the last item
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement == NULL) // List is empty
		return;

	if (list->firstElement == list->activeElement) // First item is active
		list->activeElement = NULL; // Activity is lost
	
	if (list->firstElement->nextElement == NULL){ // Only one item in the list
		free(list->firstElement);
		list->activeElement = NULL;
		list->firstElement = NULL;
		list->lastElement = NULL;
	} else { // More than one item in the list
		list->firstElement = list->firstElement->nextElement; // Set the first item to the second item
		free(list->firstElement->previousElement); // Delete the previous first item
		list->firstElement->previousElement = NULL; // The new first item should not point to any previous item
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	DLLElementPtr tmp_item; // Temporary variable
	if (list->firstElement == NULL) // List is empty
		return;

	if (list->lastElement == list->activeElement) // Last item is active
		list->activeElement = NULL; // Activity is lost

	if (list->lastElement->previousElement == NULL) { // Only one item in the list
		free(list->lastElement);
		list->activeElement = NULL;
		list->firstElement = NULL;
		list->lastElement = NULL;
	} else { // More than one item in the list
		tmp_item = list->lastElement; // Temporarily save the current last item 
		list->lastElement = list->lastElement->previousElement; // Set the last item to the second last item
		free(tmp_item); // Delete the previous last item
		list->lastElement->nextElement = NULL; // The new last item should not point to any next item
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	DLLElementPtr tmp_item; // Temporary variable
	
	if (list->activeElement == NULL || list->activeElement == list->lastElement) // List is not active or the last item is active
		return;

	if (list->activeElement->nextElement == list->lastElement){ // There is only the last item after the active item
		free(list->lastElement);
		list->lastElement = list->activeElement; // Set the new last item to be the active item
		list->lastElement->nextElement = NULL; // The new last item should not point to any next item
	} else { // There are other items after the active item
		tmp_item = list->activeElement->nextElement; // Temporarily save the item to be deleted
		list->activeElement->nextElement = tmp_item->nextElement; // Link the active item to the new next item
		free(tmp_item); // Delete the correct item
		list->activeElement->nextElement->previousElement = list->activeElement; // Link the new next item to the active item (the other way)
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	DLLElementPtr tmp_item; // Temporary variable

	if (list->activeElement == NULL || list->activeElement == list->firstElement) // List is not active or the first item is active
		return;

	if (list->activeElement->previousElement == list->firstElement){  // // There is only the first item before the active item
		list->firstElement = list->firstElement->nextElement; // Set the new first item to be the active item
		free(list->firstElement->previousElement); // Delete the previous first item
		list->firstElement->previousElement = NULL; // The new first item should not point to any previous item
	} else { // There are other items before the active item
		tmp_item = list->activeElement->previousElement->previousElement; // Temporarily save the item before the item that should be deleted
		free(list->activeElement->previousElement); // Delete the correct item
		list->activeElement->previousElement = tmp_item; // Link the active item to the new previous item
		tmp_item->nextElement = list->activeElement; // Link the new previous item to the active item (the other way)
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if (list->activeElement == NULL) // List is not active
		return;

	DLLElementPtr item_insert = malloc(sizeof(struct DLLElement)); // Item to be inserted after
	DLLElementPtr tmp_item; // Temporary variable

	if (item_insert == NULL){  // Failed at malloc
		DLL_Error();
		return;
	}
	
	// Initialize the inserted item
	DLL_Init_New(item_insert, data);

	if (list->activeElement == list->lastElement){ // Active item is the last item
		list->activeElement->nextElement = item_insert; // Link the active item to the inserted item
		item_insert->previousElement = list->activeElement; // Link the inserted item to the active item
		list->lastElement = item_insert; // Set the inserted item to be the last item
	} else { // Active item is not the last item
		tmp_item = list->activeElement->nextElement;
		list->activeElement->nextElement = item_insert; // Link the active item to the inserted item
		item_insert->previousElement = list->activeElement; // Link the inserted item to the active item
		item_insert->nextElement = tmp_item;
		tmp_item->previousElement = item_insert;
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if (list->activeElement == NULL) // List is not active
		return;
		
	DLLElementPtr item_insert = malloc(sizeof(struct DLLElement)); // Item to be inserted before
	DLLElementPtr tmp_item; // Temporary variable

	if (item_insert == NULL){  // Failed at malloc
		DLL_Error();
		return;
	}

	// Initialize the inserted item
	DLL_Init_New(item_insert, data);

	if (list->activeElement == list->firstElement){ // Active item is the first item
		list->activeElement->previousElement = item_insert; // Link the active item to the inserted item
		item_insert->nextElement = list->activeElement; // Link the inserted item to the active item
		list->firstElement = item_insert;  // Set the inserted item to be the first item
	} else { // Active item is not the first item
		tmp_item = list->activeElement->previousElement;
		list->activeElement->previousElement = item_insert; // Link the active item to the inserted item
		item_insert->nextElement = list->activeElement; // Link the inserted item to the active item
		item_insert->previousElement = tmp_item;
		tmp_item->nextElement = item_insert;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement == NULL){ // List is not active
		DLL_Error();
		return;
	}

	*dataPtr = list->activeElement->data; // Save the data of the active item
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement == NULL) // List is not active
		return;
	
	list->activeElement->data = data; // Set new data to the active item
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement == NULL) // List is not active
		return;

	list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement == NULL) // List is not active
		return;

	list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	
	return (list->activeElement) ? 1 : 0;
}

/* Konec c206.c */
