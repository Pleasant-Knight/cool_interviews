#include <iostream>
#include <vector>
#include <chrono>
#include "parkingLot.h"

using namespace std;

int main() {
    ParkingLot lot(3, 10, 20);
    Car car1, car2;
    Bike bike1, bike2;
    HandicappedVehicle hv1;

    car1.setParkedTime();
    lot.park(&car1, 0, 0, 0);
    car2.setParkedTime();
    lot.park(&car2, 0, 0, 1);
    bike1.setParkedTime();
    lot.park(&bike1, 0, 0, 2);
    hv1.setParkedTime();
    lot.park(&hv1, 2, 9, 19);

    cout << "Available spots on floor zero: " << lot.availableSpots(0) << "\n";
    cout << "Handicapped spots on floor # 2: " << lot.handicappedSpots(2) << "\n";

    lot.leave(&car1);
    lot.leave(&bike2);

    std::cout << "Available spots on floor zero is: " << lot.availableSpots(0) << "\n";

    return 0;
}
