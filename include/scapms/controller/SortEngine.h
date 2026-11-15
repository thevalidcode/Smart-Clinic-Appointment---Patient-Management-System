#pragma once

#include "scapms/model/Appointment.h"
#include "scapms/model/PatientRecord.h"

#include <memory>
#include <vector>

namespace scapms {

class SortEngine {
public:
    void selectionSortByName(
        std::vector<std::unique_ptr<PatientRecord>>& patients) const;
    void insertionSortById(
        std::vector<std::unique_ptr<PatientRecord>>& patients) const;
    void selectionSortByAge(
        std::vector<std::unique_ptr<PatientRecord>>& patients) const;
    void insertionSortAppointmentsByDate(
        std::vector<Appointment>& appointments) const;
};

} // namespace scapms
