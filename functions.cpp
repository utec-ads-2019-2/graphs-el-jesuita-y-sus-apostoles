//
// Created by Usuario on 05/10/2019.
//
#include "Airports.h"
#include "graph.h"
#include <fstream>
#include <utility>
#include <cmath>

using pairOfIntsAndNodes = pair<int,Node<Airport>*>;

Airport* newAirport(json dato){
    Airport* newAir;
    try {
        newAir = new Airport(std::move(dato));
    }catch (exception &e){
        cout<<"Failed to create Airport"<<endl;
        exit(-1);
    }
    return newAir;
}

void newDestinationAirport(Graph<Airport>* graph,int idFrom, int idTo){
    if(graph->insertEgde(idFrom,idTo))
        graph->getMap()->at(idFrom)->getObject()->getDestinations()->push_back(idTo);
}

void deleteDestinationAirport(Graph<Airport>* graph,int idFrom, int idTo){
    if(graph->deleteEdge(idFrom,idTo)){
        vector<int>* temp =  graph->getMap()->at(idFrom)->getObject()->getDestinations();
        for (auto i = temp->begin(); i !=temp->end(); i++) {
            if(*i == idTo){
                temp->erase(i);
                return;
            }
        }
    }
}

void HalfDestinationDelete(Graph<Airport>* graph, int idTo){
    auto it = graph->getMap()->begin();
    for (int i = 0; i < graph->getMap()->size() ; i++) {
        deleteDestinationAirport(graph,it->first,idTo);
        it++;
    }
}

void deleteAirportFromGraph(Graph<Airport>* graph, int idTo){
    HalfDestinationDelete(graph,idTo);
    graph->deleteNode(idTo);
}

double calculateWeight(double x1, double x2, double y1, double y2){
    double distanceBetweenLatitudes = (x2 - x1) *
                                      M_PI / 180.0;
    double distanceBetweenLongitudes = (y2 - y1) *
                                       M_PI / 180.0;

    x1 *= M_PI / 180.0;
    x2 *= M_PI / 180.0;

    return 12742 * asin(sqrt(pow(sin(distanceBetweenLatitudes / 2), 2) +
                             pow(sin(distanceBetweenLongitudes / 2), 2) * cos(x1) * cos(x2)));
}

void buildEdgeFromGraph(map<int,Node<Airport>*>* Nodes, Node<Airport>* node, Graph<Airport>* graph){
    if(node != nullptr){
        vector<int>* destination = (node->getObject())->getDestinations();
        for (int & i : *destination) {
            // por qué hay un edge de tipo airport, no se supone que esto solo se asigna a vertices?
            auto * edge = new Edge<Airport>();
            if(Nodes->operator[](i) == nullptr){
                // por qué verificas esto? por qué borras el nodo?
                Nodes->erase(i);
            }else{
                graph->setEdges(graph->getEdges()+1);
                edge->setFrom(node);
                edge->setTo(Nodes->operator[](i));
                // se supone que x1 es otro aeropuerto?
                double x1 = node->getObject()->getLatitude(), y1 = node->getObject()->getLongitude();
                double x2 = Nodes->operator[](i)->getObject()->getLatitude(), y2 = Nodes->operator[](i)->getObject()->getLongitude();
                edge->setWeight(calculateWeight(x1,x2,y1,y2));
                (node->getEdges())->push_back(edge);
            }
        }

    } else cout<<"error"<<endl;
}


Graph<Airport>* buildGraph(json file){
    auto* graph = new Graph<Airport>();
    json json1 = std::move(file);
    map< int, Node<Airport>* >* maps = graph->getMap();
    for (const auto& json2 : json1) {
        auto* airport = new Airport(json2);
        auto* node = new Node<Airport>(airport);
        node->setID(node->getObject()->getId());
        (maps)->insert(pairOfIntsAndNodes(node->getID(),node));
    }
    int vertexes = json1.size();
    graph->setCorner(vertexes);
    for (auto j = maps->begin(); j != maps->end(); j++) {
        if (j->second != nullptr)
            buildEdgeFromGraph(maps, j->second, graph);
        else
            cout<<j->first<<" "<<endl;
    }
    return graph;
}

Graph<Airport>* readJsonAndReturnAirportGraph(const string& nameOfJSON) {
    ifstream ifs(nameOfJSON);
    if (ifs.fail()) {
        cout << "Failed loading JSON" << endl;
        return nullptr;
    }

    json ifsJSON = json::parse(ifs);

    Graph<Airport>* graph = buildGraph(ifsJSON);
    return graph;
}
