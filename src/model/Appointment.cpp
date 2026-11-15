#include "scapms/model/Appointment.h"

#include <stdexcept>
#include <utility>

namespace scapms {

Appointment::Appointment(std::string appointmentId,
                         std::string patientId,
                         std::string date,
                         std::string time,
                         std::string doctorName,
                         std::string purpose,
                         AppointmentStatus status)
    : appointmentId_(std::move(appointmentId)),
      patientId_(std::move(patientId)),
      date_(std::move(date)),
      time_(std::move(time)),
      doctorName_(std::move(doctorName)),
      purpose_(std::move(purpose)),
      status_(status) {}

const std::string& Appointment::getAppointmentId() const noexcept { return appointmentId_; }
const std::string& Appointment::getPatientId() const noexcept { return patientId_; }
const std::string& Appointment::getDate() const noexcept { return date_; }
const std::string& Appointment::getTime() const noexcept { return time_; }
const std::string& Appointment::getDoctorName() const noexcept { return doctorName_; }
const std::string& Appointment::getPurpose() const noexcept { return purpose_; }
AppointmentStatus Appointment::getStatus() const noexcept { return status_; }
void Appointment::setStatus(AppointmentStatus status) noexcept { status_ = status; }

std::string toString(AppointmentStatus status) {
    switch (status) {
    case AppointmentStatus::Pending: return "Pending";
    case AppointmentStatus::Completed: return "Completed";
    case AppointmentStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

AppointmentStatus appointmentStatusFromString(const std::string& value) {
    if (value == "Pending") return AppointmentStatus::Pending;
    if (value == "Completed") return AppointmentStatus::Completed;
    if (value == "Cancelled") return AppointmentStatus::Cancelled;
    throw std::invalid_argument("Unknown appointment status: " + value);
}

} // namespace scapms
