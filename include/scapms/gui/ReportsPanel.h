#pragma once

#include "scapms/controller/ClinicManager.h"

#include <QWidget>

class QComboBox;
class QTableWidget;
class QTextEdit;

namespace scapms {

class ReportsPanel final : public QWidget {
    Q_OBJECT
public:
    explicit ReportsPanel(ClinicManager& manager, QWidget* parent = nullptr);

private slots:
    void generateReport();

private:
    ClinicManager& manager_;
    QComboBox* reportCombo_;
    QTableWidget* table_;
    QTextEdit* summary_;
};

} // namespace scapms
