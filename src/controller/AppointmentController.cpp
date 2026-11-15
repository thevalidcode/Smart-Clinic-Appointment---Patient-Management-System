#include "scapms/controller/AppointmentController.h"

#include "scapms/exceptions/InvalidAppointmentException.h"
#include "scapms/utils/InputValidator.h"

namespace scapms {

AppointmentController::AppointmentController(ClinicDatabase& database) : database_(database) {}

void AppointmentController::schedule(const Appointment& appointment) {
    InputValidator::validateAppointment(
        appointment.getDate(), appointment.getTime(), appointment.getPatientId());
    database_.addAppointment(appointment);
}

void AppointmentController::cancel(const std::string& appointmentId) {
    Appointment* appointment = database_.findAppointment(appointmentId);
    if (appointment == nullptr) {
        throw InvalidAppointmentException("Appointment was not found: " + appointmentId);
    }
    appointment->setStatus(AppointmentStatus::Cancelled);
    if (PatientRecord* patient = database_.findPatient(appointment->getPatientId())) {
        patient->updateAppointmentStatus(appointmentId, AppointmentStatus::Cancelled);
    }
}

void AppointmentController::complete(const std::string& appointmentId) {
    Appointment* appointment = database_.findAppointment(appointmentId);
    if (appointment == nullptr) {
        throw InvalidAppointmentException("Appointment was not found: " + appointmentId);
    }
    if (appointment->getStatus() == AppointmentStatus::Cancelled) {
        throw InvalidAppointmentException("A cancelled appointment cannot be completed.");
    }
    appointment->setStatus(AppointmentStatus::Completed);
    if (PatientRecord* patient = database_.findPatient(appointment->getPatientId())) {
        patient->updateAppointmentStatus(appointmentId, AppointmentStatus::Completed);
    }
}

} // namespace scapms
