#include "scapms/gui/PatientPanel.h"

#include "scapms/exceptions/ScapmsException.h"
#include "scapms/model/EmergencyPatient.h"
#include "scapms/model/FollowUpPatient.h"
#include "scapms/model/GeneralPatient.h"

#include <QComboBox>
#include <QColor>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace scapms {

namespace {
void status(QWidget* widget, const QString& message) {
    if (auto* window = qobject_cast<QMainWindow*>(widget->window())) {
        window->statusBar()->showMessage(message, 5000);
    }
}
}

PatientPanel::PatientPanel(ClinicManager& manager, QWidget* parent)
    : QWidget(parent), manager_(manager),
      idEdit_(new QLineEdit(this)), nameEdit_(new QLineEdit(this)),
      ageEdit_(new QSpinBox(this)), contactEdit_(new QLineEdit(this)),
      typeCombo_(new QComboBox(this)), preferredDoctorEdit_(new QLineEdit(this)),
      emergencyDescriptionEdit_(new QLineEdit(this)), triageLevelEdit_(new QSpinBox(this)),
      previousAppointmentEdit_(new QLineEdit(this)), followUpReasonEdit_(new QLineEdit(this)),
      updateButton_(new QPushButton("Update Patient", this)), table_(new QTableWidget(this)) {
    typeCombo_->addItems({"General", "Emergency", "Follow-Up"});
    ageEdit_->setRange(0, 120);
    triageLevelEdit_->setRange(1, 5);
    table_->setColumnCount(6);
    table_->setHorizontalHeaderLabels({"ID", "Name", "Age", "Contact", "Category", "Appointments"});
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);

    auto* form = new QFormLayout;
    form->addRow("Patient ID", idEdit_);
    form->addRow("Name", nameEdit_);
    form->addRow("Age", ageEdit_);
    form->addRow("Contact", contactEdit_);
    form->addRow("Type", typeCombo_);
    form->addRow("Preferred doctor", preferredDoctorEdit_);
    form->addRow("Emergency description", emergencyDescriptionEdit_);
    form->addRow("Triage level", triageLevelEdit_);
    form->addRow("Previous appointment", previousAppointmentEdit_);
    form->addRow("Follow-up reason", followUpReasonEdit_);

    auto* addButton = new QPushButton("Add Patient", this);
    auto* editButton = new QPushButton("Edit Selected", this);
    auto* deleteButton = new QPushButton("Delete Selected", this);
    updateButton_->setEnabled(false);
    addButton->setToolTip("Register a new patient after validating all fields.");
    editButton->setToolTip("Load the selected patient record into the form.");
    updateButton_->setToolTip("Save changes to the patient currently in edit mode.");
    deleteButton->setToolTip("Delete the selected patient after confirmation.");
    auto* buttons = new QHBoxLayout;
    buttons->addWidget(addButton);
    buttons->addWidget(editButton);
    buttons->addWidget(updateButton_);
    buttons->addWidget(deleteButton);
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(14);
    auto* title = new QLabel("Patient records", this);
    title->setObjectName("sectionTitle");
    layout->addWidget(title);
    auto* helper = new QLabel("Maintain accurate patient details and category-specific clinical context.", this);
    helper->setObjectName("helperText");
    layout->addWidget(helper);
    layout->addLayout(form);
    layout->addLayout(buttons);
    layout->addWidget(table_);
    connect(addButton, &QPushButton::clicked, this, &PatientPanel::addPatient);
    connect(editButton, &QPushButton::clicked, this, &PatientPanel::loadSelectedPatient);
    connect(updateButton_, &QPushButton::clicked, this, &PatientPanel::updatePatient);
    connect(deleteButton, &QPushButton::clicked, this, &PatientPanel::deleteSelectedPatient);
    connect(typeCombo_, &QComboBox::currentTextChanged, this, &PatientPanel::updateDynamicFields);
    updateDynamicFields();
    refresh();
}

void PatientPanel::refresh() {
    table_->setRowCount(0);
    for (const auto& patient : manager_.database().patients()) {
        const int row = table_->rowCount();
        table_->insertRow(row);
        table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(patient->getPatientId())));
        table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(patient->getFullName())));
        table_->setItem(row, 2, new QTableWidgetItem(QString::number(patient->getAge())));
        table_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(patient->getContactInformation())));
        table_->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(toString(patient->getCategory()))));
        table_->setItem(row, 5, new QTableWidgetItem(QString::number(patient->getAppointments().size())));
        const QColor colour = patient->getCategory() == PatientCategory::Emergency
            ? QColor("#ffe0e0") : patient->getCategory() == PatientCategory::FollowUp
            ? QColor("#fff4cc") : QColor("#e6f0ff");
        for (int column = 0; column < table_->columnCount(); ++column) table_->item(row, column)->setBackground(colour);
    }
}

void PatientPanel::updateDynamicFields() {
    const auto type = typeCombo_->currentText();
    const bool general = type == "General";
    const bool emergency = type == "Emergency";
    preferredDoctorEdit_->setVisible(general);
    emergencyDescriptionEdit_->setVisible(emergency);
    triageLevelEdit_->setVisible(emergency);
    previousAppointmentEdit_->setVisible(!general && !emergency);
    followUpReasonEdit_->setVisible(!general && !emergency);
}

void PatientPanel::addPatient() {
    try {
        const auto id = idEdit_->text().trimmed().toStdString();
        const auto name = nameEdit_->text().trimmed().toStdString();
        const auto contact = contactEdit_->text().trimmed().toStdString();
        std::unique_ptr<PatientRecord> patient;
        if (typeCombo_->currentText() == "Emergency") {
            patient = std::make_unique<EmergencyPatient>(id, name, ageEdit_->value(), contact,
                emergencyDescriptionEdit_->text().toStdString(), triageLevelEdit_->value());
        } else if (typeCombo_->currentText() == "Follow-Up") {
            patient = std::make_unique<FollowUpPatient>(id, name, ageEdit_->value(), contact,
                previousAppointmentEdit_->text().toStdString(), followUpReasonEdit_->text().toStdString());
        } else {
            patient = std::make_unique<GeneralPatient>(id, name, ageEdit_->value(), contact,
                preferredDoctorEdit_->text().toStdString());
        }
        manager_.patients().registerPatient(std::move(patient));
        refresh();
        status(this, "Patient registered successfully");
        idEdit_->clear(); nameEdit_->clear(); contactEdit_->clear();
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to add patient", QString::fromStdString(error.what()));
    }
}

void PatientPanel::loadSelectedPatient() {
    const int row = table_->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Edit patient", "Select a patient record first.");
        return;
    }
    const auto id = table_->item(row, 0)->text().toStdString();
    auto* patient = manager_.database().findPatient(id);
    if (patient == nullptr) {
        QMessageBox::warning(this, "Edit patient", "The selected patient no longer exists.");
        return;
    }
    editingPatientId_ = QString::fromStdString(id);
    idEdit_->setText(editingPatientId_);
    idEdit_->setEnabled(false);
    nameEdit_->setText(QString::fromStdString(patient->getFullName()));
    ageEdit_->setValue(patient->getAge());
    contactEdit_->setText(QString::fromStdString(patient->getContactInformation()));
    typeCombo_->setCurrentText(QString::fromStdString(toString(patient->getCategory())));
    if (const auto* general = dynamic_cast<const GeneralPatient*>(patient))
        preferredDoctorEdit_->setText(QString::fromStdString(general->getPreferredDoctor()));
    if (const auto* emergency = dynamic_cast<const EmergencyPatient*>(patient)) {
        emergencyDescriptionEdit_->setText(QString::fromStdString(emergency->getEmergencyDescription()));
        triageLevelEdit_->setValue(emergency->getTriageLevel());
    }
    if (const auto* followUp = dynamic_cast<const FollowUpPatient*>(patient)) {
        previousAppointmentEdit_->setText(QString::fromStdString(followUp->getPreviousAppointmentId()));
        followUpReasonEdit_->setText(QString::fromStdString(followUp->getFollowUpReason()));
    }
    updateButton_->setEnabled(true);
    status(this, "Editing selected patient");
}

void PatientPanel::updatePatient() {
    if (editingPatientId_.isEmpty()) {
        QMessageBox::information(this, "Update patient", "Choose Edit Selected first.");
        return;
    }
    try {
        manager_.patients().editPatient(editingPatientId_.toStdString(),
            nameEdit_->text().trimmed().toStdString(), ageEdit_->value(),
            contactEdit_->text().trimmed().toStdString());
        refresh();
        idEdit_->setEnabled(true);
        updateButton_->setEnabled(false);
        editingPatientId_.clear();
        status(this, "Patient updated successfully");
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to update patient", QString::fromStdString(error.what()));
    }
}

void PatientPanel::deleteSelectedPatient() {
    const int row = table_->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Delete patient", "Select a patient record first.");
        return;
    }
    if (QMessageBox::question(this, "Delete patient", "Delete the selected patient?") != QMessageBox::Yes) return;
    try {
        manager_.patients().deletePatient(table_->item(row, 0)->text().toStdString());
        refresh();
        status(this, "Patient deleted");
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to delete patient", QString::fromStdString(error.what()));
    }
}

} // namespace scapms
