# Hospital Management System Level 2

CSC241 Object Oriented Programming, BEE-3A, Fall 2026

Team: Ali Zia Syed (FA25-BEE-042), Aaryan Hassan (FA25-BEE-014),
Affan Tariq (FA25-BEE-031), Abdullah Shamim (FA25-BEE-025).

This is the partial implementation for Level 2. It runs a scripted console
demonstration of the class design. Interactive menus and complete record
loading are planned for Level 3.

## Build and run

From this directory, using a C++17 compiler:

```sh
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/HMS.cpp src/main.cpp -o hms
./hms
```

For Windows MinGW use `-o hms.exe` and run `hms.exe` (or `./hms.exe` in PowerShell).
Alternatively add src/HMS.cpp and src/main.cpp to a C++ console project, add
include to the compiler's include search path, and enable C++17.

The program writes hms_summary.txt in the current working directory. This
human-readable export is overwritten on each run; it is not a reloadable database.
The demo uses fictional data only.

## Tests

```sh
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/HMS.cpp tests/test_hms.cpp -o test_hms
./test_hms
```

Run tests from this project directory, with no folder named missing_folder.
Keep assertions enabled (do not use -DNDEBUG). Expected result:
`All Level 2 tests passed.` A test_summary.txt file is created.

## Design

Seven classes: Person (abstract), Patient, Doctor, MedicalRecord, Appointment,
Bill and HospitalSystem. Patient and Doctor inherit Person; display() is virtual.
Patient owns its records by value. HospitalSystem owns its entity collections.
Appointment, Bill and MedicalRecord use IDs for associations.

design contains editable Graphviz UML sources and rendered diagrams. The two
views describe one class model. `+` is public, `-` is private, an empty triangle
is inheritance, a filled diamond is composition, and plain lines are associations.
ID accessors are omitted in the second view for space; HMS.hpp has all signatures.

## Current limits and next steps

- All source classes and methods shown in HMS.hpp are implemented for this prototype.
- Appointment conflicts compare exact, non-empty slot strings. Calendar validation,
  consistent date formatting and duration overlap handling remain to be implemented.
- Add interactive registration/search/update menus and appointment rescheduling.
- Add service descriptions, currency-safe billing, payment history and stronger validation.
- Add structured save/load, loading-error checks and ID generation.
- Do not retain references returned by getters across additions to the same vector.
- The Person virtual destructor and standard containers handle object cleanup.
- The alternative console application and treatment of the SFML rubric need instructor agreement.

## Team repository

Repository URL: TO BE ADDED BY THE TEAM.

Create the team GitHub repository and upload source, README, diagrams and manuals.
Each member should commit their own work as development continues. The supplied
folder does not claim an existing repository, contribution history or instructor approval.
