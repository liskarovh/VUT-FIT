#include <stddef.h>
#include "cnf.h"

//
// LOGIN: xliskah00
//

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 1)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
// Pole streets ma velikost num_of_streets a obsahuje vsechny existujuci ulice
//    - pro 0 <= i < num_of_streets predstavuje streets[i] jednu existujici
//      ulici od krizovatky streets[i].crossroad_from ke krizovatce streets[i].crossroad_to
void at_least_one_valid_street_for_each_step(CNF *formula, unsigned num_of_crossroads, unsigned num_of_streets, const Street *streets)
{
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
    assert(streets != NULL);

    // ZDE PRIDAT KOD - FUNGUJE
    for (unsigned i = 0; i < num_of_streets; i++) //pro každý krok postupu mapou
    {
        // vytvoření klauzule
        Clause *at_least_one = create_new_clause(formula);

        for (unsigned z = 0; z < num_of_crossroads; ++z)
        {
            for (unsigned k = 0; k < num_of_crossroads; ++k)
            {
                
                //ověření existence ulice
                for (int street_check = 0; street_check < num_of_streets; street_check++)
                {
                    if (streets[street_check].crossroad_from == z) 
                    {
                        if (streets[street_check].crossroad_to == k)
                        {
                            //přidání existující ulice do klauzule
                            add_literal_to_clause(at_least_one, true, i, z, k);
                        }
                    }
                }
            }
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 2)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void at_most_one_street_for_each_step(CNF *formula, unsigned num_of_crossroads, unsigned num_of_streets)
{
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);

    // ZDE PRIDAT KOD - FUNGUJE
    for (unsigned i = 0; i < num_of_streets; i++) // krok postupu
    {
        // vytvoření klauzule

        Clause *at_most_one = create_new_clause(formula);
        for (unsigned z = 0; z < num_of_crossroads; ++z)
        {

            for (unsigned k = 0; k < num_of_crossroads; ++k)
            {
                //přidání proměnné do klauzule
                add_literal_to_clause(at_most_one, false, i, z, k);

                for (unsigned higher_k = 0; higher_k < num_of_crossroads; ++higher_k)
                {

                    if (k >= higher_k)
                    {
                        //přidání druhé proměnné do klauzule
                        add_literal_to_clause(at_most_one, true, i, z, higher_k);
                    }
                }
            }
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 3)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void streets_connected(CNF *formula, unsigned num_of_crossroads, unsigned num_of_streets)
{
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);

    // ZDE PRIDAT KOD - FUNGUJE

    for (unsigned i = 0; i < num_of_streets; i++) //pro každý krok
    {

        for (unsigned z = 0; z < num_of_crossroads; ++z)
        {
            for (unsigned k = 0; k < num_of_crossroads; ++k)
            {

                for (unsigned higher_z = 0; higher_z < num_of_crossroads; ++higher_z)
                {
                    //proměnná pro začátek jiné ulice
                    for (unsigned higher_k = 0; higher_k < num_of_crossroads; ++higher_k)
                    {
                        //proměnná pro konec jiné ulice

                        if (higher_z != k) //pokud na sebe 2 ulice nenavazují
                        {

                            if (i + 1 <= num_of_streets) //zapezpečení rozmezí možných kroků u proměnné ulice
                            {
                                //vytvoření klauzule
                                Clause *connected1 = create_new_clause(formula);

                                //přidání proměnných ulic do klauzule
                                add_literal_to_clause(connected1, false, i, z, k);
                                add_literal_to_clause(connected1, false, i+1, higher_z, higher_k); //ulice o krok dál
                            }
                        }
                    }
                }
            }
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 4)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void streets_do_not_repeat(CNF *formula, unsigned num_of_crossroads, unsigned num_of_streets)
{
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);

    for (unsigned i = 0; i < num_of_streets; ++i)
    {
        // pro kazdy krok i
        for (unsigned j = 0; j < num_of_streets; ++j)
        {
            if (i != j)
            {
                // pro kazdy jiny krok j
                for (unsigned z = 0; z < num_of_crossroads; ++z)
                {
                    for (unsigned k = 0; k < num_of_crossroads; ++k)
                    {
                        // pro kazdu dvojici krizovatek (z, k)
                        Clause *cl = create_new_clause(formula);
                        add_literal_to_clause(cl, false, i, z, k);
                        add_literal_to_clause(cl, false, j, z, k);
                    }
                }
            }
        }
    }
}
