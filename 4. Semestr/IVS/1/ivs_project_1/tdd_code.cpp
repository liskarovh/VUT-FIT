//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Hana Liškařová <xliskah00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"



//inicialization of the empty graph - default constructor
Graph::Graph(){}

//destructor of the graph - free alocated memory, clear the graph
Graph::~Graph(){
    clear();
}

//return vector of nodes
std::vector<Node*> Graph::nodes() {
    std::vector<Node*> nodes;
    nodes.reserve(node_map.size()); // Reserve memory to prevent reallocation

    for (auto& [id, node] : node_map) {
        //ads node pointer to the vector
        nodes.push_back(&node);
    }
    return nodes;
}

//return vector of edges
std::vector<Edge> Graph::edges() const{
    std::vector<Edge> edges;
    edges.reserve(edgeCount()); // Reserve memory to prevent reallocation

    //iterate over all nodes and their neighbors
    for (const auto& [node, neighbors] : adjacency_list) {
        for (const auto& neighbor : neighbors) {
            //avoid adding duplicate edges
            if (node < neighbor) {
                edges.emplace_back(node, neighbor);
            }
        }
    }
    return edges;
}

//add node to the graph
Node* Graph::addNode(size_t nodeId) {
    //if node with this id already exists, return nullptr - indication that it was not added
    if (node_map.find(nodeId) != node_map.end()) {
        return nullptr;
    }

    //add node to the map and adjacency list
    node_map.emplace(nodeId, Node{nodeId, 0}); //using emplace to avoid copying
    adjacency_list[nodeId];

    //return pointer to the node
    return &node_map[nodeId];
}

//add edge to the graph
bool Graph::addEdge(const Edge& edge){
    //if edge is a loop or already exists, return false - indication that it was not added
    if (edge.a == edge.b || adjacency_list[edge.a].count(edge.b)) {
        return false;
    }

    //add nodes if they do not exist
    addNode(edge.a);
    addNode(edge.b);

    //add edge to the adjacency list - add to the graph
    //store in both direction - graph is undirected
    adjacency_list[edge.a].insert(edge.b);
    adjacency_list[edge.b].insert(edge.a);

    return true;

}

void Graph::addMultipleEdges(const std::vector<Edge>& edges) {
    //for edge in edges - avoids copying edges
    for (const auto& edge : edges) {
        addEdge(edge);
    }
}

Node* Graph::getNode(size_t nodeId){
    //search for the node - if it exists, return pointer to the node, otherwise return nullptr
    auto node = node_map.find(nodeId);
    return node != node_map.end() ? &node->second : nullptr;
}

bool Graph::containsEdge(const Edge& edge) const{
    //check if edge exists in the graph
    //if edge.a is in the adjacency list and edge.b is in the set of neighbors of edge.a, return true
    return adjacency_list.count(edge.a) && adjacency_list.at(edge.a).count(edge.b);
}

void Graph::removeNode(size_t nodeId){
    //if node not existent, throw exception
    if (!node_map.count(nodeId)) {
        throw std::out_of_range("Node does not exist");
    }

    //remove node from the adjacency list
    //iterate over all neighbors of the node and remove the node from their neighbors
    for (auto neighbor : adjacency_list[nodeId]) {
        adjacency_list[neighbor].erase(nodeId);
    }

    //remove node from the node map and adjacency list - remove from the graph
    adjacency_list.erase(nodeId);
    node_map.erase(nodeId);
}

void Graph::removeEdge(const Edge& edge){
    //if edge not existent, throw exception
    if (!containsEdge(edge)) {
        throw std::out_of_range("Edge does not exist");
    }
    //remove edge from the adjacency list - remove from the graph
    adjacency_list[edge.a].erase(edge.b);
    adjacency_list[edge.b].erase(edge.a);
}

size_t Graph::nodeCount() const{
    //return number of nodes in the graph - size of the node map [nod emap does not contain duplicates]
    return node_map.size();
}

size_t Graph::edgeCount() const{
    size_t count = 0;
    //iterate over all nodes, count their neighbors and divide by 2 - each edge is counted twice
    for (const auto& [node, neighbors] : adjacency_list) {
        count += neighbors.size();
    }
    return count / 2;
}

//gives number of edges that have the node as one of their endpoints
size_t Graph::nodeDegree(size_t nodeId) const{
    //node exists? if not, throw exception
    if (!node_map.count(nodeId)) {
        throw std::out_of_range("Node does not exist");
    }
    //return number of neighbors of the node - size of the set of neighbors
    return adjacency_list.at(nodeId).size();
}

size_t Graph::graphDegree() const{
    //maximum degree found in the graph
    size_t max_degree = 0;

    //iterate over all nodes and find the maximum degree
    for (const auto& [node, neighbors] : adjacency_list) {
        //update max degree - compare with current max degree, if bigger, update
        max_degree = std::max(max_degree, neighbors.size());
    }
    return max_degree;
}

void Graph::coloring(){
    //reset all colors in every node to 0
    for (auto& node : node_map) {
        node.second.color = 0;
    }

    //iterate over all nodes
    for (auto& node : node_map) {
        //set of colors of neighbors
        std::unordered_set<size_t> neighbor_colors;

        //insert neigbouhrs colors to the set
        for (auto neighbor : adjacency_list[node.first]) {
            neighbor_colors.insert(node_map[neighbor].color);
        }

        //find the first color that is not in the set of neighbors colors
        size_t color = 1;
        while (neighbor_colors.count(color)) {
            ++color;
        }

        //set the color to the node
        node.second.color = color;
    }
}

void Graph::clear() {
    //remove all node data, release memory, map is now empty
    node_map.clear();

    //same but with ewdges
    adjacency_list.clear();
}

/*** Konec souboru tdd_code.cpp ***/
