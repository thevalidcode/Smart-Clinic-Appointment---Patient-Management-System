#pragma once

#include "scapms/model/AppointmentStatus.h"

#include <string>

namespace scapms {

class Appointment {
public:
    Appointment() = default;
    Appointment(std::string appointmentId,
                std::string patientId,
                std::string date,
                std::string time,
                std::string doctorName,
                std::string purpose,
                AppointmentStatus status = AppointmentStatus::Pending);

    const std::string& getAppointmentId() const noexcept;
    const std::string& getPatientId() const noexcept;
    const std::string& getDate() const noexcept;
    const std::string& getTime() const noexcept;
    const std::string& getDoctorName() const noexcept;
    const std::string& getPurpose() const noexcept;
    AppointmentStatus getStatus() const noexcept;

    void setStatus(AppointmentStatus status) noexcept;

private:
    std::string appointmentId_;
    std::string patientId_;
    std::string date_;
    std::string time_;
    std::string doctorName_;
    std::string purpose_;
    AppointmentStatus status_{AppointmentStatus::Pending};
};

} // namespace scapms
