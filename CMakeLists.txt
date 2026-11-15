cmake_minimum_required(VERSION 3.16)

project(SCAPMS VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

add_library(scapms_model
    src/model/Appointment.cpp
    src/model/ClinicDatabase.cpp
    src/model/EmergencyPatient.cpp
    src/model/FollowUpPatient.cpp
    src/model/GeneralPatient.cpp
    src/model/PatientRecord.cpp
    src/model/WaitingQueue.cpp
    src/controller/PatientController.cpp
    src/controller/AppointmentController.cpp
    src/controller/QueueController.cpp
    src/controller/SearchEngine.cpp
    src/controller/SortEngine.cpp
    src/controller/UndoController.cpp
    src/controller/ReportController.cpp
    src/controller/ClinicManager.cpp
    src/persistence/FileHandler.cpp
    src/persistence/PatientCsvRepository.cpp
    src/persistence/AppointmentCsvRepository.cpp
    src/exceptions/ScapmsException.cpp
    src/exceptions/ValidationException.cpp
    src/exceptions/DuplicatePatientIdException.cpp
    src/exceptions/InvalidAgeException.cpp
    src/exceptions/InvalidAppointmentException.cpp
    src/exceptions/PatientNotFoundException.cpp
    src/exceptions/FileOperationException.cpp
    src/exceptions/SearchException.cpp
    src/utils/InputValidator.cpp
    src/utils/RecentPatientsCache.cpp
)

target_include_directories(scapms_model
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(CMAKE_AUTOMOC ON)
    add_executable(scapms
        src/main.cpp
        src/gui/MainWindow.cpp
        src/gui/PatientPanel.cpp
        src/gui/AppointmentPanel.cpp
        src/gui/QueuePanel.cpp
        src/gui/SearchSortPanel.cpp
        src/gui/ReportsPanel.cpp
        include/scapms/gui/MainWindow.h
        include/scapms/gui/PatientPanel.h
        include/scapms/gui/AppointmentPanel.h
        include/scapms/gui/QueuePanel.h
        include/scapms/gui/SearchSortPanel.h
        include/scapms/gui/ReportsPanel.h
    )
    target_compile_definitions(scapms PRIVATE SCAPMS_WITH_QT)
    target_link_libraries(scapms PRIVATE scapms_model Qt6::Widgets)
else()
    add_executable(scapms src/main.cpp)
    target_link_libraries(scapms PRIVATE scapms_model)
endif()

add_executable(scapms_tests tests/ScapmsTests.cpp)
target_link_libraries(scapms_tests PRIVATE scapms_model)

enable_testing()
add_test(NAME scapms_smoke_test COMMAND scapms --smoke)
add_test(NAME scapms_unit_tests COMMAND scapms_tests)
