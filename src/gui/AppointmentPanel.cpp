#include "scapms/gui/AppointmentPanel.h"

#include "scapms/exceptions/ScapmsException.h"

#include <QComboBox>
#include <QColor>
#include <QDateEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMainWindow>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStatusBar>
#include <QAbstractItemView>
#include <QTimeEdit>
#include <QVBoxLayout>

namespace scapms {

AppointmentPanel::AppointmentPanel(ClinicManager& manager, QWidget* parent)
    : QWidget(parent), manager_(manager), patientCombo_(new QComboBox(this)),
      dateEdit_(new QDateEdit(QDate::currentDate(), this)),
      timeEdit_(new QTimeEdit(QTime::currentTime(), this)),
      doctorEdit_(new QLineEdit(this)), purposeEdit_(new QLineEdit(this)),
      table_(new QTableWidget(this)) {
    table_->setColumnCount(7);
    table_->setHorizontalHeaderLabels({"ID", "Patient", "Date", "Time", "Doctor", "Purpose", "Status"});
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    dateEdit_->setCalendarPopup(true);
    auto* form = new QFormLayout;
    form->addRow("Patient", patientCombo_);
    form->addRow("Date", dateEdit_);
    form->addRow("Time", timeEdit_);
    form->addRow("Doctor", doctorEdit_);
    form->addRow("Purpose", purposeEdit_);
    auto* scheduleButton = new QPushButton("Schedule", this);
    auto* completeButton = new QPushButton("Mark Completed", this);
    auto* cancelButton = new QPushButton("Cancel Selected", this);
    auto* buttons = new QHBoxLayout;
    buttons->addWidget(scheduleButton);
    buttons->addWidget(completeButton);
    buttons->addWidget(cancelButton);
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(14);
    auto* title = new QLabel("Appointment management", this);
    title->setObjectName("sectionTitle");
    layout->addWidget(title);
    auto* helper = new QLabel("Schedule and manage the clinic's daily appointment flow.", this);
    helper->setObjectName("helperText");
    layout->addWidget(helper);
    layout->addLayout(form);
    layout->addLayout(buttons);
    layout->addWidget(table_);
    connect(scheduleButton, &QPushButton::clicked, this, &AppointmentPanel::scheduleAppointment);
    connect(completeButton, &QPushButton::clicked, this, &AppointmentPanel::completeAppointment);
    connect(cancelButton, &QPushButton::clicked, this, &AppointmentPanel::cancelAppointment);
    refresh();
}

void AppointmentPanel::refresh() {
    patientCombo_->clear();
    for (const auto& patient : manager_.database().patients()) {
        patientCombo_->addItem(QString::fromStdString(patient->getFullName()),
                               QString::fromStdString(patient->getPatientId()));
    }
    table_->setRowCount(0);
    for (const auto& appointment : manager_.database().appointments()) {
        const int row = table_->rowCount();
        table_->insertRow(row);
        table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(appointment.getAppointmentId())));
        table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(appointment.getPatientId())));
        table_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(appointment.getDate())));
        table_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(appointment.getTime())));
        table_->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(appointment.getDoctorName())));
        table_->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(appointment.getPurpose())));
        table_->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(toString(appointment.getStatus()))));
        const QColor colour = appointment.getStatus() == AppointmentStatus::Completed
            ? QColor("#dff5e1") : appointment.getStatus() == AppointmentStatus::Cancelled
            ? QColor("#eeeeee") : QColor("#fff0d6");
        for (int column = 0; column < table_->columnCount(); ++column) table_->item(row, column)->setBackground(colour);
    }
}

void AppointmentPanel::scheduleAppointment() {
    if (patientCombo_->currentIndex() < 0) return;
    try {
        const auto patientId = patientCombo_->currentData().toString().toStdString();
        std::size_t nextId = manager_.database().appointments().size() + 1;
        while (manager_.database().findAppointment("A" + std::to_string(nextId)) != nullptr) ++nextId;
        const auto appointmentId = "A" + std::to_string(nextId);
        manager_.appointments().schedule(Appointment(
            appointmentId, patientId, dateEdit_->date().toString("yyyy-MM-dd").toStdString(),
            timeEdit_->time().toString("HH:mm").toStdString(), doctorEdit_->text().toStdString(),
            purposeEdit_->text().toStdString()));
        refresh();
        if (auto* window = qobject_cast<QMainWindow*>(this->window()))
            window->statusBar()->showMessage("Appointment scheduled", 5000);
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to schedule appointment",
                             QString::fromStdString(error.what()));
    }
}

void AppointmentPanel::cancelAppointment() {
    const int row = table_->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Cancel appointment", "Select an appointment first.");
        return;
    }
    try {
        manager_.appointments().cancel(table_->item(row, 0)->text().toStdString());
        refresh();
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to cancel appointment",
                             QString::fromStdString(error.what()));
    }
}

void AppointmentPanel::completeAppointment() {
    const int row = table_->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Complete appointment", "Select an appointment first.");
        return;
    }
    try {
        manager_.appointments().complete(table_->item(row, 0)->text().toStdString());
        refresh();
        if (auto* window = qobject_cast<QMainWindow*>(this->window()))
            window->statusBar()->showMessage("Appointment marked completed", 5000);
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to complete appointment",
                             QString::fromStdString(error.what()));
    }
}

} // namespace scapms
