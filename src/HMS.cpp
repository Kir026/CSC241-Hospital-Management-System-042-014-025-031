#include "HMS.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace {
void requireId(int id) {
    if (id <= 0) throw std::invalid_argument("ID must be positive.");
}
void requireText(const std::string& text) {
    if (text.find_first_not_of(" \t\r\n") == std::string::npos)
        throw std::invalid_argument("Text must not be blank.");
}
}

Person::Person(int id, const std::string& name) : id(id) {
    requireId(id);
    setName(name);
}
int Person::getId() const { return id; }
const std::string& Person::getName() const { return name; }
void Person::setName(const std::string& value) {
    requireText(value);
    name = value;
}

MedicalRecord::MedicalRecord(int id, int doctorId,
    const std::string& diagnosis, const std::string& prescription)
    : recordId(id), doctorId(doctorId), diagnosis(diagnosis),
      prescription(prescription) {
    requireId(id); requireId(doctorId);
    requireText(diagnosis); requireText(prescription);
}
int MedicalRecord::getId() const { return recordId; }
int MedicalRecord::getDoctorId() const { return doctorId; }
void MedicalRecord::display(std::ostream& out) const {
    out << "Record " << recordId << " | Doctor " << doctorId
        << " | " << diagnosis << " | " << prescription << '\n';
}

Patient::Patient(int id, const std::string& name, int age)
    : Person(id, name) { setAge(age); }
void Patient::setAge(int value) {
    if (value < 0 || value > 130)
        throw std::invalid_argument("Age must be between 0 and 130.");
    age = value;
}
void Patient::addRecord(const MedicalRecord& record) {
    for (const auto& item : records)
        if (item.getId() == record.getId())
            throw std::invalid_argument("Duplicate record ID for patient.");
    records.push_back(record);
}
void Patient::display(std::ostream& out) const {
    out << "Patient " << getId() << " | " << getName()
        << " | Age " << age << '\n';
}
void Patient::displayRecords(std::ostream& out) const {
    for (const auto& record : records) record.display(out);
}

Doctor::Doctor(int id, const std::string& name,
    const std::string& specialization)
    : Person(id, name), specialization(specialization), available(true) {
    requireText(specialization);
}
bool Doctor::isAvailable() const { return available; }
void Doctor::setAvailable(bool value) { available = value; }
void Doctor::display(std::ostream& out) const {
    out << "Doctor " << getId() << " | " << getName()
        << " | " << specialization
        << " | " << (available ? "Available" : "Unavailable") << '\n';
}

Appointment::Appointment(int id, int patientId, int doctorId,
    const std::string& slot)
    : appointmentId(id), patientId(patientId), doctorId(doctorId),
      slot(slot), cancelled(false) {
    requireId(id); requireId(patientId); requireId(doctorId);
    requireText(slot);
}
int Appointment::getId() const { return appointmentId; }
int Appointment::getPatientId() const { return patientId; }
int Appointment::getDoctorId() const { return doctorId; }
const std::string& Appointment::getSlot() const { return slot; }
bool Appointment::isCancelled() const { return cancelled; }
void Appointment::cancel() { cancelled = true; }
void Appointment::display(std::ostream& out) const {
    out << "Appointment " << appointmentId << " | Patient " << patientId
        << " | Doctor " << doctorId << " | " << slot
        << " | " << (cancelled ? "Cancelled" : "Booked") << '\n';
}

Bill::Bill(int id, int patientId, double fee, double charges)
    : billId(id), patientId(patientId), consultationFee(fee),
      serviceCharges(charges), paid(false) {
    requireId(id); requireId(patientId);
    if (!std::isfinite(fee) || !std::isfinite(charges) ||
        fee < 0 || charges < 0 || !std::isfinite(fee + charges))
        throw std::invalid_argument("Charges must be finite and non-negative.");
}
int Bill::getId() const { return billId; }
int Bill::getPatientId() const { return patientId; }
double Bill::calculateTotal() const {
    return consultationFee + serviceCharges;
}
void Bill::markPaid() { paid = true; }
void Bill::display(std::ostream& out) const {
    out << std::fixed << std::setprecision(2)
        << "Bill " << billId << " | Patient " << patientId
        << " | PKR " << calculateTotal()
        << " | " << (paid ? "Paid" : "Unpaid") << '\n';
}

void HospitalSystem::addPatient(const Patient& patient) {
    for (const auto& item : patients)
        if (item.getId() == patient.getId())
            throw std::invalid_argument("Duplicate patient ID.");
    patients.push_back(patient);
}
void HospitalSystem::addDoctor(const Doctor& doctor) {
    for (const auto& item : doctors)
        if (item.getId() == doctor.getId())
            throw std::invalid_argument("Duplicate doctor ID.");
    doctors.push_back(doctor);
}
Patient& HospitalSystem::findPatient(int id) {
    for (auto& patient : patients)
        if (patient.getId() == id) return patient;
    throw std::invalid_argument("Patient ID not found.");
}
const Patient& HospitalSystem::getPatient(int id) const {
    for (const auto& patient : patients)
        if (patient.getId() == id) return patient;
    throw std::invalid_argument("Patient ID not found.");
}
const Doctor& HospitalSystem::getDoctor(int id) const {
    for (const auto& doctor : doctors)
        if (doctor.getId() == id) return doctor;
    throw std::invalid_argument("Doctor ID not found.");
}
void HospitalSystem::addMedicalRecord(int patientId,
    const MedicalRecord& record) {
    getDoctor(record.getDoctorId());
    findPatient(patientId).addRecord(record);
}
void HospitalSystem::bookAppointment(const Appointment& appointment) {
    getPatient(appointment.getPatientId());
    const Doctor& doctor = getDoctor(appointment.getDoctorId());
    if (!doctor.isAvailable() || appointment.isCancelled())
        throw std::invalid_argument("Doctor unavailable or appointment cancelled.");
    for (const auto& item : appointments) {
        if (item.getId() == appointment.getId())
            throw std::invalid_argument("Duplicate appointment ID.");
        if (!item.isCancelled() && item.getSlot() == appointment.getSlot() &&
            (item.getDoctorId() == appointment.getDoctorId() ||
             item.getPatientId() == appointment.getPatientId()))
            throw std::invalid_argument("Doctor or patient already booked.");
    }
    appointments.push_back(appointment);
}
void HospitalSystem::cancelAppointment(int id) {
    for (auto& item : appointments)
        if (item.getId() == id) { item.cancel(); return; }
    throw std::invalid_argument("Appointment ID not found.");
}
void HospitalSystem::addBill(const Bill& bill) {
    getPatient(bill.getPatientId());
    for (const auto& item : bills)
        if (item.getId() == bill.getId())
            throw std::invalid_argument("Duplicate bill ID.");
    bills.push_back(bill);
}
void HospitalSystem::payBill(int id) {
    for (auto& bill : bills)
        if (bill.getId() == id) { bill.markPaid(); return; }
    throw std::invalid_argument("Bill ID not found.");
}
void HospitalSystem::displayAll(std::ostream& out) const {
    for (const auto& patient : patients) {
        patient.display(out); patient.displayRecords(out);
    }
    for (const auto& doctor : doctors) doctor.display(out);
    for (const auto& appointment : appointments) appointment.display(out);
    for (const auto& bill : bills) bill.display(out);
}
void HospitalSystem::saveSummary(const std::string& path) const {
    std::ofstream file(path);
    if (!file) throw std::runtime_error("Cannot open summary file.");
    displayAll(file);
    file.close();
    if (!file) throw std::runtime_error("Cannot finish writing summary file.");
}
