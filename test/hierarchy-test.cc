#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

#include <unordered_map>

using namespace std;

class Vehicle {
public:
    double litrePerKm;
    virtual string toString() {
        return "";
    }
};

class Car: public Vehicle {
public:
    Car() {
        litrePerKm = 4;
    }

    string toString() {
        stringstream ss;
        ss << "Car(" << litrePerKm << ")";
        return ss.str();
    }
};

class Truck: public Vehicle {
public:
    Truck() {
        litrePerKm = 9;
    }

    string toString() {
        stringstream ss;
        ss << "Truck(" << litrePerKm << ")";
        return ss.str();
    }
};


class ClassificationDetails {};

class EnsembleClassifierDetails: public ClassificationDetails {
public:
    int binaryCode;
};

int main(int argc, char** argv) {
    vector<Vehicle*> vehicles;

    Vehicle* car = new Car();
    Vehicle* truck = new Truck();

    vehicles.push_back(car);
    vehicles.push_back(truck);

    for (int i = 0; i < 2; i++) {
        Vehicle* vehicle = vehicles[i];
        string vehicleString = vehicle->toString();
        printf("This vehicle is %s\n", vehicleString.c_str());
    }

    unordered_map<string, ClassificationDetails*> detailMap;

    EnsembleClassifierDetails* ensembleDetails = new EnsembleClassifierDetails();
    ensembleDetails->binaryCode = 6870;

    ClassificationDetails* detailRet = detailMap["ensemble"] = ensembleDetails;
    EnsembleClassifierDetails* ensembleDetailsRet = (EnsembleClassifierDetails*) detailRet;
    printf("BC: %d\n", ensembleDetailsRet->binaryCode);

    return 0;
}
