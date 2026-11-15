#pragma once

#include "scapms/model/PatientRecord.h"

namespace scapms {

class GeneralPatient final : public PatientRecord, public Appointable {
public:
    GeneralPatient(std::string patientId,
                   std::string fullName,
                   int age,
                   std::string contactInformation,
                   std::string preferredDoctor = {});

    PatientCategory getCategory() const noexcept override;
    std::string getDisplayDescription() const override;

    const std::string& getPreferredDoctor() const noexcept;
    void scheduleAppointment(const Appointment& appointment) override;
    void cancelAppointment(const std::string& appointmentId) override;
    bool hasAppointment(const std::string& appointmentId) const override;

private:
    std::string preferredDoctor_;
};

} // namespace scapms
