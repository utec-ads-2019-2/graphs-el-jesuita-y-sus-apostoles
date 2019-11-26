//
// Created by Carlos Reátegui on 11/12/19.
//

#ifndef ADS_PROJECT_BELLMAN_FORD_H
#define ADS_PROJECT_BELLMAN_FORD_H

#include <vector>
#include "graph.h"

template <class T>
class BellmanFord
{
    vector<Edge<T> *> listOfEdgesOfGraph;
    int numberOfVertexes;
    map<int, double> distanceToNode;
    map<int, int> previous;
    list<Edge<T>* > *closestPathsEdges;
    vector<Edge<T>* > *closestPathsEdgesV;
    int idOfSourceVertex;

    vector<int> buildPath(int idOfNodeTo)
    {
        vector<int> currentPath;
        currentPath.insert(currentPath.begin(), idOfNodeTo);
        while (previous[idOfNodeTo] != idOfSourceVertex)
        {
            currentPath.insert(currentPath.begin(), previous[idOfNodeTo]);
            idOfNodeTo = previous[idOfNodeTo];
        }
        currentPath.insert(currentPath.begin(), idOfSourceVertex);
        return currentPath;
    }


    Edge<T>* findEdge(int idFrom, int idTo)
    {
        for (auto it = listOfEdgesOfGraph.begin(); it != listOfEdgesOfGraph.end(); ++it)
            if ((*it)->getTo()->getID() == idTo and (*it)->getFrom()->getID() == idFrom)
                return *it;
    }

public:
    BellmanFord(Graph<T> *graph, int idOfSourceVertex)
    {
        for (auto it = graph->getMap()->begin(); it != graph->getMap()->end(); ++it)
        {
            for (auto it2 = (*it).second->getEdges()->begin(); it2 != (*it).second->getEdges()->end(); ++it2)
                listOfEdgesOfGraph.push_back(*it2);
            distanceToNode[(*it).first] = INT_MAX;
        }

        numberOfVertexes = graph->getMap()->size();
        distanceToNode[idOfSourceVertex] = 0;

        closestPathsEdges = new list<Edge<T>* >;
        closestPathsEdgesV = new vector<Edge<T>* >;
        this->idOfSourceVertex = idOfSourceVertex;
    }

    vector<Edge<T> *> *getClosestPathsEdgesV() {
        for (auto it = previous.begin(); it != previous.end(); ++it)
        {
            vector<int> currentPath = buildPath((*it).first);
            for (unsigned long i = 0; i < currentPath.size() - 1; ++i)
                if (find(closestPathsEdgesV->begin(), closestPathsEdgesV->end(), findEdge(currentPath[i], currentPath[i + 1])) == closestPathsEdgesV->end())
                    closestPathsEdgesV->push_back(findEdge(currentPath[i], currentPath[i + 1]));
        }
        return closestPathsEdgesV;
    }

    void calculate()
    {
        for (int i = 0; i < numberOfVertexes; ++i)
        {
            bool somethingHasChanged = false;
            for (auto it = listOfEdgesOfGraph.begin(); it != listOfEdgesOfGraph.end(); ++it)
            {
                //cout << "from " << (*it)->getFrom()->getID() << " to " << (*it)->getTo()->getID() << " weight " << (*it)->getWeight() << endl;
                if (distanceToNode[(*it)->getTo()->getID()] > distanceToNode[(*it)->getFrom()->getID()] + (*it)->getWeight())
                {
                    distanceToNode[(*it)->getTo()->getID()] = distanceToNode[(*it)->getFrom()->getID()] + (*it)->getWeight();
                    previous[(*it)->getTo()->getID()] = (*it)->getFrom()->getID();
                    somethingHasChanged = true;
                }
            }
            if (i == numberOfVertexes - 1 and somethingHasChanged)
                throw invalid_argument ("There is a negative cycle in the graph");
        }
    }

    list<Edge<T> *> *getClosestPathsEdges()
    {
        for (auto it = previous.begin(); it != previous.end(); ++it)
        {
            vector<int> currentPath = buildPath((*it).first);
            for (unsigned long i = 0; i < currentPath.size() - 1; ++i)
                if (find(closestPathsEdges->begin(), closestPathsEdges->end(), findEdge(currentPath[i], currentPath[i + 1])) == closestPathsEdges->end())
                    closestPathsEdges->push_back(findEdge(currentPath[i], currentPath[i + 1]));
        }
        return closestPathsEdges;
    }

    void print()
    {
        cout << setw(15) << "node" << setw(15) << "weight" << endl;
        for (auto it : distanceToNode)
            cout << setw(15) << it.first << setw(15) << it.second << endl;
        cout << setw(15) << "node" << setw(15) << "predecessor" << endl;
        for (auto it : previous)
            cout << setw(15) << it.first << setw(15) << it.second << endl;
    }
};

#endif //ADS_PROJECT_BELLMAN_FORD_H
