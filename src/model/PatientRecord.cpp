#include "scapms/model/PatientRecord.h"

#include <algorithm>
#include <utility>

namespace scapms {

PatientRecord::PatientRecord(std::string patientId,
                             std::string fullName,
                             int age,
                             std::string contactInformation)
    : patientId_(std::move(patientId)),
      fullName_(std::move(fullName)),
      age_(age),
      contactInformation_(std::move(contactInformation)) {}

const std::string& PatientRecord::getPatientId() const noexcept { return patientId_; }
const std::string& PatientRecord::getFullName() const noexcept { return fullName_; }
int PatientRecord::getAge() const noexcept { return age_; }
const std::string& PatientRecord::getContactInformation() const noexcept { return contactInformation_; }
const std::vector<Appointment>& PatientRecord::getAppointments() const noexcept { return appointments_; }

void PatientRecord::setFullName(std::string fullName) { fullName_ = std::move(fullName); }
void PatientRecord::setAge(int age) { age_ = age; }
void PatientRecord::setContactInformation(std::string contactInformation) {
    contactInformation_ = std::move(contactInformation);
}

void PatientRecord::updateAppointmentStatus(const std::string& appointmentId,
                                            AppointmentStatus status) {
    for (Appointment& appointment : appointments_) {
        if (appointment.getAppointmentId() == appointmentId) {
            appointment.setStatus(status);
            return;
        }
    }
}

void PatientRecord::addAppointment(const Appointment& appointment) {
    appointments_.push_back(appointment);
}

void PatientRecord::removeAppointment(const std::string& appointmentId) {
    appointments_.erase(
        std::remove_if(appointments_.begin(), appointments_.end(),
                       [&appointmentId](const Appointment& appointment) {
                           return appointment.getAppointmentId() == appointmentId;
                       }),
        appointments_.end());
}

} // namespace scapms
