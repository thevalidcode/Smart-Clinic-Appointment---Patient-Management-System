#pragma once

#include "scapms/model/Appointable.h"
#include "scapms/model/PatientCategory.h"

#include <string>
#include <vector>

namespace scapms {

class PatientRecord {
public:
    PatientRecord(std::string patientId,
                  std::string fullName,
                  int age,
                  std::string contactInformation);
    virtual ~PatientRecord() = default;

    const std::string& getPatientId() const noexcept;
    const std::string& getFullName() const noexcept;
    int getAge() const noexcept;
    const std::string& getContactInformation() const noexcept;
    const std::vector<Appointment>& getAppointments() const noexcept;

    void setFullName(std::string fullName);
    void setAge(int age);
    void setContactInformation(std::string contactInformation);
    void updateAppointmentStatus(const std::string& appointmentId,
                                 AppointmentStatus status);

    virtual PatientCategory getCategory() const noexcept = 0;
    virtual std::string getDisplayDescription() const = 0;

protected:
    void addAppointment(const Appointment& appointment);
    void removeAppointment(const std::string& appointmentId);

private:
    std::string patientId_;
    std::string fullName_;
    int age_;
    std::string contactInformation_;
    std::vector<Appointment> appointments_;
};

} // namespace scapms
