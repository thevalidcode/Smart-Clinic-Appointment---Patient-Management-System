#include "scapms/controller/QueueController.h"

#include "scapms/exceptions/PatientNotFoundException.h"

namespace scapms {

QueueController::QueueController(ClinicDatabase& database) : database_(database) {}

void QueueController::addPatient(const std::string& patientId) {
    if (!database_.containsPatient(patientId)) throw PatientNotFoundException(patientId);
    database_.waitingQueue().enqueue(patientId);
}

std::string QueueController::callNextPatient() {
    return database_.waitingQueue().dequeue();
}

const std::string& QueueController::nextPatient() const {
    return database_.waitingQueue().peek();
}

std::vector<std::string> QueueController::getWaitingPatients() const {
    return database_.waitingQueue().snapshot();
}

} // namespace scapms
