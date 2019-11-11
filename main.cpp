#include <iostream>
#include "nlohmann/json.hpp"
#include "functions.cpp"
#include "node.h"
#include "Dijsktra.h"
#include "floyd_warshall.h"

using namespace std;
using json = nlohmann::json;

class caracter{
    char letra;
    int id;
public:
    caracter(char letra,int id){
        this->letra=letra;
        this->id = id;
    }
    int getId(){
        return id;
    }
};


int main() {
    Graph<caracter>* grafo = new Graph<caracter>;
    for (int i = 0; i <10 ; ++i) {
        caracter* a = new caracter(65+i,i+1);
        Node<caracter>* newnode = new Node<caracter>;
        newnode->setID(i+1);
        newnode->setObject(a);
        grafo->insertNode(newnode);
    }

    grafo->insertEdge(1, 3, 4); grafo->insertEdge(3, 1, 4);
    grafo->insertEdge(1, 4, 13); grafo->insertEdge(4, 1, 13);
    grafo->insertEdge(1,8,5); grafo->insertEdge(8,1,5);
    grafo->insertEdge(8,4,5); grafo->insertEdge(4,8,5);
    grafo->insertEdge(4,3,39); grafo->insertEdge(3,4,39);
    grafo->insertEdge(3,9,37); grafo->insertEdge(9,3,37);
    grafo->insertEdge(9,6,7); grafo->insertEdge(6,9,7);
    grafo->insertEdge(3,6,9); grafo->insertEdge(6,3,9);
    grafo->insertEdge(9,1,11); grafo->insertEdge(1,9,11);
    grafo->insertEdge(9,5,26); grafo->insertEdge(5,9,26);
    grafo->insertEdge(9,7,39); grafo->insertEdge(7,9,39);
    grafo->insertEdge(1,5,13);grafo->insertEdge(5,1,13);
    grafo->insertEdge(5,7,2); grafo->insertEdge(7,5,2);
    grafo->insertEdge(7,2,2); grafo->insertEdge(2,7,2);
    grafo->insertEdge(2,5,24); grafo->insertEdge(5,2,24);
    grafo->insertEdge(5,8,19); grafo->insertEdge(8,5,19);
    grafo->insertEdge(2,8,29); grafo->insertEdge(8,2,29);
    grafo->insertEdge(8,10,15); grafo->insertEdge(10,8,15);
    grafo->insertEdge(2,10,15); grafo->insertEdge(10,2,15);
    grafo->insertEdge(4,10,15); grafo->insertEdge(10,4,15);

    Graph<Airport>* connectedGraph = readJsonAndReturnAirportGraph("../jsonFiles/airports.json");
    dijsktra<caracter>* node = new dijsktra<caracter>(grafo,grafo->getMap()->at(1));
    dijsktra<Airport>* airports = new dijsktra<Airport>(connectedGraph,connectedGraph->getMap()->at(1));
    node->calculate();
    airports->calculate();
    parseToJsonTxt(airports->getClosestPath(4),"trydo.json");
    if(grafo->setIsConnected()){
        cout<<"Es conexo"<<endl;
    }
    if(!grafo->setIsBipartite()){
        cout<<"No es bipartito"<<endl;
    }
    if(grafo->setIsNotDirected()){
        cout<<"No es dirigido"<<endl;
    }
    auto prim = grafo->prim(1);
    auto krus = grafo->Kruskal();

    FloydWarshall<caracter> *floydWarshall = new FloydWarshall<caracter>(grafo);
    floydWarshall->printAdjacencyMatrix();


    return EXIT_SUCCESS;
}