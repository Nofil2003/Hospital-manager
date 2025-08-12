#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "Hospital_module.h"
#include "pharmacy.h"
using namespace std;


Patient* findPatientById(const vector<Patient*>& v,int id)
{
    for(auto* p : v)
    {
        if(p && p->getId()==id)
            return p;
    }
    return nullptr;
}

Doctor* findDoctorById(const vector<Doctor*>& v,int id)
{
    for(auto* d : v)
    {
        if(d && d->getId()==id)
            return d;
    }
        return nullptr;
}

Appointment* findAppointmentById(const vector<Appointment*>& v,int id)
{
    for(auto* a : v)
    {
        if(a && a->getId()==id)
             return a;
    }
    return nullptr;
}


void seedPharmacyInventory(Pharmacy& ph)
{
    ph.getInventory().addMedicine(new Tablet("Paracetamol 500","ABC Pharma","2026-12-31",1,1.5f,50,10,500));
    ph.getInventory().addMedicine(new Tablet("Azithromycin 250","MediLabs","2026-05-31",2,2.2f,30,10,250));
    ph.getInventory().addMedicine(new Syrup("Cough Syrup","HealCo","2026-03-31",3,3.0f,20,5,100));
    ph.getInventory().addMedicine(new Injection("Ceftriaxone 1g","Injecta","2027-01-31",4,5.5f,15,5,1));
}

Billing* findBillingByPatientId(const vector<Billing*>& v,int pid)
{
    for (auto* b : v) if (b&&b->getPatient()&&b->getPatient()->getId()==pid) return b;
    return nullptr;
}
Sale* findSaleById(vector<Sale>& v,int saleId)
{
    for (auto& s : v) if (s.getId()==saleId) return &s;
    return nullptr;
}
void listPatients(const vector<Patient*>& patients)
{
    if (patients.empty())
    {
        cout<<"No patients.\n";
        return;
    }
    cout<<"-- Patients --\n";
    for (auto* p : patients)
    {
        if (!p) continue;
        cout<<"ID="<<p->getId()<<", Name="<<p->getName()<<", Gender="<<p->getGender()<<", Age="<<p->getAge()<<"\n";
    }
}

void listDoctors(const vector<Doctor*>& doctors)
{
    if (doctors.empty())
    {
        cout<<"No doctors.\n";
        return;
    }
    cout<<"-- Doctors --\n";
    for (auto* d : doctors)
    {
        if (!d) continue;
        cout<<"ID="<<d->getId()<<", Name="<<d->getName()<<", Age="<<d->getAge()<<", Spec="<<d->getSpecialization()<<", Slots="<<d->getAvailableSlots()<<"\n";
    }
}

void listAppointments(const vector<Appointment*>& appts)
{
    if (appts.empty())
    {
        cout<<"No appointments.\n";
        return;
    }
    cout<<"-- Appointments --\n";
    for (auto* a : appts)
    {
        if (!a) continue;
        cout<<"ID="<<a->getId()<<", Date="<<a->getDate()<<", Status="<<a->getStatus();
        if (a->getPatient()) cout<<", Patient="<<a->getPatient()->getName();
        if (a->getDoctor()) cout<<", Doctor="<<a->getDoctor()->getName();
        if (a->getPrescription()) cout<<", PrescriptionID="<<a->getPrescription()->getId();
        cout<<"\n";
    }
}

void listInventory(Pharmacy& ph)
{
    auto& meds = ph.getInventory().getMedicines();
    if (meds.empty())
    {
        cout<<"Inventory empty.\n";
        return;
    }
    cout<<"-- Inventory --\n";
    for (auto* m : meds)
    {
        if (!m) continue;
        cout<<m->getDetails()<<"\n";
    }
}

void addTreatmentSimple(Patient* p,Billing* bill,int type,int tid,const string& desc,float cost,int dur)
{
    Treatment* t = (type==1)
        ? static_cast<Treatment*>(new Surgery(tid,desc,cost,dur))
        : static_cast<Treatment*>(new Therapy(tid,desc,cost,dur));

    bill->addTreatment(t);
    t->applyTreatment(p);
}
int main()
{
    vector<Patient*> patients;
    vector<Doctor*> doctors;
    vector<Appointment*> appts;

    vector<Billing*> treatmentBills;
    vector<Sale> pharmacySales;

    Hospital* hosp = Hospital::getInstance("CityGeneral");
    Receptionist rec(1,"Alice",30);
    hosp->setReceptionist(&rec);
    Pharmacy pharmacy;
    seedPharmacyInventory(pharmacy);
    Supplier mainSupplier("CentralSupplier"); 

    bool running = true;
    while (running)
    {
        cout<<"\n===== HOSPITAL SYSTEM MENU =====\n";
        cout<<"1) Add Patient\n";
        cout<<"2) Add Doctor\n";
        cout<<"3) List Patients\n";
        cout<<"4) List Doctors\n";
        cout<<"5) Schedule Appointment\n";
        cout<<"6) List Appointments\n";
        cout<<"7) Create Prescription for Appointment\n";
        cout<<"8) View Prescription for Appointment\n";
        cout<<"9) Dispense Prescription at Pharmacy\n";
        cout<<"10) View Pharmacy Inventory\n";
        cout<<"11) Restock Low Items\n";
        cout << "12) Add Treatment to Patient\n";
        cout << "13) Generate Combined Bill (Pharmacy + Treatments)\n";
        cout << "14) Export Treatment Bill to JSON (by PatientID)\n";
        cout << "15) Export Pharmacy Sale to JSON (by SaleID)\n";
        cout<<"0) Exit\n";
        cout<<"Select: ";

        int choice; cin>>choice;

        switch (choice)
        {
        case 1:
        {
            int id,billId,age;
            string name,gender;
            cout<<"PatientID: ";
            cin>>id;
            cout<<"Name(no-spaces): ";
            cin>>name;
            cout<<"Gender(no-spaces): ";
            cin>>gender;
            cout<<"Age: ";
            cin>>age;
            cout<<"BillID(for this patient): ";
            cin>>billId;

            auto* p = new Patient(id,name,gender,age);
            patients.push_back(p);
            auto* b = new Billing(billId,p);
            treatmentBills.push_back(b);

            cout<<"Added patient.\n";
            break;
        }
        case 2:
        {
            int id,age,slots;
            string name,spec;
            cout<<"DoctorID: ";
            cin>>id;
            cout<<"Name(no-spaces): ";
            cin>>name;
            cout<<"Age: ";
            cin>>age;
            cout<<"Specialization(no-spaces): ";
            cin>>spec;
            cout<<"AvailableSlots: ";
            cin>>slots;
            auto* d = new Doctor(id,name,age,spec,slots);
            doctors.push_back(d);
            cout<<"Added doctor.\n";
            break;
        }
        case 3:
        {
            listPatients(patients);
            break;
        }
        case 4:
        {
            listDoctors(doctors);
            break;
        }
        case 5:
        {
            if (patients.empty()||doctors.empty())
            {
                cout<<"Need at least one patient and one doctor.\n";
                break;
            }
            int apptId,pid,did;
            string date;
            cout<<"AppointmentID: ";
            cin>>apptId;
            cout<<"PatientID: ";
            cin>>pid;
            cout<<"DoctorID: ";
            cin>>did;
            cout<<"DateTime(no-spaces): ";
            cin>>date;

            Patient* p = findPatientById(patients,pid);
            Doctor* d = findDoctorById(doctors,did);
            if(!p||!d)
            {
                cout<<"Invalid patient/doctor id.\n";
                break;
            }
            auto* a = new Appointment(apptId,date,p,d,"Pending");
            a->schedule(); // uses Receptionist + slots
            appts.push_back(a);
            cout<<"Appointment created. Status="<<a->getStatus()<<"\n";
            break;
        }
        case 6:
        {
            listAppointments(appts);
            break;
        }
        case 7:
        {
            if (appts.empty())
            {
                cout<<"No appointments.\n";
                break;
            }
            int aid;
            cout<<"AppointmentID: ";
            cin>>aid;
            Appointment* a = findAppointmentById(appts,aid);
            if (!a||!a->getPatient()||!a->getDoctor())
            {
                cout<<"Invalid appointment.\n";
                break;
            }
            int presId;
            cout<<"PrescriptionID: ";
            cin>>presId;
            Prescription* pres = a->getDoctor()->createPrescription(presId,a->getPatient());
            if (!pres)
            {
                cout<<"Failed to create prescription.\n";
                break;
            }
            cout<<"Enter medicine id to add ( type 0 to finish):\n";
            while (true)
            {
                int medid;
                cout<<"Medicine: ";
                cin>>medid;

                if(medid == 0)
                    break;
                Medicine* m = pharmacy.getInventory().findMedicineByid(medid);
                if(!m)
                    cout<<"Not found.\n";
                else
                {
                    pres->addMedicine(m);
                    cout<<"Added "<<m->getName()<<"\n";
                }
            }
            a->setPrescription(pres);
            cout<<"Prescription attached.\n";
            break;
        }
        case 8:
        {
            int aid;
            cout<<"AppointmentID: ";
            cin>>aid;
            Appointment* a = findAppointmentById(appts,aid);
            if (!a||!a->getPrescription())
            {
                cout<<"No prescription for this appointment.\n";
                break;
            }
            a->getPrescription()->printPrescription();
            break;
        }
        case 9:
        {
            int aid;
            cout<<"AppointmentID: ";
            cin>>aid;
            Appointment* a = findAppointmentById(appts,aid);
            if (!a||!a->getPrescription())
            {
                cout<<"No prescription to dispense.\n";
                break;
            }

            Sale sale = pharmacy.dispensePrescription(*a->getPrescription(),nullptr);
            float total = sale.generateBill();
            cout<<"Dispensed. Total=$"<<total<<"\n";
            pharmacySales.push_back(sale);

            break;
        }

        case 10:
        {
            listInventory(pharmacy);
            break;
        }
        case 11:
        {
            cout<<"Low stock before restock:\n";
            pharmacy.getInventory().checkLowStock();
            cout<<"Auto-restocking low items...\n";
            pharmacy.getInventory().autoRestock(mainSupplier);
            cout<<"Low stock after restock:\n";
            pharmacy.getInventory().checkLowStock();
            break;
        }
        case 13:
        {
            int pid,saleId;
            cout<<"PatientID: ";
            cin>>pid;
            cout<<"SaleID: "; 
            cin>>saleId;
            Billing* b = findBillingByPatientId(treatmentBills,pid);
            Sale* s = findSaleById(pharmacySales,saleId);
            if (!b||!s)
            {
                cout<<"Bill or Sale not found.\n";
                break;
            }
            cout<<"Combined total=$"<<(b->calculateBill()+s->generateBill())<<"\n";
            break;
        }
        case 14:
        {
            int pid;
            cout<<"PatientID: ";
            cin>>pid;
            Billing* b = findBillingByPatientId(treatmentBills,pid);
            if(!b)
            {
                cout<<"No billing for this patient.\n";
                break;
            }
            string file;
            cout<<"FileName(no-spaces, e.g., bill_treatment.json): ";
            cin>>file;
            b->exportToJSON(file);
            cout<<"Treatment bill exported to "<<file<<"\n";
            break;
        }
        case 15:
        {
            if (pharmacySales.empty())
            {
                cout<<"No pharmacy sales.\n";
                break;
            }
            int saleId;
            cout<<"SaleID: ";
            cin>>saleId;
            Sale* s = findSaleById(pharmacySales,saleId);
            if (!s)
            {
                cout<<"Sale not found.\n";
                break;
            }
            string file; cout<<"FileName(no-spaces, e.g., bill_pharmacy.json): ";
            cin>>file;
            s->exportToJSON(file);
            cout<<"Pharmacy sale exported to "<<file<<"\n";
            break;
        }
        case 12:
        {
            int pid; cout<<"PatientID: "; cin>>pid;
            Patient* p = findPatientById(patients,pid);
            Billing* bill = findBillingByPatientId(treatmentBills,pid);
            if (!p||!bill) { cout<<"Invalid patient or no bill.\n"; break; }

            int type; cout<<"Type(1=Surgery,2=Therapy): "; cin>>type;
            int tid; cout<<"TreatmentID: "; cin>>tid;
            string desc; cout<<"Desc(no-spaces): "; cin>>desc;
            float cost; cout<<"Cost: "; cin>>cost;
            int dur; cout<<"Duration(min): "; cin>>dur;

            addTreatmentSimple(p,bill,type,tid,desc,cost,dur);
            cout<<"Treatment added to bill and applied to patient.\n";
            break;
        }

        case 0:
        {
            running = false;
            break;
        }
        default:
            cout<<"Invalid choice.\n";
            break;
        }
    }

    for(auto* a : appts)
        delete a;
    for(auto* d : doctors)
        delete d;
    for(auto* p : patients)
        delete p;

    cout<<"Goodbye!\n";
    return 0;
}