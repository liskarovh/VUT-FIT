/*
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Rozšíření pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Daniel Dolejška, září 2024
 */

#include "c206-ext.h"

bool error_flag;
bool solved;

/**
 * Tato metoda simuluje příjem síťových paketů s určenou úrovní priority.
 * Přijaté pakety jsou zařazeny do odpovídajících front dle jejich priorit.
 * "Fronty" jsou v tomto cvičení reprezentovány dvousměrně vázanými seznamy
 * - ty totiž umožňují snazší úpravy pro již zařazené položky.
 *
 * Parametr `packetLists` obsahuje jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Pokud fronta s odpovídající prioritou neexistuje, tato metoda ji alokuje
 * a inicializuje. Za jejich korektní uvolnení odpovídá volající.
 *
 * V případě, že by po zařazení paketu do seznamu počet prvků v cílovém seznamu
 * překročil stanovený MAX_PACKET_COUNT, dojde nejdříve k promazání položek seznamu.
 * V takovémto případě bude každá druhá položka ze seznamu zahozena nehledě
 * na její vlastní prioritu ovšem v pořadí přijetí.
 *
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param packet Ukazatel na strukturu přijatého paketu
 */
void receive_packet(DLList *packetLists, PacketPtr packet)
{
	if (packet == NULL)
	{
		return; // Nic k zpracování
	}

	bool found = false;
	// zjištění a uložení priority packetu
	char priority_recieved = packet->priority;
	char priority_searched;

	// zjištění existence řady s stejnou prioritou
	// vytvoření proměnné hedaného elementu a přiřazení prvního elementu
	DLLElementPtr elementBeingSearched = packetLists->firstElement;
	long ptrToDataAsLong;

	// přetypování packetu na long
	long packetAsLong = (long)packet;

	while (elementBeingSearched != NULL)
	{
		// přiřazení dat na přetypování
		ptrToDataAsLong = elementBeingSearched->data;

		// PŘENESENÍ LONG NA POINTER - dostanu pointer na list
		QosPacketListPtr packetListPtr = (QosPacketListPtr)ptrToDataAsLong;

		// porovná prioritu s získano prioritou
		priority_searched = packetListPtr->priority;

		if (priority_recieved == priority_searched)
		{
			// byla nalezena fronta s danou priotou
			found = true;

			// kontrola plnosti seznamu
			if ((packetListPtr->list->currentLength +1) != MAX_PACKET_COUNT)
			{

				// packet se zařadí nakonec

				DLL_InsertLast(packetListPtr->list, packetAsLong);
			}
			else // promazání každého 2. prvku seznamu
			{
				// nastavení prvního prvku seznamu jako aktivní
				packetListPtr->list->activeElement = packetListPtr->list->firstElement;

// Smaže druhý prvek (2. packet)
if (packetListPtr->list->activeElement->nextElement != NULL) {
    DLL_DeleteAfter(packetListPtr->list);
}

// Smaže čtvrtý prvek (4. packet) - aktivujeme třetí prvek a smažeme následující
if (packetListPtr->list->activeElement->nextElement != NULL) {
    packetListPtr->list->activeElement = packetListPtr->list->activeElement->nextElement; // Přejít na třetí prvek
    if (packetListPtr->list->activeElement->nextElement != NULL) {
        DLL_DeleteAfter(packetListPtr->list);
    }
}

// Přijatý packet zařadí na konec
DLL_InsertLast(packetListPtr->list, packetAsLong);
			}

			return; // ukončení funkce
		}

		// posunutí se na další prvek v seznamu packetLists
		elementBeingSearched = elementBeingSearched->nextElement;
	}

	if (!found) // nebyla nalezena příslušná řada s prioritou
	{
		// vytvoření a dynamická alokace

		QosPacketListPtr newPacketList = (QosPacketListPtr)malloc(sizeof(QosPacketList));
		if (newPacketList == NULL)
		{
			fprintf(stderr, "malloc failed");
        	exit(1);
		}
		// přetypování na long

		long newPacketListLong = (long)newPacketList;

		// zařazení nového prvku do seznamu packetLists
		
		//kontrola, zda existuje list s menší prioritou
		bool foundLessPriority = false;
		while (elementBeingSearched != NULL)
		{
			packetLists->activeElement = elementBeingSearched;
			// přiřazení dat na přetypování
			ptrToDataAsLong = elementBeingSearched->data;

			// PŘENESENÍ LONG NA POINTER - dostanu pointer na list
			QosPacketListPtr packetListPtr = (QosPacketListPtr)ptrToDataAsLong;

			// porovná prioritu s získano prioritou
			priority_searched = packetListPtr->priority;
		
		
			if (priority_searched<priority_recieved)
			{
				
				//existuje, insert před něj
				DLL_InsertBefore(packetLists, newPacketListLong);
				foundLessPriority = true;
				break;
			}
		
			// posunutí se na další prvek v seznamu packetLists
			elementBeingSearched = elementBeingSearched->nextElement;
		}
		if (!foundLessPriority)
		{
			//neexistuje, insert nakonec
			DLL_InsertLast(packetLists, newPacketListLong);
		}
		
		

		// inicializace
		newPacketList->priority = priority_recieved;
		//alokace pro danou frontu
		newPacketList->list = (DLList *)malloc(sizeof(DLList));
		//ověření úspěšnosti mallocu
		if (newPacketList->list == NULL)
		{
			fprintf(stderr, "malloc failed");
        	exit(1);
		}
		DLL_Init(newPacketList->list);

		// vložení packetu do nově vytvořené fronty
		DLL_InsertLast(newPacketList->list, packetAsLong);
	}
}

/**
 * Tato metoda simuluje výběr síťových paketů k odeslání. Výběr respektuje
 * relativní priority paketů mezi sebou, kde pakety s nejvyšší prioritou
 * jsou vždy odeslány nejdříve. Odesílání dále respektuje pořadí, ve kterém
 * byly pakety přijaty metodou `receive_packet`.
 *
 * Odeslané pakety jsou ze zdrojového seznamu při odeslání odstraněny.
 *
 * Parametr `packetLists` obsahuje ukazatele na jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Parametr `outputPacketList` obsahuje ukazatele na odeslané pakety (`PacketPtr`).
 *
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param outputPacketList Ukazatel na seznam paketů k odeslání
 * @param maxPacketCount Maximální počet paketů k odeslání
 */

void send_packets(DLList *packetLists, DLList *outputPacketList, int maxPacketCount)
{
    // Kontrola platnosti vstupních parametrů
    if (packetLists == NULL || outputPacketList == NULL || maxPacketCount <= 0)
    {
        return; // Neplatný vstup
    }

    int sentPackets = 0;
    DLLElementPtr elementBeingSearched = packetLists->firstElement; // Ukazatel na data s listem

    // Dokud existují fronty paketů k zpracování
    while (elementBeingSearched != NULL && sentPackets < maxPacketCount) 
    {
        long ptrToDataAsLong = elementBeingSearched->data;
        QosPacketListPtr packetListPtr = (QosPacketListPtr)ptrToDataAsLong;

        DLLElementPtr currentElementInQueue = packetListPtr->list->firstElement;

        // Dokud existují pakety k odeslání
        while (currentElementInQueue != NULL && sentPackets < maxPacketCount) 
        {
            long packetToBeSentDataLong = currentElementInQueue->data;
            DLL_InsertLast(outputPacketList, packetToBeSentDataLong); // Vložení paketu do výstupního seznamu

            currentElementInQueue = currentElementInQueue->nextElement; // Přechod na další paket

            // Pokud jsme dosáhli konce fronty, přejděme na další frontu paketů
            if (currentElementInQueue == NULL && packetListPtr->list->lastElement == currentElementInQueue)
            {
                break; // Přerušení, abychom zpracovali další frontu paketů
            }

            sentPackets++; // Inkrementace počtu odeslaných paketů
        }

        elementBeingSearched = elementBeingSearched->nextElement; // Přejdeme na další frontu paketů
    }
}
