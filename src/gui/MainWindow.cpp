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
#include <QKeySequence>
#include <QLabel>
#include <QMessageBox>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>

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
    auto* dashboardLayout = new QVBoxLayout(dashboard);
    dashboardLayout->addWidget(dashboardLabel_);
    tabs->addTab(dashboard, "Dashboard");
    tabs->addTab(patientPanel_, "Patient Records");
    tabs->addTab(appointmentPanel_, "Appointments");
    tabs->addTab(queuePanel_, "Waiting Queue");
    tabs->addTab(searchSortPanel_, "Search & Sort");
    tabs->addTab(reportsPanel_, "Reports");
    setCentralWidget(tabs);
    createMenus();
    statusBar()->showMessage("Ready");
    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::refreshDashboard);
    timer->start(30000);
    refreshDashboard();
    resize(1000, 700);
}

void MainWindow::createMenus() {
    auto* fileMenu = menuBar()->addMenu("File");
    auto* save = fileMenu->addAction("Save");
    save->setShortcut(QKeySequence::Save);
    auto* load = fileMenu->addAction("Load");
    load->setShortcut(QKeySequence::Open);
    fileMenu->addSeparator();
    auto* exit = fileMenu->addAction("Exit");
    exit->setShortcut(QKeySequence::Quit);
    auto* editMenu = menuBar()->addMenu("Edit");
    auto* undo = editMenu->addAction("Undo Last Administrative Operation");
    undo->setShortcut(QKeySequence::Undo);
    connect(save, &QAction::triggered, this, &MainWindow::saveData);
    connect(load, &QAction::triggered, this, &MainWindow::loadData);
    connect(exit, &QAction::triggered, qApp, &QApplication::quit);
    connect(undo, &QAction::triggered, this, &MainWindow::undoLastOperation);
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
    try {
        FileHandler::load(manager_.database(), "data/patients.csv", "data/appointments.csv");
        refreshAllPanels();
        statusBar()->showMessage("Data loaded");
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
    dashboardLabel_->setText(QString("Patients: %1\nAppointments: %2\nWaiting: %3")
        .arg(static_cast<int>(manager_.database().patients().size()))
        .arg(static_cast<int>(manager_.database().appointments().size()))
        .arg(static_cast<int>(manager_.database().waitingQueue().size())));
}

void MainWindow::refreshAllPanels() {
    patientPanel_->refresh();
    appointmentPanel_->refresh();
    queuePanel_->refresh();
    refreshDashboard();
}

} // namespace scapms
