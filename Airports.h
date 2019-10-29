#ifndef GRAFO01_AIRPORTS_H
#define GRAFO01_AIRPORTS_H

#include <string>
#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class Airport{
private:
    string City;
    string Name;
    string Country;
    double Longitude;
    double Latitude;
    int Id;
    vector<int> *destination;
public:
    Airport(json json1) {
        destination = new vector<int>;
        City = json1["City"];
        Name = json1["Name"];
        Country = json1["Country"];
        string latitude = json1["Latitude"];
        Latitude = atof(latitude.c_str());
        string longitude = json1["Longitude"];
        Longitude = atof(longitude.c_str());
        string id = json1["Id"];
        Id = atoi(id.c_str());
        json json2 = json1["destinations"];
        for (auto i = (json2).begin(); i != json2.end() ; i++) {
            string desti = *i;
            destination->push_back(atoi(desti.c_str()));
        }
    }

    void printAirport() {
        cout << City << endl;
        cout << Name << endl;
        cout << Country << endl;
        cout << Longitude << endl;
        cout << Latitude << endl;
        cout << Id << endl;
        cout << "Destination:" << endl;
        for (int & i : *destination) {
            cout << "   " << i << endl;
        }
    }

    double getLatitude() {
        return Latitude;
    }

    double getLongitude() {
        return Longitude;
    }

    int getId() {
        return Id;
    }

    vector<int>* getDestinations() {
        return destination;
    }

    const string &getCity() const {
        return City;
    }

    const string &getName() const {
        return Name;
    }

    const string &getCountry() const {
        return Country;
    }

    vector<int> *getDestination() const {
        return destination;
    }

    ~Airport(){
        delete destination;
    }
};

#endif //GRAFO01_AIRPORTS_H
