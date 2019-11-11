//
// Created by Usuario on 05/10/2019.
//
#include "Airports.h"
#include "graph.h"
#include <fstream>
#include <utility>
#include <cmath>
#include <iomanip>

void makeAirportsToJson(Graph<Airport> *graph, json &finalParsedJson);

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
    if(graph->insertEdge(idFrom,idTo))
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

void buildEdgeFromGraph(Node<Airport>* node, Graph<Airport>* graph){
    auto Nodes = graph->getMap();
    if(node != nullptr) {
        vector<int>* destination = (node->getObject())->getDestinations();
        for (int & i : *destination) {
            auto * edge = new Edge<Airport>();
            if (Nodes->operator[](i) == nullptr) {
                Nodes->erase(i);
            } else {
                graph->setEdges(graph->getEdges()+1);
                edge->setFrom(node);
                edge->setTo(Nodes->operator[](i));
                double x1 = node->getObject()->getLatitude(), y1 = node->getObject()->getLongitude();
                double x2 = Nodes->operator[](i)->getObject()->getLatitude(), y2 = Nodes->operator[](i)->getObject()->getLongitude();
                edge->setWeight(calculateWeight(x1,x2,y1,y2));
                (node->getEdges())->push_back(edge);
            }
        }

    } else cout<<"error"<<endl;
}

void parseToJsonTxt(Graph<Airport>* graph,  const string& fileout){
    ofstream ofs(fileout);
    json finalParsedJson;
    makeAirportsToJson(graph, finalParsedJson);
    ofs<<std::setw(4)<<finalParsedJson<<endl;

}

void makeAirportsToJson(Graph<Airport> *graph, json &finalParsedJson) {
    int sizeOfGraph = graph->getMap()->size();
    map<int,Node<Airport>*>::iterator iterator = graph->getMap()->begin();
    for (int i = 0; i < sizeOfGraph; i++) {
        Airport* airportActual = iterator->second->getObject();
        list<Edge<Airport>*>* edgeActual = iterator->second->getEdges();
        vector<int> destinos;
        for (list<Edge<Airport>*>::iterator j = edgeActual->begin(); j != edgeActual->end(); ++j) {
            destinos.push_back((*j)->getTo()->getID());
        }
        json departures(destinos);
        json parsedJson = {
                {"City", airportActual->getCity()},
                {"Name", airportActual->getName()},
                {"Country", airportActual->getCountry()},
                {"Longitude", airportActual->getLongitude()},
                {"Latitude", airportActual->getLatitude()},
                {"Id", airportActual->getId()}
        };
        parsedJson["destinations"] = departures;
        finalParsedJson.push_back(parsedJson);
        iterator++;
    }
}

void makeAirportsToJson(list<Edge<Airport>*> *edges, json &finalParsedJson) {
    int sizeOfGraph = edges->size();
    list<Edge<Airport>*>::iterator iterator = edges->begin();
    for (int i = 0; i < sizeOfGraph; i++) {
        Airport* airportActual = (*iterator)->getTo()->getObject();
        list<Edge<Airport>*>* edgeActual = (*iterator)->getTo()->getEdges();
        vector<int> destinos;
        for (list<Edge<Airport>*>::iterator j = edgeActual->begin(); j != edgeActual->end(); ++j) {
            destinos.push_back((*j)->getTo()->getID());
        }
        json departures(destinos);
        json parsedJson = {
                {"City", airportActual->getCity()},
                {"Name", airportActual->getName()},
                {"Country", airportActual->getCountry()},
                {"Longitude", airportActual->getLongitude()},
                {"Latitude", airportActual->getLatitude()},
                {"Id", airportActual->getId()}
        };
        parsedJson["destinations"] = departures;
        finalParsedJson.push_back(parsedJson);
        iterator++;
    }
}

void makePathToJson(list<Node<Airport>*>* closesPath, json &finalParsedJson){
    for (auto iterador = closesPath->begin() ; iterador != closesPath->end() ; ++iterador) {
        Airport* airportActual = (*iterador)->getObject();
        list<Edge<Airport>*>* edgeActual = (*iterador)->getEdges();
        vector<int> destinos;
        for (list<Edge<Airport>*>::iterator j = edgeActual->begin(); j != edgeActual->end(); ++j) {
            destinos.push_back((*j)->getTo()->getID());
        }
        json departures(destinos);
        json parsedJson = {
                {"City", airportActual->getCity()},
                {"Name", airportActual->getName()},
                {"Country", airportActual->getCountry()},
                {"Longitude", airportActual->getLongitude()},
                {"Latitude", airportActual->getLatitude()},
                {"Id", airportActual->getId()}
        };
        parsedJson["destinations"] = departures;
        finalParsedJson.push_back(parsedJson);
    }
}
void parseToJsonTxt(list<Node<Airport>*>* graph,  const string& fileout){
    ofstream ofs(fileout);
    json finalParsedJson;
    makePathToJson(graph, finalParsedJson);
    ofs<<std::setw(4)<<finalParsedJson<<endl;

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
    graph->setVertexes(vertexes);
    for (auto j = maps->begin(); j != maps->end(); j++) {
        if (j->second != nullptr)
            buildEdgeFromGraph(j->second, graph);
        else
            cout<<j->first<<" "<<endl;
    }
    return graph;
}

Graph<Airport>* readJsonAndReturnAirportGraph(const string& nameOfJSON) {
    ifstream inputFile(nameOfJSON);
    if (inputFile.fail()) {
        throw invalid_argument("File not found");
    }
    json inputJson = json::parse(inputFile);
    Graph<Airport>* graph = buildGraph(inputJson);
    return graph;
}

void floydWarshall() {
    int numberOfVertexes;
    cout << "Enter the number of vertexes: ";
    cin >> numberOfVertexes;

    double distanceMatrix[numberOfVertexes][numberOfVertexes], nodeOrderMatrix[numberOfVertexes][numberOfVertexes];

    for (int i = 0; i < numberOfVertexes; ++i) {
        for (int j = i; j < numberOfVertexes; ++j) {
            if (i == j)
                distanceMatrix[i][j] = INT_MAX;
            else {
                cout << "Enter the weight of the incident edge to " << i + 1 << " and " << j + 1 << ": ";
                cin >> distanceMatrix[i][j];
                distanceMatrix[j][i] = distanceMatrix[i][j];
            }
        }
    }

    for (int i = 0; i < numberOfVertexes; ++i)
        for (int j = 0; j < numberOfVertexes; ++j)
            i == j ? nodeOrderMatrix[i][j] == INT_MAX : nodeOrderMatrix[i][j] = j + 1;

    double distanceMatrixIterations[numberOfVertexes][numberOfVertexes], nodeOrderMatrixIterations[numberOfVertexes][numberOfVertexes];

    for (int i = 0; i < numberOfVertexes; i++) {
        for (int j = 0; j < numberOfVertexes; j++) {
            if (i == j) {
                distanceMatrixIterations[i][j] = INT_MAX;
                nodeOrderMatrixIterations[i][j] = INT_MAX;
            } else {
                distanceMatrixIterations[i][j] = 0;
                nodeOrderMatrixIterations[i][j] = 0;
            }
        }
    }

    for (int iterationNumber = 0; iterationNumber < numberOfVertexes; ++iterationNumber) {
        for (int i = 0; i < numberOfVertexes; ++i) {
            distanceMatrixIterations[iterationNumber][i] = distanceMatrix[iterationNumber][i];
            distanceMatrixIterations[i][iterationNumber] = distanceMatrix[i][iterationNumber];
            nodeOrderMatrixIterations[iterationNumber][i] = nodeOrderMatrix[iterationNumber][i];
            nodeOrderMatrixIterations[i][iterationNumber] = nodeOrderMatrix[i][iterationNumber];
        }

        for (int i = 0; i < numberOfVertexes; ++i) {
            if (i == iterationNumber) continue;
            for (int j = 0; j < numberOfVertexes; ++j) {
                if (j == iterationNumber) continue;
                if (i == j) continue;
                if(distanceMatrix[i][j] > distanceMatrix[i][iterationNumber] + distanceMatrix[iterationNumber][j]) {
                    distanceMatrixIterations[i][j] = distanceMatrix[i][iterationNumber] + distanceMatrix[iterationNumber][j];
                    nodeOrderMatrixIterations[i][j] = iterationNumber + 1;
                } else {
                    distanceMatrixIterations[i][j] = distanceMatrix[i][j];
                    nodeOrderMatrixIterations[i][j] = nodeOrderMatrix[i][j];
                }
            }
        }

        for(int i = 0; i < numberOfVertexes; ++i) {
            for(int j = 0; j < numberOfVertexes; ++j) {
                distanceMatrix[i][j] = distanceMatrixIterations[i][j];
                nodeOrderMatrix[i][j] = nodeOrderMatrixIterations[i][j];
            }
        }
    }

    cout << "Distance Matrix: " << endl;
    for (int i = 0; i < numberOfVertexes; ++i) {
        for (int j = 0; j < numberOfVertexes; ++j) {
            distanceMatrix[i][j] != INT_MAX ? cout << distanceMatrix[i][j] : cout << "i";
            cout << " ";
        }
        cout << endl;
    }

    cout << endl << "Nodes order Matrix: " << endl;

    for (int i = 0; i < numberOfVertexes; ++i) {
        for (int j = 0; j < numberOfVertexes; ++j) {
            nodeOrderMatrix[i][j] != INT_MAX ? cout << nodeOrderMatrix[i][j] : cout << "i";
            cout << " ";
        }
        cout << endl;
    }
}