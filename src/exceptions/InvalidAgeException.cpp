#include "scapms/exceptions/InvalidAgeException.h"

#include <string>

namespace scapms {
InvalidAgeException::InvalidAgeException(int age)
    : ValidationException("Patient age must be between 0 and 120: " + std::to_string(age)) {}
} // namespace scapms
