# Manual GUI test record

The application was built successfully and the smoke path was executed.
Automated CTest covers model, controller, persistence, search, report, and
undo behavior.

The current Copilot execution environment does not expose a macOS display
session, so `screencapture` cannot capture the live Qt window. The application
is ready for the following short manual capture pass on a desktop session:

1. Dashboard and Patient Records tabs.
2. Add a General patient, then Edit Selected and Update Patient.
3. Schedule an appointment, mark it completed, and undo the completion.
4. Search by Category and sort by name/ID.
5. Add and call a waiting-queue patient.
6. Generate each report.
7. Open Save Data As and Load Data dialogs.
8. Trigger a validation error and demonstrate Undo.

The final PDF includes the report, UML diagram, and source-code package index;
this record documents the only evidence step blocked by the headless session.
