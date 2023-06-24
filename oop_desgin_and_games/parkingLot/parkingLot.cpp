#include "parkingLot.h"


ParkingLot::ParkingLot(int floors, int rows, int spotsPerRow) {
    this->floors = floors;
    this->rows = rows;
    this->spotsPerRow = spotsPerRow;
    spots.resize(floors);
    for (int i = 0; i < floors; i++) {
        spots[i].resize(rows);
        for (int j = 0; j < rows; j++) {
            spots[i][j].resize(spotsPerRow);
        }
    }
}


bool ParkingLot::park(Vehicle* v, int floor, int row, int spot) {
    if (spots[floor][row][spot] == nullptr) {
        spots[floor][row][spot] = v;
        std::cout << "Vehicle " << v->getType() << " has parked successfully at floor "
                  << floor << ", row " << row << ", spot " << spot << ".\n";
        return true;
    }
    std::cout << "Spot is already occupied, try again!\n";
    return false;
}

bool ParkingLot::leave(Vehicle* v) {
    for (int i = 0; i < floors; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < spotsPerRow; k++) {
                if (spots[i][j][k] == v) {
                    double hours = calculateHoursParked(spots[i][j][k]);
                    double cost = spots[i][j][k]->calculateCost(hours);
                    spots[i][j][k] = nullptr;
                    std::cout << v->getType() << " left successfully.\n";
                    std::cout << "Total cost : " << cost << "\n";
                    return true;
                }
            }
        }
    }
    std::cout << "Vehicle " << v->getType() << " is not found!\n";
    return false;
}

int ParkingLot::availableSpots(int floor) {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < spotsPerRow; j++) {
            if (spots[floor][i][j] == nullptr) {
                count++;
            }
        }
    }
    return count;
}

int ParkingLot::handicappedSpots(int floor) {
    int count = 0;
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < spotsPerRow; j++) {
            if (dynamic_cast<HandicappedVehicle*>(spots[floor][i][j]) != nullptr) {
                count++;
            }
        }
    }
    return count;
}

double ParkingLot::calculateHoursParked(Vehicle* v) {
    for (int i = 0; i < floors; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < spotsPerRow; k++) {
                if (spots[i][j][k] == v) {
                    // Current time.
                    auto now = std::chrono::system_clock::now();
                    // Time the vehicle was parked.
                    auto parkedTime = spots[i][j][k]->getParkedTime();
                    // Calculate the actual hours vehicle was parekd.
                    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - parkedTime);
                    return duration.count();
                }
            }
        }
    }
    return 0.0;
}
