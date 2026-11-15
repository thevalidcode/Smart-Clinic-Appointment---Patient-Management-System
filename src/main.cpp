#include "scapms/controller/ClinicManager.h"
#include "scapms/model/EmergencyPatient.h"
#include "scapms/model/FollowUpPatient.h"
#include "scapms/model/GeneralPatient.h"

#include <iostream>
#include <memory>

#ifdef SCAPMS_WITH_QT
#include "scapms/gui/MainWindow.h"

#include <QApplication>
#include <QStyleFactory>
#endif

int main(int argc, char* argv[]) {
#ifdef SCAPMS_WITH_QT
    if (argc > 1 && std::string(argv[1]) == "--smoke") {
        scapms::ClinicManager manager;
        manager.patients().registerPatient(std::make_unique<scapms::GeneralPatient>(
            "P001", "Jane Doe", 34, "0712345678", "Dr Smith"));
        std::cout << manager.database().patients().front()->getDisplayDescription() << '\n';
        return 0;
    }
    // Avoid loading the GTK desktop theme, whose schema may be incomplete on
    // systems using a minimal or mixed desktop environment.
    qputenv("QT_QPA_PLATFORMTHEME", "generic");
    QApplication application(argc, argv);
    if (QStyleFactory::keys().contains("Fusion", Qt::CaseInsensitive)) {
        application.setStyle("Fusion");
    }
    scapms::MainWindow window;
    window.show();
    return application.exec();
#else
    scapms::ClinicManager manager;
    manager.patients().registerPatient(std::make_unique<scapms::GeneralPatient>(
        "P001", "Jane Doe", 34, "0712345678", "Dr Smith"));
    manager.patients().registerPatient(std::make_unique<scapms::EmergencyPatient>(
        "P002", "John Smith", 51, "0723456789", "Chest pain", 1));
    manager.patients().registerPatient(std::make_unique<scapms::FollowUpPatient>(
        "P003", "Mary Jones", 42, "0734567890", "A001", "Medication review"));

    for (const auto& patient : manager.database().patients()) {
        std::cout << patient->getPatientId() << ": "
                  << patient->getFullName() << " - "
                  << patient->getDisplayDescription() << '\n';
    }

    return 0;
#endif
}
