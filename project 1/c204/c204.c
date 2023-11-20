/***********************************/
/*  Author: Marek Čupr (xcuprm01)  */
/*  Subject: IAL - project 1       */
/*  Date: 5. 10. 2023              */
/*  Points: 1.75/4                 */
/***********************************/

/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	if (Stack_IsEmpty(stack)) // Stack is empty
		return;

	char top_char;
	Stack_Top(stack, &top_char); // Get the top char from the stack

	while (top_char != '('){
		postfixExpression[(*postfixExpressionLength)++] = top_char; // Save the top char
		Stack_Pop(stack); // Delete the top char from the stack
		Stack_Top(stack, &top_char); // Get the new top char from the stack
	}
	Stack_Pop(stack); // Delete the left par as well
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	if (Stack_IsEmpty(stack)){ // Stack is empty
		Stack_Push(stack, c); // Push the operator on top of the stack
		return;
	}

	char top_char;
	Stack_Top(stack, &top_char); // Get the top char from the stack

	if (c == '(' || top_char == '(' || ((c == '*' || c == '/') && (top_char == '+' || top_char == '-'))){ // The top char is either a '(' or the operator is a '(' or the operator has a higher priority than the current operator on top of the stack
		Stack_Push(stack, c); // Push the operator on top of the stack
		return;
	}

	postfixExpression[(*postfixExpressionLength)++] = top_char; // Save the top char
	Stack_Pop(stack); // Delete the top char from the stack
	doOperation(stack, c, postfixExpression, postfixExpressionLength); // Call the function again until we push the operator on top of the stack (or until the stack gets emptied)
}

/**
 * VLASTNÍ FUNKCE
 * Kompletně vyčistí stack včetně všech položek této struktury
 *
 * @param stack Ukazatel na strukturu zásobníku, která má být vyčištěna
 */
void clean_Stack(Stack *stack){
	Stack_Dispose(stack); // Free the stack array and set the stack to the empty state
	free(stack); // free the stack
	stack = NULL; // Set the stack to NULL pointer
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	// Alocate memory
	char *postFixExpression = malloc(sizeof(char) * MAX_LEN); // Postfix
	unsigned int postFixExpressionLength = 0;

	if (postFixExpression == NULL){ // Failed at malloc
		return NULL;
	}

	Stack *stack = malloc(sizeof(Stack)); // Stack

	if (stack == NULL){ // Failed at malloc
		free(postFixExpression);
		postFixExpression = NULL;
		return NULL;
	}

	Stack_Init(stack); // Initialize the stack

	// Main functionality
	for (unsigned int i = 0; infixExpression[i] != '='; i++){ // Iterate until we reach the '='
		char infix_char = infixExpression[i];

		if (infix_char == '+' || infix_char == '-' || infix_char == '*' || infix_char == '/'){ // Infix char is an operator
			doOperation(stack, infix_char, postFixExpression, &postFixExpressionLength); // Call the doOperation function to handle the operator
		}
		else if ((infix_char >= 'a' && infix_char <= 'z') || (infix_char >= 'A' && infix_char <= 'Z') || (infix_char >= '0' && infix_char <= '9')){ // Infix char is an operand
			postFixExpression[postFixExpressionLength++] = infix_char; // Save the infix char
		}
		else if (infix_char == '('){ // Infix char is a left par
			Stack_Push(stack, infix_char); // Push the left par on top of the stack
		}
		else if (infix_char == ')'){ // Infix char is a right par
			untilLeftPar(stack, postFixExpression, &postFixExpressionLength); // Clear the stack until we reach the left par
		}
	}

	while(!Stack_IsEmpty(stack)){ // Empty the stack completely
		Stack_Top(stack, &(postFixExpression[postFixExpressionLength++])); // Save the top char
		Stack_Pop(stack); // Delete the top char from the stack
	}

	postFixExpression[postFixExpressionLength++] = '='; // Add eval symbol to the end
	postFixExpression[postFixExpressionLength] = '\0'; // Add string closing symbol to the end

	// Clean stack
	clean_Stack(stack);

	return postFixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	// Use (& 0xFF) to only get the least significant 8 bits (byte)

	char byte_1 = (char) ((value >> 24) & (0xFF)); // First byte
	char byte_2 = (char) ((value >> 16) & (0xFF)); // Second byte
	char byte_3 = (char) ((value >> 8) & (0xFF)); // Third byte
	char byte_4 = (char) (value & 0xFF); // Fourth byte

	// Gradually push all the bytes to the top of the stack
	Stack_Push(stack, byte_1);
	Stack_Push(stack, byte_2);
	Stack_Push(stack, byte_3);
	Stack_Push(stack, byte_4);
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	// Save all the bytes
	char num_bytes[4];
	Stack_Top(stack, &num_bytes[0]);
	Stack_Pop(stack);
	Stack_Top(stack, &num_bytes[1]);
	Stack_Pop(stack);
	Stack_Top(stack, &num_bytes[2]);
	Stack_Pop(stack);
	Stack_Top(stack, &num_bytes[3]);
	Stack_Pop(stack);

	// Type casting
	*value = *(int*)num_bytes;
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	// Alocate memory
	Stack *stack = malloc(sizeof(Stack));

	if (stack == NULL){ // Failed at malloc
		return false;
	}
	Stack_Init(stack); // Initialize the stack

	char *postFixExpression = infix2postfix(infixExpression); // Convert infix to postfix
	if (postFixExpression == NULL){ // Failed at malloc
		free(stack);
		stack = NULL;
		return false;
	}

	int operand1; // First operand
	int operand2; // Second operand
	char top_char;

	for (int i = 0; (top_char = postFixExpression[i]) != '\0'; i++){ // Go through every char
		if (top_char == '='){ // Top char is an '='
			expr_value_pop(stack, value); // Push it on top of the stack
			// Free all the memory
			clean_Stack(stack);
			free(postFixExpression);
			postFixExpression = NULL;
			return true;
		}
		else if (top_char == '+' || top_char == '-' || top_char == '*' || top_char == '/'){ // Top char is a +, -, * or /
			// Get the 2 operands for the operation
			expr_value_pop(stack, &operand1);
			expr_value_pop(stack, &operand2);
			// Operator type
			switch (top_char){
				case '*':
					*value = operand1 * operand2; // Order doesn't matter
					break;
				case '/':
					if (operand1 == 0) // Division by zero
						return false;
					*value = operand2 / operand1; // Change to the correct order
					break;
				case '+':
					*value = operand1 + operand2; // Order doesn't matter
					break;
				case '-':
					*value = operand2 - operand1; // Change to the correct order
					break;
				default:
					return false; // Incorrect operator
					break;
			}
			expr_value_push(stack, *value); // Push the result on top of the stack
		}
		else { // We have an operand (variable)
			for (int j = 0; j < variableValueCount; j++){ // Iterate through all the variables
				if (top_char == variableValues[j].name){ // Find the matching operand name to get the value
					expr_value_push(stack, variableValues[j].value); // Push the correct value on top
				}
			}
		}
	}

	// Free all the memory
	clean_Stack(stack);
	free(postFixExpression);
	postFixExpression = NULL;
	return NULL;
}

/* Konec c204.c */
