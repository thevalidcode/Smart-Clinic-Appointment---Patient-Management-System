#include "scapms/controller/AppointmentController.h"

#include "scapms/controller/UndoController.h"
#include "scapms/exceptions/InvalidAppointmentException.h"
#include "scapms/utils/InputValidator.h"

namespace scapms {

AppointmentController::AppointmentController(ClinicDatabase& database, UndoController* undo)
    : database_(database), undo_(undo) {}

void AppointmentController::schedule(const Appointment& appointment) {
    InputValidator::validateAppointment(
        appointment.getDate(), appointment.getTime(), appointment.getPatientId());
    database_.addAppointment(appointment);
    if (undo_ != nullptr) {
        const std::string appointmentId = appointment.getAppointmentId();
        undo_->record("Undo appointment scheduling", [this, appointmentId] {
            database_.removeAppointment(appointmentId);
        });
    }
}

void AppointmentController::cancel(const std::string& appointmentId) {
    Appointment* appointment = database_.findAppointment(appointmentId);
    if (appointment == nullptr) {
        throw InvalidAppointmentException("Appointment was not found: " + appointmentId);
    }
    const AppointmentStatus previousStatus = appointment->getStatus();
    appointment->setStatus(AppointmentStatus::Cancelled);
    if (PatientRecord* patient = database_.findPatient(appointment->getPatientId())) {
        patient->updateAppointmentStatus(appointmentId, AppointmentStatus::Cancelled);
    }
    if (undo_ != nullptr) {
        undo_->record("Undo appointment cancellation", [this, appointmentId, previousStatus] {
            restoreStatus(appointmentId, previousStatus);
        });
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
    const AppointmentStatus previousStatus = appointment->getStatus();
    appointment->setStatus(AppointmentStatus::Completed);
    if (PatientRecord* patient = database_.findPatient(appointment->getPatientId())) {
        patient->updateAppointmentStatus(appointmentId, AppointmentStatus::Completed);
    }
    if (undo_ != nullptr) {
        undo_->record("Undo appointment completion", [this, appointmentId, previousStatus] {
            restoreStatus(appointmentId, previousStatus);
        });
    }
}

void AppointmentController::restoreStatus(const std::string& appointmentId,
                                           AppointmentStatus status) {
    Appointment* appointment = database_.findAppointment(appointmentId);
    if (appointment == nullptr) return;
    appointment->setStatus(status);
    if (PatientRecord* patient = database_.findPatient(appointment->getPatientId())) {
        patient->updateAppointmentStatus(appointmentId, status);
    }
}

} // namespace scapms
