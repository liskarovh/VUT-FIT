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
int get_hash(char *key)
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++)
  {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table)
{
  for (int i = 0; i < HT_SIZE; i++)
  {
    (*table)[i] = NULL; // inicializace jednotlivých políček tabulky jako null
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  int index = get_hash(key);         // vypočítání indexu daného klíče
  ht_item_t *item = (*table)[index]; // získání prvku na daném indexu

  // iterace přes řetězec synonym, v případě že existuje
  while (item != NULL)
  {
    if (strcmp(item->key, key) == 0)
    {              // Klíč je stejný
      return item; // Vrátime nalezený prvek
    }
    item = item->next; // skip na další prvek
  }
  return NULL; // klíč nebyl nalezen
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value)
{
  int index = get_hash(key);               // Vypočítání hashe pro klíč
  ht_item_t *item = ht_search(table, key); // vyhledání existujícího prvku s daným klíčem

  if (item != NULL)
  { // Prvek s daným klíčem existuje ->aktualizace hodnoty
    item->value = value;
    return;
  }
  else // prvek neexistuje - alokace a vytvoření
  {
    item = (ht_item_t *)malloc(sizeof(ht_item_t));
    if (item == NULL)
    {
      // Chyba při alokaci paměti

      return;
    }

    // nastavení hodnot nového prvku
    // Duplikace klíče
    item->key = (char *)malloc(strlen(key) + 1); // Alokace paměti pro klíč
    if (item->key == NULL)
    {
      return; // Pokud alokace selže, ukončit funkci
    }
    strcpy(item->key, key); // Zkopíruje klíč do nově alokované paměti

    item->value = value;
    item->next = (*table)[index]; // Nový prvek přidat na začátek seznamu
    (*table)[index] = item;       // Nastavení prvního prvku tabulky na náš insertnutý
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
float *ht_get(ht_table_t *table, char *key)
{
  // vyhledání prvku pomocí htsearch
  ht_item_t *item = ht_search(table, key);
  if (item != NULL)
  {
    return &item->value; // Pokud prvek existuje, vrátíme ukazatel na jeho hodnotu
  }
  else
  {
    return NULL; // prvek neexistuje, vrací NULL
  }
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
  int index = get_hash(key);         // hash index pro daný klíč
  ht_item_t *item = (*table)[index]; // První prvek na tomto indexu
  ht_item_t *prev = NULL;            // uložení předchozího prvku

  while (item != NULL)
  {
    if (strcmp(item->key, key) == 0)
    { // Pokud najde prvek se shodným klíčem
      if (prev == NULL)
      {
        // Pokud je prvek první v seznamu
        (*table)[index] = item->next; // Ukazatel na tento index se změní na další prvek
      }
      else
      {
        prev->next = item->next; // Propojení předchozího prvku na následující
      }

      // Uvolnění paměti
      free(item->key); // Uvolnění klíče
      free(item);      // Uvolnění prvku
      return;          // Prvek je smazán
    }

    prev = item;       // předchozí prvek na aktuální
    item = item->next; // Přesuneme se na další prvek
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table)
{
  // pro každý prvek tabulky
  for (int i = 0; i < HT_SIZE; i++)
  {
    // pomocná proměnná item na uložení aktuálního prvku tabulky
    ht_item_t *item = (*table)[i];
    while (item != NULL)
    {
      ht_item_t *next = item->next; // pomocné úložení následujícího prvku
      free(item->key);              // Uvolnění klíče
      free(item);                   // Uvolnění samotného prvku
      item = next;                  // přesunutí na další prvek
    }
    (*table)[i] = NULL; // Nastaveí ukazatele na NULL pro daný index
  }
}
