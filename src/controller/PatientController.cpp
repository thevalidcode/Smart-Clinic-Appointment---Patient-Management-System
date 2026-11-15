#include "scapms/controller/PatientController.h"

#include "scapms/controller/UndoController.h"
#include "scapms/exceptions/PatientNotFoundException.h"
#include "scapms/exceptions/ValidationException.h"
#include "scapms/utils/InputValidator.h"

#include <utility>

namespace scapms {

PatientController::PatientController(ClinicDatabase& database, UndoController* undo)
    : database_(database), undo_(undo) {}

PatientRecord& PatientController::registerPatient(std::unique_ptr<PatientRecord> patient) {
    if (!patient) throw ValidationException("Patient record cannot be null.");
    InputValidator::validatePatientId(patient->getPatientId());
    InputValidator::validateName(patient->getFullName());
    InputValidator::validateAge(patient->getAge());
    InputValidator::validateContact(patient->getContactInformation());
    const std::string patientId = patient->getPatientId();
    PatientRecord& added = database_.addPatient(std::move(patient));
    if (undo_ != nullptr) {
        undo_->record("Undo patient registration", [this, patientId] {
            database_.removePatient(patientId);
        });
    }
    return added;
}

void PatientController::editPatient(const std::string& patientId,
                                    std::string name,
                                    int age,
                                    std::string contact) {
    PatientRecord* patient = database_.findPatient(patientId);
    if (patient == nullptr) throw PatientNotFoundException(patientId);
    const std::string oldName = patient->getFullName();
    const int oldAge = patient->getAge();
    const std::string oldContact = patient->getContactInformation();
    InputValidator::validateName(name);
    InputValidator::validateAge(age);
    InputValidator::validateContact(contact);
    patient->setFullName(std::move(name));
    patient->setAge(age);
    patient->setContactInformation(std::move(contact));
    if (undo_ != nullptr) {
        undo_->record("Undo patient edit", [this, patientId, oldName, oldAge, oldContact] {
            PatientRecord* restored = database_.findPatient(patientId);
            if (restored != nullptr) {
                restored->setFullName(oldName);
                restored->setAge(oldAge);
                restored->setContactInformation(oldContact);
            }
        });
    }
}

void PatientController::deletePatient(const std::string& patientId) {
    PatientRecord* patient = database_.findPatient(patientId);
    if (patient == nullptr) throw PatientNotFoundException(patientId);
    std::shared_ptr<PatientRecord> snapshot(patient->clone().release());
    database_.removePatient(patientId);
    if (undo_ != nullptr) {
        undo_->record("Undo patient deletion", [this, snapshot] {
            database_.addPatient(snapshot->clone());
        });
    }
}

PatientRecord& PatientController::getPatient(const std::string& patientId) {
    PatientRecord* patient = database_.findPatient(patientId);
    if (patient == nullptr) throw PatientNotFoundException(patientId);
    recordPatientAccess(patientId);
    return *patient;
}

void PatientController::recordPatientAccess(const std::string& patientId) {
    if (!database_.containsPatient(patientId)) throw PatientNotFoundException(patientId);
    database_.recentPatients().recordAccess(patientId);
}

} // namespace scapms
