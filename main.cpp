#include <iostream>
#include "nlohmann/json.hpp"
#include "functions.cpp"
#include "node.h"

using namespace std;
using json = nlohmann::json;

int main() {
    Graph<Airport>* bipartiteGraph = readJsonAndReturnAirportGraph("jsonFiles/bipartito.json");
    Graph<Airport>* notDirectedGraph = readJsonAndReturnAirportGraph("jsonFiles/notDirected.json");
    Graph<Airport>* directedGraph = readJsonAndReturnAirportGraph("jsonFiles/directed.json");
    Graph<Airport>* connectedGraph = readJsonAndReturnAirportGraph("jsonFiles/conexo.json");

    if (notDirectedGraph->findEdge(1,2))
        cout << "Edge found" << endl;

    //auto krusca=notDirectedGraph->Krusca();

    if (connectedGraph->setIsConnected())
        cout << "The graph is connected" << endl;

    if(bipartiteGraph->setIsBipartite())
        cout << "The graph is bipartite" << endl;

    if(notDirectedGraph->setIsNotDirected())
        cout << "The graph is not directed" << endl;

    if (directedGraph->setIsNotDirected())
        cout << "The graph is directed" << endl;

    auto krusca = notDirectedGraph->Kruskal();
    string filename;
    cin>>filename;
    filename = filename+".json";
    parseToJsonTxt(krusca,filename);

    delete bipartiteGraph;
    delete directedGraph;
    delete connectedGraph;
    delete notDirectedGraph;
    return EXIT_SUCCESS;
}
