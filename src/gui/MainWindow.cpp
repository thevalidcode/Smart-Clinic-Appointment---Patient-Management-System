#include "scapms/gui/MainWindow.h"

#include "scapms/exceptions/ScapmsException.h"
#include "scapms/gui/AppointmentPanel.h"
#include "scapms/gui/PatientPanel.h"
#include "scapms/gui/QueuePanel.h"
#include "scapms/gui/ReportsPanel.h"
#include "scapms/gui/SearchSortPanel.h"
#include "scapms/persistence/FileHandler.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QFrame>
#include <QKeySequence>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace scapms {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), dashboardLabel_(new QLabel(this)),
      patientPanel_(new PatientPanel(manager_, this)),
      appointmentPanel_(new AppointmentPanel(manager_, this)),
      queuePanel_(new QueuePanel(manager_, this)),
      searchSortPanel_(new SearchSortPanel(manager_, this)),
      reportsPanel_(new ReportsPanel(manager_, this)) {
    auto* tabs = new QTabWidget(this);
    auto* dashboard = new QWidget(this);
    dashboard->setObjectName("dashboard");
    auto* dashboardLayout = new QVBoxLayout(dashboard);
    dashboardLayout->setContentsMargins(26, 24, 26, 24);
    dashboardLayout->setSpacing(18);
    dashboardLabel_->setObjectName("dashboardHero");
    dashboardLabel_->setTextFormat(Qt::RichText);
    dashboardLabel_->setWordWrap(true);
    dashboardLabel_->setMinimumHeight(155);
    dashboardLayout->addWidget(dashboardLabel_);
    tabs->addTab(dashboard, "Dashboard");
    tabs->addTab(patientPanel_, "Patient Records");
    tabs->addTab(appointmentPanel_, "Appointments");
    tabs->addTab(queuePanel_, "Waiting Queue");
    tabs->addTab(searchSortPanel_, "Search & Sort");
    tabs->addTab(reportsPanel_, "Reports");
    setCentralWidget(tabs);
    setWindowTitle("Smart Clinic Appointment & Patient Management System");
    createMenus();
    statusBar()->showMessage("Ready");
    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::refreshDashboard);
    timer->start(30000);
    refreshDashboard();
    resize(1000, 700);
    QFile theme(":/scapms.qss");
    if (theme.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(QString::fromUtf8(theme.readAll()));
    }
}

void MainWindow::createMenus() {
    auto* fileMenu = menuBar()->addMenu("File");
    auto* save = fileMenu->addAction("Save");
    save->setShortcut(QKeySequence::Save);
    auto* saveAs = fileMenu->addAction("Save Data As...");
    auto* load = fileMenu->addAction("Load Data...");
    load->setShortcut(QKeySequence::Open);
    fileMenu->addSeparator();
    auto* exit = fileMenu->addAction("Exit");
    exit->setShortcut(QKeySequence::Quit);
    auto* editMenu = menuBar()->addMenu("Edit");
    auto* undo = editMenu->addAction("Undo Last Administrative Operation");
    undo->setShortcut(QKeySequence::Undo);
    auto* focusSearch = editMenu->addAction("Focus Search");
    focusSearch->setShortcut(QKeySequence("Ctrl+F"));
    connect(save, &QAction::triggered, this, &MainWindow::saveData);
    connect(saveAs, &QAction::triggered, this, &MainWindow::saveDataAs);
    connect(load, &QAction::triggered, this, &MainWindow::loadData);
    connect(exit, &QAction::triggered, qApp, &QApplication::quit);
    connect(undo, &QAction::triggered, this, &MainWindow::undoLastOperation);
    connect(focusSearch, &QAction::triggered, searchSortPanel_, &SearchSortPanel::focusSearch);
}

void MainWindow::saveDataAs() {
    const auto patientsPath = QFileDialog::getSaveFileName(
        this, "Save patient data", QString(), "CSV files (*.csv);;All files (*)");
    if (patientsPath.isEmpty()) return;
    const auto appointmentsPath = QFileDialog::getSaveFileName(
        this, "Save appointment data", QString(), "CSV files (*.csv);;All files (*)");
    if (appointmentsPath.isEmpty()) return;
    try {
        FileHandler::save(manager_.database(), patientsPath.toStdString(),
                          appointmentsPath.toStdString());
        statusBar()->showMessage(QString("Saved patients to %1 and appointments to %2")
                                     .arg(patientsPath, appointmentsPath), 8000);
    } catch (const ScapmsException& error) {
        QMessageBox::critical(this, "Save failed", QString::fromStdString(error.what()));
    }
}

void MainWindow::saveData() {
    try {
        FileHandler::save(manager_.database(), "data/patients.csv", "data/appointments.csv");
        statusBar()->showMessage("Data saved");
    } catch (const ScapmsException& error) {
        QMessageBox::critical(this, "Save failed", QString::fromStdString(error.what()));
    }
}

void MainWindow::loadData() {
    const auto patientsPath = QFileDialog::getOpenFileName(
        this, "Load patient data", QString(), "CSV files (*.csv);;All files (*)");
    if (patientsPath.isEmpty()) return;
    const auto appointmentsPath = QFileDialog::getOpenFileName(
        this, "Load appointment data", QString(), "CSV files (*.csv);;All files (*)");
    if (appointmentsPath.isEmpty()) return;
    try {
        FileHandler::load(manager_.database(), patientsPath.toStdString(),
                          appointmentsPath.toStdString());
        refreshAllPanels();
        statusBar()->showMessage(QString("Loaded data from %1 and %2")
                                     .arg(patientsPath, appointmentsPath), 8000);
    } catch (const ScapmsException& error) {
        QMessageBox::critical(this, "Load failed", QString::fromStdString(error.what()));
    }
}

void MainWindow::undoLastOperation() {
    try {
        statusBar()->showMessage(QString::fromStdString(manager_.undo().undo()));
        refreshAllPanels();
    } catch (const std::exception& error) {
        QMessageBox::information(this, "Undo", QString::fromStdString(error.what()));
    }
}

void MainWindow::refreshDashboard() {
    int pending = 0;
    for (const auto& appointment : manager_.database().appointments())
        if (appointment.getStatus() == AppointmentStatus::Pending) ++pending;
    dashboardLabel_->setText(QString(
        "<div style='font-size:25px; font-weight:700;'>Smart Clinic operations</div>"
        "<div style='font-size:14px; color:#d5e8f7; margin-top:6px;'>"
        "A clear view of today’s patient flow, appointments, and care operations.</div>"
        "<table cellspacing='0' cellpadding='0' style='margin-top:22px; width:100%;'>"
        "<tr><td style='padding-right:44px;'><span style='font-size:28px; font-weight:700;'>%1</span><br>"
        "<span style='color:#b9d4e8;'>Total patients</span></td>"
        "<td style='padding-right:44px;'><span style='font-size:28px; font-weight:700;'>%2</span><br>"
        "<span style='color:#b9d4e8;'>Appointments</span></td>"
        "<td style='padding-right:44px;'><span style='font-size:28px; font-weight:700;'>%3</span><br>"
        "<span style='color:#b9d4e8;'>Pending</span></td>"
        "<td><span style='font-size:28px; font-weight:700;'>%4</span><br>"
        "<span style='color:#b9d4e8;'>Waiting now</span></td></tr></table></div>")
        .arg(static_cast<int>(manager_.database().patients().size()))
        .arg(static_cast<int>(manager_.database().appointments().size()))
        .arg(pending)
        .arg(static_cast<int>(manager_.database().waitingQueue().size())));
}

void MainWindow::refreshAllPanels() {
    patientPanel_->refresh();
    appointmentPanel_->refresh();
    queuePanel_->refresh();
    refreshDashboard();
}

} // namespace scapms
