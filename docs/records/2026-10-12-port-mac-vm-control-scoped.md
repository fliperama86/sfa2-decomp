## The VM control file is registered and runs only where it can (2026-10-12)

The review of the VM pull request found that `test_vm.py` had no line in
`tools/ai_workflow/port_subjects.toml`, so every change of the port selected it
(an edit of `port/src/xa.c` among them), and that on Windows and Linux it exits 3
because the Hypervisor framework is not there, which would break reviews of changes
that have nothing to do with it.

- `test_vm` has a line naming the machine's files (`port/mac/vm.c`, `vm.h`, `elf.c`,
  `elf.h`, `vm.entitlements`, `test/guest.c`, `test/vmtest.c`); the tools it imports
  are read from its text.
- `review.py` has `MAC_ONLY = ('test_vm',)`: such a control file is selected only by a
  changed file that its line names, never by the wide rules (a control file without a
  line, a changed file that no control file names), and a table that lacks the line of
  one is refused. Where the review is not on macOS with Apple Silicon and a change
  selects one, it stops with a line that says so: an unexecuted control file is neither
  a pass nor a silent skip.
- `test_vm.py` prints the passing line the review reads from the file's text, only when
  the host program said all passed and its count is the count of `ok` lines.
- `test_guestlink.py` (the guest image's control file) prints its passing line in the
  same readable form; the workflow's own control `test_workflow.py` has a case for the
  scoping, and its expectation for a changed file that nothing names leaves the Mac-only
  control files out.

Checked: the table's own cases and the new one pass (8 of 8); the whole workflow suite
shows the failures it showed before this change (the form of three older records, the
passing lines of `test_settings.py`, `test_ui_layout.py` and `test_video.py`, and
eleven errors of the program-file cases); nothing new. The VM suite passes (21 cases).
