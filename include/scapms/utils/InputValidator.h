#pragma once

#include <string>

namespace scapms {

class InputValidator {
public:
    static void validatePatientId(const std::string& patientId);
    static void validateName(const std::string& name);
    static void validateAge(int age);
    static void validateContact(const std::string& contact);
    static void validateAppointment(const std::string& date,
                                    const std::string& time,
                                    const std::string& patientId);
};

} // namespace scapms
