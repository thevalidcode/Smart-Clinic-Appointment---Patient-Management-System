#pragma once

#include <functional>
#include <memory>
#include <stack>
#include <string>

namespace scapms {

class UndoController {
public:
    using UndoAction = std::function<void()>;

    void record(std::string description, UndoAction action);
    bool canUndo() const noexcept;
    std::string undo();
    void clear() noexcept;

private:
    struct Operation {
        std::string description;
        UndoAction action;
    };

    std::stack<Operation> operations_;
};

} // namespace scapms
