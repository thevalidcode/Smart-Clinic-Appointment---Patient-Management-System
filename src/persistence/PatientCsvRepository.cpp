#include "scapms/persistence/PatientCsvRepository.h"

#include "scapms/exceptions/FileOperationException.h"
#include "scapms/exceptions/ValidationException.h"
#include "scapms/model/EmergencyPatient.h"
#include "scapms/model/FollowUpPatient.h"
#include "scapms/model/GeneralPatient.h"
#include "scapms/utils/InputValidator.h"

#include <fstream>
#include <sstream>

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

void PatientCsvRepository::save(
    const std::vector<std::unique_ptr<PatientRecord>>& patients,
    const std::string& path) {
    std::ofstream output(path);
    if (!output) throw FileOperationException("Unable to open patient file for writing: " + path);
    output << "patient_id,patient_type,name,age,contact,detail_1,detail_2\n";
    for (const auto& patient : patients) {
        output << csvEscape(patient->getPatientId()) << ','
               << csvEscape(toString(patient->getCategory())) << ','
               << csvEscape(patient->getFullName()) << ','
               << patient->getAge() << ','
               << csvEscape(patient->getContactInformation()) << ',';
        if (const auto* general = dynamic_cast<const GeneralPatient*>(patient.get())) {
            output << csvEscape(general->getPreferredDoctor()) << ',';
        } else if (const auto* emergency = dynamic_cast<const EmergencyPatient*>(patient.get())) {
            output << csvEscape(emergency->getEmergencyDescription()) << ',';
            output << emergency->getTriageLevel() << '\n';
            continue;
        } else if (const auto* followUp = dynamic_cast<const FollowUpPatient*>(patient.get())) {
            output << csvEscape(followUp->getPreviousAppointmentId()) << ','
                   << csvEscape(followUp->getFollowUpReason()) << '\n';
            continue;
        }
        output << "\"\"\n";
    }
}

std::vector<std::unique_ptr<PatientRecord>> PatientCsvRepository::load(
    const std::string& path) {
    std::ifstream input(path);
    if (!input) throw FileOperationException("Unable to open patient file for reading: " + path);

    std::string line;
    if (!std::getline(input, line) || line != "patient_id,patient_type,name,age,contact,detail_1,detail_2") {
        throw FileOperationException("Patient file has an invalid header: " + path);
    }

    std::vector<std::unique_ptr<PatientRecord>> patients;
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        const auto fields = parseCsvLine(line);
        if (fields.size() != 7) throw FileOperationException("Invalid patient row in: " + path);
        try {
            const int age = std::stoi(fields[3]);
            InputValidator::validatePatientId(fields[0]);
            InputValidator::validateName(fields[2]);
            InputValidator::validateAge(age);
            InputValidator::validateContact(fields[4]);
            if (fields[1] == "General") {
                patients.push_back(std::make_unique<GeneralPatient>(
                    fields[0], fields[2], age, fields[4], fields[5]));
            } else if (fields[1] == "Emergency") {
                patients.push_back(std::make_unique<EmergencyPatient>(
                    fields[0], fields[2], age, fields[4], fields[5], std::stoi(fields[6])));
            } else if (fields[1] == "Follow-Up") {
                patients.push_back(std::make_unique<FollowUpPatient>(
                    fields[0], fields[2], age, fields[4], fields[5], fields[6]));
            } else {
                throw ValidationException("Unknown patient type: " + fields[1]);
            }
        } catch (const std::exception& error) {
            throw FileOperationException("Invalid patient data: " + std::string(error.what()));
        }
    }
    return patients;
}

} // namespace scapms
