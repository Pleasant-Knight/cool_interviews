#pragma once

#ifndef _PARKING_LOT_
#define _PARKING_LOT_

#include <iostream>
#include <vector>
#include <chrono>

using std::chrono::time_point;

class Payment {
public:
    virtual double calculateCost(double hours) = 0;
};

class CarPayment : public Payment {
public:
    double calculateCost(double hours) { return hours * 2; }
};

class BikePayment : public Payment {
public:
    double calculateCost(double hours) { return hours * 1; }
};

class HandicappedPayment : public Payment {
public:
    double calculateCost(double hours) { return 0; }
};

class Vehicle {
protected:
    Payment* payment;
    time_point<std::chrono::system_clock> parkedTime;

public:
    virtual std::string getType() = 0;
    virtual double calculateCost(double hours) {
        return payment->calculateCost(hours);
    }

    void setParkedTime() {
        parkedTime = std::chrono::system_clock::now();
    }

    time_point<std::chrono::system_clock> getParkedTime() {
        return parkedTime;
    }
};


class Car : public Vehicle {
public:
    Car() { payment = new CarPayment(); }
    std::string getType() { return "Car"; }
};

class Bike : public Vehicle {
public:
    Bike() { payment = new BikePayment(); }
    std::string getType() { return "Bike"; }
};

class HandicappedVehicle : public Vehicle {
public:
    HandicappedVehicle() { payment = new HandicappedPayment(); }
    std::string getType() { return "Handicapped"; }
};

class ParkingLot {
private:
    std::vector<std::vector<std::vector<Vehicle*>>> spots;
    int floors;
    int rows;
    int spotsPerRow;

public:
    ParkingLot(int floors, int rows, int spotsPerRow);

    bool park(Vehicle* v, int floor, int row, int spot);
    bool leave(Vehicle* v);
    int availableSpots(int floor);
    int handicappedSpots(int floor);

    double calculateHoursParked(Vehicle* v);
};


#endif //_PARKING_LOT_
