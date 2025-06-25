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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  while (tree != NULL) // pokud existuje node
  {
    if (key < tree->key)
    {
      tree = tree->left; // přesun do levého podstromu
    }
    else if (key > tree->key)
    {
      tree = tree->right; // přesun do pravého podstromu
    }
    else
    {
      *value = &tree->content; // klíč nalezen, nastavení value
      return true;             // exit, klíč nalezen
    }
  }

  return false; // klíč nenalezen
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
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  // Pokud je strom prázdný, vytvoříme nový kořenový uzel
  if (*tree == NULL)
  {
    *tree = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (*tree == NULL)
    {
      return; // check failu mallocu
    }
    (*tree)->key = key;       // Přiřazení klíče novému uzlu
    (*tree)->content = value; // Přiřazení hodnoty novému uzlu
    (*tree)->left = NULL;     // Levý potomek je NULL
    (*tree)->right = NULL;    // Pravý potomek je NULL
    return;
  }

  bst_node_t *current = *tree; // Ukazatel pro procházení stromu, momentální prvek
  bst_node_t *parent = NULL;   // Ukazatel na rodičovský uzel

  // Procházení stromu za účelem nalezení správné pozice pro nový uzel
  while (current != NULL)
  {
    parent = current;

    if (key == current->key)
    {
      // Pokud uzel s daným klíčem už existuje, aktualizujeme jeho hodnotu
      free((*tree)->content.value);
      current->content = value;
      return;
    }
    else if (key < current->key)
    {
      // Pokud je klíč menší, procházíme levý podstrom
      current = current->left;
    }
    else
    {
      // Pokud je klíč větší, procházíme pravý podstrom
      current = current->right;
    }
  }

  // Vytvoření nového uzlu na správném místě
  bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
  if (new_node == NULL)
  {
    return; // chceck failu mallocu
  }

  new_node->key = key;       // Nastavení klíče pro nový uzel
  new_node->content = value; // Nastavení hodnoty pro nový uzel
  new_node->left = NULL;     // Levý potomek je NULL
  new_node->right = NULL;    // Pravý potomek je NULL

  // Připojení nového uzlu k rodiči
  if (key < parent->key)
  {
    parent->left = new_node; // Nový uzel bude levý potomek
  }
  else
  {
    parent->right = new_node; // Nový uzel bude pravý potomek
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
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *parent = NULL;
  bst_node_t *rightmost = target->left;

  // Najde nejpravější uzel v pravém podstromu
  while (rightmost != NULL && rightmost->right != NULL)
  {
    parent = rightmost;           // rodiče pravého potomka
    rightmost = rightmost->right; // Přejdeme na pravého potomka
  }

  // neexistuje pravý podstrom
  if (rightmost == NULL)
  {
    return;
  }

  // Nahrazení hodnoty target hodnotou pravého nejpravějšího uzlu
  target->key = rightmost->key;
  free(target->content.value);
  target->content = rightmost->content;

  // Odstranění nejpravějšího uzlu
  if (parent != NULL)
  {
    parent->right = rightmost->left; // Pravý potomek rodiče -> levý potomek pravého nejpravějšího
    
  }
  else
  {
    *tree = rightmost->left; // pravý nejpravější uzel kořen, připojíme jeho levý potomek
  }
  
  free(rightmost); // uvolnění paměti nejpravějšího uzlu
  
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
void bst_delete(bst_node_t **tree, char key)
{
  // zavedení pomocných proměnných
  bst_node_t *current = *tree;
  bst_node_t *parent = NULL;

  // Najde uzel, který odpovídá klíči
  while (current != NULL && current->key != key)
  {
    parent = current;
    if (key < current->key)
    {
      current = current->left; // Pokračuje do levého podstromu
    }
    else
    {
      current = current->right; // Pokračuje do pravého podstromu
    }
  }

  // uzel neexistuje
  if (current == NULL)
  {
    return;
  }

  // dva podstromy - náhrada pravého nejpravějšího uzlu
  if (current->left != NULL && current->right != NULL)
  {
    bst_replace_by_rightmost(current, &current->left); // Nahrazení levým podstromem nejpravějšího uzlu
  }

  // jen levý podstrom - připojit levý podstrom k rodiči
  else if (current->left != NULL)
  {
    if (parent == NULL)
    { // Pokud kořenový uzel
      *tree = current->left;
    }
    else if (parent->left == current)
    {
      parent->left = current->left;
    }
    else
    {
      parent->right = current->left;
    }
    // uvolňování paměti
    free(current->content.value); // obsah hodnoty
    free(current);                // paměť uzlu
  }
  // jen pravý podstrom - připojit pravý podstrom k rodiči
  else if (current->right != NULL)
  {

    if (parent == NULL)
    { // Pokud kořenový uzel
      *tree = current->right;
    }
    else if (parent->left == current)
    {
      parent->left = current->right;
    }
    else
    {
      parent->right = current->right;
    }
    // uvolňování paměti
    free(current->content.value); // obsah hodnoty
    free(current);                // paměť uzlu
  }
  // žádný podstrom (je list) - odstranění
  else
  {

    if (parent == NULL)
    { // Pokud je  kořenový uzel

      *tree = NULL;
    }
    else if (parent->left == current)
    {

      parent->left = NULL;
    }
    else
    {

      parent->right = NULL;
    }
    // uvolňování paměti
    free(current->content.value); // obsah hodnoty
    free(current);                // paměť uzlu
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
void bst_dispose(bst_node_t **tree)
{
  if (*tree == NULL)
  {
    return; // strom už je prázdný
  }

  // tvorba zásobníku, inicializace
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, *tree); // Přidá kořen do zásobníku

  while (!stack_bst_empty(&stack))
  {
    bst_node_t *current = stack_bst_pop(&stack); // Vybere uzel ze zásobníku

    // Pokud má uzel levý podstrom, přidá ho do zásobníku
    if (current->left != NULL)
    {
      stack_bst_push(&stack, current->left);
    }

    // Pokud má uzel pravý podstrom, přidá ho do zásobníku
    if (current->right != NULL)
    {
      stack_bst_push(&stack, current->right);
    }

    // Uvolní paměť pro obsah hodnoty uzlu
    free(current->content.value);
    free(current); // Uvolní paměť pro samotný uzel
  }
  // strom zrušen

  *tree = NULL; // Uvedení do stavu po inicializaci
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
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  bst_node_t *current = tree;

  // Procházení po levé větvi k nejlevějšímu uzlu
  while (current != NULL)
  {
    // Uloží uzel do zásobníku
    stack_bst_push(to_visit, current);

    // přidání uzlu do items
    bst_add_node_to_items(current, items);

    // Přejde na levého potomka
    current = current->left;
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
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  // check existence stromu
  if (tree == NULL)
  {
    return;
  }

  stack_bst_t to_visit;      // Zásobník pro uchovávání uzlů, které je třeba zpracovat
  stack_bst_init(&to_visit); // Inicializace zásobníku

  bst_leftmost_preorder(tree, &to_visit, items); // Prochází levé podstromy a zpracovává uzly

  // Iterativní procházení stromu
  while (!stack_bst_empty(&to_visit))
  {
    bst_node_t *current = stack_bst_pop(&to_visit); // Vybere uzel ze zásobníku

    // Pokud má uzel pravý podstrom
    if (current->right != NULL)
    {
      bst_leftmost_preorder(current->right, &to_visit, items); // Prochází pravý podstrom
    }
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
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  bst_node_t *current = tree;

  // Procházení levého podstromu k nejlevějšímu uzlu
  while (current != NULL)
  {
    stack_bst_push(to_visit, current); // Uložení uzlu do zásobníku
    current = current->left;           // na levého potomka
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
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  // check existence stromu
  if (tree == NULL)
  {
    return;
  }

  stack_bst_t to_visit;      // Zásobník pro uzly, které je třeba zpracovat
  stack_bst_init(&to_visit); // Inicializace zásobníku

  bst_leftmost_inorder(tree, &to_visit); // Procházení levé větve stromu

  // Iterativní procházení stromu
  while (!stack_bst_empty(&to_visit))
  {
    bst_node_t *current = stack_bst_pop(&to_visit); // Vyjmutí uzlu ze zásobníku

    // Zpracování aktuálního uzlu
    bst_add_node_to_items(current, items); // Přidání uzlu do seznamu

    // Pokud má uzel pravý podstrom
    if (current->right != NULL)
    {
      bst_leftmost_inorder(current->right, &to_visit); // Procházení pravého podstromu
    }
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
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  bst_node_t *current = tree;

  // Procházení levého podstromu k nejlevějšímu uzlu
  while (current != NULL)
  {
    stack_bst_push(to_visit, current);  // Uložení uzlu do zásobníku
    stack_bool_push(first_visit, true); // uzel je navštíven poprvé
    current = current->left;            // Pokračuje na levého potomka
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
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  // check existence stromu
  if (tree == NULL)
  {
    return;
  }

  stack_bst_t to_visit;          // Zásobník pro uzly, které je třeba zpracovat
  stack_bool_t first_visit;      // Zásobník pro bool hodnoty určené k určení prvního návštěvného uzlu
  stack_bst_init(&to_visit);     // Inicializace zásobníku uzlů
  stack_bool_init(&first_visit); // Inicializace zásobníku bool hodnot

  bst_leftmost_postorder(tree, &to_visit, &first_visit); // Procházení levé větve stromu

  // Iterativní procházení stromu
  while (!stack_bst_empty(&to_visit))
  {
    bst_node_t *current = stack_bst_pop(&to_visit);     // Vyjmutí uzlu ze zásobníku
    bool is_first_visit = stack_bool_pop(&first_visit); // Získání informace o první návštěvě

    // uzel navštíven poprvé, přidá jeho pravý podstrom do zásobníku
    if (is_first_visit)
    {
      stack_bst_push(&to_visit, current);   // vloží aktuální uzel do zásobníku
      stack_bool_push(&first_visit, false); // tento uzel bude navštíven po podstromu

      // uzel má pravý podstrom
      if (current->right != NULL)
      {
        bst_leftmost_postorder(current->right, &to_visit, &first_visit); // Procházení pravého podstromu
      }
    }
    else
    {
      // druhá návštěva uzlu, přidáme ho do výsledného seznamu
      bst_add_node_to_items(current, items); // Přidání uzlu do seznamu zpracovaných uzlů
    }
  }
}
