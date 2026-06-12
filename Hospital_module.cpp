#include "Hospital_module.h"
#include <iostream>
using namespace std;

/// Staff
int Staff::getId() const
{
    return id;
}
const string& Staff::getName() const
{
    return name;
}
int Staff::getAge() const
{
    return age;
}
void Staff::setName(const string& n)
{
    name = n;
}
void Staff::setAge(int a)
{
    age = a;
}

/// Doctor
void Doctor::viewPatientHistory(int patientId) const
{
    Patient* patient = nullptr;
    cout<<"\n=== Patient History ===\n";
    cout<<"Doctor: "<<name<<" (ID: "<<id<<")\n";
    if (!patient) {
        cerr<<"Error: Patient #"<<patientId<<" not found.\n";
        return;
    }
    cout<<"\n[Patient Details]\n";
    cout<<"ID: "<<patient->getId()<<"\n";
    cout<<"Name: "<<patient->getName()<<"\n";
    cout<<"Age/Gender: "<<patient->getAge()<<"yo/"<<patient->getGender()<<"\n";
    cout<<"\n[Treatment History]\n";
    if (patient->getTreatments().empty()) {
        cout<<"No treatments recorded.\n";
    } else {
        for (const auto* treatment : patient->getTreatments()) {
            cout<<"- "<<treatment->getDescription()<<" (ID: "<<treatment->getId()<<", Cost: $"<<treatment->getCost()<<", Duration: "<<treatment->getDuration()<<" mins)\n";
        }
    }
    cout<<"\n[Your Appointments with Patient]\n";
    bool foundAppointments = false;
    for (const auto* appt : patient->getAppointments()) {
        if (appt->getDoctor() == this) {
            cout<<"- Appointment #"<<appt->getId()<<" on "<<appt->getDate()<<" ("<<appt->getStatus()<<")\n";
            foundAppointments = true;
        }
    }
    if (!foundAppointments) {
        cout<<"No past appointments with this patient.\n";
    }
    cout<<"\n[Financial Summary]\n";
    float totalSpent = 0.0f;
    for (const auto* treatment : patient->getTreatments()) {
        totalSpent += treatment->getCost();
    }
    cout<<"Total spent on treatments: $"<<totalSpent<<"\n";
    cout<<"======================\n";
}
string Doctor::getInfo() const
{
    return "Doctor{id=" + to_string(id) + ", name=" + name + ", age=" + to_string(age) + ", specialization=" + specialization + ", slots=" + to_string(availableSlots) + "}";
}
const string& Doctor::getSpecialization() const
{
    return specialization;
}
void Doctor::setSpecialization(const string& s)
{
    specialization = s;
}
int Doctor::getAvailableSlots() const
{
    return availableSlots;
}
void Doctor::setAvailableSlots(int s)
{
    availableSlots = s;
}
ostream& operator<<(ostream& os,const Patient& p)
{
    os<<"Patient{id="<<p.getId()<<", name="<<p.getName()<<", gender="<<p.getGender()<<", age="<<p.getAge()<<"}";
    return os;
}

ostream& operator<<(ostream& os,const Doctor& d)
{
    os<<"Doctor{id="<<d.getId()<<", name="<<d.getName()<<", age="<<d.getAge()<<", specialization="<<d.getSpecialization()<<", slots="<<d.getAvailableSlots()<<"}";
    return os;
}
Prescription* Doctor::createPrescription(int presId, Patient* p)
{
    if(!p)
        return nullptr;
    auto* pres = new Prescription(presId, p->getName());
    return pres;
}

void Appointment::setPrescription(Prescription* pres)
{
    prescription = pres;
}

Prescription* Appointment::getPrescription() const
{
    return prescription;
}
/// Nurse
string Nurse::getInfo() const
{
    return "Nurse{id=" + to_string(id) + ", name=" + name + ", age=" + to_string(age) + ", shift=" + shift + "}";
}
const string& Nurse::getShift() const
{
    return shift;
}
void Nurse::setShift(const string& s)
{
    shift = s;
}

/// Receptionist
void Receptionist::scheduleAppointment(Appointment& appt)
{
    try
    {
        if (!appt.getDoctor())
            throw invalid_argument("Doctor not assigned");
        if (!appt.getPatient())
            throw invalid_argument("Patient not registered");
        if (appt.getStatus() != "Pending")
            throw logic_error("Cannot schedule - current status: " + appt.getStatus());
        if (appt.getDoctor()->getAvailableSlots() <= 0)
            throw runtime_error("Dr. " + appt.getDoctor()->getName() + " has no available slots");
        appt.setStatus("Scheduled");
        appt.getDoctor()->setAvailableSlots(appt.getDoctor()->getAvailableSlots() - 1);
        appt.getPatient()->addAppointment(&appt);
        cout<<"[Reception] Successfully scheduled: - Patient: "<<appt.getPatient()->getName()<<" (ID: "<<appt.getPatient()->getId()<<") - Doctor: "<<appt.getDoctor()->getName()<<" - Date/Time: "<<appt.getDate()<<" - Appointment ID: "<<appt.getId()<<"\n";
    }
    catch (const invalid_argument& e)
    {
        appt.setStatus("Invalid - " + string(e.what()));
        cerr<<"[ERROR] Invalid appointment: "<<e.what()<<endl;
    }
    catch (const logic_error& e)
    {
        appt.setStatus("Conflict - " + string(e.what()));
        cerr<<"[WARNING] Scheduling conflict: "<<e.what()<<endl;
    }
    catch (const exception& e)
    {
        appt.setStatus("Failed - System Error");
        cerr<<"[CRITICAL] Unexpected error: "<<e.what()<<endl;
    }
}
void Receptionist::cancelAppointment(Appointment& appt)
{
    if (appt.getStatus() != "Scheduled")
    {
        cerr<<"[Reception] Failed: Cannot cancel non-scheduled appointment (Status: "<<appt.getStatus()<<")\n";
        return;
    }
    if (appt.getDoctor())
    {
        appt.getDoctor()->setAvailableSlots(appt.getDoctor()->getAvailableSlots() + 1);
    }
    appt.setStatus("Cancelled");
    cout<<"[Reception] Cancelled Appointment #"<<appt.getId()<<": - Patient: "<<appt.getPatient()->getName()<<" - Doctor: "<<appt.getDoctor()->getName()<<" - Original Date: "<<appt.getDate()<<"\n";
}
string Receptionist::getInfo() const
{
    return "Receptionist{id=" + to_string(id) + ", name=" + name + ", age=" + to_string(age) + "}";
}

/// Patient
void Patient::getAllAppointments() const
{
    cout<<"\n=== All Appointments for "<<name<<" (ID: "<<id<<") ===\n";
    if (appointments.empty())
    {
        cout<<"No appointments found.\n";
        cout<<"===================================\n";
        return;
    }
    for (const auto& appt : appointments)
    {
        cout<<"- ID: "<<appt->getId()<<"\n";
        cout<<"  Date: "<<appt->getDate()<<"\n";
        cout<<"  Status: "<<appt->getStatus()<<"\n";
        if (appt->getDoctor()) {
            cout<<"  Doctor: "<<appt->getDoctor()->getName()<<" ("<<appt->getDoctor()->getSpecialization()<<")\n";
        } else {
            cout<<"  Doctor: [Not Assigned]\n";
        }
        cout<<"\n";
    }
    cout<<"===================================\n";
}
int Patient::getId() const
{
    return id;
}
const string& Patient::getName() const
{
    return name;
}
const string& Patient::getGender() const
{
    return gender;
}
int Patient::getAge() const
{
    return age;
}
void Patient::setName(const string& n)
{
    name = n;
}
void Patient::setGender(const string& g)
{
    gender = g;
}
void Patient::setAge(int a)
{
    age = a;
}
void Patient::addTreatment(Treatment* t)
{
    treatments.push_back(t);
}
const vector<Treatment*>& Patient::getTreatments() const
{
    return treatments;
}
void Patient::addAppointment(Appointment* a)
{
    appointments.push_back(a);
}
const vector<Appointment*>& Patient::getAppointments() const
{
    return appointments;
}
/// Appointment
void Appointment::schedule()
{
    if (!Hospital::getInstance()->getReceptionist()) {
        cerr<<"Error: No receptionist available\n";
        status = "Failed - No Receptionist";
        return;
    }
    Hospital::getInstance()->getReceptionist()->scheduleAppointment(*this);
}
void Appointment::cancel()
{
    if (!Hospital::getInstance()->getReceptionist()) {
        cerr<<"Error: No receptionist available\n";
        return;
    }
    Hospital::getInstance()->getReceptionist()->cancelAppointment(*this);
}
int Appointment::getId() const
{
    return id;
}
const string& Appointment::getDate() const
{
    return date;
}
const string& Appointment::getStatus() const
{
    return status;
}
Patient* Appointment::getPatient() const
{
    return patient;
}
Doctor* Appointment::getDoctor() const
{
    return doctor;
}
void Appointment::setDate(const string& d)
{
    date = d;
}
void Appointment::setStatus(const string& s)
{
    status = s;
}
void Appointment::setPatient(Patient* p)
{
    patient = p;
}
void Appointment::setDoctor(Doctor* d)
{
    doctor = d;
}

/// Treatment
int Treatment::getId() const
{
    return treatmentId;
}
const string& Treatment::getDescription() const
{
    return description;
}
void Treatment::setDescription(const string& d)
{
    description = d;
}
float Treatment::getCost() const
{
    return baseCost;
}
int Treatment::getDuration() const
{
    return duration;
}
float verifyTreatmentCost(const Treatment& t,float maxAllowed)
{
    if (t.baseCost < 0)
    {
        cerr<<"Invalid treatment cost!\n";
        return -1.0f;
    }
    if (t.baseCost > maxAllowed)
    {
        cout<<"Warning: Treatment #"<<t.treatmentId<<" exceeds Insurance limit ($"<<maxAllowed<<")\n";
        return maxAllowed;
    }
    return t.baseCost;
}

/// Surgery
void Surgery::applyTreatment(Patient* p)
{
    if (!p)
    {
        cerr<<"Error: No patient specified for surgery!\n";
        return;
    }
    cout<<"Operating Room #"<<(treatmentId % 10 + 1)<<" assigned\n";
    cout<<"\n=== Performing Surgery ===\n";
    cout<<"Patient: "<<p->getName()<<" (ID: "<<p->getId()<<")\n";
    cout<<"Procedure: "<<description<<"\n";
    cout<<"Duration: "<<duration<<" minutes\n";
    cout<<"Surgery ID: "<<treatmentId<<"\n";
    p->addTreatment(this);
    cout<<"\nSurgery successfully completed.\n";
    cout<<"==============================\n";
}
string Surgery::generateReport() const
{
    return "Surgery Report: id=" + to_string(treatmentId) + ", desc=" + description;
}

/// Therapy
void Therapy::applyTreatment(Patient* p)
{
    if (!p)
    {
        cerr<<"Error: No patient specified for therapy!\n";
        return;
    }
    cout<<"\n=== Administering Therapy ===\n";
    cout<<"Patient: "<<p->getName()<<" (ID: "<<p->getId()<<")\n";
    cout<<"Therapy Type: "<<description<<"\n";
    cout<<"Duration: "<<duration<<" minutes\n";
    cout<<"Therapy ID: "<<treatmentId<<"\n";
    p->addTreatment(this);
    cout<<"\nTherapy session completed.\n";
    cout<<"===========================\n";
}
string Therapy::generateReport() const
{
    return "Therapy Report: id=" + to_string(treatmentId) + ", desc=" + description;
}

/// Department
void Department::addDoctor(Doctor* d)
{
    doctors.push_back(d);
}
const string& Department::getName() const
{
    return name;
}
const vector<Doctor*>& Department::getDoctors() const
{
    return doctors;
}

/// Hospital
Hospital* Hospital::instance = nullptr;
void Hospital::addDepartment(Department* d)
{
    departments.push_back(d);
}
void Hospital::setReceptionist(Receptionist* r)
{
    receptionist = r;
}
const string& Hospital::getName() const
{
    return name;
}
const vector<Department*>& Hospital::getDepartments() const
{
    return departments;
}
Receptionist* Hospital::getReceptionist() const
{
    return receptionist;
}

/// Billing
void Billing::addTreatment(Treatment* t)
{
    treatments.push_back(t);
}
int Billing::getBillId() const
{
    return billId;
}
Patient* Billing::getPatient() const
{
    return patient;
}
float Billing::getTotalAmount() const
{
    return totalAmount;
}
float Billing::calculateBill()
{
    totalAmount = 0.0f;
    if (!patient)
    {
        cerr<<"[Billing] Error: No patient associated with bill #"<<billId<<endl;
        return -1.0f;
    }
    if (treatments.empty())
    {
        cerr<<"[Billing] Warning: No treatments for patient "<<patient->getName()<<" (Bill #"<<billId<<")\n";
        return 0.0f;
    }
    for (const auto* treatment : treatments)
    {
        if (!treatment)
        {
            cerr<<"[Billing] Warning: Skipped invalid treatment in bill #"<<billId<<endl;
            continue;
        }
        totalAmount += treatment->getCost();
    }
    const float HOSPITAL_FEE = 50.0f;
    totalAmount += HOSPITAL_FEE;
    cout<<"[Billing] Calculated total: $"<<totalAmount<<" for patient "<<patient->getName()<<" ("<<treatments.size()<<" treatments)\n";
    return totalAmount;
}
void Billing::exportToJSON(const string& filePath) const
{
    json j;

    j["billId"] = billId;
    if (patient)
    {
        j["patient"]["id"] = patient->getId();
        j["patient"]["name"] = patient->getName();
    }
    else
    {
        j["patient"] = nullptr;
    }
    j["totalAmount"] = totalAmount;

    for(const auto* t : treatments)
    {
        if(t)
        {
            j["treatments"].push_back({
                {"id", t->getId()},
                {"description", t->getDescription()},
                {"cost", t->getCost()},
                {"duration", t->getDuration()}
            });
        }
        else
        {
            j["treatments"].push_back(nullptr);
        }
    }
    ofstream out(filePath);
    if (!out.is_open())
    {
        cerr << "[Billing] Error: Cannot open file " << filePath << " for writing\n";
        return;
    }
    out << j.dump(4);
    out.close();

    cout << "[Billing] exportToJSON -> " << filePath << "\n";
}

