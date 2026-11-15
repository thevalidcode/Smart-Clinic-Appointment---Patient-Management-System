#pragma once

#include <array>
#include <string>
#include <vector>

namespace scapms {

class RecentPatientsCache {
public:
    static constexpr std::size_t Capacity = 10;

    void recordAccess(const std::string& patientId);
    void remove(const std::string& patientId);
    std::vector<std::string> snapshot() const;

private:
    std::array<std::string, Capacity> patientIds_{};
};

} // namespace scapms
