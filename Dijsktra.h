//
// Created by Usuario on 01/11/2019.
//

#ifndef GRAFO01_DIJSKTRA_H
#define GRAFO01_DIJSKTRA_H

#include <map>
#include <queue>
#include "graph.h"

using namespace std;


template <class A>
class NodeDijsktra{
     Node<A>* node;
     NodeDijsktra<A>* before;
     int distance;
     bool visited;
public:
    NodeDijsktra(Node<A>* _node){
        this->node=_node;
        before = NULL;
        distance=INT_MAX;
        visited = node->isVisited();
    }
    void setBefore(NodeDijsktra<A>* nodeBefore){
        this->before=nodeBefore;
    }
    void setDistance(int _distance){
        this->distance=_distance;
    }

    void nodeIsVisited(){
        this->visited= true;
    }
    Node<A>* getActual(){ return node;}
    Node<A>* getBefore(){ return before;}
    list<Edge<A>*>* getEdge(){ return node->getEdges();}
    int getDistance(){ return distance;}
    ~NodeDijsktra(){
        this->node= nullptr;
        this->before= nullptr;
    }
    bool getIsVisited(){ return visited;}
};

template <class Q>
class Compare{
public:
    bool operator()(NodeDijsktra<Q> *v1, NodeDijsktra<Q> *v2){
        return v1->getDistance() > v2->getDistance()? true : false;
    }
};

template <class T>
class dijsktra{
    map<int,NodeDijsktra<T>* >* nodes;
    NodeDijsktra<T>* source;
    priority_queue<NodeDijsktra<T>*, vector<NodeDijsktra<T>*>, Compare<T>> nodeOrderByDistance;
public:
    dijsktra(Graph<T>* graph,Node<T>* idNode){
        nodes = new map<int,NodeDijsktra<T>*>;
        for (auto iter = graph->getMap()->begin(); iter != graph->getMap()->end(); ++iter) {
            NodeDijsktra<T> * toInsert= new NodeDijsktra<T>(iter->second);
            nodes->insert(pair<int,NodeDijsktra<T>* >(iter->first,toInsert));
        }
        source =new NodeDijsktra<T>(idNode);
    }

    void calculate(){
        source->setDistance(0);
        auto iteradorListEdges =source->getEdge()->begin();
        nodeOrderByDistance.push(source);
        while (!nodeOrderByDistance.empty()){
            NodeDijsktra<T>* actual = nodeOrderByDistance.top();
            for (iteradorListEdges = actual->getEdge()->begin(); iteradorListEdges != actual->getEdge()->end(); ++iteradorListEdges) {
                NodeDijsktra<T>* visited =nodes->at((*iteradorListEdges)->getTo()->getID());
                if(!visited->getIsVisited()){
                    if(actual->getDistance()+(*iteradorListEdges)->getWeight()< visited->getDistance()){
                        visited->setDistance(actual->getDistance()+(*iteradorListEdges)->getWeight());
                        visited->setBefore(actual);
                        nodeOrderByDistance.push(visited);
                    }
                }
            }
            nodeOrderByDistance.pop();
            actual->nodeIsVisited();
        }
    }
};
#endif //GRAFO01_DIJSKTRA_H
