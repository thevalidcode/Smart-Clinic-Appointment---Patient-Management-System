#pragma once

#include "scapms/model/ClinicDatabase.h"

#include <memory>
#include <string>

namespace scapms {

class UndoController;

class PatientController {
public:
    explicit PatientController(ClinicDatabase& database, UndoController* undo = nullptr);

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
    UndoController* undo_;
};

} // namespace scapms
