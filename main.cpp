#include <iostream>
#include "nlohmann/json.hpp"
#include "functions.cpp"
#include "node.h"

using namespace std;
using json = nlohmann::json;

int main() {
    Graph<Airport>* bipartiteGraph = readJsonAndReturnAirportGraph("../jsonFiles/airports.json");
    Graph<Airport>* notDirectedGraph = readJsonAndReturnAirportGraph("../jsonFiles/notDirected.json");
    Graph<Airport>* directedGraph = readJsonAndReturnAirportGraph("../jsonFiles/directed.json");
    Graph<Airport>* connectedGraph = readJsonAndReturnAirportGraph("../jsonFiles/conexo.json");

    if (notDirectedGraph->findEdge(1,2))
        cout << "Edge found" << endl;

    //auto krusca=notDirectedGraph->Krusca();

    if (bipartiteGraph->setIsConnected())
        cout << "The graph is connected" << endl;

    if(bipartiteGraph->setIsBipartite())
        cout << "The graph is bipartite" << endl;

    if(bipartiteGraph->setIsNotDirected())
        cout << "The graph is not directed" << endl;

    if (!directedGraph->setIsNotDirected())
        cout << "The graph is directed" << endl;

    auto prim = notDirectedGraph->prim(1);
    auto krusca = notDirectedGraph->Kruskal();

    return EXIT_SUCCESS;
}