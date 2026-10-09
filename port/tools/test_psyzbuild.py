#!/usr/bin/env python3
"""Controls for psyzbuild.py using made-up trees and made-up patch texts.

Needs no compiler, no CMake and no PsyZ. The patch reader (parse_patch,
find_places, apply_patch, patch_commit) and the small pure functions
(triplet_of, toolchain_text, probe_link_items) are called on invented text.
The exit statuses and the output lines are checked by running the tool as a
program on an invented PsyZ source (a git repository of a few files) with
stand-in `cmake` and compiler scripts that this file writes: the stand-in
CMake records its arguments, writes a `build.ninja` with a link line for the
probe and creates `libpsyz.a`, or fails when a case asks it to. The real
`port/psyz.patch` is applied to a file made of the three lines it expects
plus invented neighbours. The expected values are worked out here from each
fixture, never read back from the tool.
"""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
SCRIPT = TOOLS / "psyzbuild.py"
REAL_PATCH = TOOLS.parent / "psyz.patch"
sys.path.insert(0, str(TOOLS))

import psyzbuild as pb  # noqa: E402

Problem = pb.Problem


def same(got, want):
    return None if got == want else f"wanted {want!r}, got {got!r}"


def raises(fn, *needles):
    """None when fn raises a Problem whose text has every needle."""
    try:
        fn()
    except Problem as err:
        missing = [n for n in needles if n not in str(err)]
        return None if not missing else f"the error {str(err)!r} does not name {missing}"
    return "no error was raised"


def write(path: Path, text: str) -> Path:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)
    return path


def tree_of(root: Path) -> dict[str, str]:
    return {str(p.relative_to(root)): p.read_text() for p in sorted(root.rglob("*")) if p.is_file() and ".git" not in p.relative_to(root).parts}


# Invented patch texts.

COMMIT_A = "0123456789abcdef0123456789abcdef01234567"
COMMIT_B = "fedcba9876543210fedcba9876543210fedcba98"
HEAD = f"Invented patch for a test. It applies to commit {COMMIT_A}.\nIt says what it fixes.\n\n"


def patch(*files, head=HEAD):
    """files: (path, [hunk text, ...]); a hunk text is its header line and its lines."""
    out = head
    for path, hunks in files:
        out += f"--- a/{path}\n+++ b/{path}\n"
        for h in hunks:
            out += h if h.endswith("\n") else h + "\n"
    return out


counter = [0]


def apply(root: Path, files: dict[str, str], text: str):
    """Make a fresh tree of `files`, apply `text`; returns (result or Problem, tree after)."""
    counter[0] += 1
    base = root / f"apply{counter[0]}"
    for name, body in files.items():
        write(base / name, body)
    try:
        got = pb.apply_patch(text, base)
    except Problem as err:
        got = err
    return got, tree_of(base)


A = "one\ntwo\nthree\nfour\nfive\nsix\n"


# The reader: what it applies.


def apply_cases(root):
    h = "@@ -2,3 +2,3 @@\n one\n-two\n+TWO\n three\n"
    got, after = apply(root, {"f.c": A}, patch(("f.c", [h])))
    yield "apply-one-place-result-exact", same(after, {"f.c": "one\nTWO\nthree\nfour\nfive\nsix\n"})
    yield "apply-reports-one-line-per-hunk", same(got, ["f.c line 1"])
    got, after = apply(root, {"f.c": "a\nb\nc"}, patch(("f.c", ["@@ -2,1 +2,1 @@\n-b\n+B\n"])))
    yield "apply-keeps-missing-final-newline", same(after, {"f.c": "a\nB\nc"})
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -9,2 +9,2 @@\n four\n-five\n+FIVE\n"])))
    yield "apply-ignores-the-line-numbers-of-the-header", same((got, after["f.c"]), (["f.c line 4"], "one\ntwo\nthree\nfour\nFIVE\nsix\n"))
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,2 +1,2 @@\n-one\n+ONE\n two\n", "@@ -5,2 +5,2 @@\n five\n-six\n+SIX\n"])))
    yield "apply-two-hunks", same((got, after["f.c"]), (["f.c line 1", "f.c line 5"], "ONE\ntwo\nthree\nfour\nfive\nSIX\n"))
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -5,2 +5,2 @@\n five\n-six\n+SIX\n", "@@ -1,2 +1,2 @@\n-one\n+ONE\n two\n"])))
    yield "apply-hunks-in-reverse-order", same((got, after["f.c"]), (["f.c line 5", "f.c line 1"], "ONE\ntwo\nthree\nfour\nfive\nSIX\n"))
    # Hunks that change the length, written in reverse order: the later place must not shift.
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -5,2 +5,1 @@\n-five\n-six\n+SIX\n", "@@ -1,2 +1,4 @@\n-one\n+x\n+y\n+z\n two\n"])))
    yield "apply-length-changing-hunks-out-of-order", same(after["f.c"], "x\ny\nz\ntwo\nthree\nfour\nSIX\n")
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,2 +1,2 @@\n-one\n+ONE\n two\n", "@@ -3,2 +3,2 @@\n-three\n+THREE\n four\n"])))
    yield "apply-adjacent-hunks-are-not-overlapping", same(after["f.c"], "ONE\ntwo\nTHREE\nfour\nfive\nsix\n")
    got, after = apply(root, {"f.c": A, "d/g.c": "x\ny\n"}, patch(("f.c", ["@@ -1,1 +1,1 @@\n-one\n+ONE\n"]), ("d/g.c", ["@@ -2,1 +2,1 @@\n-y\n+Y\n"])))
    yield "apply-two-files", same((got, after), (["f.c line 1", "d/g.c line 2"], {"f.c": "ONE\ntwo\nthree\nfour\nfive\nsix\n", "d/g.c": "x\nY\n"}))
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -2,1 +2,0 @@\n-two\n"])))
    yield "apply-deletion", same(after["f.c"], "one\nthree\nfour\nfive\nsix\n")
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -2,1 +2,3 @@\n two\n+a\n+b\n"])))
    yield "apply-addition-after-context", same(after["f.c"], "one\ntwo\na\nb\nthree\nfour\nfive\nsix\n")
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -2 +2 @@\n-two\n+TWO\n"])))
    yield "apply-header-without-counts-means-one-line", same(after["f.c"], "one\nTWO\nthree\nfour\nfive\nsix\n")
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -2 +2 @@\n-two\n+TWO\n\\ No newline at end of file\n"])))
    yield "apply-no-newline-marker-is-not-a-line", same(after["f.c"], "one\nTWO\nthree\nfour\nfive\nsix\n")
    got, after = apply(root, {"f.c": "a\n-- b\nc\n"}, patch(("f.c", ["@@ -1,3 +1,3 @@\n a\n--- b\n+++ B\n c\n"])))
    yield "apply-removed-line-that-looks-like-a-file-header", same((got, after["f.c"]), (["f.c line 1"], "a\n++ B\nc\n"))
    got, after = apply(root, {"f.c": "a\nb\nc"}, patch(("f.c", ["@@ -3,1 +3,1 @@\n-c\n+C\n"])))
    yield "apply-last-line-of-a-file-without-final-newline", same((got, after), (["f.c line 3"], {"f.c": "a\nb\nC"}))
    got, after = apply(root, {"f.c": "a\nb\nc"}, patch(("f.c", ["@@ -3,1 +3,1 @@\n-c\n\\ No newline at end of file\n+C\n\\ No newline at end of file\n"])))
    yield "apply-no-newline-marker-between-lines", same((got, after), (["f.c line 3"], {"f.c": "a\nb\nC"}))
    text = patch(("f.c", ["@@ -1,1 +1,1 @@\n-one\n+ONE\n"]))
    got, after = apply(root, {"f.c": A}, text.replace("+++ b/f.c", "+++ b/f.c\t2026-01-01 00:00"))
    yield "apply-file-name-ends-at-a-tab", same(after["f.c"], "ONE\ntwo\nthree\nfour\nfive\nsix\n")
    got, after = apply(root, {"f.c": A}, text.replace("a/f.c", "f.c").replace("b/f.c", "f.c"))
    yield "apply-names-without-a-and-b-prefix", same(after["f.c"], "ONE\ntwo\nthree\nfour\nfive\nsix\n")
    got, after = apply(root, {"f.c": A}, text.rstrip("\n"))
    yield "apply-patch-without-final-newline", same(after["f.c"], "ONE\ntwo\nthree\nfour\nfive\nsix\n")
    got, after = apply(root, {"f.c": A}, "intro\n\n" + text.split("\n", 3)[3] + "\n\n\n")
    yield "apply-blank-lines-after-the-last-hunk", same(after["f.c"], "ONE\ntwo\nthree\nfour\nfive\nsix\n")


# The reader: what it refuses, and then leaves alone.


def refuse_cases(root):
    ctx = "@@ -2,2 +2,2 @@\n two\n-three\n+THREE\n"
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -2,2 +2,2 @@\n two\n-THREE\n+x\n"])))
    yield "refuse-no-place-message", same(isinstance(got, Problem) and all(n in str(got) for n in ("@@ -2,2 +2,2 @@", "f.c", "no place", "exactly one")), True)
    yield "refuse-no-place-leaves-the-file", same(after, {"f.c": A})
    two = "x\nTHREE\ny\nx\nTHREE\ny\n"
    got, after = apply(root, {"f.c": two}, patch(("f.c", ["@@ -1,3 +1,3 @@\n x\n-THREE\n+three\n y\n"])))
    yield "refuse-two-places-message", same(isinstance(got, Problem) and all(n in str(got) for n in ("f.c", "2 places", "exactly one")), True)
    yield "refuse-two-places-leaves-the-file", same(after, {"f.c": two})
    three = "q\nq\nq\n"
    got, after = apply(root, {"f.c": three}, patch(("f.c", ["@@ -1,1 +1,1 @@\n-q\n+r\n"])))
    yield "refuse-three-places-counts-them", same(isinstance(got, Problem) and "3 places" in str(got), True)
    got, after = apply(root, {"f.c": A}, patch(("g.c", ["@@ -1,1 +1,1 @@\n-one\n+ONE\n"])))
    yield "refuse-missing-file-message", same(isinstance(got, Problem) and all(n in str(got) for n in ("g.c", "no such file")), True)
    yield "refuse-missing-file-leaves-the-tree", same(after, {"f.c": A})
    # The second file fails: the first must stay as it was.
    got, after = apply(root, {"f.c": A, "g.c": "x\n"}, patch(("f.c", ["@@ -1,1 +1,1 @@\n-one\n+ONE\n"]), ("g.c", ["@@ -1,1 +1,1 @@\n-nope\n+y\n"])))
    yield "refuse-second-file-leaves-the-first-unpatched", same((isinstance(got, Problem), after), (True, {"f.c": A, "g.c": "x\n"}))
    got, after = apply(root, {"f.c": A, "g.c": "x\n"}, patch(("f.c", ["@@ -1,1 +1,1 @@\n-one\n+ONE\n"]), ("h.c", ["@@ -1,1 +1,1 @@\n-x\n+y\n"])))
    yield "refuse-missing-second-file-leaves-the-first-unpatched", same((isinstance(got, Problem), after), (True, {"f.c": A, "g.c": "x\n"}))
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,1 +1,1 @@\n-one\n+ONE\n", "@@ -2,1 +2,1 @@\n-nope\n+x\n"])))
    yield "refuse-second-hunk-leaves-the-first-unapplied", same((isinstance(got, Problem), after), (True, {"f.c": A}))
    # Overlap: two hunks that each fit one place, and the places share a line.
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,2 +1,2 @@\n one\n-two\n+x\n", "@@ -2,2 +2,2 @@\n-two\n+y\n three\n"])))
    yield "refuse-overlapping-hunks-message", same(isinstance(got, Problem) and all(n in str(got) for n in ("overlap", "f.c")), True)
    yield "refuse-overlapping-hunks-leaves-the-file", same(after, {"f.c": A})
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,1 +1,1 @@\n-one\n+x\n", "@@ -1,1 +1,1 @@\n-one\n+y\n"])))
    yield "refuse-the-same-place-twice", same(isinstance(got, Problem) and "overlap" in str(got), True)
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,0 +1,1 @@\n+new\n"])))
    yield "refuse-addition-without-context", same(isinstance(got, Problem) and "places" in str(got), True)
    # Hunks whose counts disagree with their header.
    more = "@@ -9,1 +9,1 @@\n-z\n+y\n"
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,3 +1,3 @@\n one\n-two\n+TWO\n", more])))
    yield "refuse-hunk-with-too-few-lines-message", same(isinstance(got, Problem) and all(n in str(got) for n in ("@@ -1,3 +1,3 @@", "2 old and 2 new lines, not 3 and 3")), True)
    yield "refuse-hunk-with-too-few-lines-leaves-the-file", same(after, {"f.c": A})
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,2 +1,3 @@\n one\n-two\n+TWO\n", more])))
    yield "refuse-hunk-whose-new-count-is-wrong", same(isinstance(got, Problem) and "2 old and 2 new lines, not 2 and 3" in str(got), True)
    got, after = apply(root, {"f.c": A}, patch(("f.c", ["@@ -1,2 +1,2 @@\n one\n-two\nfour\n+TWO\n"])))
    yield "refuse-hunk-cut-by-a-line-without-a-tag", same(isinstance(got, Problem) and "2 old and 1 new lines, not 2 and 2" in str(got), True)
    yield "refuse-count-check-comes-before-any-place", raises(lambda: pb.parse_patch(patch(("f.c", ["@@ -1,4 +1,1 @@\n-a\n", "@@ -9,1 +9,1 @@\n-z\n+y\n"]))), "1 old and 0 new lines, not 4 and 1")
    # A body must have exactly the counts of its header.
    cut = "@@ -1,3 +1,3 @@\n one\n-two\n+TWO\n"
    got, after = apply(root, {"f.c": A}, patch(("f.c", [cut])))
    yield "refuse-truncated-last-hunk-at-the-end-of-the-text", same(
        isinstance(got, Problem) and all(n in str(got) for n in ("@@ -1,3 +1,3 @@", "2 old and 2 new lines, not 3 and 3")), True
    )
    yield "refuse-truncated-last-hunk-leaves-the-file", same(after, {"f.c": A})
    yield "refuse-truncated-last-hunk-without-final-newline", raises(lambda: pb.parse_patch(patch(("f.c", [cut])).rstrip("\n")), "2 old and 2 new lines, not 3 and 3")
    yield "refuse-truncated-hunk-before-the-next-file-header", raises(
        lambda: pb.parse_patch(patch(("f.c", [cut]), ("g.c", ["@@ -1,1 +1,1 @@\n-x\n+y\n"]))), "@@ -1,3 +1,3 @@", "2 old and 2 new lines, not 3 and 3"
    )
    yield "refuse-truncated-hunk-before-the-next-hunk-header", raises(
        lambda: pb.parse_patch(patch(("f.c", [cut, "@@ -9,1 +9,1 @@\n-z\n+y\n"]))), "@@ -1,3 +1,3 @@", "2 old and 2 new lines, not 3 and 3"
    )
    full = "@@ -1,1 +1,1 @@\n-one\n+ONE\n"
    for what, extra in (("added-line", "+EXTRA\n"), ("removed-line", "-EXTRA\n"), ("context-line", " EXTRA\n"), ("untagged-line", "EXTRA\n")):
        got, after = apply(root, {"f.c": A}, patch(("f.c", [full + extra])))
        yield f"refuse-surplus-{what}", same(isinstance(got, Problem) and all(n in str(got) for n in ("surplus line", "@@ -1,1 +1,1 @@", "EXTRA")), True)
        yield f"refuse-surplus-{what}-leaves-the-file", same(after, {"f.c": A})
    yield "refuse-surplus-line-after-the-first-of-two-hunks", raises(
        lambda: pb.parse_patch(patch(("f.c", [full + "+EXTRA\n", "@@ -3,1 +3,1 @@\n-three\n+x\n"]))), "surplus line", "@@ -1,1 +1,1 @@"
    )
    yield "refuse-surplus-line-after-the-last-file", raises(lambda: pb.parse_patch(patch(("f.c", [full])) + "stray\n"), "surplus line", "stray")
    got, after = apply(root, {"f.c": A, "g.c": "x\n"}, patch(("f.c", [full + "\n\n"]), ("g.c", ["@@ -1,1 +1,1 @@\n-x\n+y\n\n"])))
    yield "apply-blank-lines-between-hunks-and-files-and-at-the-end", same((got, after["g.c"]), (["f.c line 1", "g.c line 1"], "y\n"))
    got, after = apply(root, {"f.c": A}, patch(("f.c", [full + "\\ No newline at end of file\n\n"])))
    yield "apply-marker-after-the-counts-is-kept-allowed", same(after["f.c"], "ONE\ntwo\nthree\nfour\nfive\nsix\n")
    # The patch itself.
    yield "refuse-empty-patch", raises(lambda: pb.parse_patch(""), "no hunk")
    yield "refuse-text-without-a-diff", raises(lambda: pb.parse_patch("only a header\nand another line\n"), "no hunk")
    yield "refuse-file-without-hunk", raises(lambda: pb.parse_patch(HEAD + "--- a/f.c\n+++ b/f.c\n"), "no hunk")
    yield "refuse-second-file-without-hunk", raises(lambda: pb.parse_patch(patch(("f.c", ["@@ -1,1 +1,1 @@\n-one\n+ONE\n"]), ("g.c", []))), "no hunk")
    yield "refuse-hunk-before-any-file", raises(lambda: pb.parse_patch(HEAD + "@@ -1,1 +1,1 @@\n-one\n+ONE\n"), "no hunk")
    yield "refuse-minus-without-plus-message", raises(lambda: pb.parse_patch(HEAD + "--- a/f.c\n@@ -1,1 +1,1 @@\n-one\n+ONE\n"), "`---`", "`+++`")
    yield "refuse-minus-at-the-end-of-the-text", raises(lambda: pb.parse_patch(HEAD + "--- a/f.c"), "`---`", "`+++`")
    yield "refuse-second-minus-without-plus", raises(
        lambda: pb.parse_patch(patch(("f.c", ["@@ -1,1 +1,1 @@\n-one\n+ONE\n"])) + "--- a/g.c\n@@ -1,1 +1,1 @@\n-x\n+y\n"), "without a `+++`"
    )


# Blanks count.


def blank_cases(root):
    blank_ctx = "@@ -1,3 +1,3 @@\n a\n\n-b\n+B\n"  # the context blank line is written as an empty line
    got, after = apply(root, {"f.c": "a\n\nb\nc\n"}, patch(("f.c", [blank_ctx])))
    yield "blank-empty-line-in-the-patch-is-a-blank-context-line", same(after["f.c"], "a\n\nB\nc\n")
    got, after = apply(root, {"f.c": "a\n\nb\nc\n"}, patch(("f.c", ["@@ -1,3 +1,3 @@\n a\n \n-b\n+B\n"])))
    yield "blank-space-line-in-the-patch-is-a-blank-context-line", same(after["f.c"], "a\n\nB\nc\n")
    got, after = apply(root, {"f.c": "a\n  \nb\n"}, patch(("f.c", ["@@ -1,3 +1,3 @@\n a\n \n-b\n+B\n"])))
    yield "blank-a-line_of_spaces_is_not_blank", same((isinstance(got, Problem), after["f.c"]), (True, "a\n  \nb\n"))
    got, after = apply(root, {"f.c": "a\nb\n"}, patch(("f.c", ["@@ -1,3 +1,3 @@\n a\n \n-b\n+B\n"])))
    yield "blank-missing-blank-line-refuses", same(isinstance(got, Problem) and "no place" in str(got), True)
    got, after = apply(root, {"f.c": "a\n\n\nb\n"}, patch(("f.c", ["@@ -1,3 +1,3 @@\n a\n \n-b\n+B\n"])))
    yield "blank-two-blank-lines-are-not-one", same(isinstance(got, Problem), True)
    twice = "a\n\nb\na\n\n\nb\n"
    got, after = apply(root, {"f.c": twice}, patch(("f.c", ["@@ -1,3 +1,3 @@\n a\n \n-b\n+B\n"])))
    yield "blank-count-of-blanks-picks-one-place", same((got, after["f.c"]), (["f.c line 1"], "a\n\nB\na\n\n\nb\n"))
    got, after = apply(root, {"f.c": "a\nb\n"}, patch(("f.c", ["@@ -1,2 +1,3 @@\n a\n+\n b\n"])))
    yield "blank-added-blank-line", same(after["f.c"], "a\n\nb\n")
    got, after = apply(root, {"f.c": "a\n\nb\n"}, patch(("f.c", ["@@ -1,3 +1,2 @@\n a\n-\n b\n"])))
    yield "blank-removed-blank-line", same(after["f.c"], "a\nb\n")
    got, after = apply(root, {"f.c": "a\n b\n"}, patch(("f.c", ["@@ -1,2 +1,2 @@\n a\n-b\n+B\n"])))
    yield "blank-leading-space-of-a-line-counts", same(isinstance(got, Problem), True)
    got, after = apply(root, {"f.c": "a\nb \n"}, patch(("f.c", ["@@ -1,2 +1,2 @@\n a\n-b\n+B\n"])))
    yield "blank-trailing-space-of-a-line-counts", same(isinstance(got, Problem), True)
    got, after = apply(root, {"f.c": "\ta\n"}, patch(("f.c", ["@@ -1,1 +1,1 @@\n-    a\n+b\n"])))
    yield "blank-tab-is-not-four-spaces", same(isinstance(got, Problem), True)


# The commit in the header.


def commit_cases():
    yield "commit-from-the-header", same(pb.patch_commit(HEAD + "--- a/f\n+++ b/f\n@@ -1 +1 @@\n-a\n+b\n"), COMMIT_A)
    yield "commit-in-the-middle-of-the-header", same(pb.patch_commit(f"x\nfor {COMMIT_B}, then\n--- a/f\n"), COMMIT_B)
    yield "commit-the-first-one", same(pb.patch_commit(f"{COMMIT_A} and {COMMIT_B}\n--- a/f\n"), COMMIT_A)
    yield "commit-none-in-the-header", same(pb.patch_commit("nothing here\n--- a/f\n+++ b/f\n"), None)
    yield "commit-after-the-diff-does-not-count", same(pb.patch_commit(f"header\n--- a/f\n+++ b/f\n@@ -1 +1 @@\n-{COMMIT_A}\n+x\n"), None)
    yield "commit-39-digits-is-no-commit", same(pb.patch_commit(f"for {COMMIT_A[:39]}\n--- a/f\n"), None)
    yield "commit-41-digits-is-no-commit", same(pb.patch_commit(f"for {COMMIT_A}0\n--- a/f\n"), None)
    yield "commit-uppercase-is-no-commit", same(pb.patch_commit(f"for {COMMIT_A.upper()}\n--- a/f\n"), None)
    yield "commit-on-its-own-line", same(pb.patch_commit(f"a\n{COMMIT_B}\nb\n--- a/f\n"), COMMIT_B)
    yield "commit-in-text-without-a-diff", same(pb.patch_commit(f"for {COMMIT_A}\n"), COMMIT_A)


# The toolchain file and the names.


def toolchain_cases():
    cc = "/opt/mw/bin/i686-w64-mingw32-gcc"
    want = (
        "set(CMAKE_SYSTEM_NAME Windows)\n"
        "set(CMAKE_SYSTEM_PROCESSOR i686)\n"
        "set(CMAKE_C_COMPILER /opt/mw/bin/i686-w64-mingw32-gcc)\n"
        "set(CMAKE_CXX_COMPILER /opt/mw/bin/i686-w64-mingw32-g++)\n"
        "set(CMAKE_RC_COMPILER /opt/mw/bin/i686-w64-mingw32-windres)\n"
        "set(CMAKE_AR /opt/mw/bin/i686-w64-mingw32-ar)\n"
        "set(CMAKE_RANLIB /opt/mw/bin/i686-w64-mingw32-ranlib)\n"
        "set(CMAKE_FIND_ROOT_PATH /opt/mw/i686-w64-mingw32)\n"
        "set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)\n"
        "set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)\n"
        "set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)\n"
        'set(CMAKE_EXE_LINKER_FLAGS_INIT "-static")\n'
    )
    yield "toolchain-i686-file-content", same(pb.toolchain_text(cc), want)
    got = pb.toolchain_text("/x/bin/x86_64-w64-mingw32-gcc")
    yield "toolchain-names-follow-the-compiler", same(
        [x for x in got.split("\n") if "x86_64" in x],
        [
            "set(CMAKE_SYSTEM_PROCESSOR x86_64)",
            "set(CMAKE_C_COMPILER /x/bin/x86_64-w64-mingw32-gcc)",
            "set(CMAKE_CXX_COMPILER /x/bin/x86_64-w64-mingw32-g++)",
            "set(CMAKE_RC_COMPILER /x/bin/x86_64-w64-mingw32-windres)",
            "set(CMAKE_AR /x/bin/x86_64-w64-mingw32-ar)",
            "set(CMAKE_RANLIB /x/bin/x86_64-w64-mingw32-ranlib)",
            "set(CMAKE_FIND_ROOT_PATH /x/x86_64-w64-mingw32)",
        ],
    )
    yield "toolchain-static-link-and-no-host-programs", same(("-static" in got, "MODE_PROGRAM NEVER" in got), (True, True))
    yield "triplet-without-gcc", same(pb.triplet_of("i686-w64-mingw32-gcc"), "i686-w64-mingw32")
    yield "triplet-of-a-path", same(pb.triplet_of("/opt/mw/bin/i686-w64-mingw32-gcc"), "i686-w64-mingw32")
    yield "triplet-of-a-plain-name", same((pb.triplet_of("cc"), pb.triplet_of("clang")), ("cc", "clang"))
    yield "triplet-only-a-final-gcc-is-cut", same(pb.triplet_of("gcc-12"), "gcc-12")


def link_cases():
    build = Path("/b/obj")
    ninja = (
        "build libpsyz.a: AR a.o\n  LINK_LIBRARIES = nope.a\n"
        "build probe.exe: C_EXECUTABLE_LINKER probe.o libpsyz.a sdl/libSDL3.a\n"
        "  FLAGS = -O2\n"
        "  LINK_LIBRARIES = libpsyz.a sdl/libSDL3.a /abs/libz.a -lgdi32 -lm\n"
        "build other.exe: link x\n  LINK_LIBRARIES = other.a\n"
    )
    yield "link-items-in-order-files-made-absolute", same(
        pb.probe_link_items(ninja, build), ["/b/obj/libpsyz.a", "/b/obj/sdl/libSDL3.a", "/abs/libz.a", "-lgdi32", "-lm"]
    )
    yield "link-items-probe-without-extension", same(
        pb.probe_link_items("build probe: link\n\tLINK_LIBRARIES = -lm a.a\n", build), ["-lm", "/b/obj/a.a"]
    )
    yield "link-items-dotdot-is-resolved", same(pb.probe_link_items("build probe: l\n  LINK_LIBRARIES = ../x/a.a\n", build), ["/b/x/a.a"])
    for name, text in (
        ("link-no-probe-fails", "build libpsyz.a: AR a.o\n  LINK_LIBRARIES = a\n"),
        ("link-probe-without-libraries-fails", "build probe.exe: l\n  FLAGS = -O2\nbuild x: y\n  LINK_LIBRARIES = a.a\n"),
        ("link-empty-ninja-fails", ""),
    ):
        try:
            pb.probe_link_items(text, build)
            yield name, "no error was raised"
        except pb.Failure as err:
            yield name, same("no link line for the probe" in str(err), True)


# The real patch.

LIB = ["#ifdef __EMSCRIPTEN__", "        Draw_FlushBuffer();", "#endif"]
NEIGHBOURS_BEFORE = ["void GPU_Enqueue(void) {", "    int n = 0;", "", "    if (n) {"]
NEIGHBOURS_AFTER = ["    }", "    return;", "}", ""]
LIB_PATH = "psyz/src/psyz/libgpu.c"


def real_cases(root):
    text = REAL_PATCH.read_text()
    files = pb.parse_patch(text)
    yield "real-patch-one-file-one-hunk", same([(f.path, len(f.hunks)) for f in files], [(LIB_PATH, 1)])
    yield "real-patch-expects-the-three-lines", same(files[0].hunks[0].old, LIB)
    yield "real-patch-leaves-the-flush-only", same(files[0].hunks[0].new, ["        Draw_FlushBuffer();"])
    yield "real-patch-commit-from-its-header", same(pb.patch_commit(text), "4e4b3e8dc7ae740c085fd190d635f54142d2d552")

    def body(lines):
        return "\n".join(lines) + "\n"

    exact = NEIGHBOURS_BEFORE + LIB + NEIGHBOURS_AFTER
    got, after = apply(root, {LIB_PATH: body(exact)}, text)
    want = NEIGHBOURS_BEFORE + ["        Draw_FlushBuffer();"] + NEIGHBOURS_AFTER
    yield "real-patch-applies-at-one-place", same(got, [f"{LIB_PATH} line {len(NEIGHBOURS_BEFORE) + 1}"])
    yield "real-patch-result-is-exact", same(after[LIB_PATH], body(want))
    # The flush alone elsewhere in the file is not a place.
    other = NEIGHBOURS_BEFORE + ["        Draw_FlushBuffer();"] + LIB + NEIGHBOURS_AFTER
    got, after = apply(root, {LIB_PATH: body(other)}, text)
    yield "real-patch-ignores-other-flush-lines", same(
        (got, after[LIB_PATH]), ([f"{LIB_PATH} line {len(NEIGHBOURS_BEFORE) + 2}"], body(NEIGHBOURS_BEFORE + ["        Draw_FlushBuffer();"] * 2 + NEIGHBOURS_AFTER))
    )
    got, after = apply(root, {LIB_PATH: body(exact + LIB + ["tail"])}, text)
    yield "real-patch-refused-at-two-places", same((isinstance(got, Problem) and "2 places" in str(got), after[LIB_PATH]), (True, body(exact + LIB + ["tail"])))
    altered = {
        "the first line altered": [LIB[0].replace("ifdef", "if defined(") + ")", LIB[1], LIB[2]],
        "the first line indented": [" " + LIB[0], LIB[1], LIB[2]],
        "the middle line re-indented": [LIB[0], LIB[1][1:], LIB[2]],
        "the middle line with a tab": [LIB[0], "\tDraw_FlushBuffer();", LIB[2]],
        "the middle line changed": [LIB[0], "        Draw_FlushBuffer(1);", LIB[2]],
        "the last line altered": [LIB[0], LIB[1], "#endif /* x */"],
        "the last line with a trailing space": [LIB[0], LIB[1], "#endif "],
        "the first line missing": [LIB[1], LIB[2]],
        "a line between added": [LIB[0], "", LIB[1], LIB[2]],
        "the last line missing": [LIB[0], LIB[1]],
    }
    for what, lines in altered.items():
        file = NEIGHBOURS_BEFORE + lines + NEIGHBOURS_AFTER
        got, after = apply(root, {LIB_PATH: body(file)}, text)
        yield f"real-patch-refused-{what.replace(' ', '-')}", same((isinstance(got, Problem) and "no place" in str(got), after[LIB_PATH]), (True, body(file)))
    got, after = apply(root, {"other/libgpu.c": body(exact)}, text)
    yield "real-patch-refused-in-another-path", same(isinstance(got, Problem) and "no such file" in str(got), True)


# The program: output lines and exit statuses, with stand-in tools.

CMAKE_STANDIN = """#!{py}
import json, os, sys
args = sys.argv[1:]
with open(os.environ["STANDIN_LOG"], "a") as f:
    f.write(json.dumps(args) + "\\n")
fail = os.environ.get("STANDIN_FAIL")
if args and args[0] == "--build":
    if fail == "build":
        for k in range(30):
            print("compiling " + str(k))
        print("error: no such member", file=sys.stderr)
        sys.exit(1)
    open(os.path.join(args[1], "libpsyz.a"), "w").write("lib")
else:
    if fail == "configure":
        print("CMake Error: no such generator")
        sys.exit(1)
    obj = args[args.index("-B") + 1]
    os.makedirs(obj, exist_ok=True)
    with open(os.path.join(obj, "build.ninja"), "w") as f:
        f.write("build probe.exe: link a.o libpsyz.a\\n  LINK_LIBRARIES = libpsyz.a sdl/libSDL3.a -lgdi32 -lm\\n")
"""
SHELL_STANDIN = "#!/bin/sh\nexit 0\n"

SRC_FILES = {
    "psyz/CMakeLists.txt": "project(psyz)\n",
    "psyz/src/a.c": "x\nTHREE\ny\n",
    "psyz/.github/w.yml": "w\n",
    "decomp/d.c": "d\n",
    "external/SDL/CMakeLists.txt": "project(SDL)\n",
    "external/SDL/s.c": "s\n",
}
PATCH_OK = patch(("psyz/src/a.c", ["@@ -1,3 +1,3 @@\n x\n-THREE\n+three\n y\n"]), head=f"Invented patch, for commit @COMMIT@.\n\n")


class World:
    """An invented PsyZ source (a git repository) and stand-in tools, in one folder."""

    def __init__(self, base: Path, git: bool = True):
        counter[0] += 1
        self.base = base / f"world{counter[0]}"
        self.src = self.base / "psyz-src"
        for name, body in SRC_FILES.items():
            write(self.src / name, body)
        self.commit = "unknown"
        if git:
            git_cmd = ["git", "-c", "user.name=t", "-c", "user.email=t@example.com", "-C", str(self.src)]
            subprocess.run(git_cmd + ["init", "-q"], check=True, capture_output=True, timeout=60)
            subprocess.run(git_cmd + ["add", "-A"], check=True, capture_output=True, timeout=60)
            subprocess.run(git_cmd + ["commit", "-q", "-m", "x"], check=True, capture_output=True, timeout=60)
            self.commit = subprocess.run(git_cmd + ["rev-parse", "HEAD"], check=True, capture_output=True, text=True, timeout=60).stdout.strip()
        bin_dir = self.base / "bin"
        self.cmake = write(bin_dir / "cmake", CMAKE_STANDIN.format(py=sys.executable))
        self.ninja = write(bin_dir / "ninja", SHELL_STANDIN)
        self.mingw = write(bin_dir / "i686-w64-mingw32-gcc", SHELL_STANDIN)
        self.cc = write(bin_dir / "cc", SHELL_STANDIN)
        for tool in (self.cmake, self.ninja, self.mingw, self.cc):
            tool.chmod(0o755)
        self.patch = write(self.base / "p.patch", PATCH_OK.replace("@COMMIT@", self.commit if git else COMMIT_A))
        self.build = self.base / "out"
        self.log = self.base / "cmake.log"

    def run(self, fail: str | None = None, **over):
        args = {"--psyz": self.src, "--patch": self.patch, "--cc": self.mingw, "--build": self.build, "--cmake": self.cmake, "--ninja": self.ninja}
        args.update(over)
        argv = [sys.executable, str(SCRIPT)]
        for key, val in args.items():
            if val is not None:
                argv += [key, str(val)]
        env = dict(os.environ, STANDIN_LOG=str(self.log))
        env.pop("STANDIN_FAIL", None)
        if fail:
            env["STANDIN_FAIL"] = fail
        env["GIT_CEILING_DIRECTORIES"] = str(self.base)
        return subprocess.run(argv, capture_output=True, text=True, env=env, timeout=120)

    def calls(self):
        return [json.loads(x) for x in self.log.read_text().splitlines()] if self.log.is_file() else []


def stdout_lines(proc):
    return proc.stdout.splitlines()


def program_cases(root):
    w = World(root)
    before = tree_of(w.src)
    proc = w.run()
    obj = w.build.resolve() / "obj"
    include = w.build.resolve() / "src" / "psyz" / "include"
    want_link = [str(obj / "libpsyz.a"), str(obj / "sdl" / "libSDL3.a"), "-lgdi32", "-lm"]
    yield "run-exit-status-0", same((proc.returncode, proc.stderr), (0, ""))
    yield "run-output-lines-in-order", same(
        stdout_lines(proc),
        [
            f"psyz: {w.commit}",
            "patch: p.patch applied at 1 place",
            f"library: {obj / 'libpsyz.a'}",
            f"include: {include}",
            "link: " + " ".join(want_link),
        ],
    )
    yield "run-source-folder-unchanged", same(tree_of(w.src), before)
    yield "run-copy-is-patched", same((w.build / "src" / "psyz" / "src" / "a.c").read_text(), "x\nthree\ny\n")
    yield "run-copy-has-the-three-folders-without-git-dirs", same(
        sorted(tree_of(w.build / "src")),
        sorted(["CMakeLists.txt", "probe.c", "psyz/CMakeLists.txt", "psyz/src/a.c", "decomp/d.c", "external/SDL/CMakeLists.txt", "external/SDL/s.c"]),
    )
    yield "run-copy-keeps-file-times", same(
        (w.build / "src" / "decomp" / "d.c").stat().st_mtime_ns, (w.src / "decomp" / "d.c").stat().st_mtime_ns
    )
    yield "run-json-round-trip", same(
        json.loads((w.build / "psyz.json").read_text()), {"include": str(include), "define": ["__psyz"], "link": want_link, "commit": w.commit}
    )
    yield "run-wrapper-project-and-probe", same(
        ("add_subdirectory(psyz)" in (w.build / "src" / "CMakeLists.txt").read_text(), (w.build / "src" / "probe.c").read_text()),
        (True, "int main(void) { return 0; }\n"),
    )
    configure, build = w.calls()
    yield "run-configure-options", same(
        [x for x in configure if x.startswith(("-G", "-DCMAKE_BUILD", "-DGTE", "-DPSYZ"))],
        ["-GNinja", "-DCMAKE_BUILD_TYPE=Release", "-DGTE_USE_HW_SQRT=ON", "-DPSYZ_RENDERER=sdl3_gpu"],
    )
    yield "run-configure-folders", same(
        (configure[configure.index("-S") + 1], configure[configure.index("-B") + 1], build), (str(w.build.resolve() / "src"), str(obj), ["--build", str(obj)])
    )
    yield "run-mingw-compiler-gets-a-toolchain-file", same(
        (f"-DCMAKE_TOOLCHAIN_FILE={w.build.resolve() / 'toolchain.cmake'}" in configure, any(x.startswith("-DCMAKE_C_COMPILER") for x in configure)),
        (True, False),
    )
    yield "run-toolchain-file-names-the-compiler", same(
        (w.build / "toolchain.cmake").read_text().splitlines()[2], f"set(CMAKE_C_COMPILER {w.mingw})"
    )

    # The commit the patch was written for is another one: a note line, and the patch still applies.
    w2 = World(root)
    write(w2.patch, PATCH_OK.replace("@COMMIT@", COMMIT_B))
    proc = w2.run()
    lines = stdout_lines(proc)
    yield "run-note-when-the-patch-is-for-another-commit", same(
        (proc.returncode, lines[:3]), (0, [f"psyz: {w2.commit}", "patch: p.patch applied at 1 place", f"note: the patch was written for {COMMIT_B}"])
    )
    yield "run-note-comes-before-library", same([x.split(":")[0] for x in lines], ["psyz", "patch", "note", "library", "include", "link"])
    # No git: unknown, and no note even though the header names a commit.
    w3 = World(root, git=False)
    proc = w3.run()
    yield "run-without-git-the-commit-is-unknown", same((proc.returncode, stdout_lines(proc)[:2]), (0, ["psyz: unknown", "patch: p.patch applied at 1 place"]))
    yield "run-without-git-no-note", same(any(x.startswith("note:") for x in stdout_lines(proc)), False)
    yield "run-without-git-json-commit", same(json.loads((w3.build / "psyz.json").read_text())["commit"], "unknown")
    # A patch with two hunks says "places".
    w4 = World(root)
    write(w4.src / "psyz/src/b.c", "p\nq\n")
    two = patch(("psyz/src/a.c", ["@@ -1,3 +1,3 @@\n x\n-THREE\n+three\n y\n"]), ("psyz/src/b.c", ["@@ -2,1 +2,1 @@\n-q\n+Q\n"]), head=f"for {w4.commit}\n")
    write(w4.patch, two)
    proc = w4.run()
    yield "run-two-hunks-say-places", same(stdout_lines(proc)[1] if proc.returncode == 0 else proc.stderr, "patch: p.patch applied at 2 places")
    # A compiler that is not MinGW: the machine's own, no toolchain file.
    w5 = World(root)
    proc = w5.run(**{"--cc": w5.cc})
    configure = w5.calls()[0]
    yield "run-other-compiler-builds-for-this-machine", same(
        (proc.returncode, f"-DCMAKE_C_COMPILER={w5.cc}" in configure, any("TOOLCHAIN" in x for x in configure), (w5.build / "toolchain.cmake").exists()),
        (0, True, False, False),
    )
    # A compiler named by its bare name is found on PATH.
    w6 = World(root)
    env_path = os.environ["PATH"]
    os.environ["PATH"] = str(w6.base / "bin") + os.pathsep + env_path
    try:
        proc = w6.run(**{"--cc": "i686-w64-mingw32-gcc"})
    finally:
        os.environ["PATH"] = env_path
    yield "run-compiler-found-on-path", same((proc.returncode, f"set(CMAKE_C_COMPILER {w6.mingw})" in (w6.build / "toolchain.cmake").read_text()), (0, True))
    # A second run on the same folder gives the same lines.
    again = w.run()
    yield "run-second-run-gives-the-same-lines", same((again.returncode, stdout_lines(again)), (0, stdout_lines(w.run())))


def status_cases(root):
    # Status 1: the build failed. The lines so far are on standard output, the log tail on standard error.
    for stage, needle, last in (("configure", "cmake configure failed (status 1)", "CMake Error: no such generator"), ("build", "cmake build failed (status 1)", "error: no such member")):
        w = World(root)
        proc = w.run(fail=stage)
        yield f"status-1-{stage}-exit-and-message", same((proc.returncode, needle in proc.stderr, last in proc.stderr), (1, True, True))
        yield f"status-1-{stage}-lines-so-far", same(stdout_lines(proc), [f"psyz: {w.commit}", "patch: p.patch applied at 1 place"])
        yield f"status-1-{stage}-log-has-the-output", same(last in (w.build / "build.log").read_text(), True)
        yield f"status-1-{stage}-no-json", same((w.build / "psyz.json").exists(), False)
    w = World(root)
    proc = w.run(fail="build")
    tail = proc.stderr.split("last lines:\n", 1)[1].splitlines()
    log = (w.build / "build.log").read_text()
    yield "status-1-stderr-keeps-the-last-twelve-lines", same((len(tail), tail[-1], "compiling 0" in proc.stderr, "compiling 0" in log), (12, "error: no such member", False, True))
    # No library file made: the probe's link line must name one that exists.
    w = World(root)
    write(w.cmake, CMAKE_STANDIN.format(py=sys.executable).replace('open(os.path.join(args[1], "libpsyz.a"), "w").write("lib")', "pass"))
    proc = w.run()
    yield "status-1-library-missing", same((proc.returncode, "libpsyz.a" in proc.stderr), (1, True))

    # Status 2: an input is missing or the patch is refused; one line on standard error, nothing on standard output.
    def two(proc, *needles):
        lines = proc.stderr.splitlines()
        bad = [n for n in needles if n not in proc.stderr]
        return same((proc.returncode, proc.stdout, len(lines), lines[0].startswith("psyzbuild.py: ") if lines else False, bad), (2, "", 1, True, []))

    w = World(root)
    yield "status-2-patch-file-missing", two(w.run(**{"--patch": w.base / "none.patch"}), "none.patch", "does not exist")
    w = World(root)
    yield "status-2-compiler-missing", two(w.run(**{"--cc": w.base / "bin" / "nocc"}), "nocc", "not an executable")
    w = World(root)
    yield "status-2-cmake-named-and-missing", two(w.run(**{"--cmake": w.base / "bin" / "nocmake"}), "nocmake", "cmake", "not an executable")
    w = World(root)
    yield "status-2-ninja-named-and-missing", two(w.run(**{"--ninja": w.base / "bin" / "noninja"}), "noninja", "ninja")
    w = World(root)
    write(w.patch, "no diff in here\n")
    proc = w.run()
    yield "status-2-patch-without-hunk", two(proc, "no hunk")
    yield "status-2-bad-patch-builds-nothing", same(w.build.exists() or w.log.exists(), False)
    w = World(root)
    write(w.patch, PATCH_OK.replace("@COMMIT@", COMMIT_A).replace("+++ b/psyz/src/a.c", "@@ nothing"))
    yield "status-2-minus-without-plus", two(w.run(), "`---`", "`+++`")
    w = World(root)
    shutil_rmtree(w.src / "external")
    yield "status-2-source-folder-missing", two(w.run(), "external/SDL", "is missing")
    w = World(root)
    (w.src / "external" / "SDL" / "CMakeLists.txt").unlink()
    yield "status-2-sdl-not-checked-out", two(w.run(), "SDL submodule", "is not checked out")
    w = World(root)
    (w.src / "psyz" / "CMakeLists.txt").unlink()
    yield "status-2-psyz-cmake-missing", two(w.run(), "psyz/CMakeLists.txt", "is missing")
    # The patch fits no place: refused, the copy is left unpatched and nothing is built.
    w = World(root)
    write(w.src / "psyz/src/a.c", "x\nTHREE!\ny\n")
    proc = w.run()
    yield "status-2-patch-fits-no-place", two(proc, "psyz/src/a.c", "no place", "@@ -1,3 +1,3 @@")
    yield "status-2-refused-patch-leaves-the-copy-unpatched", same((w.build / "src" / "psyz" / "src" / "a.c").read_text(), "x\nTHREE!\ny\n")
    yield "status-2-refused-patch-builds-nothing", same((w.calls(), (w.build / "obj").exists(), (w.build / "psyz.json").exists(), (w.build / "toolchain.cmake").exists()), ([], False, False, False))
    w = World(root)
    write(w.src / "psyz/src/a.c", "x\nTHREE\ny\nx\nTHREE\ny\n")
    proc = w.run()
    yield "status-2-patch-fits-two-places", two(proc, "2 places", "exactly one")
    w = World(root)
    shutil_rmtree(w.src / "psyz" / "src")
    write(w.src / "psyz/other.c", "o\n")
    yield "status-2-patched-file-missing", two(w.run(), "psyz/src/a.c", "no such file")


def shutil_rmtree(path: Path) -> None:
    import shutil

    shutil.rmtree(path)


def groups(root: Path):
    yield apply_cases(root)
    yield refuse_cases(root)
    yield blank_cases(root)
    yield commit_cases()
    yield toolchain_cases()
    yield link_cases()
    yield real_cases(root)
    yield program_cases(root)
    yield status_cases(root)


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory(prefix="psyzbuild-test-") as tmp:
        for produced in groups(Path(tmp)):
            while True:
                try:
                    name, detail = next(produced)
                except StopIteration:
                    break
                except Exception as err:  # a control must report, not crash
                    print(f"FAIL the control itself raised {type(err).__name__}: {err}")
                    failed += 1
                    break
                if detail is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {detail}")
                    failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
