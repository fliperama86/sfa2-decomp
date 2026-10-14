## PS1-first intro animation-table search (2026-10-14)

The owner prefers all intro code/data from PS1 and explicitly requested a
search for the animation table before adopting Saturn's implementation.
`port/intro/animation.py` makes the first native-format search reproducible.
This does not introduce a Saturn runtime input, player or setting, and nothing
under `ps1/` changed.

### Findings

The scanner read and hash-verified all 248 nonstream files in the supplied PS1
baseline inventory, 150,884,569 bytes. Twelve STR/XA files were excluded. It
searched every halfword-aligned file offset, not only known live functions.
Original file bytes were inspected without decompression or executing code.

The five sprite sequences referenced by the first Saturn opening scene were
normalized for PS1's endianness and record layout. Durations, flags and active
relative branches had to agree; artwork addresses and unused fields did not.

- No match for the main six-step loop with four updates per step.
- The four smaller particle schedules produced 23 variant matches at nine
  distinct file offsets. They map to Sodom, Rose and Dhalsim character data,
  and Chun-Li, Dan and Dhalsim stage data. The referenced drawings are larger
  multi-tile characters/grids, unlike the reference's single-tile particles.
  Equal timing is not evidence that these are the intro tables.
- A separate RGB555 search found none of the three distinct cycling palettes,
  even ignoring transparent pen zero and the top control bit of every color.
  Reordered colors, palette indices or generated colors could evade this check.
- The already identified PS1 portrait sequences do exist. Their records are
  short display steps followed by a self-looping empty layout, not the intro's
  six-frame sprite loop. Their existence does not supply its timing/effects.

Additional CPS2 data exploration found a structurally corresponding six-step
loop with the same six sprite-cell counts as Saturn. Nearby particle records
use a different terminal-hold schedule. Searching those looser hold schedules
also gives many PS1 candidates, including suffixes of longer animations; they
are not all semantically resolved. This reinforces the need to follow artwork
and consumers, and not to equate Saturn and arcade behavior automatically.

No complete retained PS1 intro animation table has been confirmed. This is a
bounded negative result for the main loop's native record format, not proof
of whole-disc absence. Differently encoded or retimed tables, compression,
stream files, disc slack and background-page controls remain outside that
negative conclusion. The PS1-first preference remains in force.

### Validation and publication

The 11 initial synthetic controls passed in 0.003 seconds on source identical
to `5819b564`. Two added palette controls passed in 0.001 seconds on source
identical to `6464aeff`; the unchanged controls were reused. The real sequence
scan names the first commit and tool digest in its private report. The palette
pass names the second. No game launch, build or prior asset-test rerun.

Private evidence is in `port/local/intro-asset-probe/`: `animation-search.json`,
`animation-palette-search.json`, candidate-location/shape reports and the
separate arcade exploratory reports. The evidence manifest records hashes.
These contain extracted values and are not published. Published source is
project-authored format/search logic and synthetic tests, with no game bytes,
pseudocode, third-party implementation, personal paths or credentials.

### Review correction

The owner found that the animation control also depends on `port/intro/assets.py`.
The dependency collector follows imports only within `port/tools`, so its
explicit subject entry now includes that helper. This is a selector-only
correction. A targeted `port_controls` selection check confirmed that changing
`assets.py` selects exactly the asset and animation controls; unchanged animation
controls, game builds and disc scans were not rerun. The lesson is recorded in active memory. The owner subsequently raised
single-input ergonomics and a possible CPS2-only asset backend; that is a
feasibility question, not approval to change runtime inputs.
