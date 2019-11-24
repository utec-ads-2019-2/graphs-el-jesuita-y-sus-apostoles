//
// Created by David Lazo on 21/11/2019.
//

#ifndef GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H
#define GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H

#include <iostream>

#include "graph.h"
using namespace std;
template <typename A>
class NodeAStar{
    Node<A>* node;
    int distance;
public:
    //uSAR UN FUNTOR
    NodeAStar(Node<A>* _node){
        this->node=_node;

    }

    void setDistance(int _distance){
        this->distance=_distance;
    }

    int getDistance(){ return distance;}
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
    map<int,NodeAStar<T>* >* nodes;
    NodeAStar<T>* source;
    NodeAStar<T>* destiny;
    unordered_map<int, double> heuristic;

public:
    Astar(Graph<T>* graph,Node<T>* idNodeSource, Node<T>* idNodeDestiny) {
        nodes = new map<int,NodeAStar<T>*>;

        source = new NodeAStar<T>(idNodeSource);
        destiny = new NodeAStar<T>(idNodeDestiny);
        //recorro todos los nodos
        for (auto iter = graph->getMap()->begin(); iter != graph->getMap()->end(); ++iter) {
            NodeAStar<T> * toInsert= new NodeAStar<T>(iter->second);
            nodes->insert(pair<int,NodeAStar<T>* >(iter->first,toInsert));
            //calculo de la heurística

            if ( iter->second->getID() != idNodeSource->getID()) {

                auto a = pow(idNodeDestiny->getObject()->getLatitude() -
                             idNodeSource->getObject()->getLatitude(), 2);

                auto b = pow(idNodeDestiny->getObject()->getLongitude() -
                             idNodeSource->getObject()->getLongitude(), 2);

                heuristic[iter->second->getID()] = a + b;
            }

        }
    }
};

#endif //GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H
