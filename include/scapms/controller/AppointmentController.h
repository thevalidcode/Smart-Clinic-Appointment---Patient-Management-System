#pragma once

#include "scapms/model/ClinicDatabase.h"

#include <string>

namespace scapms {

class AppointmentController {
public:
    explicit AppointmentController(ClinicDatabase& database);

    void schedule(const Appointment& appointment);
    void cancel(const std::string& appointmentId);
    void complete(const std::string& appointmentId);

private:
    ClinicDatabase& database_;
};

} // namespace scapms
