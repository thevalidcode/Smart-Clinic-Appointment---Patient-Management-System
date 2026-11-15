#pragma once

#include "scapms/model/Appointment.h"

#include <string>
#include <vector>

namespace scapms {

class AppointmentCsvRepository {
public:
    static void save(const std::vector<Appointment>& appointments,
                     const std::string& path);
    static std::vector<Appointment> load(const std::string& path);
};

} // namespace scapms
