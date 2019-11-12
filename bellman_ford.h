//
// Created by Carlos Reátegui on 11/12/19.
//

#ifndef ADS_PROJECT_BELLMAN_FORD_H
#define ADS_PROJECT_BELLMAN_FORD_H

#include <vector>
#include "graph.h"

template <class T>
class BellmanFord {
    vector<float> *distances;
    vector<int> *predecessors;
    int sourceVertex;
    vector<Edge<T>* > *listOfEdgesOfGraph;
    vector<int> idsOfNodes;
    unsigned int numberOfVertexes;
public:
    BellmanFord(Graph<T>* graph, int sourceVertex) {
        idsOfNodes.push_back(sourceVertex);
        for (auto it : *graph->getMap())
            if (it.first != sourceVertex)
                idsOfNodes.push_back(it.first);

        this->sourceVertex = sourceVertex;
        distances = new vector<float>(graph->getMap()->size(), INT_MAX);
        predecessors = new vector<int>(graph->getMap()->size(), 0);
        listOfEdgesOfGraph = new vector<Edge<T>* >;

        (*distances)[0] = 0;

        for (auto it = graph->getMap()->begin(); it != graph->getMap()->end(); ++it)
            if (!it->second->getEdges()->empty())
                for (auto it2 = it->second->getEdges()->begin(); it2 != it->second->getEdges()->end(); ++it2)
                    listOfEdgesOfGraph->push_back(*it2);

        numberOfVertexes = graph->getMap()->size();
    }

    void calculate() {
        for (int i = 0; i < numberOfVertexes; ++i) {
            bool somethingHasChangedInThisIteration = false;
            for (auto it = listOfEdgesOfGraph->begin(); it != listOfEdgesOfGraph->end(); ++it) {
                int from = (*it)->getFrom()->getID(), to = (*it)->getTo()->getID(),
                indexOfNodeFrom = distance(idsOfNodes.begin(), find(idsOfNodes.begin(), idsOfNodes.end(), from)),
                indexOfNodeTo = distance(idsOfNodes.begin(), find(idsOfNodes.begin(), idsOfNodes.end(), to));
                if ((*distances)[indexOfNodeTo] > (*distances)[indexOfNodeFrom] + (*it)->getWeight()) {
                    somethingHasChangedInThisIteration = true;
                    distances->at(indexOfNodeTo) = (*distances)[indexOfNodeFrom] + (*it)->getWeight();
                    predecessors->at(indexOfNodeTo) = from;
                }
            }
            if (i == numberOfVertexes - 1 and somethingHasChangedInThisIteration)
                throw invalid_argument("There is a negative cycle in this graph.");
        }
    }

    void print() {
        cout << setw(15) << "IDs";
        for (int i = 0; i < numberOfVertexes; ++i)
            cout << setw(5) << idsOfNodes[i];

        cout << endl << setw(15) << "Distances";

        for (float & distance : *distances)
            cout << setw(5) << distance;

        cout << endl << setw(15) << "Predecessors";

        for (int & id : *predecessors)
            cout << setw(5) << id;

        cout << endl;
    }

    ~BellmanFord() {
        distances->clear();
        predecessors->clear();
        idsOfNodes.clear();
    }
};

#endif //ADS_PROJECT_BELLMAN_FORD_H
