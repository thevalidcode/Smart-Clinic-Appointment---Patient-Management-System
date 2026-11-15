#pragma once

#include "scapms/model/PatientRecord.h"

namespace scapms {

class EmergencyPatient final : public PatientRecord, public Appointable {
public:
    EmergencyPatient(std::string patientId,
                     std::string fullName,
                     int age,
                     std::string contactInformation,
                     std::string emergencyDescription = {},
                     int triageLevel = 1);

    PatientCategory getCategory() const noexcept override;
    std::string getDisplayDescription() const override;

    const std::string& getEmergencyDescription() const noexcept;
    int getTriageLevel() const noexcept;
    void scheduleAppointment(const Appointment& appointment) override;
    void cancelAppointment(const std::string& appointmentId) override;
    bool hasAppointment(const std::string& appointmentId) const override;

private:
    std::string emergencyDescription_;
    int triageLevel_;
};

} // namespace scapms
