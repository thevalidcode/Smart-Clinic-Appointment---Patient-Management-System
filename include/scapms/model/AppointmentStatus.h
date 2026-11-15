#pragma once

#include <string>

namespace scapms {

enum class AppointmentStatus {
    Pending,
    Completed,
    Cancelled
};

std::string toString(AppointmentStatus status);
AppointmentStatus appointmentStatusFromString(const std::string& value);

} // namespace scapms
