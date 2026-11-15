#pragma once

#include "scapms/model/PatientRecord.h"

#include <memory>
#include <string>
#include <vector>

namespace scapms {

class PatientCsvRepository {
public:
    static void save(const std::vector<std::unique_ptr<PatientRecord>>& patients,
                     const std::string& path);
    static std::vector<std::unique_ptr<PatientRecord>> load(const std::string& path);
};

} // namespace scapms
