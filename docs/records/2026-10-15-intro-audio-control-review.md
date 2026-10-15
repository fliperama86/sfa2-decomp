## Native audio selector linker portability (2026-10-15)

The owner requested changes on PR371 at `bc6c3c15`: the new selector control
unconditionally passed GNU `--gc-sections`, which Apple's linker rejects.
The reviewer reproduced the compile/link failure on Apple clang without
running a game. This was a test portability bug, not a runtime change.

`test_intro_audio_host.py` now selects `-Wl,-dead_strip` on Darwin and
`-Wl,--gc-sections` elsewhere. The synthetic selector, native CD source,
assertions and all other controls are unchanged. Project memory records the
platform-linker lesson.

Only the requested selector control ran on this Linux worker:

```sh
python port/tools/test_intro_audio_host.py HostAudioControls.test_stream_selector
```

It compiled, linked and ran the synthetic fixture successfully: one test,
0.618 seconds. A separate command-only probe called the same control with
`sys.platform` set to Darwin and subprocess execution mocked; it confirmed
Apple's flag replaces GNU's flag. That probe is not Apple compile/runtime
evidence. This worker has no available native Apple toolchain, so no native
Mac run is claimed. The reviewer can run exactly the same single-case command
on the Mac; no wider suite is needed.

Private logs are `pr371-selector-linux.log` and
`pr371-selector-darwin-command.log` in the existing intro evidence folder.
No port/library rebuild, game launch, unrelated control, or prior evidence
rerun occurred. The previously validated executable and imported pack are unchanged.
