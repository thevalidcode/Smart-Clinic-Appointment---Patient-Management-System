#pragma once

#include "scapms/controller/ClinicManager.h"

#include <QWidget>

class QComboBox;
class QLineEdit;
class QTableWidget;

namespace scapms {

class SearchSortPanel final : public QWidget {
    Q_OBJECT
public:
    explicit SearchSortPanel(ClinicManager& manager, QWidget* parent = nullptr);

private slots:
    void search();
    void sort();

private:
    void showPatients(const std::vector<PatientRecord*>& patients);

    ClinicManager& manager_;
    QLineEdit* keywordEdit_;
    QComboBox* searchAlgorithmCombo_;
    QComboBox* sortAlgorithmCombo_;
    QTableWidget* table_;
};

} // namespace scapms
