## Fast loads on by default (2026-10-12)

The owner played the port with `fast_loads` on and asked for it to be on by default. `port_settings_defaults` now sets it, and `port/settings.ini` ships
`fast_loads = true`. A settings file without the key, or no settings file, gets fast loading; `fast_loads = false` in the file or `--ps1-loading` on the
command line restores the console's waits. The setting stays under `[experimental]` and the other settings stay off by default. A build or test that does
not load settings (the zeroed `port_settings` of the control programs) still has it off.

Evidence: `test_settings.py` and `settings_test.c` (defaults, file without the key, explicit false, both overrides). Not run: the game with the default (the
owner plays it with the flag on). Unchanged: the documented uncertainty of the load overrides in `docs/records/2026-10-12-fast-disc-reads.md`.
