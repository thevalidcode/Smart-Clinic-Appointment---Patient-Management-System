#include "scapms/persistence/FileHandler.h"

#include "scapms/persistence/AppointmentCsvRepository.h"
#include "scapms/persistence/PatientCsvRepository.h"

#include <utility>

namespace scapms {

void FileHandler::save(const ClinicDatabase& database,
                       const std::string& patientsPath,
                       const std::string& appointmentsPath) {
    PatientCsvRepository::save(database.patients(), patientsPath);
    AppointmentCsvRepository::save(database.appointments(), appointmentsPath);
}

void FileHandler::load(ClinicDatabase& database,
                       const std::string& patientsPath,
                       const std::string& appointmentsPath) {
    auto patients = PatientCsvRepository::load(patientsPath);
    const auto appointments = AppointmentCsvRepository::load(appointmentsPath);
    ClinicDatabase replacement;
    for (auto& patient : patients) replacement.addPatient(std::move(patient));
    for (const Appointment& appointment : appointments) replacement.addAppointment(appointment);
    database = std::move(replacement);
}

} // namespace scapms
