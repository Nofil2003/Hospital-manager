#include "pharmacy.h"


Medicine::Medicine(const string& name, const string& brand, const string& expiryDate,int id,
                   float price, int quantity, int lowStockThreshold)
    : name(name), brand(brand), expiryDate(expiryDate), price(price),
      quantity(quantity), lowStockThreshold(lowStockThreshold) ,id(id){}

bool Medicine::isLowStock() const {
    return quantity <= lowStockThreshold;
}

void Medicine::reduceQuantity(int qty) {
    if (qty > quantity) throw runtime_error("Not enough stock to reduce.");
    quantity -= qty;
}

void Medicine::increaseQuantity(int qty) {
    if (qty < 0) throw invalid_argument("Quantity to increase cannot be negative.");
    quantity += qty;
}



Tablet::Tablet(const string& name, const string& brand, const string& expiryDate,int id,
               float price, int quantity, int lowStockThreshold, int dosage)
    : Medicine(name, brand, expiryDate,id, price, quantity, lowStockThreshold), dosage(dosage) {}

string Tablet::getDetails() const {
    stringstream ss;
    ss << "Tablet: " << name << " (" << brand << "), Dosage: " << dosage << "mg, Price: "
       << price << ", Qty: " << quantity << ", Expiry: " << expiryDate;
    return ss.str();
}


Syrup::Syrup(const string& name, const string& brand, const string& expiryDate,int id,
             float price, int quantity, int lowStockThreshold, int volume)
    : Medicine(name, brand, expiryDate,id, price, quantity, lowStockThreshold), volume(volume) {}

string Syrup::getDetails() const {
    stringstream ss;
    ss << "Syrup: " << name << " (" << brand << "), Volume: " << volume << "ml, Price: "
       << price << ", Qty: " << quantity << ", Expiry: " << expiryDate;
    return ss.str();
}



Injection::Injection(const string& name, const string& brand, const string& expiryDate,int id,
                     float price, int quantity, int lowStockThreshold, int concentration)
    : Medicine(name, brand, expiryDate,id, price, quantity, lowStockThreshold), concentration(concentration) {}

string Injection::getDetails() const {
    stringstream ss;
    ss << "Injection: " << name << " (" << brand << "), Concentration: " << concentration
       << "mg/ml, Price: " << price << ", Qty: " << quantity << ", Expiry: " << expiryDate;
    return ss.str();
}


BulkDiscount::BulkDiscount(float rate) : discountRate(rate) {}

float BulkDiscount::applyDiscount(float amount) const {
    return amount - (amount * discountRate);
}


SeniorCitizenDiscount::SeniorCitizenDiscount(float rate) : discountRate(rate) {}

float SeniorCitizenDiscount::applyDiscount(float amount) const {
    return amount - (amount * discountRate);
}


Inventory::~Inventory() {
    for (auto med : medicines) {
        delete med;
    }
}

void Inventory::addMedicine(Medicine* med) {
    medicines.push_back(med);
}

void Inventory::checkLowStock() const {
    for (auto med : medicines) {
        if (med->isLowStock()) {
            cout << "[Low Stock] " << med->getName() << " (" << med->getQuantity() << " left)" << endl;
        }
    }
}
Medicine* Inventory::findMedicineByid(int id)
{
    for (auto med : medicines)
    {
        if (med->getId() == id)
        {
            return med;
        }
    }
    return nullptr;
}

void Inventory::autoRestock(Supplier& supplier) {
    for (auto med : medicines) {
        if (med->isLowStock()) {
            supplier.restockOnRequest(*med);
        }
    }
}

Medicine* Inventory::findMedicineByName(const string& name) {
    for (auto med : medicines) {
        if (med->getName() == name) {
            return med;
        }
    }
    return nullptr;
}


Supplier::Supplier(const string& name) : name(name) {}

void Supplier::restockOnRequest(Medicine& med) {
    cout << "Restocking medicine: " << med.getName() << " by supplier " << name << endl;
    med.increaseQuantity(50); // Example: add 50 units
}
Sale Pharmacy::dispensePrescription(const Prescription& pres, Discount* discount)
{
    Sale sale(pres.getId(), discount);

    for (auto* presMed : pres.getMedicines())
    {
        if (!presMed) continue;

        Medicine* invMed = inventory.findMedicineByName(presMed->getName());
        if (!invMed) continue;

        if (invMed->getQuantity() > 0)
        {
            sale.addMedicine(invMed);
            invMed->reduceQuantity(1);
        }
    }

    return sale;
}


void Pharmacy::viewSalesReport() const {
    cout << "Pharmacy Sales Report (Placeholder)" << endl;
}

void Pharmacy::addSupplier(const Supplier& supplier) {
    suppliers.push_back(supplier);
}


Prescription::Prescription(int id, const string& patientName)
    : id(id), patientName(patientName) {}

void Prescription::addMedicine(Medicine* med) {
    prescribedMedicines.push_back(med);
}

void Prescription::printPrescription() const {
    cout << "Prescription ID: " << id << " for " << patientName << endl;
    for (auto med : prescribedMedicines) {
        cout << " - " << med->getDetails() << endl;
    }
}


Sale::Sale(int id, Discount* discount) : id(id), discount(discount) {}

void Sale::addMedicine(Medicine* med) {
    medicines.push_back(med);
}

float Sale::generateBill() const {
    float total = 0;
    for (auto med : medicines) {
        total += med->getPrice();
    }
    if (discount) {
        total = discount->applyDiscount(total);
    }
    return total;
}

void Sale::exportToJSON(const string& filePath) const
{
    json j;
    j["SaleID"] = id;
    j["Medicines"] = json::array();

    for (const auto& med : medicines)
    {
        json m;
        m["Name"] = med->getName();
        m["Price"] = med->getPrice();
        j["Medicines"].push_back(m);
    }

    j["DiscountApplied"] = (discount != nullptr);
    j["Total"] = generateBill();

    ofstream file(filePath);
    if (!file)
    {
        cerr << "[Sale] Error: Cannot open file for writing: " << filePath << endl;
        return;
    }
    file << j.dump(4);
}



