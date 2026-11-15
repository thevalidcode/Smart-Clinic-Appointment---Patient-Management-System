#pragma once

#include "scapms/exceptions/ValidationException.h"

namespace scapms {

class InvalidAppointmentException final : public ValidationException {
public:
    explicit InvalidAppointmentException(const std::string& message);
};

} // namespace scapms
