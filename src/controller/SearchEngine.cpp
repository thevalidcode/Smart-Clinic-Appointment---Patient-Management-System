#include "scapms/controller/SearchEngine.h"

namespace scapms {

PatientRecord* SearchEngine::linearSearchById(
    const std::vector<std::unique_ptr<PatientRecord>>& patients,
    const std::string& patientId) const {
    for (const auto& patient : patients) {
        if (patient->getPatientId() == patientId) return patient.get();
    }
    return nullptr;
}

PatientRecord* SearchEngine::binarySearchByName(
    const std::vector<std::unique_ptr<PatientRecord>>& sortedPatients,
    const std::string& name) const {
    std::size_t first = 0;
    std::size_t last = sortedPatients.size();
    while (first < last) {
        const std::size_t middle = first + (last - first) / 2;
        const std::string& currentName = sortedPatients[middle]->getFullName();
        if (currentName == name) return sortedPatients[middle].get();
        if (currentName < name) first = middle + 1;
        else last = middle;
    }
    return nullptr;
}

void SearchEngine::recursiveSearchByName(
    const std::vector<std::unique_ptr<PatientRecord>>& patients,
    const std::string& name,
    std::size_t index,
    std::vector<PatientRecord*>& results) const {
    if (index >= patients.size()) return;
    if (patients[index]->getFullName() == name) results.push_back(patients[index].get());
    recursiveSearchByName(patients, name, index + 1, results);
}

std::vector<PatientRecord*> SearchEngine::searchByCategory(
    const std::vector<std::unique_ptr<PatientRecord>>& patients,
    PatientCategory category) const {
    std::vector<PatientRecord*> results;
    for (const auto& patient : patients) {
        if (patient->getCategory() == category) results.push_back(patient.get());
    }
    return results;
}

} // namespace scapms
