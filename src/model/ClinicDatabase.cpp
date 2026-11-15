#include "scapms/model/ClinicDatabase.h"

#include "scapms/exceptions/DuplicatePatientIdException.h"
#include "scapms/exceptions/PatientNotFoundException.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace scapms {

PatientRecord& ClinicDatabase::addPatient(std::unique_ptr<PatientRecord> patient) {
    if (!patient) throw std::invalid_argument("Cannot add a null patient.");
    if (containsPatient(patient->getPatientId())) {
        throw DuplicatePatientIdException(patient->getPatientId());
    }
    patients_.push_back(std::move(patient));
    return *patients_.back();
}

void ClinicDatabase::removePatient(const std::string& patientId) {
    auto iterator = std::find_if(
        patients_.begin(), patients_.end(),
        [&patientId](const std::unique_ptr<PatientRecord>& patient) {
            return patient->getPatientId() == patientId;
        });
    if (iterator == patients_.end()) throw PatientNotFoundException(patientId);
    patients_.erase(iterator);
    recentPatients_.remove(patientId);
}

PatientRecord* ClinicDatabase::findPatient(const std::string& patientId) noexcept {
    for (const auto& patient : patients_) {
        if (patient->getPatientId() == patientId) return patient.get();
    }
    return nullptr;
}

const PatientRecord* ClinicDatabase::findPatient(const std::string& patientId) const noexcept {
    for (const auto& patient : patients_) {
        if (patient->getPatientId() == patientId) return patient.get();
    }
    return nullptr;
}

bool ClinicDatabase::containsPatient(const std::string& patientId) const noexcept {
    return findPatient(patientId) != nullptr;
}

void ClinicDatabase::addAppointment(const Appointment& appointment) {
    if (findAppointment(appointment.getAppointmentId()) != nullptr) {
        throw std::invalid_argument("Appointment ID already exists.");
    }
    PatientRecord* patient = findPatient(appointment.getPatientId());
    if (patient == nullptr) throw PatientNotFoundException(appointment.getPatientId());

    auto* appointable = dynamic_cast<Appointable*>(patient);
    if (appointable == nullptr) throw std::logic_error("Patient cannot be scheduled.");
    appointable->scheduleAppointment(appointment);
    appointments_.push_back(appointment);
}

void ClinicDatabase::removeAppointment(const std::string& appointmentId) {
    auto iterator = std::find_if(
        appointments_.begin(), appointments_.end(),
        [&appointmentId](const Appointment& appointment) {
            return appointment.getAppointmentId() == appointmentId;
        });
    if (iterator == appointments_.end()) {
        throw std::invalid_argument("Appointment was not found: " + appointmentId);
    }
    PatientRecord* patient = findPatient(iterator->getPatientId());
    if (patient != nullptr) {
        if (auto* appointable = dynamic_cast<Appointable*>(patient)) {
            appointable->cancelAppointment(appointmentId);
        }
    }
    appointments_.erase(iterator);
}

Appointment* ClinicDatabase::findAppointment(const std::string& appointmentId) noexcept {
    for (Appointment& appointment : appointments_) {
        if (appointment.getAppointmentId() == appointmentId) return &appointment;
    }
    return nullptr;
}

const Appointment* ClinicDatabase::findAppointment(const std::string& appointmentId) const noexcept {
    for (const Appointment& appointment : appointments_) {
        if (appointment.getAppointmentId() == appointmentId) return &appointment;
    }
    return nullptr;
}

std::vector<std::unique_ptr<PatientRecord>>& ClinicDatabase::patients() noexcept { return patients_; }
const std::vector<std::unique_ptr<PatientRecord>>& ClinicDatabase::patients() const noexcept { return patients_; }
std::vector<Appointment>& ClinicDatabase::appointments() noexcept { return appointments_; }
const std::vector<Appointment>& ClinicDatabase::appointments() const noexcept { return appointments_; }
WaitingQueue& ClinicDatabase::waitingQueue() noexcept { return waitingQueue_; }
const WaitingQueue& ClinicDatabase::waitingQueue() const noexcept { return waitingQueue_; }
RecentPatientsCache& ClinicDatabase::recentPatients() noexcept { return recentPatients_; }
const RecentPatientsCache& ClinicDatabase::recentPatients() const noexcept { return recentPatients_; }

} // namespace scapms
