#pragma once

#include "scapms/model/ClinicDatabase.h"

#include <string>
#include <vector>

namespace scapms {

class QueueController {
public:
    explicit QueueController(ClinicDatabase& database);

    void addPatient(const std::string& patientId);
    std::string callNextPatient();
    const std::string& nextPatient() const;
    std::vector<std::string> getWaitingPatients() const;

private:
    ClinicDatabase& database_;
};

} // namespace scapms
