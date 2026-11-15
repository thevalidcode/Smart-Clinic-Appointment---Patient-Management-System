#include "scapms/persistence/AppointmentCsvRepository.h"

#include "scapms/exceptions/FileOperationException.h"

#include <fstream>

namespace scapms {
namespace {

std::string csvEscape(const std::string& value) {
    std::string escaped = "\"";
    for (char character : value) {
        if (character == '"') escaped += '"';
        escaped += character;
    }
    escaped += '"';
    return escaped;
}

std::vector<std::string> parseCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool quoted = false;
    for (std::size_t index = 0; index < line.size(); ++index) {
        const char character = line[index];
        if (character == '"') {
            if (quoted && index + 1 < line.size() && line[index + 1] == '"') {
                field += '"';
                ++index;
            } else {
                quoted = !quoted;
            }
        } else if (character == ',' && !quoted) {
            fields.push_back(field);
            field.clear();
        } else {
            field += character;
        }
    }
    fields.push_back(field);
    return fields;
}

} // namespace

void AppointmentCsvRepository::save(const std::vector<Appointment>& appointments,
                                    const std::string& path) {
    std::ofstream output(path);
    if (!output) throw FileOperationException("Unable to open appointment file for writing: " + path);
    output << "appointment_id,patient_id,date,time,doctor,purpose,status\n";
    for (const Appointment& appointment : appointments) {
        output << csvEscape(appointment.getAppointmentId()) << ','
               << csvEscape(appointment.getPatientId()) << ','
               << csvEscape(appointment.getDate()) << ','
               << csvEscape(appointment.getTime()) << ','
               << csvEscape(appointment.getDoctorName()) << ','
               << csvEscape(appointment.getPurpose()) << ','
               << csvEscape(toString(appointment.getStatus())) << '\n';
    }
}

std::vector<Appointment> AppointmentCsvRepository::load(const std::string& path) {
    std::ifstream input(path);
    if (!input) throw FileOperationException("Unable to open appointment file for reading: " + path);
    std::string line;
    if (!std::getline(input, line) ||
        line != "appointment_id,patient_id,date,time,doctor,purpose,status") {
        throw FileOperationException("Appointment file has an invalid header: " + path);
    }

    std::vector<Appointment> appointments;
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        const auto fields = parseCsvLine(line);
        if (fields.size() != 7) throw FileOperationException("Invalid appointment row in: " + path);
        try {
            appointments.emplace_back(fields[0], fields[1], fields[2], fields[3],
                                      fields[4], fields[5],
                                      appointmentStatusFromString(fields[6]));
        } catch (const std::exception& error) {
            throw FileOperationException("Invalid appointment data: " + std::string(error.what()));
        }
    }
    return appointments;
}

} // namespace scapms
