#pragma once

#include "scapms/model/Appointment.h"
#include "scapms/model/PatientRecord.h"
#include "scapms/model/WaitingQueue.h"
#include "scapms/utils/RecentPatientsCache.h"

#include <memory>
#include <string>
#include <vector>

namespace scapms {

class ClinicDatabase {
public:
    ClinicDatabase() = default;
    ClinicDatabase(const ClinicDatabase&) = delete;
    ClinicDatabase& operator=(const ClinicDatabase&) = delete;
    ClinicDatabase(ClinicDatabase&&) noexcept = default;
    ClinicDatabase& operator=(ClinicDatabase&&) noexcept = default;

    PatientRecord& addPatient(std::unique_ptr<PatientRecord> patient);
    void removePatient(const std::string& patientId);
    PatientRecord* findPatient(const std::string& patientId) noexcept;
    const PatientRecord* findPatient(const std::string& patientId) const noexcept;
    bool containsPatient(const std::string& patientId) const noexcept;

    void addAppointment(const Appointment& appointment);
    void removeAppointment(const std::string& appointmentId);
    Appointment* findAppointment(const std::string& appointmentId) noexcept;
    const Appointment* findAppointment(const std::string& appointmentId) const noexcept;

    std::vector<std::unique_ptr<PatientRecord>>& patients() noexcept;
    const std::vector<std::unique_ptr<PatientRecord>>& patients() const noexcept;
    std::vector<Appointment>& appointments() noexcept;
    const std::vector<Appointment>& appointments() const noexcept;
    WaitingQueue& waitingQueue() noexcept;
    const WaitingQueue& waitingQueue() const noexcept;
    RecentPatientsCache& recentPatients() noexcept;
    const RecentPatientsCache& recentPatients() const noexcept;

private:
    std::vector<std::unique_ptr<PatientRecord>> patients_;
    std::vector<Appointment> appointments_;
    WaitingQueue waitingQueue_;
    RecentPatientsCache recentPatients_;
};

} // namespace scapms
