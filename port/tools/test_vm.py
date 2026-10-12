#!/usr/bin/env python3
"""Controls for the macOS port's virtual machine (port/mac/vm.c, elf.c), on made-up guest code. No disc, no game.

    python3 test_vm.py [--build DIR] [--cc-guest aarch64-elf-gcc] [--keep]

Builds the guest side (port/mac/test/guest.c with `aarch64-elf-gcc -mabi=ilp32`, the host stubs of guestlink.py and the guest linker
script) and the host side (vmtest.c, vm.c, elf.c with the system's `clang -framework Hypervisor`), signs the host program ad hoc with the
hypervisor entitlement, runs it and prints its lines. The cases: a list summed through two views of the RAM, the three views alias, the
scratchpad is not the RAM, accesses of the I/O page exit with their size and value, a hostcall, a call from the host back into the
guest that calls the host again, a switch between two tasks through saved contexts, faults (an unmapped read, an undefined
instruction, a write to code, a run of data) that name their address and function and leave the machine usable, an interrupt from
another thread, and the speed of a recursive function.

Needs an Apple Silicon Mac, aarch64-elf-gcc and aarch64-elf-ld (`brew install aarch64-elf-gcc`), and clang. Without them it says so and
exits with status 3. Cost: a few seconds, of which one is the machine's own work. The exit status is the host program's.
"""

from __future__ import annotations

import argparse
import platform
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
MAC = TOOLS.parent / "mac"
sys.path.insert(0, str(TOOLS))

import guestlink  # noqa: E402

HOST_STUBS = ["port_test_add", "port_test_callback", "port_test_ctx"]


def run(argv: list[str], what: str) -> None:
    proc = subprocess.run(argv, capture_output=True, text=True)
    if proc.returncode != 0:
        print(f"FAIL {what}: {' '.join(argv[:2])} ended with {proc.returncode}\n{proc.stderr.strip()[:2000]}")
        sys.exit(2)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--build", type=Path, default=None)
    parser.add_argument("--cc-guest", default=guestlink.DEFAULT_CC)
    parser.add_argument("--keep", action="store_true")
    args = parser.parse_args()
    if platform.system() != "Darwin" or platform.machine() != "arm64":
        print("these controls need a Mac with Apple Silicon")
        return 3
    ld = args.cc_guest[:-3] + "ld" if args.cc_guest.endswith("gcc") else "aarch64-elf-ld"
    for tool in (args.cc_guest, ld, "clang", "codesign"):
        if not shutil.which(tool):
            print(f"{tool} is not installed (brew install aarch64-elf-gcc)")
            return 3
    work = args.build or Path(tempfile.mkdtemp(prefix="vmtest-"))
    work.mkdir(parents=True, exist_ok=True)
    try:
        (work / "stubs.s").write_text(guestlink.render_host_stubs(HOST_STUBS))
        (work / "guest.ld").write_text(guestlink.render_script())
        flags = [*guestlink.TARGET_FLAGS]
        run([args.cc_guest, *flags, "-O1", "-ffreestanding", "-nostdlib", "-fno-pic", "-fno-asynchronous-unwind-tables", "-c",
             str(MAC / "test" / "guest.c"), "-o", str(work / "guest.o")], "the guest's C")
        run([args.cc_guest, *flags, "-c", str(work / "stubs.s"), "-o", str(work / "stubs.o")], "the host stubs")
        run([ld, "-m", guestlink.EMULATION, "-T", str(work / "guest.ld"), str(work / "guest.o"), str(work / "stubs.o"), "-o", str(work / "guest.elf")], "the guest's link")
        host = work / "vmtest"
        run(["clang", "-O1", "-Wall", "-Wextra", "-Werror", "-o", str(host), str(MAC / "test" / "vmtest.c"), str(MAC / "vm.c"), str(MAC / "elf.c"),
             "-framework", "Hypervisor"], "the host's C")
        run(["codesign", "-s", "-", "--entitlements", str(MAC / "vm.entitlements"), "--force", str(host)], "the signature")
        proc = subprocess.run([str(host), str(work / "guest.elf")], capture_output=True, text=True, timeout=60)
        sys.stdout.write(proc.stdout)
        if proc.stderr:
            sys.stdout.write(proc.stderr)
        return proc.returncode
    finally:
        if not args.keep and not args.build:
            shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
