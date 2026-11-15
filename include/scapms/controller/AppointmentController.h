#pragma once

#include "scapms/model/ClinicDatabase.h"

#include <string>

namespace scapms {

class UndoController;

class AppointmentController {
public:
    explicit AppointmentController(ClinicDatabase& database, UndoController* undo = nullptr);

    void schedule(const Appointment& appointment);
    void cancel(const std::string& appointmentId);
    void complete(const std::string& appointmentId);

private:
    void restoreStatus(const std::string& appointmentId, AppointmentStatus status);

    ClinicDatabase& database_;
    UndoController* undo_;
};

} // namespace scapms
