#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"
using namespace std;
using nlohmann::json;

class Supplier;
class Sale;
class Prescription;
class Pharmacy;
class Medicine {
protected:
    string name;
    string brand;
    string expiryDate;
    int id;
    float price;
    int quantity;
    int lowStockThreshold;

public:
    Medicine(const string& name, const string& brand, const string& expiryDate,int id,
             float price, int quantity, int lowStockThreshold);

    virtual ~Medicine() {}

    virtual string getDetails() const = 0; // Abstract
    bool isLowStock() const;
    void reduceQuantity(int qty);
    void increaseQuantity(int qty);

    string getName() const { return name; }
    float getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getId() const { return id; }
};


class Tablet : public Medicine {
private:
    int dosage; // mg

public:
    Tablet(const string& name, const string& brand, const string& expiryDate,int id,
           float price, int quantity, int lowStockThreshold, int dosage);

    string getDetails() const override;
};



class Syrup : public Medicine {
private:
    int volume; // ml

public:
    Syrup(const string& name, const string& brand, const string& expiryDate,int id,
          float price, int quantity, int lowStockThreshold, int volume);

    string getDetails() const override;
};



class Injection : public Medicine {
private:
    int concentration; // mg/ml

public:
    Injection(const string& name, const string& brand, const string& expiryDate,int id,
              float price, int quantity, int lowStockThreshold, int concentration);

    string getDetails() const override;
};


class Discount {
public:
    virtual ~Discount() {}
    virtual float applyDiscount(float amount) const = 0; // Abstract
};


class BulkDiscount : public Discount {
private:
    float discountRate; // e.g., 0.1 for 10%
public:
    BulkDiscount(float rate);
    float applyDiscount(float amount) const override;
};



class SeniorCitizenDiscount : public Discount {
private:
    float discountRate; // e.g., 0.15 for 15%
public:
    SeniorCitizenDiscount(float rate);
    float applyDiscount(float amount) const override;
};



class Inventory {
private:
    vector<Medicine*> medicines;

public:
    ~Inventory();
    void addMedicine(Medicine* med);
    void checkLowStock() const;
    void autoRestock(Supplier& supplier);
    Medicine* findMedicineByName(const string& name);
    Medicine* findMedicineByid(int id);
    const vector<Medicine*>& getMedicines() const { return medicines; }
};


class Supplier {
private:
    string name;

public:
    Supplier(const string& name);
    void restockOnRequest(Medicine& med);
    string getName() const { return name; }
};

class Pharmacy {
private:
    Inventory inventory;
    vector<Supplier> suppliers;

public:
    Sale dispensePrescription(const Prescription& pres, Discount* discount = nullptr);
    void viewSalesReport() const;
    Inventory& getInventory() { return inventory; }
    void addSupplier(const Supplier& supplier);
    vector<Supplier>& getSuppliers() { return suppliers; }
};


class Prescription {
private:
    int id;
    string patientName;
    vector<Medicine*> prescribedMedicines;

public:
    Prescription(int id, const string& patientName);
    void addMedicine(Medicine* med);
    void printPrescription() const;
    int getId() const { return id; }
    const string& getPatientName() const { return patientName; }
    const vector<Medicine*>& getMedicines() const { return prescribedMedicines; }
};


class Sale {
private:
    int id;
    vector<Medicine*> medicines;
    Discount* discount;

public:
    int getId() const { return id; }
    Sale(int id, Discount* discount = nullptr);
    void addMedicine(Medicine* med);
    float generateBill() const;
    void exportToJSON(const string& filePath) const;
    void loadFromJSON(const string& filePath);
};





