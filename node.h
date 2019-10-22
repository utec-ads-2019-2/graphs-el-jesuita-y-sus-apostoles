//
// Created by Usuario on 05/10/2019.
//

#ifndef GRAFO01_NODE_H
#define GRAFO01_NODE_H

#include <list>

using namespace std;

template <class U>
class Edge;

template <class T>
class Node {
    T* Object;
    list< Edge<T>* >* edge;
    Node<T>* selfPtr;
    int Id;
    char color;
public:
    Node(T* object){
        Object = object;
        selfPtr = this;
        edge = new list<Edge<T>*>;
        color = 'B';}

    void setID(int id){Id = id;}

    int getID(){ return Id;}

    char getColor(){ return color;}

    list< Edge<T>* >* getEdges(){ return edge;}

    T* getObject(){ return Object;}

    ~Node(){
        delete edge;
        selfPtr = nullptr;
        Object = nullptr;
    }

    void setColor(char color) {
        Node::color = color;
    }

    void grafic();
};


#endif //GRAFO01_NODE_H
