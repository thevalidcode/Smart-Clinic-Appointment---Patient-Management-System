#include "scapms/model/FollowUpPatient.h"

#include <utility>

namespace scapms {

FollowUpPatient::FollowUpPatient(std::string patientId,
                                 std::string fullName,
                                 int age,
                                 std::string contactInformation,
                                 std::string previousAppointmentId,
                                 std::string followUpReason)
    : PatientRecord(std::move(patientId), std::move(fullName), age,
                    std::move(contactInformation)),
      previousAppointmentId_(std::move(previousAppointmentId)),
      followUpReason_(std::move(followUpReason)) {}

PatientCategory FollowUpPatient::getCategory() const noexcept {
    return PatientCategory::FollowUp;
}

std::string FollowUpPatient::getDisplayDescription() const {
    return "Follow-up patient";
}

const std::string& FollowUpPatient::getPreviousAppointmentId() const noexcept {
    return previousAppointmentId_;
}

const std::string& FollowUpPatient::getFollowUpReason() const noexcept {
    return followUpReason_;
}

void FollowUpPatient::scheduleAppointment(const Appointment& appointment) {
    addAppointment(appointment);
}

void FollowUpPatient::cancelAppointment(const std::string& appointmentId) {
    removeAppointment(appointmentId);
}

bool FollowUpPatient::hasAppointment(const std::string& appointmentId) const {
    for (const Appointment& appointment : getAppointments()) {
        if (appointment.getAppointmentId() == appointmentId) return true;
    }
    return false;
}

} // namespace scapms
