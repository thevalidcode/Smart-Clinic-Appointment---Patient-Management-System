#include "scapms/gui/SearchSortPanel.h"

#include "scapms/exceptions/ScapmsException.h"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>

namespace scapms {

SearchSortPanel::SearchSortPanel(ClinicManager& manager, QWidget* parent)
    : QWidget(parent), manager_(manager), keywordEdit_(new QLineEdit(this)),
      searchAlgorithmCombo_(new QComboBox(this)), sortAlgorithmCombo_(new QComboBox(this)),
      table_(new QTableWidget(this)) {
    searchAlgorithmCombo_->addItems({"Linear Search", "Binary Search", "Recursive Search"});
    sortAlgorithmCombo_->addItems({"Selection Sort by Name", "Insertion Sort by ID"});
    table_->setColumnCount(4);
    table_->setHorizontalHeaderLabels({"ID", "Name", "Age", "Category"});
    table_->horizontalHeader()->setStretchLastSection(true);
    auto* grid = new QGridLayout;
    grid->addWidget(new QLabel("Keyword", this), 0, 0);
    grid->addWidget(keywordEdit_, 0, 1);
    grid->addWidget(searchAlgorithmCombo_, 0, 2);
    auto* searchButton = new QPushButton("Search", this);
    grid->addWidget(searchButton, 0, 3);
    grid->addWidget(sortAlgorithmCombo_, 1, 2);
    auto* sortButton = new QPushButton("Sort", this);
    grid->addWidget(sortButton, 1, 3);
    auto* layout = new QVBoxLayout(this);
    layout->addLayout(grid);
    layout->addWidget(table_);
    connect(searchButton, &QPushButton::clicked, this, &SearchSortPanel::search);
    connect(sortButton, &QPushButton::clicked, this, &SearchSortPanel::sort);
}

void SearchSortPanel::showPatients(const std::vector<PatientRecord*>& patients) {
    table_->setRowCount(0);
    for (const auto* patient : patients) {
        const int row = table_->rowCount();
        table_->insertRow(row);
        table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(patient->getPatientId())));
        table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(patient->getFullName())));
        table_->setItem(row, 2, new QTableWidgetItem(QString::number(patient->getAge())));
        table_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(toString(patient->getCategory()))));
    }
}

void SearchSortPanel::search() {
    const auto keyword = keywordEdit_->text().toStdString();
    std::vector<PatientRecord*> results;
    if (searchAlgorithmCombo_->currentIndex() == 0) {
        if (auto* patient = manager_.search().linearSearchById(manager_.database().patients(), keyword)) {
            results.push_back(patient);
        }
    } else if (searchAlgorithmCombo_->currentIndex() == 1) {
        manager_.sort().selectionSortByName(manager_.database().patients());
        if (auto* patient = manager_.search().binarySearchByName(manager_.database().patients(), keyword)) {
            results.push_back(patient);
        }
    } else {
        manager_.search().recursiveSearchByName(manager_.database().patients(), keyword, 0, results);
    }
    showPatients(results);
}

void SearchSortPanel::sort() {
    if (sortAlgorithmCombo_->currentIndex() == 0) {
        manager_.sort().selectionSortByName(manager_.database().patients());
    } else {
        manager_.sort().insertionSortById(manager_.database().patients());
    }
    std::vector<PatientRecord*> results;
    for (const auto& patient : manager_.database().patients()) results.push_back(patient.get());
    showPatients(results);
}

} // namespace scapms
