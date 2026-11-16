#include "scapms/gui/QueuePanel.h"

#include "scapms/exceptions/ScapmsException.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QStatusBar>

namespace scapms {

QueuePanel::QueuePanel(ClinicManager& manager, QWidget* parent)
    : QWidget(parent), manager_(manager), patientCombo_(new QComboBox(this)),
      queueList_(new QListWidget(this)) {
    auto* addButton = new QPushButton("Add to Queue", this);
    auto* nextButton = new QPushButton("Call Next", this);
    auto* controls = new QHBoxLayout;
    controls->addWidget(patientCombo_);
    controls->addWidget(addButton);
    controls->addWidget(nextButton);
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(14);
    auto* title = new QLabel("Waiting queue", this);
    title->setObjectName("sectionTitle");
    layout->addWidget(title);
    auto* helper = new QLabel("Manage the FIFO patient flow and call the next patient when ready.", this);
    helper->setObjectName("helperText");
    layout->addWidget(helper);
    layout->addLayout(controls);
    auto* queueTitle = new QLabel("Patients currently waiting", this);
    queueTitle->setObjectName("helperText");
    layout->addWidget(queueTitle);
    layout->addWidget(queueList_);
    connect(addButton, &QPushButton::clicked, this, &QueuePanel::addSelectedPatient);
    connect(nextButton, &QPushButton::clicked, this, &QueuePanel::callNextPatient);
    refresh();
}

void QueuePanel::refresh() {
    patientCombo_->clear();
    for (const auto& patient : manager_.database().patients()) {
        patientCombo_->addItem(QString::fromStdString(patient->getFullName()),
                               QString::fromStdString(patient->getPatientId()));
    }
    queueList_->clear();
    for (const auto& patientId : manager_.queue().getWaitingPatients()) {
        queueList_->addItem(QString::fromStdString(patientId));
    }
}

void QueuePanel::addSelectedPatient() {
    if (patientCombo_->currentIndex() < 0) return;
    try {
        manager_.queue().addPatient(patientCombo_->currentData().toString().toStdString());
        refresh();
        if (auto* window = qobject_cast<QMainWindow*>(this->window()))
            window->statusBar()->showMessage("Patient added to waiting queue", 5000);
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to update queue", QString::fromStdString(error.what()));
    }
}

void QueuePanel::callNextPatient() {
    try {
        const auto patientId = manager_.queue().callNextPatient();
        QMessageBox::information(this, "Next patient",
                                 QString::fromStdString("Call patient " + patientId));
        refresh();
    } catch (const std::exception& error) {
        QMessageBox::warning(this, "Unable to call patient", QString::fromStdString(error.what()));
    }
}

} // namespace scapms
