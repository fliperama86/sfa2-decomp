#!/usr/bin/env python3
"""Verify a cc1 binary against saved golden input/output pairs.

Each golden subdirectory holds result.json (with "compiler" and "flags"),
metrics.i (input) and metrics.s (expected output). Only two lines may differ:
the leading `.file` directive and the single `# GNU C ... compiled by ...`
comment line. Everything else must be byte-identical.
"""
import argparse
import difflib
import json
import os
import re
import subprocess
import sys
import tempfile

HOST_COMMENT = re.compile(rb"^\s*#\s*GNU C .*compiled by")


def normalize(data: bytes) -> list:
    lines = data.split(b"\n")
    out = []
    for i, ln in enumerate(lines):
        if i == 0 and ln.lstrip().startswith(b".file"):
            out.append(b"<.file>")
        elif HOST_COMMENT.match(ln):
            out.append(b"<host-comment>")
        else:
            out.append(ln)
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--cc1", required=True)
    ap.add_argument("--golden", required=True)
    ap.add_argument("--compiler", default="263")
    args = ap.parse_args()

    total = bad = 0
    for name in sorted(os.listdir(args.golden)):
        d = os.path.join(args.golden, name)
        rj = os.path.join(d, "result.json")
        if not os.path.isfile(rj):
            continue
        with open(rj) as f:
            meta = json.load(f)
        if str(meta.get("compiler")) != args.compiler:
            continue
        src, ref = os.path.join(d, "metrics.i"), os.path.join(d, "metrics.s")
        total += 1
        # A selected case with a missing artifact is a failure, never a skip:
        # otherwise an incomplete golden set could report full agreement.
        missing = [os.path.basename(p) for p in (src, ref) if not os.path.isfile(p)]
        if missing:
            bad += 1
            print(f"FAIL {name}: missing {', '.join(missing)}")
            continue
        with tempfile.TemporaryDirectory() as td:
            out = os.path.join(td, "out.s")
            p = subprocess.run([args.cc1, *meta["flags"], src, "-o", out],
                               capture_output=True)
            if p.returncode != 0 or not os.path.isfile(out):
                bad += 1
                print(f"FAIL {name}: cc1 exit {p.returncode}: "
                      f"{p.stderr.decode(errors='replace')[:200]}")
                continue
            got = open(out, "rb").read()
        want = open(ref, "rb").read()
        a, b = normalize(want), normalize(got)
        if a == b:
            print(f"OK   {name}")
        else:
            bad += 1
            print(f"DIFF {name}")
            diff = list(difflib.unified_diff(
                [x.decode(errors="replace") for x in a],
                [x.decode(errors="replace") for x in b],
                "golden", "native", lineterm="", n=2))
            for ln in diff[:30]:
                print("    " + ln)
    print(f"{total - bad}/{total} identical")
    return 1 if (bad or total == 0) else 0


if __name__ == "__main__":
    sys.exit(main())
