#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;
class Employee{
    public:
        virtual double earnings() const =0;
        virtual void print() const=0;
        virtual ~Employee() {} // Hàm hủy ảo
};
class Salaried :public Employee{
    private:
     double weeklySalary;
    public:
    Salaried(double salary): weeklySalary(salary){}//Ham khoi tao muc luong hang tuan
    double earnings() const override{ //Override phuong thuc ao earnings de tinh toan va tra ve muc luong hang tuan
        return weeklySalary;
    }
    void print()const override{
        cout<<"Salaried employee - WeeklySalary:"<<weeklySalary<<endl;
    }
      
};
class Commission :public Employee{
    private:
    double grossSales;//Doanh số bán hàng của nhân viên
    double commissionRate;//Tỉ lệ hoa hồng của nhân viên
    public:
    Commission(double sales, double rate):grossSales(sales),commissionRate(rate){}//ham khoi tao doanh so va ti le hoa hong
    double earnings()const override{//override phuong thuc ao earnings de tinh toan va tra ve muc luong dua theo doanh so va ti le hoa hong
        return grossSales*(commissionRate/100.0);
    }
    void setGrossSale(double sales){
        grossSales=sales;
    }
    void setCommissonRate(double rate){
        commissionRate=rate;
    }
    double getGrossSales() const{
        return grossSales;
    }
    double getCommissionRate() const{
        return commissionRate;
    }
    void print() const override{
        cout<<"Commissioned employee - Gross Sales:"<<grossSales<<",Commission Rate:"<<commissionRate<<"%"<<endl;
    }
};
class BasePlusCommission : public Employee {
private:
    double baseSalary;      // Lương cơ bản
    double grossSales;      // Doanh số bán hàng
    double commissionRate;  // Tỷ lệ hoa hồng

public:
    BasePlusCommission(double base, double sales, double rate)
        : baseSalary(base), grossSales(sales), commissionRate(rate) {}

    // Phương thức tính lương
    double earnings() const override {
        // Lương cơ bản cộng với phần trăm của doanh số bán hàng
        return baseSalary + grossSales * (commissionRate / 100.0);
    }

    // Thiết lập lương cơ bản
    void setBaseSalary(double salary) {
        baseSalary = salary;
    }

    // Thiết lập doanh số bán hàng
    void setGrossSales(double sales) {
        grossSales = sales;
    }

    // Thiết lập tỷ lệ hoa hồng
    void setCommissionRate(double rate) {
        commissionRate = rate;
    }

    // Truy xuất lương cơ bản
    double getBaseSalary() const {
        return baseSalary;
    }

    // Truy xuất doanh số bán hàng
    double getGrossSales() const {
        return grossSales;
    }

    // Truy xuất tỷ lệ hoa hồng
    double getCommissionRate() const {
        return commissionRate;
    }
    void print() const override {
        cout << "Base Plus Commissioned employee - Base Salary: " << baseSalary << ", Gross Sales: " << grossSales << ", Commission Rate: " << commissionRate << "%" << endl;
    }
};
// Hàm sinh tên ngẫu nhiên
string randomName() {
    string name;
    name += 'A' + rand() % 26;
    name += 'a' + rand() % 26;
    return name;
}

int main() {
    srand(time(0)); // Khởi tạo seed cho hàm random
    const int numEmployees = 10;
    Employee* employees[numEmployees];
// Tạo và lưu trữ các đối tượng nhân viên vào mảng con trỏ
for (int i = 0; i < numEmployees; ++i) {
    string firstName = randomName();
    string lastName = randomName();
    // Tạo một số ngẫu nhiên để quyết định loại nhân viên
    int employeeType = rand() % 3; // 0: Salaried, 1: Commission, 2: BasePlusCommission
    
    switch (employeeType) {
        case 0:
            employees[i] = new Salaried(rand() % 1000 + 500);
            break;
        case 1:
            employees[i] = new Commission(rand() % 10000 + 5000, rand() % 10 + 5);
            break;
        case 2:
            employees[i] = new BasePlusCommission(rand() % 500 + 1000, rand() % 10000 + 5000, rand() % 10 + 5);
            break;
        default:
            break;
    }
}

    // Sắp xếp mảng các nhân viên theo thu nhập giảm dần(Bubble Sort)
    for (int i = 0; i < numEmployees - 1; ++i) {
        for (int j = i + 1; j < numEmployees; ++j) {
            if (employees[i]->earnings() < employees[j]->earnings()) {
                swap(employees[i], employees[j]);
            }
        }
    }

    // In thông tin và thu nhập của các nhân viên đã sắp xếp
    for (int i = 0; i < numEmployees; ++i) {
        string firstName = randomName(); // Tạo tên ngẫu nhiên cho mỗi nhân viên
        string lastName = randomName();
        cout << "Employee " << i + 1 << ": " << firstName << " " << lastName << " - Earnings: " << employees[i]->earnings() <<" USD"<< endl;
        employees[i]->print();
        cout << endl;
}

    // Giải phóng bộ nhớ
    for (int i = 0; i < numEmployees; ++i) {
        delete employees[i];
    }

    return 0;
}
