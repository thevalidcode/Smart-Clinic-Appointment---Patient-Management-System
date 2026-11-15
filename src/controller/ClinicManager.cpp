#include "scapms/controller/ClinicManager.h"

namespace scapms {

ClinicManager::ClinicManager()
    : database_( ),
      patientController_(database_),
      appointmentController_(database_),
      queueController_(database_) {}

ClinicDatabase& ClinicManager::database() noexcept { return database_; }
const ClinicDatabase& ClinicManager::database() const noexcept { return database_; }
PatientController& ClinicManager::patients() noexcept { return patientController_; }
AppointmentController& ClinicManager::appointments() noexcept { return appointmentController_; }
QueueController& ClinicManager::queue() noexcept { return queueController_; }
SearchEngine& ClinicManager::search() noexcept { return searchEngine_; }
SortEngine& ClinicManager::sort() noexcept { return sortEngine_; }
UndoController& ClinicManager::undo() noexcept { return undoController_; }
ReportController ClinicManager::reports() const { return ReportController(database_); }

} // namespace scapms
