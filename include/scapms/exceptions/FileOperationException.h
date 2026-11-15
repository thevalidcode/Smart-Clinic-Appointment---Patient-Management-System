#pragma once

#include "scapms/exceptions/ScapmsException.h"

namespace scapms {

class FileOperationException final : public ScapmsException {
public:
    explicit FileOperationException(const std::string& message);
};

} // namespace scapms
