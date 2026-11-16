# Smart Clinic Appointment & Patient Management System

## Project description

The Smart Clinic Appointment & Patient Management System (SCAPMS) is a C++17
desktop application for managing patient records, appointments, waiting queues,
searches, sorting, persistence, and operational reports. The graphical user
interface is implemented with Qt Widgets and follows an event-driven design:
menus, buttons, table selections, timers, and keyboard shortcuts invoke
controller operations while the model remains independent of the GUI.

The system supports General, Emergency, and Follow-Up patients. Each patient
has an ID, name, age, contact information, a category-specific description,
and associated appointments. Staff can register, edit, delete, search, sort,
and review patient records. Appointments can be scheduled, completed, or
cancelled. A FIFO waiting queue and report views support day-to-day clinic
operations.

## System features and class hierarchy

`PatientRecord` is the abstract base class. `GeneralPatient`,
`EmergencyPatient`, and `FollowUpPatient` inherit from it and override
category-specific display and cloning behavior. The `Appointable` interface
defines appointment operations and demonstrates runtime polymorphism: the
concrete patient types provide their own implementations.

`ClinicDatabase` owns the patient records and appointments, so it models
composition. Appointments associate a patient ID with a date, time, doctor,
purpose, and status. `ClinicManager` composes the controllers and exposes the
application service surface. `PatientController`, `AppointmentController`,
`QueueController`, `SearchEngine`, `SortEngine`, and `ReportController` keep
business logic out of the Qt panels. The GUI panels communicate with
`ClinicManager`, not directly with persistence internals.

## Data structures and algorithms

Patient records are stored in a `std::vector<std::unique_ptr<PatientRecord>>`,
which preserves ownership and permits polymorphic objects. Appointments are
stored in a vector and each patient stores its associated appointment records.
The waiting queue uses FIFO semantics. The undo controller stores labelled
callables in a stack-like sequence, allowing the last administrative operation
to be reversed.

The Search & Sort panel exposes the manually implemented algorithms:

* Linear search scans patient records and is used for patient ID searches.
* Binary search is used for exact name searches after selection sorting by
  name, satisfying the sorted-input precondition.
* Recursive search traverses the patient collection recursively for name
  searches.
* Selection sort orders patients by name.
* Insertion sort orders patients by patient ID.

These algorithms were selected because the assessment requires visible,
manually implemented algorithms rather than relying only on library sorting or
search functions. The interface makes the selected algorithm explicit and
prevents binary search from being used on an unsorted collection.

## GUI design and event-driven programming

The main window contains Dashboard, Patient Records, Appointments, Waiting
Queue, Search & Sort, and Reports tabs. The dashboard shows total patients,
total appointments, pending appointments, and waiting patients. Layout managers
provide responsive spacing and table stretching rather than fixed coordinates.

The Patient Records panel supports add, edit, update, and delete operations.
Selecting Edit Selected loads the record into the form; Update Patient commits
validated changes and records an undo action. The patient type selector reveals
preferred-doctor, emergency/triage, or follow-up fields dynamically.

The Appointments panel provides scheduling, completion, cancellation, and
status-aware table colours. The Waiting Queue panel shows FIFO order and the
next patient. Search controls separate field, algorithm, and category. Reports
include most frequently attended patients, pending appointments, category
distribution, and daily appointment summaries.

Menus expose Save, Save Data As, Load Data, Undo Last Administrative Operation,
Focus Search, and Exit. Ctrl+S, Ctrl+O, Ctrl+F, Ctrl+Z, and Ctrl+Q provide
keyboard access. Tooltips explain important actions and the persistent status
bar confirms successful operations.

## Exception handling, validation, and persistence

Validation is performed at the controller boundary through the existing
exception types. Blank IDs, names, and contacts; duplicate IDs; invalid ages;
invalid appointment data; missing patients; duplicate appointments; cancelled
completion; empty queues; and invalid searches produce meaningful messages.
The Qt boundary catches domain exceptions and displays a warning or critical
dialog without terminating the application.

Patient and appointment data are persisted as CSV files through
`FileHandler`, `PatientCsvRepository`, and `AppointmentCsvRepository`. Save
Data As and Load Data use `QFileDialog`, allow the user to select both files,
handle cancellation, and report the resulting paths through the status bar.

## Undo implementation

Administrative controller operations register inverse actions with
`UndoController`. Patient registration, editing, and deletion can be undone.
Appointment scheduling, completion, and cancellation can also be undone.
The Edit menu exposes Undo Last Administrative Operation and refreshes all
panels after the inverse action runs. The unit tests verify patient edit undo
and appointment status undo.

## Challenges and resolutions

The main integration challenge was exposing already-tested controller behavior
through a coherent GUI without duplicating business rules. This was resolved
by keeping validation and undo in controllers while adding only selection,
form, dialog, and refresh logic to panels. Another challenge was ensuring that
binary search remains correct; the search panel now sorts by name before
calling the binary-search implementation. A final challenge was providing
assessment-ready evidence: the application now includes visible algorithm
choices, dynamic subtype fields, status presentation, tooltips, file dialogs,
dashboard metrics, and keyboard shortcuts.

## Verification

The project builds with:

    cmake --build build --parallel 2

The configured CTest suite passes:

    ctest --test-dir build --output-on-failure

Both the smoke test and unit test pass in the verified build.
