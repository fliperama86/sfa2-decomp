## Intro review control repair evidence (2026-10-14)

On exact repair commit ff2c4a1f, with the configured Windows cross compiler:

- Constructed the existing launch Rig and called
  `program_for('base', program(ENTRY_C))`: generated runtime fixture compiled
  warning-free and linked successfully, 2.613 seconds. No executable run.
- Constructed the existing GPU Rig and consumed only the first two yields of
  `program_cases`: the no-PsyZ compile and exact defined/undefined-symbol
  assertions passed, 0.205 seconds. No PsyZ build, GPU or executable run.

The two independent checks ran concurrently. Their wrapper forbade runtime
entry points and the later GPU build. A private report pins the repair commit,
source digests, timings and scope under the existing intro-asset-probe folder.
Pad/module fixtures inherit the same repaired STUBS; no unchanged suites were
rerun. The shared stub includes intro.h for compiler-checked API signatures,
returns the disabled behavior, and explicitly refuses enabled intro requests.

Only controls and documentation changed since the owner-tested feature.
The normal executable, private pack, first-scene controls and original-sequence
comparison evidence remain unchanged and were not rebuilt or rerun.
