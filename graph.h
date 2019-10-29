//
// Created by Usuario on 05/10/2019.
//

#ifndef GRAFO01_GRAPH_H
#define GRAFO01_GRAPH_H

#include <iterator>
#include <map>
#include "node.h"
#include "edge.h"

using namespace std;

template <class T>
class Graph{
    map< int , Node<T>* > *graphNodesMap;
    int numberOfEdges = 0;
    int numberOfVertexes = 0;
    double density = 0;
    bool isNotDirected = false;
    bool isConnected = false;
    bool isBipartite = false;

    void DeleteAllEdges(int idNode){
        for (auto i = graphNodesMap->begin(); i != graphNodesMap->end() ; ++i) {
            deleteEdge(i->first, idNode);
        }
    }

    void setNodesBlank(){
        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end(); it++) {
            (it->second)->setColor('B');
        }
    }

    bool testBipartite(Node<T>* toTest, char color){
        int FinalValue= toTest->getEdges()->size();
        int thisValue=0;
        if (color == 'R'){
            for (auto it = toTest->getEdges()->begin(); thisValue < FinalValue ; ++it, thisValue++) {
                if((*it)->getTo()->getColor() == 'B' ){
                    (*it)->getTo()->setColor('A');
                    return testBipartite((*it)->getTo(), 'A');
                }else if((*it)->getTo()->getColor() == 'R')
                    return false;
                else if((*it)->getTo()->getColor() == 'A'){

                } else throw invalid_argument("Memory failure");
                }
        }else if (color == 'A') {
            for (auto it = toTest->getEdges()->begin(); thisValue < FinalValue ; ++it, thisValue++) {
                if((*it)->getTo()->getColor() == 'B' ){
                    (*it)->getTo()->setColor('R');
                    return testBipartite((*it)->getTo(), 'R');
                }else if((*it)->getTo()->getColor() == 'A')
                    return false;
                else if((*it)->getTo()->getColor() == 'R'){

                } else throw invalid_argument("Memory failure");
            }
        }
        return true;
    }

    bool findEdge(int idFrom, int idTo, typename std::list<Edge<T>*>::iterator & guidePtr ){
        if (graphNodesMap->operator[](idFrom) == nullptr){
            graphNodesMap->erase(idFrom);
            return false;
        }else if (graphNodesMap->operator[](idTo) == nullptr) {
            graphNodesMap->erase(idTo);
            return false;
        }else{
            auto* From = graphNodesMap->operator[](idFrom);
            auto* toCompare = graphNodesMap->operator[](idTo);
            for (auto i = From->getEdges()->begin(); i != From->getEdges()->end() ; i++) {
                Edge<Airport>* edge = *i;
                if(edge->getTo() == toCompare ){
                    guidePtr = (i);
                    return true;
                }
            }
            return false;
        }
    }

    list<Edge<T>* >* sortEdgesWeight(){
        auto respt= new list<Edge<T>*>;
        for (auto it_Nodes = graphNodesMap->begin(); it_Nodes != graphNodesMap->end() ; it_Nodes++) {
            Node<T>* actual = it_Nodes->second;
            for (auto it_edges = actual->getEdges()->begin(); it_edges != actual->getEdges()->end() ; it_edges++) {
                if(respt->size() == 0){
                    respt->push_back(*(it_edges));
                }else{
                    int control_size = 0;
                    typename list<Edge<T>*>::iterator it = respt->begin();
                    while (control_size<respt->size() and (*it_edges)->getWeight()<(*it)->getWeight()){
                        control_size++;
                        it++;
                    }
                    respt->insert((it),(*it_edges));
                }
            }
        }
        return respt;
    }

public:
    Graph(){graphNodesMap = new map< int , Node<T>* >;}

    map< int , Node<T>* >* getMap(){ return graphNodesMap;}

    bool insertEgde(int idFrom, int idTo){
        if (graphNodesMap->operator[](idFrom) != nullptr and graphNodesMap->operator[](idTo) != nullptr){
            if(!findEdge(idFrom,idTo)){
                auto* edge = new Edge<T>;
                edge->setTo(graphNodesMap->operator[](idTo));
                edge->setFrom(graphNodesMap->operator[](idFrom));
                graphNodesMap->operator[](idFrom)->getEdges()->push_front(edge);
                numberOfEdges++;
                return true;
            }else return false;

        } else{
            if (graphNodesMap->operator[](idFrom) == nullptr){
                graphNodesMap->erase(idFrom);}
            if (graphNodesMap->operator[](idTo) == nullptr){
                graphNodesMap->erase(idTo);}
            return false;
        }
    }

    bool insertNode(Node<T>* node){
        int id = node->getID();
        if (graphNodesMap->operator[](id) == nullptr){
            graphNodesMap->erase(id);
            graphNodesMap->insert(pair<int, Node<T>*> (id,node));
            numberOfVertexes++;
            return true;
        }else{
            throw invalid_argument("Id in use");
        }
    }

    bool deleteEdge(int idFrom, int idTo){
        typename std::list<Edge<T>*>::iterator nuevo;
        if (this->findEdge(idFrom,idTo, nuevo)){
            graphNodesMap->operator[](idFrom)->getEdges()->erase(nuevo);
            numberOfEdges--;
            return true;
        }else
            return false;
    }

    bool deleteNode(int idNode){
        if(graphNodesMap->operator[](idNode) != nullptr){
            graphNodesMap->erase(idNode);
            DeleteAllEdges(idNode);
            return true;
        }else {
            graphNodesMap->erase(idNode);
            return false;
        }
    }

    ~Graph(){
        delete graphNodesMap;
    }

    bool findEdge(int idFrom, int idTo){
        if (graphNodesMap->operator[](idFrom) == nullptr){
            graphNodesMap->erase(idFrom);
            return false;
        }else if (graphNodesMap->operator[](idTo) == nullptr) {
            graphNodesMap->erase(idTo);
            return false;
        }else{
            auto* From = graphNodesMap->operator[](idFrom);
            auto* toCompare = graphNodesMap->operator[](idTo);
            for (auto i = From->getEdges()->begin(); i != From->getEdges()->end() ; i++) {
                Edge<Airport>* edge = *i;
                if(edge->getTo() == toCompare )
                    return true;
            }
            return false;
        }
    }

    bool findNode(int id){
        if(graphNodesMap->operator[](id)== nullptr){
            graphNodesMap->erase(id);
            return false;
        } else
            return false;
    }

    double getDensity(){
        if(numberOfVertexes == 0 or numberOfVertexes == 1){
            return 0;
        }else {
            density = numberOfEdges/(numberOfVertexes*(numberOfVertexes-1));
            return density;

        }
    }

    Graph<T>* Prim(int idOfSource){
        if (setIsNotDirected()) {

            /* mientras que el numero de vertices actualmente en mi grafo sea menor que el numero de vertices
             * guardar el edge que tenga el menor peso
             * obtener el from y el to
             * agregar edge al grafo
             * sumar en uno la cantidad de vertices
             * */

            int numberOfVerticesAlreadyInThePrimGraph = 0;
            auto* primGraph = new Graph();
            primGraph->insertNode(graphNodesMap[idOfSource]);
            map<int, Node<T>* >* primGraphMap = primGraph->getMap();
            primGraph[idOfSource] = graphNodesMap[idOfSource];

            //mientras que el número de vértices conectados sea menor que el número de vértices
            while (numberOfVertexes >= numberOfVerticesAlreadyInThePrimGraph) {
                // guardar el edge que tenga el menor peso
                list<Edge<T>*>* sortedEdgesWeightPrimGraph = primGraph->sortEdgesWeight();
                Edge<T>* minEdgeWeight = sortedEdgesWeightPrimGraph->front();
                sortedEdgesWeightPrimGraph->pop_front();

                // obtener el from y el to

                // from
                auto minEdgeWeightNodeFrom = minEdgeWeight->getFrom();
                // to
                auto minEdgeWeightNodeTo = minEdgeWeight->getTo();

                // iterador que busca si el nodo al que quiero ir ya ha sido visitado
                auto findNodeTo = primGraphMap->find(minEdgeWeightNodeTo);

                if (findNodeTo == nullptr) {
                    // si es nullptr quiere decir que todavia no es parte de mi grafo

                    // obtener id para meterlo en el map de prim
                    int idFromMinEdgeWeightNodeTo = minEdgeWeightNodeTo->getID();
                    primGraph[idFromMinEdgeWeightNodeTo] = minEdgeWeightNodeTo;
                }

            }


            auto currentNode = graphNodesMap[idOfSource];
            auto currentEdges = currentNode.getEdges();



            list<Edge<T>*>* sortedEdgesWeight = this->sortEdgesWeight();


            map<int, Node<T>*>* primMap = primGraph->getMap();
            primGraph->insertNode(currentNode);

            return primGraph;
        } else {
            cout << "Can't find MST of directed graph" << endl;
            return nullptr;
        }
    }

    Graph<T>* Kruskal(){
        if(setIsNotDirected()){
            list<Edge<T>*>* Edges = this->sortEdgesWeight();
            Graph<T>* graph_Krusca = new Graph();
            map<int,Node<T>*>* mapKrusca = graph_Krusca->getMap();
            while(Edges->size()>0){
                Node<T>* nodeTo = new Node<T>(Edges->back()->getTo()->getObject());
                nodeTo->setID(nodeTo->getObject()->getId());
                Node<T>* nodeFrom = new Node<T>(Edges->back()->getFrom()->getObject());
                nodeFrom->setID(nodeFrom->getObject()->getId());
                Edges->pop_back();
                nodeTo->getEdges()->clear();
                nodeFrom->getEdges()->clear();
                if(mapKrusca->operator[](nodeFrom->getID()) == nullptr or mapKrusca->operator[](nodeTo->getID()) == nullptr){
                    if (mapKrusca->operator[](nodeFrom->getID())== nullptr){
                        mapKrusca->erase(nodeFrom->getID());
                        mapKrusca->insert(pair<int,Node<T>*>(nodeFrom->getID(),nodeFrom));
                    }
                    if (mapKrusca->operator[](nodeTo->getID())== nullptr){
                        mapKrusca->erase(nodeTo->getID());
                        mapKrusca->insert(pair<int,Node<T>*>(nodeTo->getID(),nodeTo));
                    }
                    graph_Krusca->insertEgde(nodeFrom->getID(),nodeTo->getID());
                    graph_Krusca->insertEgde(nodeTo->getID(),nodeFrom->getID());
                }
            }
            graph_Krusca->setVertexes(graph_Krusca->getMap()->size());
            return graph_Krusca;
        }else return nullptr;
    }

    bool setIsConnected(){
        map<int, pair<bool ,bool > > temp;
        auto* edges2 = new list<Edge<T> *>;

        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end() ; ++it) {
            temp.insert(pair<int,pair<bool,bool> >(it->first,pair<bool ,bool >(false,false)));
        }
        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end() ; ++it) {
            Node<T> * ptr = it->second;
            temp.at(it->first).first = true;
            if(ptr->getEdges()->empty() or ptr->getEdges()->front()->getTo() == nullptr){
                isConnected = false;
                return isConnected;
            }else {
                *edges2 = *(ptr->getEdges());
                for (int i = 0; i< edges2->size();i++) {
                    int id = edges2->front()->getTo()->getID();
                    edges2->pop_front();
                    temp.at(id).second=true;
                }
            }
        }
        for (auto & it : temp) {
            if(!it.second.first or !it.second.second){
                isConnected = false;
                return isConnected;
            }
        }
        isConnected = true;
        return isConnected;
    }

    bool setIsNotDirected(){
        if(numberOfEdges%2 == 1){
            isNotDirected= false;
            return isNotDirected;
        }
        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end() ; it++) {
            list<Edge<T>*> edges2 = *(it->second->getEdges());
            for (int i = 0; i< edges2.size();i++) {
                int id = edges2.front()->getTo()->getID();
                edges2.pop_front();
                if(!findEdge(it->first, id) or !findEdge(id,it->first)){
                    isNotDirected= false;
                    return isNotDirected;
                }
            }
        }
        isNotDirected= true;
        return isNotDirected;
    }

    void setVertexes(int _corner){numberOfVertexes=_corner;}

    int getEdges(){ return numberOfEdges;}

    void setEdges(int Edges1){numberOfEdges=Edges1;}

    bool setIsBipartite(){
        setNodesBlank();
        auto it = graphNodesMap->begin();
        (it->second)->setColor('R');

        isBipartite = testBipartite((it->second),'R');
        return isBipartite;
    }
}

;
#endif //GRAFO01_GRAPH_H
