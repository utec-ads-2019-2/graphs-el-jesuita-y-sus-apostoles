//
// Created by David Lazo on 21/11/2019.
//

#ifndef GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H
#define GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H

#include <iostream>

//#include "graph.h"//
using namespace std;
template <typename A>
class NodeAStar{
    Node<A>* node;//Nodo de clase node-star
    int distance;//
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
    NodeAStar<T>* source;//inicio
    NodeAStar<T>* destiny;//fin
    unordered_map<int, double> heuristic;
    unordered_map<int, double> dist;

public:
    Astar(Graph<T>* graph,Node<T>* idNodeSource, Node<T>* idNodeDestiny) {//Recore todos los nodo para ir calculado su distancia

        nodes = new unordered_map<int,NodeAStar<T>*>;//contenedor int es el id para busqueda rapida
        source = new NodeAStar<T>(idNodeSource);//origen
        destiny = new NodeAStar<T>(idNodeDestiny);//destino
        //recorro todos los nodos
        for (auto iter = graph->getMap()->begin(); iter != graph->getMap()->end(); ++iter) {
            NodeAStar<T> * toInsert= new NodeAStar<T>(iter->second);
            nodes->insert(pair<int,NodeAStar<T>* >(iter->first,toInsert));
            //calculo de la heurística

            if ( iter->second->getID() != idNodeSource->getID()) {
                //Latirud (x2-x1)^2
                auto a = pow(idNodeDestiny->getObject()->getLatitude() - idNodeSource->getObject()->getLatitude(), 2);
                //Longitud (y2-y1)^2
                auto b = pow(idNodeDestiny->getObject()->getLongitude() - idNodeSource->getObject()->getLongitude(), 2);
                //Calculo la heuristica la raiz
                heuristic[iter->second->getID()] = sqrt(a + b);
                //Propiedad del algoritmo -> Le paso infinito a la distancia
                //Todas comiensan de infinito ya que no las calculo
                dist[iter->second->getID()] = INFINITY;
            }

        }
    }

    void calculate() {
        //Si lo encuentro
        bool encontre = false;
        //Parentesco guarada  de nodo tiene
        unordered_map<int, int> parent;
        //Contenedor agreganod el peso pero en el minimo
        //Peso y mombre del vertice
        priority_queue<pair<double, int>, std::vector<pair<double, int>>, greater<>> pq;//greater ordena arriba mayor a menosr
        //Arreves para buscar el id cosa que no se puede
        unordered_map<int, double > qp;

        double total_dist;
        //sacar id del source
        dist[source->getID()] = 0;
        parent[source->getID()] = source->getID();
        //Guardo el peso , id (guardo los 2)
        pq.push( make_pair(0, source->getID()) );
        qp[source->getID()] = 0;


        while(!pq.empty()) {//Guardo en un Stack cada vez que recoro (mientras no sea vacio)
            //Guardo el int segun pq
            auto u_node = pq.top().second;
            //Saco 1 menos -- Inserto y ordeno al menos peso internamente
            pq.pop();
            //Propiedad erase sacando
            qp.erase(u_node);
            //Condicion cuanod llegue al destino y termino
            if (u_node == destiny->getID() ){
                encontre = true;
                break;
            }

            //Vecinos
            //Saco las aristas
            auto edges = this->nodes[u_node];
            //Tener varios vecinos y pruebo con cada uno
            for (auto edge: edges) {

                //Arista A y B jalo destino
                //auto w_node = edge->nodes[1]->data;
                auto w_node = edge.second->getID();

                //Caluclo la distancia
                //auto distance = dist[u_node] + edge.weight;
                auto distance = dist[u_node] + edge.second->getDistance();

                //Cuando lo encuentro en el qp y la distancia es mayor a la distancia del nodo de arriba
                if(qp.find(w_node) != qp.end() and dist[w_node] > distance) {
                    //Actualizo la distancia
                    dist[w_node] = distance;
                    //Actualizo el Paren y de ese nodo su paren es este
                    parent[w_node] = u_node;

                }
                //Busca el nodo y su no lo encuentra y actualiza la distancia
                else if (parent.find(w_node) == parent.end() ) {
                    //Actualizo  la distancia
                    dist[w_node] = distance;
                    //Calculo la heruristica
                    total_dist = distance + heuristic[u_node];
                    //Actualizo
                    parent[w_node] = u_node;
                    //Se guarda Dstancia en el PQ
                    pq.push({total_dist, w_node});
                    //y en QP lo guardo arreves
                    qp[w_node] = total_dist;
                    //Que siga recoriendo en el while mientras no sea vacio
                }
            }
        }
        encontre = true;
        source = destiny->getID();
        while(source != source->getID()){
            cout<<parent[source]<<" ";
            source =parent[source];
        }
            //Viendo aun
    }
};

#endif //GRAPHS_EL_JESUITA_Y_SUS_APOSTOLES_A_STAR_H
