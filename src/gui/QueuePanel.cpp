#include "scapms/gui/QueuePanel.h"

#include "scapms/exceptions/ScapmsException.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

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
    layout->addLayout(controls);
    layout->addWidget(new QLabel("Waiting patients", this));
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
