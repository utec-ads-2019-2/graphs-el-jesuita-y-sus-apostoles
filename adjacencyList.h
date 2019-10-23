#ifndef GRAPHS_ADJACENCYLIST_H
#define GRAPHS_ADJACENCYLIST_H

typedef double dataTypeOfAirport;

struct adjacencyListNode {
    dataTypeOfAirport data;
    dataTypeOfAirport weight;
    adjacencyListNode* next;
};

adjacencyListNode* newNode(dataTypeOfAirport weight, dataTypeOfAirport data) {
    auto* node = new adjacencyListNode;
    node->data = data;
    node->weight = weight;
    node->next = nullptr;
    return node;
}

struct adjacencyList {
    adjacencyListNode* head;
    adjacencyListNode* tail;
};

#endif //GRAPHS_ADJACENCYLIST_H
