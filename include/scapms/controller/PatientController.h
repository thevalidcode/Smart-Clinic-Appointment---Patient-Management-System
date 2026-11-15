#pragma once

#include "scapms/model/ClinicDatabase.h"

#include <memory>
#include <string>

namespace scapms {

class PatientController {
public:
    explicit PatientController(ClinicDatabase& database);

    PatientRecord& registerPatient(std::unique_ptr<PatientRecord> patient);
    void editPatient(const std::string& patientId,
                     std::string name,
                     int age,
                     std::string contact);
    void deletePatient(const std::string& patientId);
    PatientRecord& getPatient(const std::string& patientId);
    void recordPatientAccess(const std::string& patientId);

private:
    ClinicDatabase& database_;
};

} // namespace scapms
