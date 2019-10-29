#ifndef GRAFO01_NODE_H
#define GRAFO01_NODE_H

#include <list>
#include "edge.h"
using namespace std;


template <class T>
class Node {
    T* Object;
    list< Edge<T>* >* listOfEdges;
    int Id{};

    char color;
public:
    explicit Node(T* object){
        Object = object;
        listOfEdges = new list<Edge<T>*>;
        color = 'B';}

    void setID(int id){
        Id = id;
    }

    int getID(){
        return Id;
    }

    list< Edge<T>* >* getEdges(){
        return listOfEdges;
    }

    T* getObject(){
        return Object;
    }

    void setColor(char c) {
        this->color = c;
    }

    char getColor() const {
        return color;
    }

    ~Node(){
        delete listOfEdges;
        Object = nullptr;
    }
};


#endif //GRAFO01_NODE_H
