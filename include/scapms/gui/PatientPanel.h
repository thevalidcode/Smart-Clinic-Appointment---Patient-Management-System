#pragma once

#include "scapms/controller/ClinicManager.h"

#include <QWidget>

class QLineEdit;
class QSpinBox;
class QComboBox;
class QTableWidget;
class QPushButton;

namespace scapms {

class PatientPanel final : public QWidget {
    Q_OBJECT
public:
    explicit PatientPanel(ClinicManager& manager, QWidget* parent = nullptr);

public slots:
    void refresh();

private slots:
    void addPatient();
    void loadSelectedPatient();
    void updatePatient();
    void deleteSelectedPatient();
    void updateDynamicFields();

private:
    ClinicManager& manager_;
    QLineEdit* idEdit_;
    QLineEdit* nameEdit_;
    QSpinBox* ageEdit_;
    QLineEdit* contactEdit_;
    QComboBox* typeCombo_;
    QLineEdit* preferredDoctorEdit_;
    QLineEdit* emergencyDescriptionEdit_;
    QSpinBox* triageLevelEdit_;
    QLineEdit* previousAppointmentEdit_;
    QLineEdit* followUpReasonEdit_;
    QPushButton* updateButton_;
    QTableWidget* table_;
    QString editingPatientId_;
};

} // namespace scapms
