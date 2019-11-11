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
    vector<vector<int>* > *sequenceMatrix;
    vector<int> idsOfNodes;
    unsigned int numberOfVertexes;
public:
    explicit FloydWarshall(Graph<T>* graph) {
        if (!graph->setIsNotDirected())
            throw invalid_argument("Cannot perform Floyd Warshall's algorithm on directed graphs");

        for (auto it : *graph->getMap())
            idsOfNodes.push_back(it.first);

        adjacencyMatrix = new vector<vector<float>* >(graph->getMap()->size());
        sequenceMatrix = new vector<vector<int>* >(graph->getMap()->size());
        for (int i = 0; i < graph->getMap()->size(); ++i) {
            (*adjacencyMatrix)[i] = new vector<float>(graph->getMap()->size(), INT_MAX);
            (*sequenceMatrix)[i] = new vector<int>(graph->getMap()->size(), INT_MAX);
        }

        for (int i = 0; i < graph->getMap()->size(); ++i) {
            (*adjacencyMatrix)[i]->at(i) = INT_MAX;
            if (!(*graph->getMap())[idsOfNodes[i]]->getEdges()->empty()) {
                for (auto it = (*graph->getMap())[idsOfNodes[i]]->getEdges()->begin();
                     it != (*graph->getMap())[idsOfNodes[i]]->getEdges()->end(); ++it) {
                    vector<int>::iterator vectorIt = find(idsOfNodes.begin(), idsOfNodes.end(),
                            (*it)->getTo()->getID());
                    (*adjacencyMatrix)[i]->at(distance(idsOfNodes.begin(), vectorIt)) = (*it)->getWeight();
                }
            }
            for (int j = 0; j < graph->getMap()->size(); ++j)
                i == j ? (*sequenceMatrix)[i]->at(j) = INT_MAX : (*sequenceMatrix)[i]->at(j) = idsOfNodes[j];
        }

        numberOfVertexes = graph->getMap()->size();
    }

    void calculate() {
        //float adjacencyMatrixForIterations[numberOfVertexes][numberOfVertexes];
        //int sequenceMatrixForIterations[numberOfVertexes][numberOfVertexes];

        vector<vector<float>* > *adjacencyMatrixForIterations = new vector<vector<float>* >(numberOfVertexes);
        vector<vector<int>* >* sequenceMatrixForIterations = new vector<vector<int>* >(numberOfVertexes);
        for (int i = 0; i < numberOfVertexes; ++i) {
            (*adjacencyMatrixForIterations)[i] = new vector<float>(numberOfVertexes);
            (*sequenceMatrixForIterations)[i] = new vector<int>(numberOfVertexes);
        }

        for (int i = 0; i < numberOfVertexes; i++) {
            for (int j = 0; j < numberOfVertexes; j++) {
                if (i == j) {
                    (*adjacencyMatrixForIterations)[i]->at(j) = INT_MAX;
                    (*sequenceMatrixForIterations)[i]->at(j) = INT_MAX;
                } else {
                    (*adjacencyMatrixForIterations)[i]->at(j) = 0;
                    (*sequenceMatrixForIterations)[i]->at(j) = 0;
                }
            }
        }

        for (int iterationNumber = 0; iterationNumber < numberOfVertexes; ++iterationNumber) {
            for (int i = 0; i < numberOfVertexes; ++i) {
                (*adjacencyMatrixForIterations)[iterationNumber]->at(i) = (*adjacencyMatrix)[iterationNumber]->at(i);
                (*adjacencyMatrixForIterations)[i]->at(iterationNumber) = (*adjacencyMatrix)[i]->at(iterationNumber);
                (*sequenceMatrixForIterations)[iterationNumber]->at(i) = (*sequenceMatrix)[iterationNumber]->at(i);
                (*sequenceMatrixForIterations)[i]->at(iterationNumber) = (*sequenceMatrix)[i]->at(iterationNumber);
            }

            for (int i = 0; i < numberOfVertexes; ++i) {
                if (i == iterationNumber) continue;
                for (int j = 0; j < numberOfVertexes; ++j) {
                    if (j == iterationNumber) continue;
                    if (i == j) continue;
                    if((*adjacencyMatrix)[i]->at(j) > (*adjacencyMatrix)[i]->at(iterationNumber) + (*adjacencyMatrix)[iterationNumber]->at(j)) {
                        (*adjacencyMatrixForIterations)[i]->at(j) = (*adjacencyMatrix)[i]->at(iterationNumber) + (*adjacencyMatrix)[iterationNumber]->at(j);
                        (*sequenceMatrixForIterations)[i]->at(j) = iterationNumber + 1;
                    } else {
                        (*adjacencyMatrixForIterations)[i]->at(j) = (*adjacencyMatrix)[i]->at(j);
                        (*sequenceMatrixForIterations)[i]->at(j) = (*sequenceMatrix)[i]->at(j);
                    }
                }
            }

            for(int i = 0; i < numberOfVertexes; ++i) {
                for(int j = 0; j < numberOfVertexes; ++j) {
                    (*adjacencyMatrix)[i]->at(j) = (*adjacencyMatrixForIterations)[i]->at(j);
                    (*sequenceMatrix)[i]->at(j) = (*sequenceMatrixForIterations)[i]->at(j);
                }
            }
        }
    }

    void printAdjacencyMatrix() {
        for (auto & it : *adjacencyMatrix) {
            for (float & it2 : *it) {
                cout << setw(5);
                if (it2 == INT_MAX)
                    cout << "INF";
                else
                    cout << it2;
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void printSequenceMatrix() {
        for (auto & it : *sequenceMatrix) {
            for (int & it2 : *it) {
                cout << setw(5);
                if (it2 == INT_MAX)
                    cout << "INF";
                else
                    cout << it2;
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

#endif //ADS_PROJECT_FLOYD_WARSHALL_H
