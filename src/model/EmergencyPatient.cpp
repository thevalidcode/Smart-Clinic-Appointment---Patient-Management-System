#include "scapms/model/EmergencyPatient.h"

#include <utility>

namespace scapms {

EmergencyPatient::EmergencyPatient(std::string patientId,
                                   std::string fullName,
                                   int age,
                                   std::string contactInformation,
                                   std::string emergencyDescription,
                                   int triageLevel)
    : PatientRecord(std::move(patientId), std::move(fullName), age,
                    std::move(contactInformation)),
      emergencyDescription_(std::move(emergencyDescription)),
      triageLevel_(triageLevel) {}

PatientCategory EmergencyPatient::getCategory() const noexcept {
    return PatientCategory::Emergency;
}

std::string EmergencyPatient::getDisplayDescription() const {
    return "Emergency patient (triage " + std::to_string(triageLevel_) + ")";
}

std::unique_ptr<PatientRecord> EmergencyPatient::clone() const {
    auto copy = std::make_unique<EmergencyPatient>(
        getPatientId(), getFullName(), getAge(), getContactInformation(),
        emergencyDescription_, triageLevel_);
    for (const Appointment& appointment : getAppointments()) copy->scheduleAppointment(appointment);
    return copy;
}

const std::string& EmergencyPatient::getEmergencyDescription() const noexcept {
    return emergencyDescription_;
}

int EmergencyPatient::getTriageLevel() const noexcept { return triageLevel_; }

void EmergencyPatient::scheduleAppointment(const Appointment& appointment) {
    addAppointment(appointment);
}

void EmergencyPatient::cancelAppointment(const std::string& appointmentId) {
    removeAppointment(appointmentId);
}

bool EmergencyPatient::hasAppointment(const std::string& appointmentId) const {
    for (const Appointment& appointment : getAppointments()) {
        if (appointment.getAppointmentId() == appointmentId) return true;
    }
    return false;
}

} // namespace scapms
