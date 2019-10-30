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
    int Id;
    char color;
public:
    explicit Node(T* object){
        Object = object;
        listOfEdges = new list<Edge<T>*>;
        color = 'B';}
  
    Node(){
        Object = nullptr;
        listOfEdges = nullptr;
        color = 'B';}

    void setID(int id){Id = id;}

    int getID(){ return Id;}

    char getColor(){ return color;}

    list< Edge<T>* >* getEdges(){ return listOfEdges;}

    T* getObject(){ return Object;}

    void setColor(char c) {
        Node::color = c;
    }

    ~Node(){
        delete listOfEdges;
        delete Object;
        Object = nullptr;
        listOfEdges = nullptr;
    }
};
#endif //GRAFO01_NODE_H
