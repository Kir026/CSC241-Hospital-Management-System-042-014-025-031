#include "HMS.hpp"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

template<class Function> void rejects(Function operation) {
    bool rejected = false;
    try { operation(); } catch (const std::exception&) { rejected = true; }
    assert(rejected);
}
int main() {
    HospitalSystem h;
    h.addPatient(Patient(1, "Test Patient", 20));
    h.addPatient(Patient(2, "Second Patient", 40));
    h.addDoctor(Doctor(101, "Test Doctor", "General Medicine"));
    h.addDoctor(Doctor(102, "Second Doctor", "General Medicine"));
    Doctor unavailable(103, "Unavailable Doctor", "General Medicine");
    unavailable.setAvailable(false); h.addDoctor(unavailable);
    rejects([&] { h.addPatient(Patient(1, "Duplicate", 20)); });
    rejects([&] { h.addDoctor(Doctor(101, "Duplicate", "Medicine")); });
    rejects([] { Patient p(0, "Test", 20); });
    rejects([] { Patient p(3, " ", 20); });
    rejects([] { Patient p(3, "Test", -1); });
    rejects([] { Patient p(3, "Test", 131); });
    Patient youngest(4, "Baby", 0), oldest(5, "Boundary", 130);
    rejects([&] { h.bookAppointment(Appointment(1, 999, 101, "S1")); });
    rejects([&] { h.bookAppointment(Appointment(1, 1, 999, "S1")); });
    rejects([&] { h.bookAppointment(Appointment(1, 1, 103, "S1")); });
    h.bookAppointment(Appointment(1, 1, 101, "S1"));
    rejects([&] { h.bookAppointment(Appointment(2, 2, 101, "S1")); });
    rejects([&] { h.bookAppointment(Appointment(2, 1, 102, "S1")); });
    rejects([&] { h.bookAppointment(Appointment(1, 2, 102, "S2")); });
    h.cancelAppointment(1);
    h.bookAppointment(Appointment(2, 1, 101, "S1"));
    rejects([&] { h.cancelAppointment(999); });
    h.addMedicalRecord(1, MedicalRecord(1, 101, "Example", "Example"));
    rejects([&] { h.addMedicalRecord(1, MedicalRecord(1, 101, "X", "Y")); });
    rejects([&] { h.addMedicalRecord(1, MedicalRecord(2, 999, "X", "Y")); });
    rejects([&] { h.addMedicalRecord(999, MedicalRecord(2, 101, "X", "Y")); });
    assert(std::abs(Bill(1, 1, 1500, 500).calculateTotal() - 2000) < 0.001);
    rejects([] { Bill b(1, 1, -1, 0); });
    rejects([] { Bill b(1, 1, std::numeric_limits<double>::infinity(), 0); });
    rejects([] { Bill b(1, 1, std::numeric_limits<double>::quiet_NaN(), 0); });
    rejects([&] { h.addBill(Bill(1, 999, 100, 0)); });
    h.addBill(Bill(1, 1, 1500, 500));
    rejects([&] { h.addBill(Bill(1, 1, 100, 0)); });
    h.payBill(1);
    rejects([&] { h.payBill(999); });
    std::ostringstream output;
    const Person& p = h.getPatient(1);
    const Person& d = h.getDoctor(101);
    p.display(output); d.display(output);
    assert(output.str().find("Patient 1") != std::string::npos);
    assert(output.str().find("Doctor 101") != std::string::npos);
    output.str(""); h.displayAll(output);
    assert(output.str().find("PKR 2000.00 | Paid") != std::string::npos);
    assert(output.str().find("Record 1") != std::string::npos);
    h.saveSummary("test_summary.txt");
    std::ifstream input("test_summary.txt");
    std::ostringstream saved; saved << input.rdbuf();
    assert(saved.str() == output.str());
    rejects([&] { h.saveSummary("missing_folder/summary.txt"); });
    std::cout << "All Level 2 tests passed.\n";
}
