#include "scapms/exceptions/DuplicatePatientIdException.h"

namespace scapms {
DuplicatePatientIdException::DuplicatePatientIdException(const std::string& patientId)
    : ValidationException("Patient ID already exists: " + patientId) {}
} // namespace scapms
