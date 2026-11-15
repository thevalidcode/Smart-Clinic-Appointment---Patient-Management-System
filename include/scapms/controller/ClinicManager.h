#pragma once

#include "scapms/controller/AppointmentController.h"
#include "scapms/controller/PatientController.h"
#include "scapms/controller/QueueController.h"
#include "scapms/controller/ReportController.h"
#include "scapms/controller/SearchEngine.h"
#include "scapms/controller/SortEngine.h"
#include "scapms/controller/UndoController.h"

namespace scapms {

class ClinicManager {
public:
    ClinicManager();

    ClinicDatabase& database() noexcept;
    const ClinicDatabase& database() const noexcept;
    PatientController& patients() noexcept;
    AppointmentController& appointments() noexcept;
    QueueController& queue() noexcept;
    SearchEngine& search() noexcept;
    SortEngine& sort() noexcept;
    UndoController& undo() noexcept;
    ReportController reports() const;

private:
    ClinicDatabase database_;
    PatientController patientController_;
    AppointmentController appointmentController_;
    QueueController queueController_;
    SearchEngine searchEngine_;
    SortEngine sortEngine_;
    UndoController undoController_;
};

} // namespace scapms
