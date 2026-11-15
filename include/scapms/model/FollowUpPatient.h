#pragma once

#include "scapms/model/PatientRecord.h"

namespace scapms {

class FollowUpPatient final : public PatientRecord, public Appointable {
public:
    FollowUpPatient(std::string patientId,
                    std::string fullName,
                    int age,
                    std::string contactInformation,
                    std::string previousAppointmentId = {},
                    std::string followUpReason = {});

    PatientCategory getCategory() const noexcept override;
    std::string getDisplayDescription() const override;

    const std::string& getPreviousAppointmentId() const noexcept;
    const std::string& getFollowUpReason() const noexcept;
    void scheduleAppointment(const Appointment& appointment) override;
    void cancelAppointment(const std::string& appointmentId) override;
    bool hasAppointment(const std::string& appointmentId) const override;

private:
    std::string previousAppointmentId_;
    std::string followUpReason_;
};

} // namespace scapms
