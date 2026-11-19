// CSC241 OOP - HMS Level 2 demonstration
// FA25-BEE-042, FA25-BEE-014, FA25-BEE-031, FA25-BEE-025
#include "HMS.hpp"
#include <exception>
#include <iostream>

int main() {
    try {
        HospitalSystem hospital;
        hospital.addPatient(Patient(1, "Sample Patient", 20));
        hospital.addDoctor(Doctor(101, "Sample Doctor", "General Medicine"));

        std::cout << "POLYMORPHISM DEMONSTRATION\n";
        const Person& person1 = hospital.getPatient(1);
        const Person& person2 = hospital.getDoctor(101);
        person1.display(std::cout);
        person2.display(std::cout);

        hospital.bookAppointment(Appointment(501, 1, 101, "2026-11-20 10:00"));
        hospital.addMedicalRecord(1, MedicalRecord(701, 101,
            "Sample diagnosis", "Sample prescription"));
        hospital.addBill(Bill(901, 1, 1500, 500));
        hospital.payBill(901);

        std::cout << "\nHOSPITAL RECORDS\n";
        hospital.displayAll(std::cout);
        hospital.saveSummary("hms_summary.txt");
        std::cout << "Saved hms_summary.txt\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
