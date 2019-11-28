#include <iostream>
#include "nlohmann/json.hpp"
#include "functions.cpp"
#include "node.h"
#include "Dijsktra.h"
#include "floyd_warshall.h"
#include "bellman_ford.h"
#include <chrono>

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
    auto start = chrono::high_resolution_clock::now();

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
    Graph<Airport> *conexGraph = readJsonAndReturnAirportGraph("../jsonFiles/conexo.json");


    dijsktra<caracter>* node = new dijsktra<caracter>(grafo,grafo->getMap()->at(1));
    dijsktra<Airport>* airports = new dijsktra<Airport>(connectedGraph,connectedGraph->getMap()->at(1));
    node->calculate();
    airports->calculate();

    parseToJsonTxt(airports->getClosestPath(3400),"../output/dijkstra1.json");


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

    // - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - F L O Y D W A R S H A L L - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - -

    FloydWarshall<caracter> *floydWarshall1 = new FloydWarshall<caracter>(grafo);
    floydWarshall1->printAdjacencyMatrix();
    floydWarshall1->printSequenceMatrix();

    floydWarshall1->calculate();

    floydWarshall1->printAdjacencyMatrix();
    floydWarshall1->printSequenceMatrix();

    Graph<int> *graph2 = new Graph<int>;

    for (int i = 2; i <= 10; i += 2)
    {
        auto newNode = new Node<int>();
        newNode->setID(i);
        graph2->insertNode(newNode);
    }

    graph2->insertEdge(2, 4, 1);
    graph2->insertEdge(4, 6, 2);
    graph2->insertEdge(6, 8, 3);
    graph2->insertEdge(8, 2, 4);
    graph2->insertEdge(4, 10, 1);
    graph2->insertEdge(10, 8, 3);

    auto floyd2 = new FloydWarshall<int>(graph2);
//    floyd2->printAdjacencyMatrix();
//    floyd2->printSequenceMatrix();
//    floyd2->calculate();
//    floyd2->printAdjacencyMatrix();
//    floyd2->printSequenceMatrix();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - B E L L M A N F O R D - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

    BellmanFord<caracter> *bellmanFord = new BellmanFord<caracter>(grafo, 2);
    bellmanFord->calculate();
    bellmanFord->print();
    list<Edge<caracter> *> *edges = bellmanFord->getClosestPathsEdges();

    BellmanFord<Airport> *airports2 = new BellmanFord<Airport>(connectedGraph, 1);
    //airports2->calculate();
//    airports2->print();

    list<Edge<Airport> *> *edgesOfBellmanFord = airports2->getClosestPathsEdges();
    parseToJsonTxt(edgesOfBellmanFord, "../output/BellmanFord.json");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - D F S B F S - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Graph<caracter> *grafoDFS = grafo->DFS(1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Graph<Airport> *airportBFS = connectedGraph->BFS(1);
    parseToJsonTxt(airportBFS, "../output/airportBFS.json");

    Graph<Airport> *airportDFS = connectedGraph->BFS(1);
    parseToJsonTxt(airportDFS, "../output/airportDFS.json");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Graph<int> *graphDFStest = new Graph<int>();

    for (int i = 1; i <= 4; ++i)
    {
        Node<int> *newNode = new Node<int>();
        newNode->setID(i);
        graphDFStest->insertNode(newNode);
    }

    graphDFStest->insertEdge(1, 2, 1);
    graphDFStest->insertEdge(1, 3, 2);
    graphDFStest->insertEdge(3, 4, 3);

    auto graphDFStestResult = graphDFStest->DFS(1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Graph<caracter> *grafoBFS = grafo->BFS(1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - O T H E R - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Graph<caracter> *graphBuiltFromEdges = new Graph<caracter>(bellmanFord->getClosestPathsEdges());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - T I M E - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << duration.count() / 1000000.f << endl;

    return EXIT_SUCCESS;
}