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
** uvedeno něco jiného.
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
void DLL_Error(void)
{
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
void DLL_Init(DLList *list)
{
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
	list->currentLength = 0;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose(DLList *list)
{
	DLLElementPtr currentElement = list->firstElement; // uloží první element do pomocné proměnné
	while (currentElement != NULL)					   // dokud má dll existující položky
	{
		DLLElementPtr nextElement = currentElement->nextElement; // uloží následující element
		free(currentElement);									 // uvolní pamět momentální položky
		currentElement = nextElement;
	}

	// uvedení do stavu po inicializaci
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
	list->currentLength = 0;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst(DLList *list, long data)
{
	DLLElementPtr newElementPtr = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // alokace prostoru pro nový prvek
	// kontrola úspěšnosti malloc
	if (newElementPtr == NULL)
	{
		DLL_Error(); // zavolání errorové funkce a exit z programu
		return;
	}
	newElementPtr->data = data;
	newElementPtr->nextElement = list->firstElement; // přenastavení pointeru na začátek dll
	newElementPtr->previousElement = NULL;			 // prvek je nyní první v listu

	// vložení do seznamu
	// seznam prázdný
	if (list->firstElement == NULL)
	{
		list->lastElement = newElementPtr; // prvek je v seznamu první a poslední
	}
	// seznam neprázdný
	else
	{
		list->firstElement->previousElement = newElementPtr; // propojí starý první prvek s novým
	}

	// korekce ukazatele začátku
	list->firstElement = newElementPtr;
	list->currentLength++;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast(DLList *list, long data)
{
	DLLElementPtr newElementPtr = (DLLElementPtr)malloc(sizeof(struct DLLElement)); // alokace prostoru pro nový prvek
	// kontrola úspěšnosti malloc
	if (newElementPtr == NULL)
	{
		DLL_Error(); // zavolání errorové funkce a exit z programu
		return;
	}
	newElementPtr->data = data;
	newElementPtr->previousElement = list->lastElement; // přenastavení pointeru na konec dll
	newElementPtr->nextElement = NULL;			 // prvek je nyní poslední v listu

	// vložení do seznamu
	// seznam prázdný
	if (list->firstElement == NULL)
	{
		list->firstElement = newElementPtr; // prvek je v seznamu první a poslední
	}
	// seznam neprázdný
	else
	{
		list->lastElement->nextElement = newElementPtr; // propojí starý poslední prvek s novým
	}

	// korekce ukazatele začátku
	list->lastElement = newElementPtr;
	list->currentLength++;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First(DLList *list)
{
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last(DLList *list)
{
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst(DLList *list, long *dataPtr)
{
	if (list->firstElement != NULL) // ověření existence prvního prvku
	{
		*dataPtr = list->firstElement->data;
		list->activeElement = list->firstElement;
	}
	else
	{
		DLL_Error(); // zavolání error funkce
		return;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast(DLList *list, long *dataPtr)
{
	if (list->lastElement != NULL) // ověření existence posledního prvku
	{
		*dataPtr = list->lastElement->data;
		list->activeElement = list->lastElement;
	}
	else
	{
		DLL_Error(); // zavolání error funkce
		return;
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst(DLList *list)
{
	DLLElementPtr elementToBeDeleted;

	// ošetření existence prvního prvku
	if (list->firstElement != NULL)
	{
		elementToBeDeleted = list->firstElement;
		// kontrola aktivity prvku co bude smazán
		if (list->activeElement == list->firstElement)
		{
			// je aktivní, aktivita se ruší
			list->activeElement = NULL;
		}

		// seznam měl pouze jeden prvek - vyprázdnění seznamu
		if (list->firstElement == list->lastElement)
		{
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else // seznam měl více prvků
		{
			list->firstElement = list->firstElement->nextElement; // překlenutí prvního indexu na následující položku
			list->firstElement->previousElement = NULL;			  // levý ukazatel prvního na prázdný
		}

		free(elementToBeDeleted); // smazání položky
		list->currentLength--;
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast(DLList *list)
{
	//kontrola existence posledního prvku
	if (list->lastElement != NULL)
	{
		DLLElementPtr elementToBeDeleted = list->lastElement; //uložení prvku k smazání
		//ověření aktivnosti posledního prvku
		if (list->activeElement == list->lastElement)
		{
			list->activeElement = NULL;
		}

		//ověření zda prvek nebyl jediný v seznamu
		if (list->lastElement == list->firstElement)
		{
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else
		{
			list->lastElement = list->lastElement->previousElement; // překlenutí posledního indexu na předchozí položku
			list->lastElement->nextElement = NULL;			  // pravý ukazatel posledního prvku na prázdný
		}
		
		free(elementToBeDeleted); // smazání položky
		list->currentLength--;
		
	}
	
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter(DLList *list)
{
	// kontrola, zda není rušen poslední prvek - ověření existence aktivního prvku
	if (list->activeElement != NULL)
	{
		// ověření, zda je co rušit
		if (list->activeElement->nextElement != NULL)
		{
			DLLElementPtr elementToBeDeleted = list->activeElement->nextElement; // ukazatel na rušený prvek
			list->activeElement->nextElement = elementToBeDeleted->nextElement;	 // překlenutí rušeného prvku, navázání pravého souseda

			// kontrola, zda nerušíme poslední prvek v seznamu
			if (elementToBeDeleted == list->lastElement)
			{
				list->lastElement = list->activeElement; // přesunutí koncového ukazatele na aktivní prvek
			}
			else
			{
				elementToBeDeleted->nextElement->previousElement = list->activeElement; // navázání prvku za zrušeným z leva na aktivní
			}

			free(elementToBeDeleted);
			list->currentLength--;
		}
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore(DLList *list)
{
	//kontrola, zda není rušen první prvek
	if (list->activeElement != NULL)
	{
		//kontrola existence rušeného prvku
		if (list->activeElement->previousElement != NULL)
		{
			DLLElementPtr elementToBeDeleted = list->activeElement->previousElement; //vytvoření ukazatele na rušený prvek
			list->activeElement->previousElement = elementToBeDeleted->previousElement; //překlenutí rušeného prvku, navázání levého souseda

			//kontrola, zda není rušen první prvek seznamu
			if (elementToBeDeleted == list->firstElement)
			{
				list->firstElement = list->activeElement; //první prvek seznamu je nyní aktivní prvek
			}
			else{
				elementToBeDeleted->previousElement->nextElement = list->activeElement; //navázání souseda z prava - prvek před zrušeným na aktivní
			}
			free(elementToBeDeleted); //zrušení prvku
			list->currentLength--;
		
		}
		
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
void DLL_InsertAfter(DLList *list, long data)
{
	// kontrola, zda existuje prvek za který se dá vložit (aktivní prvek)
	if (list->activeElement != NULL)
	{
		// alokace místa pro vkládaný prvek
		DLLElementPtr elementToBeInserted = (DLLElementPtr)malloc(sizeof(struct DLLElement));
		// kontrola úspěšnosti malloc
		if (elementToBeInserted == NULL)
		{
			DLL_Error(); // zavolání errorové funkce a exit z programu
			return;
		}
		elementToBeInserted->data = data;

		elementToBeInserted->nextElement = list->activeElement->nextElement;
		elementToBeInserted->previousElement = list->activeElement;

		// navázání nového prvku na předešlý (aktivní - levý soused)
		list->activeElement->nextElement = elementToBeInserted;

		// pokud byl aktivní prvek poslední
		if (list->activeElement == list->lastElement)
		{
			// korekce koncového ukazatele
			list->lastElement = elementToBeInserted;
		}
		else
		{
			// navázání pravého souseda na vložený prvek
			elementToBeInserted->nextElement->previousElement = elementToBeInserted;
		}
		list->currentLength++;
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
void DLL_InsertBefore(DLList *list, long data) //MOŽNÉ CHYBY
{
	//kontrola existence aktivního prvku
	if (list->activeElement != NULL)
	{
		// alokace místa pro vkládaný prvek
		DLLElementPtr elementToBeInserted = (DLLElementPtr)malloc(sizeof(struct DLLElement));
		// kontrola úspěšnosti malloc
		if (elementToBeInserted == NULL)
		{
			DLL_Error(); // zavolání errorové funkce a exit z programu
			return;
		}

		elementToBeInserted->data = data;
		elementToBeInserted->nextElement = list->activeElement;
		list->activeElement->previousElement = elementToBeInserted; //provázání z levé strany na aktivní prvek

		//kontrola, zda není vkládáno na první pozici seznamu
		if (list->activeElement == list->firstElement)
		{
			list->firstElement = elementToBeInserted; //vkládaný prvek se stává prvním v seznamu
			list->firstElement->previousElement = NULL; //nastavení levého souseda - prázdno
		}
		else //vkládáme doprostřed seznamu
		{
			list->activeElement->previousElement->nextElement = elementToBeInserted; //navázání z leva
		}
		list->currentLength++;
		

	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue(DLList *list, long *dataPtr)
{
	if (list->activeElement != NULL) // pokud je prvek aktivní
	{
		*dataPtr = list->activeElement->data;
	}
	else
	{
		DLL_Error(); // error, není aktivní prvek
		return;
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue(DLList *list, long data)
{
	//kontrola aktivity seznamu
	if (list->activeElement != NULL)
	{
		list->activeElement->data = data; //přiřazení hodnoty
	}
	
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next(DLList *list)
{
	//kontrola aktivity seznamu
	if (list->activeElement != NULL)
	{
		//kontrola existence dalšího prvku
		if (list->activeElement->nextElement != NULL) //if (list->activeElement != list->lastElement)
		{
			list->activeElement = list->activeElement->nextElement; //přesunutí aktivity na další prvek v pořadí
		}
		else
		{
			list->activeElement = NULL; //zrušení aktivity seznamu
		}
		
		
	}
	
}

/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous(DLList *list)
{
	//kontrola aktivity seznamu
	if (list->activeElement != NULL)
	{
		//kontrola existence předchozího prvku
		if (list->activeElement->previousElement != NULL) //if (list->activeElement != list->firstElement)
		{
			list->activeElement = list->activeElement->previousElement; //přesunutí aktivity na další prvek v pořadí
		}
		else
		{
			list->activeElement = NULL; //zrušení aktivity seznamu
		}
		
		
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
bool DLL_IsActive(DLList *list)
{
	return list->activeElement != NULL; // vrátí 1 při existenci aktivního prvku
}

/* Konec c206.c */
