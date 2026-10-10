## The PC build leaves the library's nonmatching C out (2026-10-10)

A correction of a fault that pull request 173 brought in.

- What broke: the program built from main refused to start, with
  `refused: library: func_80166144 (sound) is listed but this build has
  no library function of that name without C`. Pull request 173 added
  nonmatching C for `func_80166144` in `ps1/src/library_nonmatching/`.
  The PC build compiled every `*_nonmatching` folder, so that function
  had C in the program, and `port/src/sound.c` has a host routine for
  it: a host routine for a function that has C is refused at start.
- Why it was not seen: the pull request's check built the program and
  did not start it. A build does not run the start check.
- The rule now: `hostbuild.py` does not compile the folder
  `library_nonmatching`. Its files are C of Sony's library, like the
  units under `sdk/`, which this build has always left out; a library
  function runs a host routine of the port or stops with its name. The
  tool's header says so. The six functions of that folder are again
  library functions without C in this program, as before pull request
  167 (which added the first four; none of those four had a host
  routine, so nothing was refused then).
- One case in `test_hostbuild.py`: a file of a made-up
  `library_nonmatching` folder is neither compiled nor declared. With
  the rule taken out of a copy of the tool that case and three others
  fail (a one-off run).
- Ran: `python3 port/tools/test_hostbuild.py` without a compiler, 360
  lines `ok`, `all cases behaved as required`. One build of the program
  from this head and one start of it with the disc, 25 seconds, not on
  a screen: it prints its start lines, loads the first module and is
  ended by the time limit with no line that begins `refused` or `stop`
  (a one-off run with a private input; no command of the repository
  repeats it).
- Not run: the other control files of the port (nothing of the runtime
  changed), the review command.
- Later: the port's sound work is to compile the library's C, `sdk/`
  and this folder together, behind an option of the build; then the
  host routines of `sound.c` go for that build. This record changes
  nothing of that.
