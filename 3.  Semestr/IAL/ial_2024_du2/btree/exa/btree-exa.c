/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 *
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 *
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je
 * uložen ve stromu (klíč vždy lowercase).
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
void letter_count(bst_node_t **tree, char *input)
{
    // strom je prázdný
    if (input == NULL)
    {
        return;
    }
    // strom má prvky

    // pro každý jednotlivý prvek z inputu
    for (int i = 0; input[i] != '\0'; i++)
    {

        // momentální písmeno
        char current_char = input[i];

        // Převede znak na malý, pokud je písmeno
        if ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z')) {
            // Převedeme velká písmena na malá
            if (current_char >= 'A' && current_char <= 'Z') {
                current_char = current_char + ('a' - 'A');
            }
        }
        // mezera
        else if (current_char == ' ')
        {
            current_char = ' '; // Mezery zůstávají tak, jak jsou
        }
        // jíný znak - nahrazuje podtržítkem
        else
        {
            current_char = '_';
        }

        // tvorba obsahu pro uzel
        bst_node_content_t content;
        // alokace místa pro hodnotu
        content.value = malloc(sizeof(int)); // Alokujeme místo pro hodnotu (počet výskytů)
        // check mallocu
        if (content.value == NULL)
        {
            return;
        }

        *(int *)content.value = 1; // Inicializuje počet výskytů na 1

        content.type = CHARACTER_T;

        // hledání existujícího uzlu v bst
        bst_node_content_t *existing_content = NULL;

        if (bst_search(*tree, current_char, &existing_content))
        {
            // Pokud uzel existuje, zvýší jeho hodnotu
            (*(int *)existing_content->value)++;
        }
        else
        {
            // Pokud uzel neexistuje, vloží nový
            bst_insert(tree, current_char, content);
        }
    }
}