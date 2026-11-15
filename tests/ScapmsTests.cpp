#include "scapms/controller/ClinicManager.h"
#include "scapms/exceptions/DuplicatePatientIdException.h"
#include "scapms/exceptions/InvalidAgeException.h"
#include "scapms/model/EmergencyPatient.h"
#include "scapms/model/FollowUpPatient.h"
#include "scapms/model/GeneralPatient.h"
#include "scapms/persistence/FileHandler.h"

#include <cassert>
#include <filesystem>
#include <memory>

namespace {

void addPatients(scapms::ClinicManager& manager) {
    manager.patients().registerPatient(std::make_unique<scapms::GeneralPatient>(
        "P003", "Zoe Patient", 30, "0700000003"));
    manager.patients().registerPatient(std::make_unique<scapms::EmergencyPatient>(
        "P001", "Amy Patient", 40, "0700000001", "Urgent", 1));
    manager.patients().registerPatient(std::make_unique<scapms::FollowUpPatient>(
        "P002", "Bob Patient", 50, "0700000002", "A000", "Review"));
}

void testPolymorphismAndValidation() {
    scapms::ClinicManager manager;
    addPatients(manager);
    assert(manager.database().patients().size() == 3);
    assert(manager.database().patients()[1]->getCategory() == scapms::PatientCategory::Emergency);

    bool duplicateRejected = false;
    try {
        manager.patients().registerPatient(std::make_unique<scapms::GeneralPatient>(
            "P001", "Duplicate", 20, "0700000000"));
    } catch (const scapms::DuplicatePatientIdException&) {
        duplicateRejected = true;
    }
    assert(duplicateRejected);

    bool ageRejected = false;
    try {
        manager.patients().registerPatient(std::make_unique<scapms::GeneralPatient>(
            "P004", "Invalid", 121, "0700000004"));
    } catch (const scapms::InvalidAgeException&) {
        ageRejected = true;
    }
    assert(ageRejected);
}

void testAppointmentsQueueAndReports() {
    scapms::ClinicManager manager;
    addPatients(manager);
    manager.appointments().schedule({"A001", "P001", "2026-11-15", "09:00",
                                     "Dr Smith", "Emergency review"});
    manager.appointments().schedule({"A002", "P002", "2026-11-15", "10:00",
                                     "Dr Jones", "Follow-up"});
    manager.appointments().complete("A001");
    manager.appointments().cancel("A002");

    assert(manager.database().findPatient("P001")->getAppointments()[0].getStatus()
           == scapms::AppointmentStatus::Completed);

    manager.queue().addPatient("P003");
    manager.queue().addPatient("P001");
    assert(manager.queue().callNextPatient() == "P003");
    assert(manager.queue().nextPatient() == "P001");

    const auto report = manager.reports();
    assert(report.dailySummary("2026-11-15").total == 2);
    assert(report.dailySummary("2026-11-15").completed == 1);
    assert(report.dailySummary("2026-11-15").cancelled == 1);
}

void testManualAlgorithms() {
    scapms::ClinicManager manager;
    addPatients(manager);
    manager.sort().selectionSortByName(manager.database().patients());
    assert(manager.database().patients()[0]->getFullName() == "Amy Patient");
    assert(manager.search().binarySearchByName(manager.database().patients(), "Bob Patient") != nullptr);

    manager.sort().insertionSortById(manager.database().patients());
    assert(manager.database().patients()[0]->getPatientId() == "P001");
    assert(manager.search().linearSearchById(manager.database().patients(), "P003") != nullptr);

    std::vector<scapms::PatientRecord*> results;
    manager.search().recursiveSearchByName(
        manager.database().patients(), "Zoe Patient", 0, results);
    assert(results.size() == 1);
}

void testCsvRoundTrip() {
    const auto directory = std::filesystem::temp_directory_path() / "scapms-tests";
    std::filesystem::create_directories(directory);
    const auto patientsPath = (directory / "patients.csv").string();
    const auto appointmentsPath = (directory / "appointments.csv").string();

    scapms::ClinicManager source;
    addPatients(source);
    source.appointments().schedule({"A001", "P001", "2026-11-15", "09:00",
                                    "Dr Smith", "Review"});
    scapms::FileHandler::save(source.database(), patientsPath, appointmentsPath);

    scapms::ClinicManager restored;
    scapms::FileHandler::load(restored.database(), patientsPath, appointmentsPath);
    assert(restored.database().patients().size() == 3);
    assert(restored.database().appointments().size() == 1);
    assert(restored.database().findPatient("P001")->getCategory()
           == scapms::PatientCategory::Emergency);
    std::filesystem::remove_all(directory);
}

void testUndoStack() {
    scapms::UndoController undo;
    int value = 2;
    undo.record("Restore value", [&value] { value = 1; });
    assert(undo.canUndo());
    assert(undo.undo() == "Restore value");
    assert(value == 1);
    assert(!undo.canUndo());
}

} // namespace

int main() {
    testPolymorphismAndValidation();
    testAppointmentsQueueAndReports();
    testManualAlgorithms();
    testCsvRoundTrip();
    testUndoStack();
    return 0;
}
