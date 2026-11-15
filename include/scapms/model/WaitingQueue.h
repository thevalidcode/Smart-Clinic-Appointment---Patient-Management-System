#pragma once

#include <cstddef>
#include <queue>
#include <string>
#include <vector>

namespace scapms {

class WaitingQueue {
public:
    void enqueue(const std::string& patientId);
    std::string dequeue();
    const std::string& peek() const;
    bool empty() const noexcept;
    std::size_t size() const noexcept;
    bool contains(const std::string& patientId) const;
    std::vector<std::string> snapshot() const;

private:
    std::queue<std::string> patientIds_;
};

} // namespace scapms
