# Remaining submission requirements

The implementation requirements from the previous plan have been completed and
removed from this file. The project builds successfully and all configured
automated tests pass.

## Submission artifacts completed

- [x] Create the UML/class diagram covering patient inheritance,
      `Appointable`, `ClinicDatabase`, controllers, GUI relationships, and
      appointments.
- [x] Write the two-to-three-page project report covering features, data
      structures, algorithms, GUI/event handling, exceptions, persistence,
      undo, and challenges.
- [x] Package labelled header files, source files, GUI files, data files, and
      tests in the final submission.
- [x] Combine the source appendix, UML diagram, report, package index, and
      manual-test record into `submission/SCAPMS_FINAL_SUBMISSION.pdf`.

## Display-session evidence

- [ ] Run the live GUI walkthrough and capture screenshots for the dashboard,
      patient registration/editing, appointment scheduling/completion,
      search/category results, queue, reports, file dialogs, validation errors,
      and undo.

The current execution environment has no accessible macOS display session:
the Qt application builds and starts in a desktop session, but `screencapture`
returns “could not create image from display”. The exact walkthrough is
documented in `submission/MANUAL_GUI_TEST_RECORD.md` for completion on a
desktop session.

## Verification completed

- [x] Professional Qt/QSS visual system added with responsive layout spacing,
      polished navigation, form controls, tables, status states, tooltips,
      and a live dashboard hero.

- [x] Patient editing, validation, update, refresh, and undo are exposed in
      the GUI.
- [x] ID, name, and category searching are exposed with linear, binary, and
      recursive choices where applicable.
- [x] Appointment completion, cancellation, status colouring, refresh, and
      undo are exposed in the GUI.
- [x] Save Data As and Load Data use CSV file chooser dialogs with cancellation
      and error handling.
- [x] Dynamic patient fields, tooltips, status colours, dashboard metrics,
      separated search/sort controls, and keyboard shortcuts are implemented.
- [x] `cmake --build build --parallel 2` succeeds.
- [x] `ctest --test-dir build --output-on-failure` passes both tests.
