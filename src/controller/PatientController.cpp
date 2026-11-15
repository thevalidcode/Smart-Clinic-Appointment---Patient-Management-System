#include "scapms/controller/PatientController.h"

#include "scapms/exceptions/PatientNotFoundException.h"
#include "scapms/exceptions/ValidationException.h"
#include "scapms/utils/InputValidator.h"

#include <utility>

namespace scapms {

PatientController::PatientController(ClinicDatabase& database) : database_(database) {}

PatientRecord& PatientController::registerPatient(std::unique_ptr<PatientRecord> patient) {
    if (!patient) throw ValidationException("Patient record cannot be null.");
    InputValidator::validatePatientId(patient->getPatientId());
    InputValidator::validateName(patient->getFullName());
    InputValidator::validateAge(patient->getAge());
    InputValidator::validateContact(patient->getContactInformation());
    return database_.addPatient(std::move(patient));
}

void PatientController::editPatient(const std::string& patientId,
                                    std::string name,
                                    int age,
                                    std::string contact) {
    PatientRecord* patient = database_.findPatient(patientId);
    if (patient == nullptr) throw PatientNotFoundException(patientId);
    InputValidator::validateName(name);
    InputValidator::validateAge(age);
    InputValidator::validateContact(contact);
    patient->setFullName(std::move(name));
    patient->setAge(age);
    patient->setContactInformation(std::move(contact));
}

void PatientController::deletePatient(const std::string& patientId) {
    database_.removePatient(patientId);
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
