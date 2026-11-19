// CSC241 OOP - Hospital Management System - Level 2
// Team: Ali Zia Syed 042, Aaryan Hassan 014,
// Affan Tariq 031, Abdullah Shamim 025 (FA25-BEE)
#ifndef HMS_HPP
#define HMS_HPP
#include <ostream>
#include <string>
#include <vector>

class Person {
private:
    int id;
    std::string name;
public:
    Person(int id, const std::string& name);
    virtual ~Person() = default;
    int getId() const;
    const std::string& getName() const;
    void setName(const std::string& value);
    virtual void display(std::ostream& out) const = 0;
};

class MedicalRecord {
private:
    int recordId;
    int doctorId;
    std::string diagnosis;
    std::string prescription;
public:
    MedicalRecord(int id, int doctorId, const std::string& diagnosis,
                  const std::string& prescription);
    int getId() const;
    int getDoctorId() const;
    void display(std::ostream& out) const;
};

class Patient : public Person {
private:
    int age;
    std::vector<MedicalRecord> records;
public:
    Patient(int id, const std::string& name, int age);
    void setAge(int value);
    void addRecord(const MedicalRecord& record);
    void display(std::ostream& out) const override;
    void displayRecords(std::ostream& out) const;
};

class Doctor : public Person {
private:
    std::string specialization;
    bool available;
public:
    Doctor(int id, const std::string& name,
           const std::string& specialization);
    bool isAvailable() const;
    void setAvailable(bool value);
    void display(std::ostream& out) const override;
};

class Appointment {
private:
    int appointmentId;
    int patientId;
    int doctorId;
    std::string slot;
    bool cancelled;
public:
    Appointment(int id, int patientId, int doctorId,
                const std::string& slot);
    int getId() const;
    int getPatientId() const;
    int getDoctorId() const;
    const std::string& getSlot() const;
    bool isCancelled() const;
    void cancel();
    void display(std::ostream& out) const;
};

class Bill {
private:
    int billId;
    int patientId;
    double consultationFee;
    double serviceCharges;
    bool paid;
public:
    Bill(int id, int patientId, double fee, double charges);
    int getId() const;
    int getPatientId() const;
    double calculateTotal() const;
    void markPaid();
    void display(std::ostream& out) const;
};

class HospitalSystem {
private:
    std::vector<Patient> patients;
    std::vector<Doctor> doctors;
    std::vector<Appointment> appointments;
    std::vector<Bill> bills;
    Patient& findPatient(int id);
public:
    void addPatient(const Patient& patient);
    void addDoctor(const Doctor& doctor);
    const Patient& getPatient(int id) const;
    const Doctor& getDoctor(int id) const;
    void addMedicalRecord(int patientId, const MedicalRecord& record);
    void bookAppointment(const Appointment& appointment);
    void cancelAppointment(int id);
    void addBill(const Bill& bill);
    void payBill(int id);
    void displayAll(std::ostream& out) const;
    void saveSummary(const std::string& path) const;
};
#endif
