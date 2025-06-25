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
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  // strom prázdný
  if (tree == NULL)
    return false;

  // nalezen uzel se stejným klíčem
  if (key == tree->key)
  {
    *value = &tree->content;
    return true;
  }
  // hledaný klíč menší, přesun do levého podstromu
  else if (key < tree->key)
  {
    return bst_search(tree->left, key, value);
  }
  // hledaný klíč větší, přesun do pravého
  else
  {
    return bst_search(tree->right, key, value);
  }
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
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  // Pokud je strom prázdný, vytvoří nový uzel
  if (*tree == NULL)
  {
    *tree = (bst_node_t *)malloc(sizeof(bst_node_t));
    (*tree)->key = key;
    (*tree)->content = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
  }
  // klíč již existuje, nahradí jeho hodnotu
  else if (key == (*tree)->key)
  {
    free((*tree)->content.value);
    (*tree)->content = value;
  }
  // hledaný klíč menší, vloží do levého podstromu
  else if (key < (*tree)->key)
  {
    bst_insert(&(*tree)->left, key, value);
  }
  // hledaný klíč větší, vloží do pravého podstromu
  else
  {
    bst_insert(&(*tree)->right, key, value);
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
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  // Pokud má uzel pravý podstrom, pokračujeme v hledání nejpravějšího potomka
    if ((*tree)->right != NULL)
    {
        bst_replace_by_rightmost(target, &(*tree)->right);
    }
    // Pokud už nemá pravý podstrom, tento uzel je nejpravější potomek
    else
    {
        // Nahrazení hodnoty a klíče cílového uzlu
        target->key = (*tree)->key;
        free(target->content.value);
        target->content = (*tree)->content;

        bst_node_t *temp = *tree;
        *tree = (*tree)->left;  // Odstranění uzlu a připojení levého podstromu
        free(temp);  // Uvolnění paměti pro tento uzel
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
void bst_delete(bst_node_t **tree, char key)
{
  // chceck prázdnosti stromu
  if (*tree == NULL)
    return;

  // Hledá uzel podle klíče
  if (key < (*tree)->key)
  {
    bst_delete(&(*tree)->left, key); // Klíč je menší, pokračuje vlevo
  }
  else if (key > (*tree)->key)
  {
    bst_delete(&(*tree)->right, key); // Klíč je větší, pokračuje vpravo
  }
  else
  { // Nalezen uzel
    bst_node_t *temp = *tree;

    // uzel nemá podstromy ->odstranění
    if ((*tree)->left == NULL && (*tree)->right == NULL)
    {
      
      free(temp->content.value);
      free(temp);
      *tree = NULL; // Smazání uzlu
    }
    // pouze pravý podstrom -> připojí pravý podstrom k rodiči
    else if ((*tree)->left == NULL)
    {
      
      free(temp->content.value);
      *tree = (*tree)->right;
      free(temp);
    }
    // pouze levý podstrom -> připojí levý podstrom k rodiči
    else if ((*tree)->right == NULL)
    {
      
      free(temp->content.value);
      *tree = (*tree)->left;
      free(temp);
    }
    // oba podstromy -> použije nejpravějšího potomka levého podstromu
    else
    {
      bst_replace_by_rightmost(*tree, &(*tree)->left);
      
    }

    // Uvolní paměť uzlu
    
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
void bst_dispose(bst_node_t **tree)
{
  // strom už prázdný
    if (*tree == NULL) return;

    // Rekurzivně ruší levý a pravý podstrom
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);

    // Uvolní paměť uzlu
    free((*tree)->content.value);
    free(*tree);
    *tree = NULL;  
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  // Check prázdnosti
    if (tree == NULL) return;

    // zpracování aktuálního uzlu
    bst_add_node_to_items(tree, items);

    // Rekurzivně prohledá levý a pravý podstrom
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  // Check prázdnosti
    if (tree == NULL) return;

  // Rekurzivně prohledá levý podstrom
    bst_inorder(tree->left, items);

    // Zpracuje aktuální uzel
    bst_add_node_to_items(tree, items);

    // Rekurzivně prohledá pravý podstrom
    bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  // Check prázdnosti
    if (tree == NULL) return;

   // Rekurzivně prohledá levý a pravý podstrom
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);

    // Zpracuje aktuální uzel
    bst_add_node_to_items(tree, items);
}
