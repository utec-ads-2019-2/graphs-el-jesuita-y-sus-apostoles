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
    vector<float> *distances;
    vector<int> *predecessors;
    int sourceVertex;
    vector<Edge<T>* > *listOfEdgesOfGraph;
    vector<int> idsOfNodes;
    unsigned int numberOfVertexes;
    Graph<T> *graph;
public:
    BellmanFord(Graph<T>* graph, int sourceVertex)
    {
        this->graph = graph;
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

    void calculate()
    {
        for (int i = 0; i < numberOfVertexes; ++i)
        {
            bool somethingHasChangedInThisIteration = false;
            for (auto it = listOfEdgesOfGraph->begin(); it != listOfEdgesOfGraph->end(); ++it)
            {
                int from = (*it)->getFrom()->getID(), to = (*it)->getTo()->getID(),
                indexOfNodeFrom = distance(idsOfNodes.begin(), find(idsOfNodes.begin(), idsOfNodes.end(), from)),
                indexOfNodeTo = distance(idsOfNodes.begin(), find(idsOfNodes.begin(), idsOfNodes.end(), to));
                if ((*distances)[indexOfNodeTo] > (*distances)[indexOfNodeFrom] + (*it)->getWeight())
                {
                    somethingHasChangedInThisIteration = true;
                    distances->at(indexOfNodeTo) = (*distances)[indexOfNodeFrom] + (*it)->getWeight();
                    predecessors->at(indexOfNodeTo) = from;
                }
            }
            if (i == numberOfVertexes - 1 and somethingHasChangedInThisIteration)
                throw invalid_argument("There is a negative cycle in this graph.");
        }
    }

    list<Edge<T> *> *getShortestPaths()
    {
        list<Edge<T> *> *shortestPaths = new list<Edge<T> *>;
        for (int i = 1; i < numberOfVertexes; ++i)
        {
            vector<int> shortestPathsIds;
            int idTemp = (*predecessors)[i];
            shortestPathsIds.push_back(idsOfNodes[i]);
            while (idTemp != sourceVertex)
            {
                shortestPathsIds.insert(shortestPathsIds.begin(), idTemp);
                idTemp = (*predecessors)[distance(idsOfNodes.begin(), find(idsOfNodes.begin(), idsOfNodes.end(), idTemp))];
            }
            shortestPathsIds.insert(shortestPathsIds.begin(), sourceVertex);
            cout << idsOfNodes[i] << ": ";
            for (auto & it : shortestPathsIds)
                cout << it << " ";
            cout << endl;
            map<int, Node<T> *> *nodesOfGraph = graph->getMap();
            for (auto idOfNode = shortestPathsIds.begin(); idOfNode != shortestPathsIds.end(); ++idOfNode)
            {
                Node<T> *currentNode = (*graph->getMap())[*idOfNode];
                for (auto edgeOfCurrentNode = currentNode->getEdges()->begin(); edgeOfCurrentNode != currentNode->getEdges()->end(); ++edgeOfCurrentNode)
                    if ((*edgeOfCurrentNode)->getFrom()->getID() == *idOfNode and (*edgeOfCurrentNode)->getTo()->getID() == *(idOfNode + 1))
                        if (find(shortestPaths->begin(), shortestPaths->end(), (*edgeOfCurrentNode)) == shortestPaths->end())
                            shortestPaths->push_back(*edgeOfCurrentNode);
            }
        }
        return shortestPaths;
    }

    list<Node<T> *> *getNodesOfShortestPaths()
    {
        list<Node<T> *> *shortestPaths = new list<Node<T> *>;
        for (int i = 1; i < numberOfVertexes; ++i)
        {
            vector<int> shortestPathsIds;
            int idTemp = (*predecessors)[i];
            shortestPathsIds.push_back(idsOfNodes[i]);
            while (idTemp != sourceVertex)
            {
                shortestPathsIds.insert(shortestPathsIds.begin(), idTemp);
                idTemp = (*predecessors)[distance(idsOfNodes.begin(), find(idsOfNodes.begin(), idsOfNodes.end(), idTemp))];
            }
            shortestPathsIds.insert(shortestPathsIds.begin(), sourceVertex);
            map<int, Node<T> *> *nodesOfGraph = graph->getMap();
            for (auto idOfNode = shortestPathsIds.begin(); idOfNode != shortestPathsIds.end(); ++idOfNode)
                for (auto it = graph->getMap()->begin(); it != graph->getMap()->end(); ++it)
                    if (it->second->getID() == *idOfNode)
                        if (find(shortestPaths->begin(), shortestPaths->end(), it->second) == shortestPaths->end())
                            shortestPaths->push_back(it->second);
        }
        return shortestPaths;
    }

    void print()
    {
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

    ~BellmanFord()
    {
        distances->clear();
        predecessors->clear();
        idsOfNodes.clear();
    }
};

#endif //ADS_PROJECT_BELLMAN_FORD_H
