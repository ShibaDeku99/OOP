#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
using namespace std;

class Vehicle {
protected:
    float position;
    float speed;

public:
    Vehicle(float x = 0, float y = 0) : position(x), speed(y) {}

    virtual void updatePosition(float dt) {
        position += speed * dt;
    }

    virtual void adjustSpeed() {
        int adjustment = rand() % 3 - 1;
        speed += adjustment;
    }

    static vector<Vehicle*> vehicles_list; // Danh sách các xe, được khai báo là static

    virtual void simulateStep(float dt, float L, int N_MAX) {
        updatePosition(dt);
        adjustSpeed();
        if (position > L) {
            position = 0;
        }
        if (vehicles_list.size() < N_MAX) {
            Vehicle* new_vehicle = new Vehicle(0, 1);
            vehicles_list.push_back(new_vehicle); // Thêm xe mới vào danh sách
        }
    }

    virtual float getPosition() const { return position; }
    virtual ~Vehicle() {}
};

vector<Vehicle*> Vehicle::vehicles_list; // Khởi tạo danh sách các xe

class Motorcycle : public Vehicle {
public:
    Motorcycle(float x = 0, float y = 0) : Vehicle(x, y) {}

    void updatePosition(float dt) override {
        position += speed * dt * 1.5; // Tốc độ xe máy nhanh hơn
    }

    void adjustSpeed() override {
        int adjustment = rand() % 5 - 2; // Điều chỉnh tốc độ lớn hơn
        speed += adjustment;
    }
};

class Car : public Vehicle {
public:
    Car(float x = 0, float y = 0) : Vehicle(x, y) {}
};

class Truck : public Vehicle {
public:
    Truck(float x = 0, float y = 0) : Vehicle(x, y) {}

    void updatePosition(float dt) override {
        position += speed * dt * 0.8; // Tốc độ xe tải chậm hơn
    }

    void adjustSpeed() override {
        int adjustment = rand() % 3 - 1;
        speed += adjustment;
    }
};

void writePositionToFile(const string& filename) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open the file for writing." << endl;
        return;
    }
    for (const auto& vehicle : Vehicle::vehicles_list) {
        outputFile << vehicle->getPosition() << endl;
    }

    // Đóng file sau khi ghi
    outputFile.close();
}

int main() {
    // Nhập các thông số từ người dùng
    int n, N_MAX;
    float L, dt;
    cout << "Nhap so luong buoc lap (n): ";
    cin >> n;
    cout << "Nhap chieu dai duong (L): ";
    cin >> L;
    cout << "Nhap khoang thoi gian giua cac timestep (dt): ";
    cin >> dt;
    cout << "Nhap so luong xe toi da (N_MAX): ";
    cin >> N_MAX;

    // Mô phỏng với số bước lặp là n
    for (int step = 0; step < n; ++step) {
        // Mô phỏng mỗi bước lặp
        for (auto& vehicle : Motorcycle::vehicles_list) {
            vehicle->simulateStep(dt, L, N_MAX);
        }
        for (auto& vehicle : Car::vehicles_list) {
            vehicle->simulateStep(dt, L, N_MAX);
        }
        for (auto& vehicle : Truck::vehicles_list) {
            vehicle->simulateStep(dt, L, N_MAX);
        }
    }

    // Ghi thông tin vị trí vào file
    writePositionToFile("mophong.txt");

    // Giải phóng bộ nhớ
    for (auto& vehicle : Vehicle::vehicles_list) {
        delete vehicle;
    }

    return 0;
}
