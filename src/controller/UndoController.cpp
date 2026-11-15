#include "scapms/controller/UndoController.h"

#include <stdexcept>
#include <utility>

namespace scapms {

void UndoController::record(std::string description, UndoAction action) {
    if (!action) throw std::invalid_argument("Undo action cannot be empty.");
    operations_.push({std::move(description), std::move(action)});
}

bool UndoController::canUndo() const noexcept { return !operations_.empty(); }

std::string UndoController::undo() {
    if (operations_.empty()) throw std::out_of_range("There is no administrative operation to undo.");
    Operation operation = std::move(operations_.top());
    operations_.pop();
    operation.action();
    return operation.description;
}

void UndoController::clear() noexcept {
    while (!operations_.empty()) operations_.pop();
}

} // namespace scapms
