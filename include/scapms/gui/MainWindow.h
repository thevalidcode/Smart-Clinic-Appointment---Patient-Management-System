#pragma once

#include "scapms/controller/ClinicManager.h"

#include <QMainWindow>

class QLabel;
class QTabWidget;

namespace scapms {

class PatientPanel;
class AppointmentPanel;
class QueuePanel;
class SearchSortPanel;
class ReportsPanel;

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void saveData();
    void loadData();
    void undoLastOperation();
    void refreshDashboard();

private:
    void createMenus();
    void refreshAllPanels();

    ClinicManager manager_;
    QLabel* dashboardLabel_;
    PatientPanel* patientPanel_;
    AppointmentPanel* appointmentPanel_;
    QueuePanel* queuePanel_;
    SearchSortPanel* searchSortPanel_;
    ReportsPanel* reportsPanel_;
};

} // namespace scapms
