## A count of the names, made by the coverage workflow (2026-10-11)

The owner asked for a measure of the naming work that the workflow computes
by itself, as it does for the coverage map, and shown as a percentage in the
README: "we don't need a fancy graph tho, just percentage indicator in README
is enough".

`ps1/tools/namestats.py` reads the build configuration, the field table and
the symbol table and prints how many functions, fields and data symbols have
a name instead of a placeholder. On main at `bd18d5bf`
(`python ps1/tools/namestats.py`):

    functions: 63 of 8835 have a name (0.7%); resident program 63 of 1465 (4.3%), modules 0 of 7370 (0.0%)
    fields: 409 of 2299 have a name (17.8%)
    data symbols: 371 of 4425 have a name (8.4%)

What is counted is in the tool's header and in "How many have a name" on
`ps1/docs/names.md`. Sony's library is left out of the functions (its names
were found, not given), a function with only nonmatching C is not counted
(it keeps its placeholder by rule), and the data symbols leave out the
function placeholders of the symbol table and the names that sit at the
address of a declared resident function. The count says how many things have
a name, not that the names are right: every name is inferred.

The workflow `coverage-map.yml` now runs the tool's control cases and the
tool on every pull request and on every push to `main`, and publishes
`names.svg` (the three percentages as one line, shown in the README under
the map) and `names.json` beside the map. The workflow writes nothing into
the repository, so a rename needs no second step.

What ran: `python3 ps1/tools/test_namestats.py`, six cases on made-up tables
with counts worked out by hand, all as required. Seven copies of the tool,
each wrong in one place, each failed the first case: library units counted,
comments of the symbol table kept, names of functions in the symbol table
counted, function placeholders of the symbol table counted, a function
placeholder with an image suffix taken for a name, a data placeholder with a
suffix taken for a name, module functions left out.

Not shown by this change: the line in the README. Its picture is published
by the first run of the workflow on `main` after the merge; until then the
link has no picture behind it.
