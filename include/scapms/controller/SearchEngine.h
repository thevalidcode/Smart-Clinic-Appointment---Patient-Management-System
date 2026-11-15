#pragma once

#include "scapms/model/PatientRecord.h"

#include <memory>
#include <string>
#include <vector>

namespace scapms {

class SearchEngine {
public:
    PatientRecord* linearSearchById(
        const std::vector<std::unique_ptr<PatientRecord>>& patients,
        const std::string& patientId) const;

    PatientRecord* binarySearchByName(
        const std::vector<std::unique_ptr<PatientRecord>>& sortedPatients,
        const std::string& name) const;

    void recursiveSearchByName(
        const std::vector<std::unique_ptr<PatientRecord>>& patients,
        const std::string& name,
        std::size_t index,
        std::vector<PatientRecord*>& results) const;

    std::vector<PatientRecord*> searchByCategory(
        const std::vector<std::unique_ptr<PatientRecord>>& patients,
        PatientCategory category) const;
};

} // namespace scapms
