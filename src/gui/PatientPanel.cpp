#include "scapms/gui/PatientPanel.h"

#include "scapms/exceptions/ScapmsException.h"
#include "scapms/model/EmergencyPatient.h"
#include "scapms/model/FollowUpPatient.h"
#include "scapms/model/GeneralPatient.h"

#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>

namespace scapms {

PatientPanel::PatientPanel(ClinicManager& manager, QWidget* parent)
    : QWidget(parent), manager_(manager),
      idEdit_(new QLineEdit(this)), nameEdit_(new QLineEdit(this)),
      ageEdit_(new QSpinBox(this)), contactEdit_(new QLineEdit(this)),
      typeCombo_(new QComboBox(this)), table_(new QTableWidget(this)) {
    typeCombo_->addItems({"General", "Emergency", "Follow-Up"});
    ageEdit_->setRange(0, 120);
    table_->setColumnCount(4);
    table_->setHorizontalHeaderLabels({"ID", "Name", "Age", "Category"});
    table_->horizontalHeader()->setStretchLastSection(true);

    auto* form = new QFormLayout;
    form->addRow("Patient ID", idEdit_);
    form->addRow("Name", nameEdit_);
    form->addRow("Age", ageEdit_);
    form->addRow("Contact", contactEdit_);
    form->addRow("Type", typeCombo_);
    auto* addButton = new QPushButton("Add Patient", this);
    auto* deleteButton = new QPushButton("Delete Selected", this);
    auto* buttons = new QHBoxLayout;
    buttons->addWidget(addButton);
    buttons->addWidget(deleteButton);
    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addLayout(buttons);
    layout->addWidget(table_);
    connect(addButton, &QPushButton::clicked, this, &PatientPanel::addPatient);
    connect(deleteButton, &QPushButton::clicked, this, &PatientPanel::deleteSelectedPatient);
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
        table_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(toString(patient->getCategory()))));
    }
}

void PatientPanel::addPatient() {
    try {
        std::unique_ptr<PatientRecord> patient;
        const auto id = idEdit_->text().toStdString();
        const auto name = nameEdit_->text().toStdString();
        const auto contact = contactEdit_->text().toStdString();
        const int age = ageEdit_->value();
        if (typeCombo_->currentText() == "Emergency") {
            patient = std::make_unique<EmergencyPatient>(id, name, age, contact);
        } else if (typeCombo_->currentText() == "Follow-Up") {
            patient = std::make_unique<FollowUpPatient>(id, name, age, contact);
        } else {
            patient = std::make_unique<GeneralPatient>(id, name, age, contact);
        }
        manager_.patients().registerPatient(std::move(patient));
        refresh();
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to add patient", QString::fromStdString(error.what()));
    }
}

void PatientPanel::deleteSelectedPatient() {
    const int row = table_->currentRow();
    if (row < 0) return;
    try {
        manager_.patients().deletePatient(table_->item(row, 0)->text().toStdString());
        refresh();
    } catch (const ScapmsException& error) {
        QMessageBox::warning(this, "Unable to delete patient", QString::fromStdString(error.what()));
    }
}

} // namespace scapms
