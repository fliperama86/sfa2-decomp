## The guest image's control file is registered in the port's table (2026-10-12)

The review of the guest-image pull request found that `test_guestlink.py` had no
line in `tools/ai_workflow/port_subjects.toml`, so every change of the port selected
it (an unrelated edit of `port/src/xa.c` among them). It now has an empty line: it is
about itself and the files it imports (`guestlink.py`, `hostbuild.py`), read from its
text. A change of `xa.c` no longer selects it; a change of either tool does. Checked by
asking `port_controls` for both.
