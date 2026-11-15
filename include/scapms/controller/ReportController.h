#pragma once

#include "scapms/model/ClinicDatabase.h"

#include <map>
#include <string>
#include <vector>

namespace scapms {

struct FrequentPatientReport {
    std::string patientId;
    std::string patientName;
    std::size_t completedAppointments{};
};

struct DailyAppointmentSummary {
    std::size_t total{};
    std::size_t completed{};
    std::size_t pending{};
    std::size_t cancelled{};
};

class ReportController {
public:
    explicit ReportController(const ClinicDatabase& database);

    std::vector<FrequentPatientReport> mostFrequentlyAttended() const;
    std::vector<const Appointment*> pendingAppointments() const;
    std::map<PatientCategory, std::size_t> categoryDistribution() const;
    DailyAppointmentSummary dailySummary(const std::string& date) const;

private:
    const ClinicDatabase& database_;
};

} // namespace scapms
