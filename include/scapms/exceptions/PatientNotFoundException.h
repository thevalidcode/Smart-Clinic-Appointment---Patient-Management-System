#pragma once

#include "scapms/exceptions/ScapmsException.h"

namespace scapms {

class PatientNotFoundException final : public ScapmsException {
public:
    explicit PatientNotFoundException(const std::string& patientId);
};

} // namespace scapms
