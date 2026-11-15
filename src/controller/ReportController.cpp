#include "scapms/controller/ReportController.h"

namespace scapms {

ReportController::ReportController(const ClinicDatabase& database) : database_(database) {}

std::vector<FrequentPatientReport> ReportController::mostFrequentlyAttended() const {
    std::vector<FrequentPatientReport> report;
    for (const auto& patient : database_.patients()) {
        std::size_t completed = 0;
        for (const Appointment& appointment : patient->getAppointments()) {
            if (appointment.getStatus() == AppointmentStatus::Completed) ++completed;
        }
        report.push_back({patient->getPatientId(), patient->getFullName(), completed});
    }
    return report;
}

std::vector<const Appointment*> ReportController::pendingAppointments() const {
    std::vector<const Appointment*> report;
    for (const Appointment& appointment : database_.appointments()) {
        if (appointment.getStatus() == AppointmentStatus::Pending) report.push_back(&appointment);
    }
    return report;
}

std::map<PatientCategory, std::size_t> ReportController::categoryDistribution() const {
    std::map<PatientCategory, std::size_t> distribution;
    for (const auto& patient : database_.patients()) ++distribution[patient->getCategory()];
    return distribution;
}

DailyAppointmentSummary ReportController::dailySummary(const std::string& date) const {
    DailyAppointmentSummary summary;
    for (const Appointment& appointment : database_.appointments()) {
        if (appointment.getDate() != date) continue;
        ++summary.total;
        switch (appointment.getStatus()) {
        case AppointmentStatus::Completed: ++summary.completed; break;
        case AppointmentStatus::Pending: ++summary.pending; break;
        case AppointmentStatus::Cancelled: ++summary.cancelled; break;
        }
    }
    return summary;
}

} // namespace scapms
