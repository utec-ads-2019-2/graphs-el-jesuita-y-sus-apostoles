//
// Created by Usuario on 05/10/2019.
//

#ifndef GRAFO01_EDGE_H
#define GRAFO01_EDGE_H
template <class U>
class Node;

template <class T>
class Edge{
    Node<T>* To;
    Node<T>* From;
    double weight;
public:
    Edge(){To = nullptr, From = nullptr, weight = 0;};

    void setTo(Node<T>* node){To = node;}

    void setFrom(Node<T>* node){From = node;}

    void setWeight(double w){weight = w;}

    int getWeight(){ return weight;}

    Node<T>* getTo(){ return To;}

    Node<T>* getFrom(){ return From;}

    ~Edge(){
        delete To,From;
        To = nullptr;
        From = nullptr;
    }
};
#endif //GRAFO01_EDGE_H
