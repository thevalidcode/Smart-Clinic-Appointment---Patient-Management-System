#include "scapms/exceptions/PatientNotFoundException.h"

namespace scapms {
PatientNotFoundException::PatientNotFoundException(const std::string& patientId)
    : ScapmsException("Patient was not found: " + patientId) {}
} // namespace scapms
