#pragma once

#include <stdexcept>
#include <string>

namespace scapms {

class ScapmsException : public std::runtime_error {
public:
    explicit ScapmsException(const std::string& message);
};

} // namespace scapms
