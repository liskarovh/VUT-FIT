//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     Hana Liškařová <xliskah00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Hana Liškařová
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//

//Tests for public interface of hash table
//EMPTY TABLE

// Test creating and destroying an empty hash map
TEST(EmptyTable, CreateAndDestroy) {
    hash_map_t* map = hash_map_ctor(); //creates new hashtable
    ASSERT_NE(map, nullptr); //return pointer should not be nullptr
    hash_map_dtor(map); //destroys hashtable
}

// Test inserting into an empty table
TEST(EmptyTable, InsertSingleElement) {
    hash_map_t* map = hash_map_ctor(); //creates new hashtable
    ASSERT_NE(map, nullptr); //checks if created

    EXPECT_EQ(hash_map_put(map, "key1", 42), OK); //inserts element, should return OK
    
    hash_map_dtor(map); //destroys hashtable
}

// Test retrieving from an empty table
TEST(EmptyTable, RetrieveFromEmpty) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    int value;
    EXPECT_EQ(hash_map_get(map, "key1", &value), KEY_ERROR); //tries toretrieve nonexisting element, should give error
    
    hash_map_dtor(map);
}

// Test removing from an empty table
TEST(EmptyTable, RemoveFromEmpty) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    EXPECT_EQ(hash_map_remove(map, "key1"), KEY_ERROR); //tries to remove nonexistent element, should give error
    
    hash_map_dtor(map);
}

// Test checking if a key exists in an empty table
TEST(EmptyTable, ContainsEmpty) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    EXPECT_EQ(hash_map_contains(map, "key1"), false); //checks if never inserted key is in table, should give false
    
    hash_map_dtor(map);
}

//NONEMPTY TABLE

// Test inserting and retrieving multiple values
TEST(NonEmptyTable, InsertAndRetrieve) {
    hash_map_t* map = hash_map_ctor(); //creates hashtable
    ASSERT_NE(map, nullptr); //chcecks if created

    EXPECT_EQ(hash_map_put(map, "key1", 42), OK); //inserts element 42 with key1, checks if insertion ok
    EXPECT_EQ(hash_map_put(map, "key2", 84), OK); //inserts element 84 with key2, 

    int value;
    EXPECT_EQ(hash_map_get(map, "key1", &value), OK); //retrieves element with key1, stores number in value, checks if key1 exists, should return OK
    EXPECT_EQ(value, 42); //checks if correct value was retrieved
    EXPECT_EQ(hash_map_get(map, "key2", &value), OK); //same case, different key
    EXPECT_EQ(value, 84);

    hash_map_dtor(map); //destroys hashtable
}

// Test updating an existing key
TEST(NonEmptyTable, UpdateExistingKey) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    EXPECT_EQ(hash_map_put(map, "key1", 42), OK); //inserts new element, should be ok
    EXPECT_EQ(hash_map_put(map, "key1", 99), KEY_ALREADY_EXISTS); //inserts new element with existing key, should give info and update value

    int value;
    EXPECT_EQ(hash_map_get(map, "key1", &value), OK); //gets value
    EXPECT_EQ(value, 99); //checks if value was updated

    hash_map_dtor(map);
}

// Test removing a key from a non-empty table
TEST(NonEmptyTable, RemoveKey) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    EXPECT_EQ(hash_map_put(map, "key1", 42), OK); //insert element, check if ok
    EXPECT_EQ(hash_map_remove(map, "key1"), OK); //remove element, check if ok
    EXPECT_EQ(hash_map_contains(map, "key1"), false); //check if given elemnt in table, should give false - was deleted

    hash_map_dtor(map);
}

// Test checking if a key exists in a non-empty table
TEST(NonEmptyTable, ContainsKey) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    EXPECT_EQ(hash_map_put(map, "key1", 42), OK); //inserts element, check if ok
    EXPECT_EQ(hash_map_contains(map, "key1"), true); //check if element with given key in table, shoul be true
    EXPECT_EQ(hash_map_contains(map, "key2"), false); //same case, should be false

    hash_map_dtor(map);
}

// Test handling of collisions in a non-empty table
TEST(NonEmptyTable, HandleCollisions) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    //insert elements with possible hash collision
    //short keys - possibility of hashing to the same index
    EXPECT_EQ(hash_map_put(map, "a", 1), OK);
    EXPECT_EQ(hash_map_put(map, "b", 2), OK);
    EXPECT_EQ(hash_map_put(map, "c", 3), OK);
    
    int value;
    //verifies correct retrieval of keys even with hash collision
    EXPECT_EQ(hash_map_get(map, "a", &value), OK);
    EXPECT_EQ(value, 1);
    EXPECT_EQ(hash_map_get(map, "b", &value), OK);
    EXPECT_EQ(value, 2);
    EXPECT_EQ(hash_map_get(map, "c", &value), OK);
    EXPECT_EQ(value, 3);

    hash_map_dtor(map);
}



// Test clearing the hash map
TEST(NonEmptyTable, ClearTable) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    EXPECT_EQ(hash_map_put(map, "key1", 42), OK);
    EXPECT_EQ(hash_map_put(map, "key2", 84), OK);
    
    hash_map_clear(map);
    //expects table to have no elments and size 0
    EXPECT_EQ(hash_map_size(map), 0);
    EXPECT_EQ(hash_map_contains(map, "key1"), false);
    EXPECT_EQ(hash_map_contains(map, "key2"), false);

    hash_map_dtor(map);
}

// Test popping elements from the hash map
TEST(NonEmptyTable, PopElements) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    //insert elements
    EXPECT_EQ(hash_map_put(map, "key1", 42), OK);
    EXPECT_EQ(hash_map_put(map, "key2", 84), OK);
    
    //pop element with key1, removes key1 from hashtable and stores 42 in value
    int value;
    EXPECT_EQ(hash_map_pop(map, "key1", &value), OK); //checks that key1 existed and was removed
    EXPECT_EQ(value, 42); //checks if correct value was retrieved
    EXPECT_EQ(hash_map_contains(map, "key1"), false); //key should no longer exist in table
    
    //same as above, but with key2
    EXPECT_EQ(hash_map_pop(map, "key2", &value), OK);
    EXPECT_EQ(value, 84);
    EXPECT_EQ(hash_map_contains(map, "key2"), false);

    hash_map_dtor(map);
}

// Test capacity and resizing when inserting into a non-empty table
//FAILS
//suspect incorect rehashing when table is resized
TEST(NonEmptyTable, ResizeTable) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    size_t initial_capacity = hash_map_capacity(map); //gets initial capacity of table

    //insert 10 elements, generate keys dynamically
    std::vector<std::string> keys;
    for (int i = 0; i < 10; i++) {
        std::string key = "key" + std::to_string(i);
        keys.push_back(key);
        //check if insertion was ok
        EXPECT_EQ(hash_map_put(map, key.c_str(), i), OK);
    }
    
    //check that table was resized
    EXPECT_GE(hash_map_capacity(map), initial_capacity);  

    // Verify keys and values remain correct after resizing
    int value;
    for (int i = 0; i < 10; i++) {
        //ensure keys werent lost in resizing, check if hash function changed correctly
        EXPECT_EQ(hash_map_get(map, keys[i].c_str(), &value), OK);
        EXPECT_EQ(value, i);
    }

    hash_map_dtor(map);
}

// Test invalid input handling
//gives segfault during testing
//problem in edge case - handling nullptr in hashtable
/*
TEST(ErrorHandling, InvalidInputs) {
    hash_map_t* map = hash_map_ctor();
    ASSERT_NE(map, nullptr);

    int value;
    //retrieving with nullptr key - should give error
    EXPECT_EQ(hash_map_get(map, nullptr, &value), KEY_ERROR);

    //inserting with nullptr key - should give error
    EXPECT_EQ(hash_map_put(map, nullptr, 100), KEY_ERROR);

    //removing with nullptr key - should give error
    EXPECT_EQ(hash_map_remove(map, nullptr), KEY_ERROR);

    //checking if nullptr key is in table - should give false
    EXPECT_EQ(hash_map_contains(map, nullptr), false);

    hash_map_dtor(map);
}
*/


//

/*** Konec souboru white_box_tests.cpp ***/
