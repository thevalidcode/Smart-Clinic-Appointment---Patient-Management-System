#pragma once

#include "scapms/controller/ClinicManager.h"

#include <QWidget>

class QComboBox;
class QLabel;
class QLineEdit;
class QTableWidget;

namespace scapms {

class SearchSortPanel final : public QWidget {
    Q_OBJECT
public:
    explicit SearchSortPanel(ClinicManager& manager, QWidget* parent = nullptr);

public slots:
    void focusSearch();

private slots:
    void search();
    void clearResults();
    void sort();
    void updateCategoryState();

private:
    void showPatients(const std::vector<PatientRecord*>& patients);

    ClinicManager& manager_;
    QLineEdit* keywordEdit_;
    QComboBox* searchAlgorithmCombo_;
    QComboBox* searchFieldCombo_;
    QComboBox* categoryCombo_;
    QComboBox* sortAlgorithmCombo_;
    QTableWidget* table_;
};

} // namespace scapms
