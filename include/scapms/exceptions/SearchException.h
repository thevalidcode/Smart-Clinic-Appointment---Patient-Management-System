#pragma once

#include "scapms/exceptions/ScapmsException.h"

namespace scapms {

class SearchException final : public ScapmsException {
public:
    explicit SearchException(const std::string& message);
};

} // namespace scapms
