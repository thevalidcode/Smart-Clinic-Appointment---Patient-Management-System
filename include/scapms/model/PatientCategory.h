#pragma once

#include <string>

namespace scapms {

enum class PatientCategory {
    General,
    Emergency,
    FollowUp
};

std::string toString(PatientCategory category);
PatientCategory patientCategoryFromString(const std::string& value);

} // namespace scapms
