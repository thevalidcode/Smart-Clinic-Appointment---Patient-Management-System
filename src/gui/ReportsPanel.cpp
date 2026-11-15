#include "scapms/gui/ReportsPanel.h"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>

namespace scapms {

ReportsPanel::ReportsPanel(ClinicManager& manager, QWidget* parent)
    : QWidget(parent), manager_(manager), reportCombo_(new QComboBox(this)),
      table_(new QTableWidget(this)), summary_(new QTextEdit(this)) {
    reportCombo_->addItems({"Most Frequently Attended", "Pending Appointments",
                            "Category Distribution", "Daily Appointment Summary"});
    auto* generateButton = new QPushButton("Generate Report", this);
    table_->setColumnCount(3);
    table_->horizontalHeader()->setStretchLastSection(true);
    summary_->setReadOnly(true);
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(reportCombo_);
    layout->addWidget(generateButton);
    layout->addWidget(table_);
    layout->addWidget(summary_);
    connect(generateButton, &QPushButton::clicked, this, &ReportsPanel::generateReport);
}

void ReportsPanel::generateReport() {
    table_->clearContents();
    summary_->clear();
    const auto report = manager_.reports();
    if (reportCombo_->currentIndex() == 0) {
        table_->setColumnCount(3);
        table_->setHorizontalHeaderLabels({"ID", "Name", "Completed"});
        const auto rows = report.mostFrequentlyAttended();
        table_->setRowCount(static_cast<int>(rows.size()));
        for (int row = 0; row < table_->rowCount(); ++row) {
            table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(rows[row].patientId)));
            table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rows[row].patientName)));
            table_->setItem(row, 2, new QTableWidgetItem(QString::number(rows[row].completedAppointments)));
        }
    } else if (reportCombo_->currentIndex() == 1) {
        table_->setColumnCount(4);
        table_->setHorizontalHeaderLabels({"ID", "Patient", "Date", "Status"});
        const auto rows = report.pendingAppointments();
        table_->setRowCount(static_cast<int>(rows.size()));
        for (int row = 0; row < table_->rowCount(); ++row) {
            table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(rows[row]->getAppointmentId())));
            table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rows[row]->getPatientId())));
            table_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(rows[row]->getDate())));
            table_->setItem(row, 3, new QTableWidgetItem("Pending"));
        }
    } else if (reportCombo_->currentIndex() == 2) {
        table_->setColumnCount(2);
        table_->setHorizontalHeaderLabels({"Category", "Count"});
        const auto rows = report.categoryDistribution();
        table_->setRowCount(static_cast<int>(rows.size()));
        int row = 0;
        for (const auto& [category, count] : rows) {
            table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(toString(category))));
            table_->setItem(row, 1, new QTableWidgetItem(QString::number(count)));
            ++row;
        }
    } else {
        const auto date = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
        const auto row = report.dailySummary(date);
        summary_->setText(QString("Date: %1\nTotal: %2\nCompleted: %3\nPending: %4\nCancelled: %5")
                              .arg(QString::fromStdString(date))
                              .arg(row.total).arg(row.completed).arg(row.pending).arg(row.cancelled));
    }
}

} // namespace scapms
