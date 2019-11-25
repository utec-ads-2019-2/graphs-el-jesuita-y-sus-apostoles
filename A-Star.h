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

    int getID() {
        return node->getID();
    }

    list< Edge<A>* >* getEdges(){
        return node->getEdges();
    }
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
    unordered_map<int, double> heuristic;
    unordered_map<int, double> dist;
public:
    Astar(Graph<T>* graph,Node<T>* idNodeSource, Node<T>* idNodeDestiny) {
        nodes = new unordered_map<int,NodeAStar<T>*>;

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

                dist[iter->second->getID()] = INFINITY;
            }

        }
    }

    void calculate() {
        unordered_map<int, int> parent;
        priority_queue<pair<double, int>, std::vector<pair<double, int>>, greater<>> pq;
        unordered_map<int, double > qp;
        double total_dist;

        dist[source->getID()] = 0;
        parent[source->getID()] = source->getID();

        pq.push( make_pair(0, source->getID()) );
        qp[source->getID()] = 0;

        while(!pq.empty()) {
            auto u_node = pq.top().second;

            pq.pop();
            qp.erase(u_node);

            if (u_node == destiny->getID() )
                break;

            //Neighboors
            auto edges = this->nodes[u_node];
            for (auto edge: edges) {
                //auto w_node = edge->nodes[1]->data;
                auto w_node = edge.second->getID();
                //auto distance = dist[u_node] + edge.weight;
                auto distance = dist[u_node] + edge.second->getDistance();

                if(qp.find(w_node) != qp.end() and dist[w_node] > distance) {
                    dist[w_node] = distance;

                    parent[w_node] = u_node;
                } else if (parent.find(w_node) == parent.end() ) {
                    dist[w_node] = distance;
                    total_dist = distance + heuristic[u_node];
                    parent[w_node] = u_node;
                    pq.push({total_dist, w_node});
                    qp[w_node] = total_dist;
                }
            }
        }
    }
};

#endif //GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H
