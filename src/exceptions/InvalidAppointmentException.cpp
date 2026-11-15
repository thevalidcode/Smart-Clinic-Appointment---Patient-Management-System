#include "scapms/exceptions/InvalidAppointmentException.h"

namespace scapms {
InvalidAppointmentException::InvalidAppointmentException(const std::string& message)
    : ValidationException(message) {}
} // namespace scapms
