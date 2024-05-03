#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib> // Để sử dụng rand() và srand()
#include <ctime>   // Để sử dụng hàm time()
#include <cmath>   // Để sử dụng hàm round()
#include <iomanip> // Để sử dụng hàm setprecision()

using namespace std;

const float RANDOM_RANGE = 1.0; // Phạm vi của số ngẫu nhiên

class Vehicle {
protected:
    float position; // Vị trí của phương tiện trên đường
    float velocity; // Vận tốc của phương tiện

public:
    Vehicle(float pos, float vel) : position(pos), velocity(vel) {}

    // Cập nhật vị trí của phương tiện sau một khoảng thời gian dt
    virtual void updatePosition(float dt) {
        position += velocity * dt;
    }

    // Điều chỉnh vận tốc của phương tiện
    void adjustVelocity() {
        const float maxChange = 0.1; // Phạm vi thay đổi tối đa cho vận tốc
        // Tăng hoặc giảm vận tốc một cách ngẫu nhiên
        velocity += (static_cast<float>(rand()) / RAND_MAX) * maxChange * 2 - maxChange;

        // Làm tròn vận tốc đến 2 chữ số thập phân
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

// Hàm mô phỏng lưu lượng giao thông
void simulateTraffic(float L, float dt, int n, int N_MAX1, int N_MAX2, int N_MAX3, vector<Vehicle*>& vehicles) {
    ofstream outFile("mophong.txt");
    if (!outFile.is_open()) {
        cerr << "Không thể mở tệp lưu!" << endl;
        return;
    }

    int totalVehicleCount = N_MAX1 + N_MAX2 + N_MAX3;
    int vehicleCounts[3] = { N_MAX1, N_MAX2, N_MAX3 };

    for (int i = 0; i < n; ++i) {
        for (auto it = vehicles.begin(); it != vehicles.end();) {
            (*it)->updatePosition(dt); // Cập nhật vị trí của phương tiện
            (*it)->adjustVelocity();   // Điều chỉnh vận tốc của phương tiện

            // Kiểm tra xem phương tiện có đi ra khỏi đường không
            if ((*it)->getPosition() > L) {
                delete* it;          // Xóa phương tiện khỏi bộ nhớ
                it = vehicles.erase(it);
            }
            else {
                ++it;
            }
        }

        // Tạo phương tiện mới nếu cần
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < vehicleCounts[j]; ++k) {
                if (vehicles.size() < totalVehicleCount) {
                    switch (j) {
                    case 0:
                        vehicles.push_back(new Motorcycle(0, 1.0)); // Xe máy
                        break;
                    case 1:
                        vehicles.push_back(new Car(0, 1.0)); // Ô tô
                        break;
                    case 2:
                        vehicles.push_back(new Truck(0, 1.0)); // Xe tải
                        break;
                    }
                }
            }
        }

        // Ghi vị trí của các phương tiện vào tệp
        for (const auto& vehicle : vehicles) {
            outFile << fixed << setprecision(2) << vehicle->getPosition() << " ";
        }
        outFile << endl;
    }

    outFile.close();
}

int main() {
    srand(time(nullptr)); // Khởi tạo bộ sinh số ngẫu nhiên

    float L, dt;
    int n, N_MAX1, N_MAX2, N_MAX3;

    cout << "Nhập độ dài đường (L): ";
    cin >> L;
    cout << "Nhập bước thời gian (dt): ";
    cin >> dt;
    cout << "Nhập số bước thời gian (n): ";
    cin >> n;
    cout << "Nhập số lượng tối đa của xe máy (N_MAX1): ";
    cin >> N_MAX1;
    cout << "Nhập số lượng tối đa của ô tô (N_MAX2): ";
    cin >> N_MAX2;
    cout << "Nhập số lượng tối đa của xe tải (N_MAX3): ";
    cin >> N_MAX3;

    vector<Vehicle*> vehicles;

    simulateTraffic(L, dt, n, N_MAX1, N_MAX2, N_MAX3, vehicles);

    // Giải phóng bộ nhớ
    for (auto& vehicle : vehicles) {
        delete vehicle;
    }

    return 0;
}
