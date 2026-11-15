#include "scapms/exceptions/FileOperationException.h"

namespace scapms {
FileOperationException::FileOperationException(const std::string& message)
    : ScapmsException(message) {}
} // namespace scapms
