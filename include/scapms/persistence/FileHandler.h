#pragma once

#include "scapms/model/ClinicDatabase.h"

#include <string>

namespace scapms {

class FileHandler {
public:
    static void save(const ClinicDatabase& database,
                     const std::string& patientsPath,
                     const std::string& appointmentsPath);
    static void load(ClinicDatabase& database,
                     const std::string& patientsPath,
                     const std::string& appointmentsPath);
};

} // namespace scapms
