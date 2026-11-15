#include "scapms/utils/InputValidator.h"

#include "scapms/exceptions/InvalidAgeException.h"
#include "scapms/exceptions/InvalidAppointmentException.h"
#include "scapms/exceptions/ValidationException.h"

#include <cctype>

namespace scapms {

namespace {
bool isBlank(const std::string& value) {
    for (unsigned char character : value) {
        if (!std::isspace(character)) return false;
    }
    return true;
}
} // namespace

void InputValidator::validatePatientId(const std::string& patientId) {
    if (isBlank(patientId)) throw ValidationException("Patient ID cannot be empty.");
}

void InputValidator::validateName(const std::string& name) {
    if (isBlank(name)) throw ValidationException("Patient name cannot be empty.");
}

void InputValidator::validateAge(int age) {
    if (age < 0 || age > 120) throw InvalidAgeException(age);
}

void InputValidator::validateContact(const std::string& contact) {
    if (isBlank(contact)) throw ValidationException("Contact information cannot be empty.");
}

void InputValidator::validateAppointment(const std::string& date,
                                         const std::string& time,
                                         const std::string& patientId) {
    if (isBlank(date) || isBlank(time) || isBlank(patientId)) {
        throw InvalidAppointmentException(
            "Appointment date, time, and patient ID are required.");
    }
}

} // namespace scapms
