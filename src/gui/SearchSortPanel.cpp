#include "scapms/gui/SearchSortPanel.h"

#include "scapms/model/PatientCategory.h"

#include <QComboBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace scapms {

SearchSortPanel::SearchSortPanel(ClinicManager& manager, QWidget* parent)
    : QWidget(parent), manager_(manager), keywordEdit_(new QLineEdit(this)),
      searchAlgorithmCombo_(new QComboBox(this)), searchFieldCombo_(new QComboBox(this)),
      categoryCombo_(new QComboBox(this)), sortAlgorithmCombo_(new QComboBox(this)),
      table_(new QTableWidget(this)) {
    searchFieldCombo_->addItems({"Patient ID", "Patient Name", "Category"});
    searchAlgorithmCombo_->addItems({"Linear Search", "Binary Search", "Recursive Search"});
    categoryCombo_->addItems({"General", "Emergency", "Follow-Up"});
    sortAlgorithmCombo_->addItems({"Selection Sort by Name", "Insertion Sort by ID"});
    categoryCombo_->setEnabled(false);
    table_->setColumnCount(4);
    table_->setHorizontalHeaderLabels({"ID", "Name", "Age", "Category"});
    table_->horizontalHeader()->setStretchLastSection(true);

    auto* grid = new QGridLayout;
    grid->addWidget(new QLabel("Search field", this), 0, 0);
    grid->addWidget(searchFieldCombo_, 0, 1);
    grid->addWidget(new QLabel("Keyword", this), 0, 2);
    grid->addWidget(keywordEdit_, 0, 3);
    grid->addWidget(new QLabel("Algorithm", this), 1, 0);
    grid->addWidget(searchAlgorithmCombo_, 1, 1);
    grid->addWidget(categoryCombo_, 1, 2);
    auto* searchButton = new QPushButton("Search", this);
    auto* clearButton = new QPushButton("Clear Results", this);
    grid->addWidget(searchButton, 1, 3);
    grid->addWidget(clearButton, 1, 4);
    grid->addWidget(new QLabel("Sort", this), 2, 0);
    grid->addWidget(sortAlgorithmCombo_, 2, 1);
    auto* sortButton = new QPushButton("Sort", this);
    grid->addWidget(sortButton, 2, 3);
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(14);
    auto* title = new QLabel("Search and sort", this);
    title->setObjectName("sectionTitle");
    layout->addWidget(title);
    auto* helper = new QLabel("Find records using a visible manual algorithm, then sort the result set for review.", this);
    helper->setObjectName("helperText");
    layout->addWidget(helper);
    layout->addLayout(grid);
    layout->addWidget(table_);
    connect(searchFieldCombo_, &QComboBox::currentIndexChanged, this,
            &SearchSortPanel::updateCategoryState);
    connect(searchButton, &QPushButton::clicked, this, &SearchSortPanel::search);
    connect(clearButton, &QPushButton::clicked, this, &SearchSortPanel::clearResults);
    connect(sortButton, &QPushButton::clicked, this, &SearchSortPanel::sort);
}

void SearchSortPanel::focusSearch() {
    keywordEdit_->setFocus();
    keywordEdit_->selectAll();
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

void SearchSortPanel::updateCategoryState() {
    categoryCombo_->setEnabled(searchFieldCombo_->currentIndex() == 2);
}

void SearchSortPanel::search() {
    if (keywordEdit_->text().trimmed().isEmpty() && searchFieldCombo_->currentIndex() != 2) {
        QMessageBox::information(this, "Search", "Enter a search value.");
        return;
    }
    const auto keyword = keywordEdit_->text().trimmed().toStdString();
    std::vector<PatientRecord*> results;
    if (searchFieldCombo_->currentIndex() == 2) {
        results = manager_.search().searchByCategory(manager_.database().patients(),
            static_cast<PatientCategory>(categoryCombo_->currentIndex()));
    } else if (searchFieldCombo_->currentIndex() == 0) {
        if (searchAlgorithmCombo_->currentIndex() != 0) {
            QMessageBox::information(this, "Search", "Patient ID uses Linear Search.");
            return;
        }
        if (auto* patient = manager_.search().linearSearchById(manager_.database().patients(), keyword))
            results.push_back(patient);
    } else if (searchAlgorithmCombo_->currentIndex() == 0) {
        for (const auto& patient : manager_.database().patients())
            if (patient->getFullName().find(keyword) != std::string::npos) results.push_back(patient.get());
    } else if (searchAlgorithmCombo_->currentIndex() == 1) {
        manager_.sort().selectionSortByName(manager_.database().patients());
        if (auto* patient = manager_.search().binarySearchByName(manager_.database().patients(), keyword))
            results.push_back(patient);
    } else {
        manager_.search().recursiveSearchByName(manager_.database().patients(), keyword, 0, results);
    }
    showPatients(results);
}

void SearchSortPanel::clearResults() {
    keywordEdit_->clear();
    table_->setRowCount(0);
}

void SearchSortPanel::sort() {
    if (sortAlgorithmCombo_->currentIndex() == 0)
        manager_.sort().selectionSortByName(manager_.database().patients());
    else
        manager_.sort().insertionSortById(manager_.database().patients());
    std::vector<PatientRecord*> results;
    for (const auto& patient : manager_.database().patients()) results.push_back(patient.get());
    showPatients(results);
}

} // namespace scapms
