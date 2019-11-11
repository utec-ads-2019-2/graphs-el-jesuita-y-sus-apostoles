//
// Created by Carlos Reátegui on 11/10/19.
//

#ifndef ADS_PROJECT_FLOYD_WARSHALL_H
#define ADS_PROJECT_FLOYD_WARSHALL_H

#include "graph.h"
#include <iomanip>

template <class T>
class FloydWarshall {
    vector<vector<float>* > *adjacencyMatrix;
    vector<int> idsOfNodes;
public:
    explicit FloydWarshall(Graph<T>* graph) {
        if (!graph->setIsNotDirected())
            throw invalid_argument("Cannot perform Floyd Warshall's algorithm on directed graphs");

        for (auto it : *graph->getMap())
            idsOfNodes.push_back(it.first);

        adjacencyMatrix = new vector<vector<float>* >(graph->getMap()->size());
        for (int i = 0; i < graph->getMap()->size(); ++i)
            (*adjacencyMatrix)[i] = new vector<float>(graph->getMap()->size(), 0);

        for (int i = 0; i < graph->getMap()->size(); ++i) {
            if (!(*graph->getMap())[idsOfNodes[i]]->getEdges()->empty()) {
                for (auto it = (*graph->getMap())[idsOfNodes[i]]->getEdges()->begin();
                     it != (*graph->getMap())[idsOfNodes[i]]->getEdges()->end(); ++it) {
                    vector<int>::iterator vectorIt = find(idsOfNodes.begin(), idsOfNodes.end(),
                            (*it)->getTo()->getID());
                    (*adjacencyMatrix)[i]->at(distance(idsOfNodes.begin(), vectorIt)) = (*it)->getWeight();
                }
            }
        }
    }

    void printAdjacencyMatrix() {
        for (auto it = adjacencyMatrix->begin(); it != adjacencyMatrix->end(); ++it) {
            for (auto it2 = (*it)->begin(); it2 != (*it)->end(); ++it2) {
                cout << setw(5) << *it2 << " ";
            }
            cout << endl;
        }
    }
};

#endif //ADS_PROJECT_FLOYD_WARSHALL_H
