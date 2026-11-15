#include "scapms/model/WaitingQueue.h"

#include <stdexcept>

namespace scapms {

void WaitingQueue::enqueue(const std::string& patientId) {
    if (!contains(patientId)) patientIds_.push(patientId);
}

std::string WaitingQueue::dequeue() {
    if (patientIds_.empty()) throw std::out_of_range("The waiting queue is empty.");
    std::string patientId = patientIds_.front();
    patientIds_.pop();
    return patientId;
}

const std::string& WaitingQueue::peek() const {
    if (patientIds_.empty()) throw std::out_of_range("The waiting queue is empty.");
    return patientIds_.front();
}

bool WaitingQueue::empty() const noexcept { return patientIds_.empty(); }
std::size_t WaitingQueue::size() const noexcept { return patientIds_.size(); }

bool WaitingQueue::contains(const std::string& patientId) const {
    std::queue<std::string> copy = patientIds_;
    while (!copy.empty()) {
        if (copy.front() == patientId) return true;
        copy.pop();
    }
    return false;
}

std::vector<std::string> WaitingQueue::snapshot() const {
    std::vector<std::string> result;
    std::queue<std::string> copy = patientIds_;
    while (!copy.empty()) {
        result.push_back(copy.front());
        copy.pop();
    }
    return result;
}

} // namespace scapms
