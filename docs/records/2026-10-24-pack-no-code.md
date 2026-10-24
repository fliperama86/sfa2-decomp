## The pack holds no module code (2026-10-24)

Toward the owner's goal of no disc at all. The port runs its own C for
every module function, so a pack no longer keeps the bytes of the module
functions: `port/arcade/code_pins.py` derives, for each of the build's 77
module images, the ranges of its functions (the matching build's exact
ones, and the inventory's other ones that end in `jr ra`) and a data pin,
the SHA-256 of the chunk with those ranges zeroed. The import leaves the
ranges out; `modules.c` accepts a chunk by its data pin when its full pin
does not match (`port_module_data_pin_hook`, set by the arcade loader).

A chunk's first word is never zeroed: the game's disc stream compares it
with the archive header's copy (check_stream_word) and reads the file
again when they differ. The first try zeroed SELECTA's, the game read the
file again and again, and the sound library ran out of sequence slots
(a crash in SsSepOpen); found by comparing the sequence opens of two runs
under the debugger.

Checked on 2026-10-24: `code_pins.py --compare-c` (77 of 77 images), the
unit test (13 cases), and a fight from the pack through the win and
continue screens with 8 modules accepted by their data pins. The pack is
86 MB (160 MB before the music filler, the movies and the code left it).
`test_hostlaunch.py --linux` links again (its stub for the new
`port_intro_available`); its 17 failing cases are the earlier card-listing
case and the extra line of the arcade intro being on by default.
