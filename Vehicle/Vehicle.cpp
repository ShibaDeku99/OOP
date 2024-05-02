#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()

using namespace std;

const float MAX_POSITION = 100.0f; // maximum position on the road
const float DEFAULT_SPEED = 1.0f; // default speed of vehicles
const float MAX_SPEED_CHANGE = 1.0f; // maximum speed change in one time-step

class Vehicle {
protected:
    float position;
    float speed;

public:
    Vehicle(float initialPosition, float initialSpeed) : position(initialPosition), speed(initialSpeed) {}

    virtual void updatePosition(float dt) {
        position += speed * dt;
    }

    virtual void adjustSpeed() {
        float speedChange = (rand() % 3 - 1) * MAX_SPEED_CHANGE; // random speed change between -1 and 1
        speed += speedChange;
    }

    float getPosition() const {
        return position;
    }

    float getSpeed() const {
        return speed;
    }
};

class Car : public Vehicle {
public:
    Car(float initialPosition, float initialSpeed) : Vehicle(initialPosition, initialSpeed) {}

    // Optionally, you can override the updatePosition and adjustSpeed methods if Car needs specific behavior.
};

class TrafficSimulation {
private:
    vector<Vehicle*> vehicles;
    float roadLength;
    float dt;
    int maxVehicles;

public:
    TrafficSimulation(float roadLength, float timeStep, int maxVehicles) : roadLength(roadLength), dt(timeStep), maxVehicles(maxVehicles) {}

    void initialize() {
        srand(static_cast<unsigned int>(time(nullptr))); // seed the random number generator

        // Create initial vehicles
        vehicles.push_back(new Car(0.0f, DEFAULT_SPEED));
    }

    void simulate(int steps) {
        ofstream outputFile("mophong.txt");
        if (!outputFile.is_open()) {
            cerr << "Error opening output file!" << endl;
            return;
        }

        for (int i = 0; i < steps; ++i) {
            for (auto it = vehicles.begin(); it != vehicles.end(); ) {
                (*it)->updatePosition(dt);
                (*it)->adjustSpeed();
                
                if ((*it)->getPosition() > roadLength) {
                    delete *it;
                    it = vehicles.erase(it);
                } else {
                    outputFile << (*it)->getPosition() << " ";
                    ++it;
                }
            }

            if (vehicles.size() < maxVehicles) {
                vehicles.push_back(new Car(0.0f, DEFAULT_SPEED));
            }

            outputFile << endl;
        }

        outputFile.close();
    }

    ~TrafficSimulation() {
        for (auto vehicle : vehicles) {
            delete vehicle;
        }
        vehicles.clear();
    }
};

int main() {
    int n; // number of steps
    float L; // road length
    float dt; // time-step
    int N_MAX; // maximum number of vehicles

    cout << "Enter the number of steps, road length, time-step, and maximum number of vehicles: ";
    cin >> n >> L >> dt >> N_MAX;

    TrafficSimulation simulation(L, dt, N_MAX);
    simulation.initialize();
    simulation.simulate(n);

    return 0;
}
