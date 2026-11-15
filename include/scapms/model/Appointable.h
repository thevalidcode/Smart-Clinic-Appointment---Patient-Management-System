#pragma once

#include "scapms/model/Appointment.h"

#include <string>

namespace scapms {

class Appointable {
public:
    virtual ~Appointable() = default;

    virtual void scheduleAppointment(const Appointment& appointment) = 0;
    virtual void cancelAppointment(const std::string& appointmentId) = 0;
    virtual bool hasAppointment(const std::string& appointmentId) const = 0;
};

} // namespace scapms
