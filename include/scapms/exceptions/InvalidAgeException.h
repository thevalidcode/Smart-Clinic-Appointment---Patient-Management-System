#pragma once

#include "scapms/exceptions/ValidationException.h"

namespace scapms {

class InvalidAgeException final : public ValidationException {
public:
    explicit InvalidAgeException(int age);
};

} // namespace scapms
