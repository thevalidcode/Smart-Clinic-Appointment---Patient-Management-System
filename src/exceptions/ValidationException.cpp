#include "scapms/exceptions/ValidationException.h"

namespace scapms {
ValidationException::ValidationException(const std::string& message) : ScapmsException(message) {}
} // namespace scapms
