#include "scapms/exceptions/ScapmsException.h"

namespace scapms {
ScapmsException::ScapmsException(const std::string& message) : std::runtime_error(message) {}
} // namespace scapms
