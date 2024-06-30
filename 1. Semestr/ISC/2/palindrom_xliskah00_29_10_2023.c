#include <stdio.h> //načtení potřebných knihoven
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int input_load_treat(char user_input[]) //funkce načítající a ošetřující uživatelský vstup
{
    printf("Zadej slovo či větu, program rozhodne zda je symetrické:\n"); //upozornění pro uživatele
    printf("Věta musí mít maximální délku 15 znaků, znaky navíc budou ignorovány. Mezera je platný znak.\n");

    fgets(user_input, 16, stdin); //načtení vstupu
    user_input[strcspn(user_input, "\n")] = '\0'; // oddělání znaku nového řádku
    printf("Zadané slovo, se kterým program bude pracovat: \n");
    int check_print = 0; //pomocná proměnná pro kontrolu tisknutelnosti

    for (int i = 0; user_input[i] != '\0'; i++) // ověření, že byly zadány pouze a jen tisknutelné znaky
    {
        if (user_input[i] >= 32 && user_input[i] <= 126)
        {
            // změní všechna písmena v zadané větě na lowercase
            user_input[i] = tolower(user_input[i]);
            printf("%c", user_input[i]);
            
        }
        else
        {
            printf("  POZOR - Slovo nelze vypsat celé. \nByl zadán netisknutelný znak, prosím o zadání pouze tisknutelných znaků.\nPro nové zadání spusťte program znovu\n");
            check_print = 1;
            break;
            
        }
    }
    printf("\n");
    return check_print; //vrací hodnotu kontrolující, zda jsou všechny znaky tisknutelné
}

void compare(char user_input[]) //funkce na rozpoznání palindromu
{

    int n = (strlen(user_input) + 1); // pomocná proměnná rovná délce zadané věty od uživatele + koncový znak
    int check = 0;                    // pomocná proměnná pro vyhodnocenní palindromu
    int i, j;

    for ((i = 0, j = n - 1); (n / 2 >= i); i++, j--)
    {
        // postupné procházení věty z obou konců, zastaví se při narazení na rozdíl
        if (user_input[i] != user_input[j - 1])
        {
            printf("Zadané slovo NENÍ symetrické : '%c' (index %i) není stejné jako '%c' (index %i)", user_input[i], i, user_input[j - 1], (j - 1));
            break;
        }
        else
        {
            check = check + 1;
        }
    }

    if (check >= n / 2)
    {
        printf("Zadané slovo JE symetrické");
    }
    printf("\n");
}

int main()
{
    char user_input[16]; // pole pro nahrátí vstupu od uživatele
    int printability_check = input_load_treat(user_input);
    if (printability_check == 0) //provedení porovnání, pouze pokud je vstup bez chyby
    {
        compare(user_input);
    }

    return 0;
}