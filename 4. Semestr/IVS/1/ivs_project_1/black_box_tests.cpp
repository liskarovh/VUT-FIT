//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Hana Liškařová <xliskah00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Hana Liškařová
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

// EmptyTree test cases
TEST(EmptyTree, InsertNode_SingleElement) {
    BinaryTree tree;
    auto result = tree.InsertNode(1);
    EXPECT_TRUE(result.first); //insertion shoul be sucesfull
}

TEST(EmptyTree, DeleteNode_NonExistentElement) {
    BinaryTree tree;
    EXPECT_FALSE(tree.DeleteNode(1)); //item should not be in the tree and should not be able to be deleted
}

TEST(EmptyTree, FindNode_NonExistentElement) {
    BinaryTree tree;
    EXPECT_EQ(tree.FindNode(1), nullptr); //node should not be found, null pointer should be returned
}

//NonEmpty tree cases
TEST(NonEmptyTree, InsertNode_MultipleElements) { //inserts multiple nodes into an astabilished tree
    BinaryTree tree; //creates tree
    tree.InsertNode(1); //inserts starting node
    EXPECT_TRUE(tree.InsertNode(2).first); //inserts second node, should return true
    EXPECT_TRUE(tree.InsertNode(5).first); //inserts third node, should return true
}

TEST(NonEmptyTree, DeleteNode_ExistingElement) {
    BinaryTree tree;
    tree.InsertNode(1); //inserts node
    EXPECT_TRUE(tree.DeleteNode(1)); //deletes existing node, should return true
}

TEST(NonEmptyTree, FindNode_ExistingElement) {
    BinaryTree tree;
    tree.InsertNode(1); //inserts node
    EXPECT_NE(tree.FindNode(1), nullptr); //finds existing node, should return pointer to the node
}


//AXIOMS
//red black tree axiom verification tests

//all leaves always black
TEST(TreeAxioms, Axiom1_AllLeavesAreBlack) {
    BinaryTree tree; //creates tree
    //loads tree with nodes
    tree.InsertNode(1);
    tree.InsertNode(2);
    tree.InsertNode(5);
    

    std::vector<BinaryTree::Node_t *> leafNodes; //creates a vector to store leaf nodes, each element is a pointer to a node
    tree.GetLeafNodes(leafNodes); //gets all leaf nodes from the tree and fills the vector with them
    
    //checks if all leaf nodes are black
    //node is recognized as a pointer to Node_t
    //node->color should be black for all nodes in vector
    for (auto node : leafNodes) {
        EXPECT_EQ(node->color, BinaryTree::BLACK);
    }
}

//children of red nodes are always black
TEST(TreeAxioms, Axiom2_RedNodeHasBlackChildren) {
    BinaryTree tree; //creates tree
    //loads tree with nodes
    tree.InsertNode(1);
    tree.InsertNode(2);
    tree.InsertNode(5);
    
    std::vector<BinaryTree::Node_t *> allNodes;
    tree.GetAllNodes(allNodes);
    
    //for each node check if is red
    
    for (auto node : allNodes) {
        if (node->color == BinaryTree::RED) {
            //if yes, check left child, should be black if exists
            if (node->pLeft) EXPECT_EQ(node->pLeft->color, BinaryTree::BLACK);
            //if yes, check right child, should be black if exists
            if (node->pRight) EXPECT_EQ(node->pRight->color, BinaryTree::BLACK);
        }
    }
}

//all paths from leaf to root have the same number of black nodes
TEST(TreeAxioms, Axiom3_BlackHeightIsConsistent) {
    BinaryTree tree;
    tree.InsertNode(1);
    tree.InsertNode(2);
    tree.InsertNode(5);
    
    std::vector<BinaryTree::Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);
    
    //stores expected black height - number of black nodes from leaf to root
    //set to -1 to indicate that it has not been set yet
    int blackHeight = -1;

    for (auto leaf : leafNodes) {
        int count = 0; //count of black nodes encountered
        BinaryTree::Node_t *current = leaf;

        //iterates until root is reached - current is a nullptr
        while (current) {
            if (current->color == BinaryTree::BLACK) { 
                count++;
            }
            current = current->pParent;
        }
        if (blackHeight == -1) {
            //first path being checked
            //sets height as reached first count
            blackHeight = count;
        } else {
            //expected height was set, compares with count of current branch
            //expects equal values
            EXPECT_EQ(count, blackHeight);
        }
    }
}



/*** Konec souboru black_box_tests.cpp ***/
