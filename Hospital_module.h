#pragma once
#include <string>
#include <vector>
#include "pharmacy.h"
using namespace std;

class Patient;
class Doctor;
class Receptionist;
class Treatment;
class Surgery;
class Therapy;
class Department;
class Hospital;
class Appointment;
class Billing;

/// Staff
class Staff
{
protected:
    int id{};
    string name;
    int age{};
public:
    Staff() = default;
    Staff(int id,const string& name,int age) : id(id),name(name),age(age) {}
    virtual ~Staff() = default;
    virtual string getInfo() const = 0;
    int getId() const;
    const string& getName() const;
    int getAge() const;
    void setName(const string& n);
    void setAge(int a);
};

/// Doctor
class Doctor : public Staff
{
private:
    string specialization;
    int availableSlots{0};
public:
    Doctor() = default;
    Doctor(int id,const string& name,int age,const string& specialization,int availableSlots=0) : Staff(id,name,age),specialization(specialization),availableSlots(availableSlots) {}
    void viewPatientHistory(int patientId) const;
    Prescription* createPrescription(int presId, Patient* p);
    string getInfo() const override;
    const string& getSpecialization() const;
    void setSpecialization(const string& s);
    int getAvailableSlots() const;
    void setAvailableSlots(int s);
};
ostream& operator<<(ostream& os,const Doctor& d);

/// Nurse
class Nurse : public Staff
{
private:
    string shift;
public:
    Nurse() = default;
    Nurse(int id,const string& name,int age,const string& shift) : Staff(id,name,age),shift(shift) {}
    string getInfo() const override;
    const string& getShift() const;
    void setShift(const string& s);
};

/// Receptionist
class Receptionist : public Staff
{
public:
    Receptionist() = default;
    Receptionist(int id,const string& name,int age) : Staff(id,name,age) {}
    void scheduleAppointment(Appointment& appt);
    void cancelAppointment(Appointment& appt);
    string getInfo() const override;
};

/// Patient
class Patient
{
private:
    int id{};
    string name;
    string gender;
    int age{};
    vector<Treatment*> treatments;
    vector<Appointment*> appointments;
public:
    Patient() = default;
    Patient(int id,const string& name,const string& gender,int age) : id(id),name(name),gender(gender),age(age) {}
    void addTreatment(Treatment* t);
    const vector<Treatment*>& getTreatments() const;
    void addAppointment(Appointment* a);
    const vector<Appointment*>& getAppointments() const;
    void getAllAppointments() const;
    int getId() const;
    const string& getName() const;
    const string& getGender() const;
    int getAge() const;
    void setName(const string& n);
    void setGender(const string& g);
    void setAge(int a);
};
ostream& operator<<(ostream& os,const Patient& p);

/// Appointment
class Appointment
{
private:
    int id{};
    string date;
    string status;
    Patient* patient{nullptr};
    Doctor* doctor{nullptr};
    Prescription* prescription{nullptr};
public:
    Appointment() = default;
    Appointment(int id,const string& date,Patient* patient,Doctor* doctor,const string& status="Scheduled") : id(id),date(date),status(status),patient(patient),doctor(doctor) {}
    void schedule();
    void cancel();
    void setPrescription(Prescription* pres);
    Prescription* getPrescription() const;
    int getId() const;
    const string& getDate() const;
    const string& getStatus() const;
    Patient* getPatient() const;
    Doctor* getDoctor() const;
    void setDate(const string& d);
    void setStatus(const string& s);
    void setPatient(Patient* p);
    void setDoctor(Doctor* d);
};

/// Treatment
class Treatment
{
protected:
    int treatmentId{};
    string description;
    float baseCost;
    int duration;
public:
    Treatment() = default;
    Treatment(int id,const string& desc,float cost,int dur) : treatmentId(id),description(desc),baseCost(cost),duration(dur) {}
    virtual ~Treatment() = default;
    friend float verifyTreatmentCost(const Treatment& t,float maxAllowed);
    virtual void applyTreatment(Patient* p) = 0;
    virtual string generateReport() const = 0;
    int getId() const;
    const string& getDescription() const;
    float getCost() const;
    int getDuration() const;
    void setDescription(const string& d);
};

class Surgery : public Treatment
{
public:
    Surgery() = default;
    Surgery(int id,const string& desc,float cost,int dur) : Treatment(id,desc,cost,dur) {}
    void applyTreatment(Patient* p) override;
    string generateReport() const override;
};

class Therapy : public Treatment
{
public:
    Therapy() = default;
    Therapy(int id,const string& desc,float cost,int dur) : Treatment(id,desc,cost,dur) {}
    void applyTreatment(Patient* p) override;
    string generateReport() const override;
};

/// Department
class Department
{
private:
    string name;
    vector<Doctor*> doctors;
public:
    Department() = default;
    explicit Department(const string& name) : name(name) {}
    void addDoctor(Doctor* d);
    const string& getName() const;
    const vector<Doctor*>& getDoctors() const;
};

/// Hospital
class Hospital
{
private:
    static Hospital* instance;
    string name;
    vector<Department*> departments;
    Receptionist* receptionist{nullptr};
    Hospital(const string& name) : name(name) {}
public:
    Hospital(const Hospital&) = delete;
    Hospital& operator=(const Hospital&) = delete;
    static Hospital* getInstance(const string& name="")
    {
        static Hospital* instance = new Hospital(name.empty() ? "General Hospital" : name);
        return instance;
    }
    void addDepartment(Department* d);
    void setReceptionist(Receptionist* r);
    const string& getName() const;
    const vector<Department*>& getDepartments() const;
    Receptionist* getReceptionist() const;
};

/// Billing
class Billing
{
private:
    int billId{};
    Patient* patient{nullptr};
    vector<Treatment*> treatments;
    float totalAmount{0.0f};
public:
    Billing() = default;
    Billing(int billId,Patient* patient) : billId(billId),patient(patient) {}
    float calculateBill();
    void exportToJSON(const string& filePath) const;
    void addTreatment(Treatment* t);
    int getBillId() const;
    Patient* getPatient() const;
    float getTotalAmount() const;
};
