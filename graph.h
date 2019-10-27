//
// Created by Usuario on 05/10/2019.
//

#ifndef GRAFO01_GRAPH_H
#define GRAFO01_GRAPH_H

#include <iterator>
#include <map>
#include "Node.h"
#include "Edge.h"

using namespace std;

template <class T>
class Graph{
    map< int , Node<T>* > *Nodes;
    int edges = 0;
    int corner = 0;
    double density=0;
    bool isNotDirected=false;
    bool isConex = false;
    bool isBipar = false;

    void DeleteAllEdges(int idNode){
        for (auto i = Nodes->begin(); i != Nodes->end() ; ++i) {
            deleteEdge(i->first, idNode);
        }
    }

    void makeblank(){
        for ( auto it = Nodes->begin(); it != Nodes->end(); it++) {
            (it->second)->setColor('B');
        }
    }

    bool testBipartito(Node<T>* toTest, char color){
        int FinalValue= toTest->getEdges()->size();
        int thisValue=0;
        if (color == 'R'){
            for (auto it = toTest->getEdges()->begin(); thisValue < FinalValue ; ++it, thisValue++) {
                if((*it)->getTo()->getColor() == 'B' ){
                    (*it)->getTo()->setColor('A');
                    return testBipartito((*it)->getTo(), 'A');
                }else if((*it)->getTo()->getColor() == 'R')
                    return false;
                else if((*it)->getTo()->getColor() == 'A'){

                } else throw invalid_argument("Memory failure");
                }
        }else if (color == 'A') {
            for (auto it = toTest->getEdges()->begin(); thisValue < FinalValue ; ++it, thisValue++) {
                if((*it)->getTo()->getColor() == 'B' ){
                    (*it)->getTo()->setColor('R');
                    return testBipartito((*it)->getTo(), 'R');
                }else if((*it)->getTo()->getColor() == 'A')
                    return false;
                else if((*it)->getTo()->getColor() == 'R'){

                } else throw invalid_argument("Memory failure");
            }
        }
        return true;
    }

    bool findEdge(int idFrom, int idTo, typename std::list<Edge<T>*>::iterator & guidePtr ){
        if (Nodes->operator[](idFrom) == nullptr){
            Nodes->erase(idFrom);
            return false;
        }else if (Nodes->operator[](idTo) == nullptr) {
            Nodes->erase(idTo);
            return false;
        }else{
            auto* From = Nodes->operator[](idFrom);
            auto* toCompare = Nodes->operator[](idTo);
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

    list<Edge<T>* >* GetOrderEdges(){
        auto respt= new list<Edge<T>*>;
        for (auto it_Nodes = Nodes->begin(); it_Nodes != Nodes->end() ; it_Nodes++) {
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
    Graph(){Nodes = new map< int , Node<T>* >;}

    map< int , Node<T>* >* getMap(){ return Nodes;}

    bool insertEgde(int idFrom, int idTo){
        if (Nodes->operator[](idFrom) != nullptr and Nodes->operator[](idTo) != nullptr){
            if(!findEdge(idFrom,idTo)){
                auto* edge = new Edge<T>;
                edge->setTo(Nodes->operator[](idTo));
                edge->setFrom(Nodes->operator[](idFrom));
                Nodes->operator[](idFrom)->getEdges()->push_front(edge);
                edges++;
                return true;
            }else return false;

        } else{
            if (Nodes->operator[](idFrom) == nullptr){
                Nodes->erase(idFrom);}
            if (Nodes->operator[](idTo) == nullptr){
                Nodes->erase(idTo);}
            return false;
        }
    }

    bool insertNode(Node<T>* node){
        int id = node->getID();
        if (Nodes->operator[](id) == nullptr){
            Nodes->erase(id);
            Nodes->insert(pair<int, Node<T>*> (id,node));
            corner++;
            return true;
        }else{
            throw invalid_argument("Id in use");
        }
    }

    bool deleteEdge(int idFrom, int idTo){
        typename std::list<Edge<T>*>::iterator nuevo;
        if (this->findEdge(idFrom,idTo, nuevo)){
            Nodes->operator[](idFrom)->getEdges()->erase(nuevo);
            edges--;
            return true;
        }else
            return false;
    }

    bool deleteNode(int idNode){
        if(Nodes->operator[](idNode) != nullptr){
            Nodes->erase(idNode);
            DeleteAllEdges(idNode);
            return true;
        }else {
            Nodes->erase(idNode);
            return false;
        }
    }

    ~Graph(){
        delete Nodes;
    }

    bool findEdge(int idFrom, int idTo){
        if (Nodes->operator[](idFrom) == nullptr){
            Nodes->erase(idFrom);
            return false;
        }else if (Nodes->operator[](idTo) == nullptr) {
            Nodes->erase(idTo);
            return false;
        }else{
            auto* From = Nodes->operator[](idFrom);
            auto* toCompare = Nodes->operator[](idTo);
            for (auto i = From->getEdges()->begin(); i != From->getEdges()->end() ; i++) {
                Edge<Airport>* edge = *i;
                if(edge->getTo() == toCompare )
                    return true;
            }
            return false;
        }
    }

    bool findNode(int id){
        if(Nodes->operator[](id)== nullptr){
            Nodes->erase(id);
            return false;
        } else
            return false;
    }

    double getDensity(){
        if(corner == 0 or corner == 1){
            return 0;
        }else {
            density = edges/(corner*(corner-1));
            return density;
        }
    }

    Graph<T>* Prim(){

    }

    Graph<T>* Krusca(){
        if(setIsNotDirected()){
            list<Edge<T>*>* Edges = this->GetOrderEdges();
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
                //Hacer control para nodos.
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
            graph_Krusca->setCorner(graph_Krusca->getMap()->size());
            return graph_Krusca;
        }else return nullptr;
    }

    //in parir temp<bool,bool> first = in, second = out
    bool setIsConex(){
        map<int, pair<bool ,bool > > temp;
        list<Edge<T> *>* edges2 = new list<Edge<T> *>;

        for (auto it = Nodes->begin(); it != Nodes->end() ; ++it) {
            temp.insert(pair<int,pair<bool,bool> >(it->first,pair<bool ,bool >(false,false)));
        }
        for (auto it = Nodes->begin(); it != Nodes->end() ; ++it) {
            Node<T> * ptr = it->second;
            temp.at(it->first).first = true;
            if(ptr->getEdges()->empty() or ptr->getEdges()->front()->getTo() == nullptr){
                isConex = false;
                return isConex;
            }else {
                *edges2 = *(ptr->getEdges());
                for (int i = 0; i< edges2->size();i++) {
                    int id = edges2->front()->getTo()->getID();
                    edges2->pop_front();
                    temp.at(id).second=true;
                }
            }
        }
        for (auto it = temp.begin(); it != temp.end() ; it++) {
            if(! it->second.first or !it->second.second){
                isConex= false;
                return isConex;
            }
        }
        isConex = true;
        return isConex;
    }

    bool setIsNotDirected(){
        if(edges%2 == 1){
            isNotDirected= false;
            return isNotDirected;
       }
        for (auto it = Nodes->begin(); it != Nodes->end() ; it++) {
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

    void setCorner(int _corner){corner=_corner;}

    int getEdges(){ return edges;}

    void setEdges(int Edges1){edges=Edges1;}

    bool setIsBipar(){
        makeblank();
        auto it = Nodes->begin();
        (it->second)->setColor('R');

        isBipar = testBipartito((it->second),'R');
        return isBipar;
    }
}

;
#endif //GRAFO01_GRAPH_H
