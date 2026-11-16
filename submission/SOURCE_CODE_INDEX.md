# Source-code package

## Header files

All public interfaces are under `include/scapms/`, including model classes,
controllers, exception classes, persistence interfaces, utilities, and GUI
panel declarations.

## Source files

All implementations are under `src/model/`, `src/controller/`,
`src/exceptions/`, `src/persistence/`, and `src/utils/`.

## GUI files

GUI implementations are under `src/gui/` and declarations are under
`include/scapms/gui/`. The application entry point is `src/main.cpp`.

## Data files

The CSV examples are `data/patients.csv` and `data/appointments.csv`.

## Tests and build

Automated tests are in `tests/ScapmsTests.cpp`. The build definition is
`CMakeLists.txt`. The project is built as C++17 with Qt6 Widgets when Qt6 is
available.
