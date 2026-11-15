#include "scapms/utils/RecentPatientsCache.h"

#include <algorithm>

namespace scapms {

void RecentPatientsCache::recordAccess(const std::string& patientId) {
    if (patientId.empty()) return;
    std::array<std::string, Capacity> updated{};
    updated[0] = patientId;
    std::size_t next = 1;
    for (const std::string& current : patientIds_) {
        if (current.empty() || current == patientId || next == Capacity) continue;
        updated[next++] = current;
    }
    patientIds_ = updated;
}

void RecentPatientsCache::remove(const std::string& patientId) {
    for (std::string& current : patientIds_) {
        if (current == patientId) current.clear();
    }
}

std::vector<std::string> RecentPatientsCache::snapshot() const {
    std::vector<std::string> result;
    for (const std::string& patientId : patientIds_) {
        if (!patientId.empty()) result.push_back(patientId);
    }
    return result;
}

} // namespace scapms
