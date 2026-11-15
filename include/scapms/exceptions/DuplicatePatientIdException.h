#pragma once

#include "scapms/exceptions/ValidationException.h"

namespace scapms {

class DuplicatePatientIdException final : public ValidationException {
public:
    explicit DuplicatePatientIdException(const std::string& patientId);
};

} // namespace scapms
