#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

const float RANDOM_RANGE = 1.0;

class Vehicle {
protected:
    float position;
    float velocity;

public:
    Vehicle(float pos, float vel) : position(pos), velocity(vel) {}

    virtual void updatePosition(float dt) {
        position += velocity * dt;
    }

    void adjustVelocity() {
        const float maxChange = 0.1; // Phạm vi thay đổi tối đa
        velocity += (static_cast<float>(rand()) / RAND_MAX) * maxChange * 2 - maxChange;

        // Làm tròn kết quả vận tốc đến 2 chữ số thập phân
        velocity = round(velocity * 100.0) / 100.0;

        // Đảm bảo vận tốc không vượt quá giới hạn
        if (velocity > 1.0) {
            velocity = 1.0;
        }
        else if (velocity < -1.0) {
            velocity = -1.0;
        }
    }

    float getPosition() const { return position; }
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(float pos, float vel) : Vehicle(pos, vel) {}
};

class Car : public Vehicle {
public:
    Car(float pos, float vel) : Vehicle(pos, vel) {}
};

class Truck : public Vehicle {
public:
    Truck(float pos, float vel) : Vehicle(pos, vel) {}
};

void simulateTraffic(float L, float dt, int n, int N_MAX1, int N_MAX2, int N_MAX3, vector<Vehicle*>& vehicles) {
    ofstream outFile("mophong.txt");
    if (!outFile.is_open()) {
        cerr << "Failed to open output file!" << endl;
        return;
    }

    int totalVehicleCount = N_MAX1 + N_MAX2 + N_MAX3;
    int vehicleCounts[3] = { N_MAX1, N_MAX2, N_MAX3 };

    for (int i = 0; i < n; ++i) {
        for (auto it = vehicles.begin(); it != vehicles.end();) {
            (*it)->updatePosition(dt);
            (*it)->adjustVelocity();

            if ((*it)->getPosition() > L) {
                delete* it;
                it = vehicles.erase(it);
            }
            else {
                ++it;
            }
        }

        // Create new vehicles if needed
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < vehicleCounts[j]; ++k) {
                if (vehicles.size() < totalVehicleCount) {
                    switch (j) {
                    case 0:
                        vehicles.push_back(new Motorcycle(0, 1.0)); // Motorcycle
                        break;
                    case 1:
                        vehicles.push_back(new Car(0, 1.0)); // Car
                        break;
                    case 2:
                        vehicles.push_back(new Truck(0, 1.0)); // Truck
                        break;
                    }
                }
            }
        }

        // Write vehicle positions to file
        for (const auto& vehicle : vehicles) {
            outFile << fixed << setprecision(2) << vehicle->getPosition() << " ";
        }
        outFile << endl;
    }

    outFile.close();
}

int main() {
    srand(time(nullptr)); // Seed random number generator

    float L, dt;
    int n, N_MAX1, N_MAX2, N_MAX3;

    cout << "Enter length of road (L): ";
    cin >> L;
    cout << "Enter time step (dt): ";
    cin >> dt;
    cout << "Enter number of time steps (n): ";
    cin >> n;
    cout << "Enter maximum number of motorcycles (N_MAX1): ";
    cin >> N_MAX1;
    cout << "Enter maximum number of cars (N_MAX2): ";
    cin >> N_MAX2;
    cout << "Enter maximum number of trucks (N_MAX3): ";
    cin >> N_MAX3;

    vector<Vehicle*> vehicles;

    simulateTraffic(L, dt, n, N_MAX1, N_MAX2, N_MAX3, vehicles);

    // Clean up memory
    for (auto& vehicle : vehicles) {
        delete vehicle;
    }

    return 0;
}
