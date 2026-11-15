#pragma once

#include "scapms/controller/ClinicManager.h"

#include <QWidget>

class QComboBox;
class QListWidget;

namespace scapms {

class QueuePanel final : public QWidget {
    Q_OBJECT
public:
    explicit QueuePanel(ClinicManager& manager, QWidget* parent = nullptr);

public slots:
    void refresh();

private slots:
    void addSelectedPatient();
    void callNextPatient();

private:
    ClinicManager& manager_;
    QComboBox* patientCombo_;
    QListWidget* queueList_;
};

} // namespace scapms
