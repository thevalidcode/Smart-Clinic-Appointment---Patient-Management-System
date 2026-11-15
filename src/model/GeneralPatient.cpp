#include "scapms/model/GeneralPatient.h"

#include <stdexcept>
#include <utility>

namespace scapms {

GeneralPatient::GeneralPatient(std::string patientId,
                               std::string fullName,
                               int age,
                               std::string contactInformation,
                               std::string preferredDoctor)
    : PatientRecord(std::move(patientId), std::move(fullName), age,
                    std::move(contactInformation)),
      preferredDoctor_(std::move(preferredDoctor)) {}

PatientCategory GeneralPatient::getCategory() const noexcept {
    return PatientCategory::General;
}

std::string GeneralPatient::getDisplayDescription() const {
    return "General patient";
}

std::unique_ptr<PatientRecord> GeneralPatient::clone() const {
    auto copy = std::make_unique<GeneralPatient>(
        getPatientId(), getFullName(), getAge(), getContactInformation(), preferredDoctor_);
    for (const Appointment& appointment : getAppointments()) copy->scheduleAppointment(appointment);
    return copy;
}

const std::string& GeneralPatient::getPreferredDoctor() const noexcept {
    return preferredDoctor_;
}

void GeneralPatient::scheduleAppointment(const Appointment& appointment) {
    addAppointment(appointment);
}

void GeneralPatient::cancelAppointment(const std::string& appointmentId) {
    removeAppointment(appointmentId);
}

bool GeneralPatient::hasAppointment(const std::string& appointmentId) const {
    for (const Appointment& appointment : getAppointments()) {
        if (appointment.getAppointmentId() == appointmentId) return true;
    }
    return false;
}

std::string toString(PatientCategory category) {
    switch (category) {
    case PatientCategory::General: return "General";
    case PatientCategory::Emergency: return "Emergency";
    case PatientCategory::FollowUp: return "Follow-Up";
    }
    return "Unknown";
}

PatientCategory patientCategoryFromString(const std::string& value) {
    if (value == "General") return PatientCategory::General;
    if (value == "Emergency") return PatientCategory::Emergency;
    if (value == "Follow-Up") return PatientCategory::FollowUp;
    throw std::invalid_argument("Unknown patient category: " + value);
}

} // namespace scapms
