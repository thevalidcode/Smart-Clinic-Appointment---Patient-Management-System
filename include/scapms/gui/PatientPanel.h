#pragma once

#include "scapms/controller/ClinicManager.h"

#include <QWidget>

class QLineEdit;
class QSpinBox;
class QComboBox;
class QTableWidget;

namespace scapms {

class PatientPanel final : public QWidget {
    Q_OBJECT
public:
    explicit PatientPanel(ClinicManager& manager, QWidget* parent = nullptr);

public slots:
    void refresh();

private slots:
    void addPatient();
    void deleteSelectedPatient();

private:
    ClinicManager& manager_;
    QLineEdit* idEdit_;
    QLineEdit* nameEdit_;
    QSpinBox* ageEdit_;
    QLineEdit* contactEdit_;
    QComboBox* typeCombo_;
    QTableWidget* table_;
};

} // namespace scapms
