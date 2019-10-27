#include <iostream>
#include "nlohmann/json.hpp"
#include "functions.cpp"
#include "Node.h"

using namespace std;
using json = nlohmann::json;

int main() {
    ifstream ifs("/Users/Usuario/Documents/Estudio/GitHub/Grafo01/directed.json");
    if (ifs.fail()){cout<<"error"<<endl; exit(-1);}
    json json1 =json::parse(ifs);
    Graph<Airport>* graph = buildGraph(json1);
    if (graph->findEdge(1,2))
        cout<<"found"<<endl;
    auto krusca=graph->Krusca();
    if(graph->setIsConex()){
        cout<<"conexo"<<endl;
    }
    if(graph->setIsBipar()){
        cout<<"Es bipartito"<<endl;
    }
    if(graph->setIsNotDirected())
        cout<<"IS not directed"<<endl;
    graph->deleteNode(1);
}

