#include "scapms/exceptions/SearchException.h"

namespace scapms {
SearchException::SearchException(const std::string& message)
    : ScapmsException(message) {}
} // namespace scapms
