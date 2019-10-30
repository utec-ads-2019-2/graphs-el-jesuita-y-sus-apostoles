#ifndef GRAFO01_NODE_H
#define GRAFO01_NODE_H

#include <list>

using namespace std;

template <class U>
class Edge;

template <class T>
class Node {
    T* Object;
    list< Edge<T>* >* listOfEdges;
    Node<T>* selfPtr;
    int Id;
    char color;
public:
    explicit Node(T* object){
        Object = object;
        selfPtr = this;
        listOfEdges = new list<Edge<T>*>;
        color = 'B';}
    Node(){
        Object = nullptr;
        selfPtr = nullptr;
        listOfEdges = nullptr;
        color = 'B';}

    void setID(int id){Id = id;}

    int getID(){ return Id;}

    char getColor(){ return color;}

    list< Edge<T>* >* getEdges(){ return listOfEdges;}

    T* getObject(){ return Object;}

    ~Node(){
        delete listOfEdges;
        delete selfPtr;
        delete Object;
        selfPtr = nullptr;
        Object = nullptr;
        listOfEdges = nullptr;
    }

    void setColor(char c) {
        Node::color = c;
    }

};


#endif //GRAFO01_NODE_H
