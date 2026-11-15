#include "scapms/controller/SortEngine.h"

#include <utility>

namespace scapms {

void SortEngine::selectionSortByName(
    std::vector<std::unique_ptr<PatientRecord>>& patients) const {
    for (std::size_t start = 0; start < patients.size(); ++start) {
        std::size_t smallest = start;
        for (std::size_t index = start + 1; index < patients.size(); ++index) {
            if (patients[index]->getFullName() < patients[smallest]->getFullName()) {
                smallest = index;
            }
        }
        if (smallest != start) std::swap(patients[start], patients[smallest]);
    }
}

void SortEngine::insertionSortById(
    std::vector<std::unique_ptr<PatientRecord>>& patients) const {
    for (std::size_t index = 1; index < patients.size(); ++index) {
        std::unique_ptr<PatientRecord> current = std::move(patients[index]);
        std::size_t position = index;
        while (position > 0 &&
               patients[position - 1]->getPatientId() > current->getPatientId()) {
            patients[position] = std::move(patients[position - 1]);
            --position;
        }
        patients[position] = std::move(current);
    }
}

void SortEngine::selectionSortByAge(
    std::vector<std::unique_ptr<PatientRecord>>& patients) const {
    for (std::size_t start = 0; start < patients.size(); ++start) {
        std::size_t youngest = start;
        for (std::size_t index = start + 1; index < patients.size(); ++index) {
            if (patients[index]->getAge() < patients[youngest]->getAge()) youngest = index;
        }
        if (youngest != start) std::swap(patients[start], patients[youngest]);
    }
}

void SortEngine::insertionSortAppointmentsByDate(
    std::vector<Appointment>& appointments) const {
    for (std::size_t index = 1; index < appointments.size(); ++index) {
        Appointment current = appointments[index];
        std::size_t position = index;
        while (position > 0 &&
               appointments[position - 1].getDate() > current.getDate()) {
            appointments[position] = appointments[position - 1];
            --position;
        }
        appointments[position] = current;
    }
}

} // namespace scapms
