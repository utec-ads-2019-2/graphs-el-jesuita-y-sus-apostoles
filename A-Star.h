
#ifndef GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H
#define GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H

#include <iostream>
#include <cmath>
#include "graph.h"
using namespace std;
template <class Y>
class Astar;

template <typename A>
class NodeAStar{
    Node<A>* node;
    double distance ;
    double distanceNonH;
    bool isVisited = false;
public:
    //uSAR UN FUNTOR
    NodeAStar(Node<A>* _node){
        distance = INT_MAX;
        distanceNonH = INT_MAX;
        this->node=_node;
    }

    void Visited(){
        isVisited= true;
    }
    void setDistance(int _distance){
        this->distance=_distance;
    }

    int getDistance(){ return distance;}

    int getID() {
        return node->getID();
    }

    list< Edge<A>* >* getEdges(){
        return node->getEdges();
    }
    friend class Astar<A>;
};

template <class Q>
class CompareA{
public:
    bool operator()(NodeAStar<Q> *v1, NodeAStar<Q> *v2){
        return v1->getDistance() > v2->getDistance();
    }
};

template <class T>
class Astar {
    unordered_map<int,NodeAStar<T>* >* nodes;
    NodeAStar<T>* source;
    NodeAStar<T>* destiny;

public:
    int calculateHeuristicDistance(NodeAStar<T>* nodeAStar1){
        auto a = pow(nodeAStar1->node->getObject()->getLatitude() - destiny->node->getObject()->getLatitude(),2) ;

        auto b = pow(nodeAStar1->node->getObject()->getLongitude() - destiny->node->getObject()->getLongitude(),2) ;

        return sqrt(a+b);
    }

    Astar(Graph<T>* graph,Node<T>* idNodeSource, Node<T>* idNodeDestiny) {
        nodes = new unordered_map<int,NodeAStar<T>*>;
        for (auto iter = graph->getMap()->begin(); iter != graph->getMap()->end(); ++iter) {
            NodeAStar<T> * toInsert= new NodeAStar<T>(iter->second);
            nodes->insert(pair<int,NodeAStar<T>* >(iter->first,toInsert));
        }
        source = nodes->operator[](idNodeSource->getID());
        source->distanceNonH = 0;
        destiny = nodes->operator[](idNodeDestiny->getID());
        calculateHeuristicDistance(source);
    }

    list<Node<T>*>* calculate() {
        list<Node<T>*>* pathBuild = new list<Node<T>*>;
        priority_queue<NodeAStar<T>*, vector<NodeAStar<T>*>, CompareA<T>> nodesOrderByDistance;
        nodesOrderByDistance.push(source);
        while(!nodesOrderByDistance.empty()) {
            NodeAStar<T>* nodeInWork = nodesOrderByDistance.top();
            nodesOrderByDistance.pop();
            pathBuild->push_back(nodeInWork->node);
            if (nodeInWork->getID() == destiny->getID() ){
                break;
            }
            list<Edge<T>*>* edgesOfNodeInWork = nodeInWork->getEdges();
            for (auto edge = edgesOfNodeInWork->begin(); edge != edgesOfNodeInWork->end() ; edge++) {
                NodeAStar<T>* nodeAStar = nodes->operator[]((*edge)->getTo()->getID());
                double calculated = calculateHeuristicDistance(nodeAStar);
                if(!nodeAStar->isVisited){
                    if (calculated + (*edge)->getWeight() + nodeInWork->distanceNonH < nodeAStar->getDistance()){
                        nodeAStar->setDistance(calculated + (*edge)->getWeight() + nodeInWork->distanceNonH);
                        nodeAStar->distanceNonH =  (*edge)->getWeight() + nodeInWork->distanceNonH;
                        nodesOrderByDistance.push(nodeAStar);
                    }
                }
            }
            nodeInWork->Visited();
        }
        return pathBuild;
    }
};

#endif //GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H
