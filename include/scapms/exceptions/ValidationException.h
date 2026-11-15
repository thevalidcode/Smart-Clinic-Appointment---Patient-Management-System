#pragma once

#include "scapms/exceptions/ScapmsException.h"

namespace scapms {

class ValidationException : public ScapmsException {
public:
    explicit ValidationException(const std::string& message);
};

} // namespace scapms
