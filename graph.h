#ifndef GRAFO01_GRAPH_H
#define GRAFO01_GRAPH_H

#include <iterator>
#include <map>
#include "node.h"
#include "edge.h"

using namespace std;

template <class T>
class Graph{
    map< int , Node<T> *> *graphNodesMap;
    int numberOfEdges = 0;
    int numberOfVertexes = 0;
    double density = 0;
    bool isNotDirected = false;
    bool isConnected = false;
    bool isBipartite = false;

    bool findEdge(int idFrom, int idTo, typename std::list<Edge<T>*>::iterator & guidePtr ){
        if (graphNodesMap->operator[](idFrom) == nullptr){
            graphNodesMap->erase(idFrom);
            return false;
        }else if (graphNodesMap->operator[](idTo) == nullptr) {
            graphNodesMap->erase(idTo);
            return false;
        }else{
            auto* From = graphNodesMap->operator[](idFrom);
            auto* toCompare = graphNodesMap->operator[](idTo);
            for (auto i = From->getEdges()->begin(); i != From->getEdges()->end() ; i++) {
                Edge<Airport>* edge = *i;
                if(edge->getTo() == toCompare ){
                    guidePtr = (i);
                    return true;
                }
            }
            return false;
        }
    }


    void deleteAllEdgesOfNode(int idNode){
        for (auto i = graphNodesMap->begin(); i != graphNodesMap->end() ; ++i) {
            deleteEdge(i->first, idNode);
        }
    }

    void deleteAllEdges() {
        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end(); ++it) {
            deleteAllEdgesOfNode(it->first);
        }
    }

    void setNodesBlank(){
        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end(); it++) {
            (it->second)->setColor('B');
        }
    }

    bool testBipartite(Node<T>* toTest, char color){
        int FinalValue= toTest->getEdges()->size();
        int thisValue=0;
        if (color == 'R'){
            for (auto it = toTest->getEdges()->begin(); thisValue < FinalValue ; ++it, thisValue++) {
                if((*it)->getTo()->getColor() == 'B' ){
                    (*it)->getTo()->setColor('A');
                    return testBipartite((*it)->getTo(), 'A');
                }else if((*it)->getTo()->getColor() == 'R')
                    return false;
                else if((*it)->getTo()->getColor() == 'A'){

                } else throw invalid_argument("Memory failure");
            }
        }else if (color == 'A') {
            for (auto it = toTest->getEdges()->begin(); thisValue < FinalValue ; ++it, thisValue++) {
                if((*it)->getTo()->getColor() == 'B' ){
                    (*it)->getTo()->setColor('R');
                    return testBipartite((*it)->getTo(), 'R');
                }else if((*it)->getTo()->getColor() == 'A')
                    return false;
                else if((*it)->getTo()->getColor() == 'R'){

                } else throw invalid_argument("Memory failure");
            }
        }
        return true;
    }

    list<Edge<T>* >* sortEdgesWeight(){
        auto respt= new list<Edge<T>*>;
        for (auto it_Nodes = graphNodesMap->begin(); it_Nodes != graphNodesMap->end() ; it_Nodes++) {
            Node<T>* actual = it_Nodes->second;
            for (auto it_edges = actual->getEdges()->begin(); it_edges != actual->getEdges()->end() ; it_edges++) {
                if(respt->size() == 0){
                    respt->push_back(*(it_edges));
                }else{
                    int control_size = 0;
                    typename list<Edge<T>*>::iterator it = respt->begin();
                    while (control_size<respt->size() and (*it_edges)->getWeight()<(*it)->getWeight()){
                        control_size++;
                        it++;
                    }
                    respt->insert((it),(*it_edges));
                }
            }
        }
        return respt;
    }

    void privateDFS(int idOfNode, map<int, bool> &visitedNodes, vector<int> &vectorDFS, Graph<T> *graph) {
        visitedNodes[idOfNode] = true;
        vectorDFS.push_back(idOfNode);

        auto listOfEdges = graphNodesMap->operator[](idOfNode)->getEdges();
        for (auto it = listOfEdges->begin(); it != listOfEdges->end(); ++it) {
            if (!visitedNodes[(*it)->getTo()->getID()]) {

                Node<T>* newNode = new Node<T>;
                newNode->setID((*it)->getTo()->getID());

                Node<T> *currentNode = (*graphNodesMap)[(*it)->getTo()->getID()];
                T objectData = *(currentNode->getObject());

                T* newObject = &objectData;
                newNode->setObject(newObject);

                graph->insertNode(newNode);

                graph->insertEdge(idOfNode, (*it)->getTo()->getID(), (*it)->getWeight());

                privateDFS((*it)->getTo()->getID(), visitedNodes, vectorDFS, graph);
            }
        }
    }

public:
    Graph(){graphNodesMap = new map< int , Node<T>* >;}

    map< int , Node<T>* >* getMap(){ return graphNodesMap;}

    list<Edge<T>* >* getSortEdgesWeight() {
        return this->sortEdgesWeight();
    }

    int getEdges() {
        return numberOfEdges;
    }

    void setEdges(int edges){
        numberOfEdges=edges;
    }

    void setVertexes(int vertexes) {
        numberOfVertexes = vertexes;
    }

    bool insertEdge(int idFrom, int idTo){
        if (graphNodesMap->operator[](idFrom) != nullptr and graphNodesMap->operator[](idTo) != nullptr){
            if(!findEdge(idFrom,idTo)){
                auto* edge = new Edge<T>;
                edge->setTo(graphNodesMap->operator[](idTo));
                edge->setFrom(graphNodesMap->operator[](idFrom));
                graphNodesMap->operator[](idFrom)->getEdges()->push_front(edge);
                numberOfEdges++;
                return true;
            }else return false;

        } else{
            if (graphNodesMap->operator[](idFrom) == nullptr){
                graphNodesMap->erase(idFrom);}
            if (graphNodesMap->operator[](idTo) == nullptr){
                graphNodesMap->erase(idTo);}
            return false;
        }
    }
    bool insertEdge(int idFrom, int idTo, int peso){
        if (graphNodesMap->operator[](idFrom) != nullptr and graphNodesMap->operator[](idTo) != nullptr){
            if(!findEdge(idFrom,idTo)){
                auto* edge = new Edge<T>;
                edge->setWeight(peso);
                edge->setTo(graphNodesMap->operator[](idTo));
                edge->setFrom(graphNodesMap->operator[](idFrom));
                graphNodesMap->operator[](idFrom)->getEdges()->push_front(edge);
                numberOfEdges++;
                return true;
            }else return false;

        } else{
            if (graphNodesMap->operator[](idFrom) == nullptr){
                graphNodesMap->erase(idFrom);}
            if (graphNodesMap->operator[](idTo) == nullptr){
                graphNodesMap->erase(idTo);}
            return false;
        }
    }

    bool insertNode(Node<T>* node){
        int id = node->getID();
        if (graphNodesMap->operator[](id) == nullptr){
            graphNodesMap->erase(id);
            graphNodesMap->insert(pair<int, Node<T>*> (id,node));
            numberOfVertexes++;
            return true;
        }else{
            throw invalid_argument("Id in use");
        }
    }

    bool deleteNode(int idNode){
        if(graphNodesMap->operator[](idNode) != nullptr){
            graphNodesMap->erase(idNode);
            deleteAllEdgesOfNode(idNode);
            return true;
        }else {
            graphNodesMap->erase(idNode);
            return false;
        }
    }

    bool findEdge(int idFrom, int idTo){
        if (graphNodesMap->operator[](idFrom) == nullptr){
            graphNodesMap->erase(idFrom);
            return false;
        }else if (graphNodesMap->operator[](idTo) == nullptr) {
            graphNodesMap->erase(idTo);
            return false;
        }else{
            auto* From = graphNodesMap->operator[](idFrom);
            auto* toCompare = graphNodesMap->operator[](idTo);
            if(From->getEdges() != NULL and !(From->getEdges()->empty())){
                for (auto i = From->getEdges()->begin(); i != From->getEdges()->end() ; i++) {
                    Edge<T>* edge = *i;
                    if(edge->getTo() == toCompare )
                        return true;
                }
            }
            return false;
        }
    }

    bool findNode(int id){
        if(graphNodesMap->operator[](id)== nullptr){
            graphNodesMap->erase(id);
            return false;
        } else
            return true;
    }

    double getDensity(){
        if(numberOfVertexes == 0 or numberOfVertexes == 1){
            return 0;
        }else {
            density = numberOfEdges/(numberOfVertexes*(numberOfVertexes-1));
            return density;

        }
    }

    Graph<T>* prim(int idOfSource) {
        if (this->setIsNotDirected()) {
            auto* primGraph = new Graph();
            Node<T>* FirstNode = new Node<T>(graphNodesMap->at(idOfSource)->getObject());
            FirstNode->getEdges()->clear();
            vector<list<Edge<T>*>> vectorOfListEdges;
            list<Edge<T>*> edgesInsert = *(graphNodesMap->operator[](idOfSource)->getEdges());
            vectorOfListEdges.push_back(edgesInsert);
            Edge<T> minEdge;
            while (!vectorOfListEdges.empty() and primGraph->getEdges()<2*graphNodesMap->size() and primGraph->getMap()->size()!=graphNodesMap->size()){
                minEdge = getMinEdgeFromVector(vectorOfListEdges);
                if(!(primGraph->findNode(minEdge.getTo()->getID()))){
                    Node<T>* nodeToInsert = new Node<T>(minEdge.getTo()->getObject());
                    nodeToInsert->setID(nodeToInsert->getObject()->getId());
                    primGraph->insertNode(nodeToInsert);
                    primGraph->insertEdge(minEdge.getFrom()->getID(),minEdge.getTo()->getID(),minEdge.getWeight());
                    primGraph->insertEdge(minEdge.getTo()->getID(),minEdge.getFrom()->getID(),minEdge.getWeight());
                    list<Edge<T>*> listToInsert = *(minEdge.getTo()->getEdges());
                    vectorOfListEdges.push_back(*(minEdge.getTo()->getEdges()));
                }
            }
            return primGraph;
        }else{
            throw invalid_argument("Graph is directed");
        }
    }

    Edge<T> getMinEdgeFromVector(vector<list<Edge<T>*>> &vectorOfListEdges){
        auto VectorIterador = (vectorOfListEdges.begin());
        auto minIterador = (*VectorIterador).begin();
        int listPositionAtVector =0;
        Edge<T> minEdge = *(*minIterador);
        int vectorPos = 0;
        for (; VectorIterador != vectorOfListEdges.end(); VectorIterador++) {
            for (auto listIterador = (*VectorIterador).begin(); listIterador !=(*VectorIterador).end(); listIterador++) {
                if(minEdge.getWeight()>(*listIterador)->getWeight()){
                    listPositionAtVector  =vectorPos;
                    minIterador=listIterador;
                    minEdge = *(*listIterador);
                }
            }
            vectorPos++;
        }
        vectorOfListEdges.at(listPositionAtVector).erase(minIterador);
        if(vectorOfListEdges.at(listPositionAtVector).empty()){
            vectorOfListEdges.erase(vectorOfListEdges.begin()+(listPositionAtVector));
        }
        return minEdge;
    }

    void printID(int id) {
        cout << "Node with ID: " << id << endl;
        cout << this->graphNodesMap->operator[](id)->getID() << endl;
    }

    void printEdges(int id) {
        cout << "Edges of node with ID: " << id << endl;
        auto edges = this->graphNodesMap->operator[](id)->getEdges();
        for (auto it = edges->begin(); it != edges->end(); ++it) {
            auto to = (*it)->getTo()->getId();
            cout << to << " ";
        }
        cout << endl;
    }

    Graph<T>* Kruskal(){
        if(this->setIsNotDirected()){
            list<Edge<T>*>* Edges = this->sortEdgesWeight();
            Graph<T>* kruskalgraph = new Graph();
            map<int,Node<T>*>* kruskalMap = kruskalgraph->getMap();
            while(Edges->size() > 0){
                Node<T>* nodeTo = new Node<T>(Edges->back()->getTo()->getObject());
                nodeTo->setID(nodeTo->getObject()->getId());
                Node<T>* nodeFrom = new Node<T>(Edges->back()->getFrom()->getObject());
                nodeFrom->setID(nodeFrom->getObject()->getId());
                int peso =Edges->back()->getWeight();
                Edges->pop_back();
                nodeTo->getEdges()->clear();
                nodeFrom->getEdges()->clear();
                if(kruskalMap->operator[](nodeFrom->getID()) == nullptr or kruskalMap->operator[](nodeTo->getID()) == nullptr){
                    if (kruskalMap->operator[](nodeFrom->getID())== nullptr){
                        kruskalMap->erase(nodeFrom->getID());
                        kruskalMap->insert(pair<int,Node<T>*>(nodeFrom->getID(),nodeFrom));
                    }
                    if (kruskalMap->operator[](nodeTo->getID())== nullptr){
                        kruskalMap->erase(nodeTo->getID());
                        kruskalMap->insert(pair<int,Node<T>*>(nodeTo->getID(),nodeTo));
                    }
                    kruskalgraph->insertEdge(nodeFrom->getID(),nodeTo->getID(),peso);
                    kruskalgraph->insertEdge(nodeTo->getID(),nodeFrom->getID(),peso);
                }
            }
            kruskalgraph->setVertexes(kruskalgraph->getMap()->size());
            return kruskalgraph;
        }else return nullptr;
    }

    bool setIsConnected(){
        map<int, pair<bool ,bool > > temp;
        auto* edges2 = new list<Edge<T> *>;

        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end() ; ++it) {
            temp.insert(pair<int,pair<bool,bool> >(it->first,pair<bool ,bool >(false,false)));
        }
        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end() ; ++it) {
            Node<T> * ptr = it->second;
            temp.at(it->first).first = true;
            if(ptr->getEdges()->empty() or ptr->getEdges()->front()->getTo() == nullptr){
                isConnected = false;
                return isConnected;
            }else {
                *edges2 = *(ptr->getEdges());
                int edgesSize = edges2->size();
                for (int i = 0; i< edgesSize;i++) {
                    int id = edges2->front()->getTo()->getID();
                    edges2->pop_front();
                    temp.at(id).second=true;
                }
            }
        }
        for (auto & it : temp) {
            if(!it.second.first or !it.second.second){
                isConnected = false;
                return isConnected;
            }
        }
        isConnected = true;
        return isConnected;
    }

    bool setIsNotDirected(){
        if(numberOfEdges%2 == 1){
            isNotDirected= false;
            return isNotDirected;
        }
        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end() ; it++) {
            list<Edge<T>*> edges2 = *(it->second->getEdges());
            int edgesSize = edges2.size();
            for (int i = 0; i<edgesSize ;i++) {
                int id = edges2.front()->getTo()->getID();
                edges2.pop_front();
                if(!findEdge(it->first, id) or !findEdge(id,it->first)){
                    isNotDirected= false;
                    return isNotDirected;
                }
            }
        }
        isNotDirected= true;
        return isNotDirected;
    }

    bool setIsBipartite(){
        setNodesBlank();
        auto it = graphNodesMap->begin();
        (it->second)->setColor('R');

        isBipartite = testBipartite((it->second),'R');
        return isBipartite;
    }

    ~Graph(){
        deleteAllEdges();
        delete graphNodesMap;
        graphNodesMap = nullptr;
    }

    bool deleteEdge(int idFrom, int idTo){
        typename std::list<Edge<T>*>::iterator nuevo;
        if (this->findEdge(idFrom,idTo, nuevo)){
            graphNodesMap->operator[](idFrom)->getEdges()->erase(nuevo);
            numberOfEdges--;
            return true;
        }else
            return false;
    }

    void initializeMapDistances(int idFrom, map<int, int> *Distancias) const {
        for (auto iterador = graphNodesMap->begin(); iterador != graphNodesMap->end() ; ++iterador) {
            if(iterador->first != idFrom){
                int INFINITE = -1;
                Distancias->insert(pair<int,int>(iterador->first,INFINITE));
            }else
                Distancias->insert(pair<int,int>(iterador->first,0));
        }
    }

    vector<int> BFS(int idOfSource) {
        map<int, bool> visitedNodes;
        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end(); ++it)
            visitedNodes[it->first] = false;

        visitedNodes[idOfSource] = true;
        list<int> keepTrackOfNodes;
        keepTrackOfNodes.push_back(idOfSource);
        vector<int> vectorBFS;
        while(!keepTrackOfNodes.empty()) {
            int idOfNode = keepTrackOfNodes.front();
            vectorBFS.push_back(idOfNode);
            keepTrackOfNodes.pop_front();
            auto listOfEdges = graphNodesMap->operator[](idOfNode)->getEdges();
            for (auto it = listOfEdges->begin(); it != listOfEdges->end(); ++it) {
                if (!visitedNodes[(*it)->getTo()->getID()]) {
                    visitedNodes[(*it)->getTo()->getID()] = true;
                    keepTrackOfNodes.push_back((*it)->getTo()->getID());
                }
            }
        }
        return vectorBFS;
    }


    Graph<T>* DFS(int idOfSourceNode) {
        map<int, bool> visitedNodes;
        vector<int> vectorDFS;

        Graph<T> *graphDFS = new Graph<T>;

        Node<T>* newNode = new Node<T>;
        newNode->setID(idOfSourceNode);

        Node<T> *currentNode = (*graphNodesMap)[idOfSourceNode];
        T objectData = *(currentNode->getObject());

        T* newObject = &objectData;
        newNode->setObject(newObject);

        graphDFS->insertNode(newNode);

        for (auto it = graphNodesMap->begin(); it != graphNodesMap->end(); ++it)
            visitedNodes[it->first] = false;


        privateDFS(idOfSourceNode, visitedNodes, vectorDFS, graphDFS);
        return graphDFS;
    }


};
#endif //GRAFO01_GRAPH_H