#pragma once

#include "scapms/controller/ClinicManager.h"

#include <QWidget>

class QComboBox;
class QDateEdit;
class QLineEdit;
class QTableWidget;
class QTimeEdit;

namespace scapms {

class AppointmentPanel final : public QWidget {
    Q_OBJECT
public:
    explicit AppointmentPanel(ClinicManager& manager, QWidget* parent = nullptr);

public slots:
    void refresh();

private slots:
    void scheduleAppointment();
    void cancelAppointment();

private:
    ClinicManager& manager_;
    QComboBox* patientCombo_;
    QDateEdit* dateEdit_;
    QTimeEdit* timeEdit_;
    QLineEdit* doctorEdit_;
    QLineEdit* purposeEdit_;
    QTableWidget* table_;
};

} // namespace scapms
