#!/usr/bin/env python3
"""Controls for difftest.py, with made-up states, made-up MIPS code and stand-ins.

    python test_difftest.py

No private input, no toolchain and no network: only Python, this repository
and Unicorn. Group A feeds `differences` made-up final states. Group B runs
the emulator path (`machine`, `run_once`, `test_function`) on short functions
written here as instruction words. Group C runs `main` with stand-ins for the
build and the run. Group D checks what difftest.py takes from matchbuild.py.
Group X checks the audit of --writes: what counts as made, what the original may change, the
lines that main prints, and that the default output is the one stored here.
The expected results are worked out here from each case, not read from the tool.
"""

from __future__ import annotations

import ast
import contextlib
import gc
import io
import random
import re
import struct
import sys
import tempfile
import types
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import difftest  # noqa: E402
import contracts  # noqa: E402
import matchbuild  # noqa: E402

D = difftest

_machine = D.machine


def machine_after_collection(code):
    """Each emulator reserves a large address range that is freed only when the object is collected;
    a test run under an address-space limit must not keep dead ones."""
    gc.collect()
    return _machine(code)


D.machine = machine_after_collection


@contextlib.contextmanager
def patched(target, **values):
    """Replace attributes of a module (or entries of `CONTRACTS` through `contract`) and restore them."""
    saved = {k: getattr(target, k) for k in values}
    for k, v in values.items():
        setattr(target, k, v)
    try:
        yield
    finally:
        for k, v in saved.items():
            setattr(target, k, v)


@contextlib.contextmanager
def contract_of(name: str, setup, control=None):
    had = name in contracts.CONTRACTS
    old = contracts.CONTRACTS.get(name)
    contracts.CONTRACTS[name] = contracts.Contract(setup, control or (lambda words: (0, 0, "none")))
    try:
        yield
    finally:
        if had:
            contracts.CONTRACTS[name] = old
        else:
            del contracts.CONTRACTS[name]


# ---------------------------------------------------------------------------
# Group A: the comparator


def state(**changes) -> dict:
    base = {"ram": bytearray(D.RAM_SIZE), "scratch": bytearray(D.SCRATCH_SIZE), "v0": 0,
            "saved": [0] * len(D.SAVED_NAMES), "sp": D.STACK_TOP}
    base.update(changes)
    return base


def with_byte(block: str, offset: int, value: int = 0x77) -> dict:
    s = state()
    s[block][offset] = value
    return s


def need(found: list[str], wanted: list[str]) -> str | None:
    """None when each wanted text opens exactly one line, and there are no other lines."""
    if len(found) != len(wanted) or any(not f.startswith(w) for f, w in zip(found, wanted)):
        return f"wanted lines starting {wanted}, got {found}"
    return None


def case_a_equal():
    return need(D.differences(state(), state(), True), [])


def case_a_ram_byte():
    return need(D.differences(state(), with_byte("ram", 0x1234), False), ["ram 0x80001234:"])


def case_a_scratch_byte():
    return need(D.differences(state(), with_byte("scratch", 0x10), False), ["scratchpad 0x1f800010:"])


def case_a_stack_borders():
    low, top = D.STACK_LOW - D.RAM_BASE, D.STACK_TOP - D.RAM_BASE
    for offset, shown in ((low - 1, True), (low, False), (top - 1, False), (top, True)):
        found = D.differences(state(), with_byte("ram", offset), False)
        wanted = [f"ram {D.RAM_BASE + offset:#x}:"] if shown else []
        if (why := need(found, wanted)):
            return f"offset {offset:#x}: {why}"
    return None


def case_a_first_and_last_bytes():
    for block, base, size in (("ram", D.RAM_BASE, D.RAM_SIZE), ("scratch", D.SCRATCH_BASE, D.SCRATCH_SIZE)):
        label = "ram" if block == "ram" else "scratchpad"
        for offset in (0, size - 1):
            found = D.differences(state(), with_byte(block, offset), False)
            if (why := need(found, [f"{label} {base + offset:#x}:"])):
                return f"{block} offset {offset:#x}: {why}"
    return None


def case_a_saved_registers():
    for i, name in enumerate(D.SAVED_NAMES):
        saved = [0] * len(D.SAVED_NAMES)
        saved[i] = 1
        if (why := need(D.differences(state(), state(saved=saved), False), [f"{name}:"])):
            return why
    if D.SAVED_NAMES != ("s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "fp"):
        return f"names {D.SAVED_NAMES}"
    return None


def case_a_sp():
    return need(D.differences(state(), state(sp=D.STACK_TOP - 8), False), ["sp:"])


def case_a_v0_only_when_returned():
    if (why := need(D.differences(state(), state(v0=5), True), ["v0:"])):
        return why
    return need(D.differences(state(), state(v0=5), False), [])


def case_a_two_pages():
    b = state()
    b["ram"][0x1000] = 1
    b["ram"][0x5000] = 1
    return need(D.differences(state(), b, False), ["ram 0x80001000:", "ram 0x80005000:"])


def case_a_same_page_two_bytes():
    b = state()
    b["ram"][0x1000] = 1
    b["ram"][0x1001] = 1
    return need(D.differences(state(), b, False), ["ram 0x80001000:", "ram 0x80001001:"])


def case_a_scratch_two_bytes_and_ram_together():
    b = state()
    b["scratch"][3] = 1
    b["ram"][3] = 1
    return need(D.differences(state(), b, False), ["ram 0x80000003:", "scratchpad 0x1f800003:"])


def case_a_page_straddling_stack():
    # A page that holds stack bytes and also a byte below the stack region.
    low = D.STACK_LOW - D.RAM_BASE
    b = state()
    b["ram"][low] = 1
    b["ram"][low - 1] = 1
    b["ram"][low + 5] = 1
    return need(D.differences(state(), b, False), [f"ram {D.STACK_LOW - 1:#x}:"])


# ---------------------------------------------------------------------------
# Group B: the emulator path on made-up code


def lui(rt, imm): return 0x3C000000 | rt << 16 | imm
def ori(rt, rs, imm): return 0x34000000 | rs << 21 | rt << 16 | imm
def addiu(rt, rs, imm): return 0x24000000 | rs << 21 | rt << 16 | (imm & 0xFFFF)
def sw(rt, base, off): return 0xAC000000 | base << 21 | rt << 16 | (off & 0xFFFF)
def lw(rt, base, off): return 0x8C000000 | base << 21 | rt << 16 | (off & 0xFFFF)
def j(target): return 0x08000000 | (target >> 2) & 0x03FFFFFF


ZERO, V0, T0, T1, S0, SP, RA = 0, 2, 8, 9, 16, 29, 31
JR_RA = 0x03E00008
NOP = 0

ORIGINAL = 0x80020000  # where the made-up original lies in the made-up RAM
FLAG = 0x80030000  # a word that the functions write
STACK_WORD = 0x801FE800  # inside the stack region
HOLE = 0x80300000  # physical 0x300000: between RAM and the test area, unmapped
LABEL = "func_80020000"


def returning(*body: int) -> list[int]:
    return [*body, JR_RA, NOP]  # the instruction after jr executes


def store(value: int, address: int = FLAG) -> list[int]:
    return [lui(T0, address >> 16), ori(T0, T0, address & 0xFFFF), ori(T1, ZERO, value), sw(T1, T0, 0)]


def make_code(words: list[int]) -> bytes:
    return struct.pack(f"<{len(words)}I", *words)


def made_up_ram(words: list[int]) -> bytes:
    ram = bytearray(D.RAM_SIZE)
    offset = ORIGINAL - D.RAM_BASE
    ram[offset : offset + 4 * len(words)] = make_code(words)
    return bytes(ram)


def plain_setup(returns_value: bool):
    return lambda state, rng, sym: contracts.Setup(args=(), returns_value=returns_value)


def run_pair(original: list[int], build: list[int], returns_value: bool = False, cases: int = 10, budget: int | None = None):
    cfg = types.SimpleNamespace(symbol_values={})
    with contract_of(LABEL, plain_setup(returns_value)), \
            patched(D, original_function=lambda cfg, name: (ORIGINAL, 4 * len(original)), BUDGET=budget or D.BUDGET):
        return D.test_function(cfg, LABEL, make_code(build), cases, 1, made_up_ram(original), bytes(D.SCRATCH_SIZE))


def expect(result, discarded, equal, different):
    got = result[:3]
    return None if got == (discarded, equal, different) else f"discarded/equal/different {got}, wanted {(discarded, equal, different)}"


def case_b_same_store():
    f = returning(*store(5))
    return expect(run_pair(f, f), 0, 10, 0)


def case_b_other_store():
    return expect(run_pair(returning(*store(5)), returning(*store(6))), 0, 0, 10)


def case_b_stack_content_only():
    return expect(run_pair(returning(*store(1, STACK_WORD)), returning(*store(2, STACK_WORD))), 0, 10, 0)


def case_b_other_s0():
    return expect(run_pair(returning(), returning(addiu(S0, ZERO, 7))), 0, 0, 10)


def case_b_other_sp():
    return expect(run_pair(returning(), returning(addiu(SP, SP, -16))), 0, 0, 10)


def case_b_original_never_returns():
    loop = [j(ORIGINAL), NOP]
    return expect(run_pair(loop, returning(*store(1)), budget=500), 10, 0, 0)


def case_b_original_unmapped_read():
    return expect(run_pair(returning(lui(T0, HOLE >> 16), lw(T1, T0, 0)), returning()), 10, 0, 0)


def case_b_build_unmapped_read():
    result = run_pair(returning(), returning(lui(T0, HOLE >> 16), lw(T1, T0, 0)))
    if (why := expect(result, 0, 0, 10)):
        return why
    return None if "build: fault" in "\n".join(result[3]) else f"report {result[3]}"


def case_b_build_never_returns():
    result = run_pair(returning(), [j(D.TEST_ADDRESS), NOP], budget=500)
    if (why := expect(result, 0, 0, 10)):
        return why
    return None if "build: instruction budget exceeded" in "\n".join(result[3]) else f"report {result[3]}"


def case_b_v0_ignored_when_no_value():
    return expect(run_pair(returning(addiu(V0, ZERO, 1)), returning(addiu(V0, ZERO, 2)), returns_value=False), 0, 10, 0)


def case_b_v0_compared_when_value():
    return expect(run_pair(returning(addiu(V0, ZERO, 1)), returning(addiu(V0, ZERO, 2)), returns_value=True), 0, 0, 10)


def case_b_v0_equal_when_value():
    f = returning(addiu(V0, ZERO, 1))
    return expect(run_pair(f, f, returns_value=True), 0, 10, 0)


def case_b_report_of_first_difference():
    first = run_pair(returning(*store(5)), returning(*store(6)))[3]
    ok = first and first[0] == "first difference: case 0, seed 1" and any(f"ram {FLAG:#x}:" in line for line in first)
    return None if ok else f"report {first}"


def case_b_other_fp():
    return expect(run_pair(returning(), returning(addiu(30, ZERO, 7))), 0, 0, 10)


def case_b_register_copy_is_seen():
    # The saved registers start with distinct values, so a copy of one into another shows.
    return expect(run_pair(returning(), returning(0x02208025)), 0, 0, 10)  # or s0, s1, zero


def case_b_report_is_capped():
    many = []
    for k in range(20):
        many += store(0x7F7F, FLAG + 16 * k)
    first = run_pair(returning(*many), returning(), cases=2)[3]
    ok = len(first) == 1 + 24 + 1 and first[-1].startswith("... and ") and first[-1].endswith(" more")
    return None if ok else f"report of {len(first)} lines"


def case_b_initial_registers():
    # The call starts with known sp, saved registers and ra; the code stores them where the test reads them.
    stores = [lui(T0, FLAG >> 16), sw(SP, T0, 0), sw(S0, T0, 4), sw(23, T0, 8), sw(30, T0, 12), sw(RA, T0, 16)]
    f = returning(*stores)
    uc = D.machine(make_code(f))
    state = D.State(made_up_ram(f), bytes(D.SCRATCH_SIZE))
    result = D.run_once(uc, state, ORIGINAL, contracts.Setup(args=(), returns_value=False))
    got = struct.unpack("<5I", bytes(result["ram"][FLAG - D.RAM_BASE : FLAG - D.RAM_BASE + 20]))
    want = (D.STACK_TOP, 0x5A5A0000, 0x5A5A0007, 0x5A5A0008, D.STOP_ADDRESS)
    return None if got == want else f"started with {[hex(g) for g in got]}"


def seen_values(seed: int, name: str = LABEL, cases: int = 4) -> list[int]:
    seen: list[int] = []

    def setup(state, rng, sym):
        seen.append(rng.getrandbits(32))
        return contracts.Setup(args=(), returns_value=False)

    f = returning()
    cfg = types.SimpleNamespace(symbol_values={})
    with contract_of(name, setup), patched(D, original_function=lambda cfg, n: (ORIGINAL, 8)):
        D.test_function(cfg, name, make_code(f), cases, seed, made_up_ram(f), bytes(D.SCRATCH_SIZE))
    return seen


def case_b_seed():
    one, again, other = seen_values(1), seen_values(1), seen_values(2)
    if one != again:
        return "the same seed gave other numbers"
    if one == other or set(one) & set(other):
        return "another seed gave the same numbers"
    if len(set(one)) != len(one):
        return "the cases of one run share their numbers"
    if set(seen_values(1, name="func_80020004")) & set(one):
        return "another function name gave the same numbers"
    return None


def case_b_argument_registers():
    # a0 reaches the code: the function stores a0 at FLAG; two values of a0 are told apart.
    f = returning(*store(0)[:2], sw(4, T0, 0))
    values = []

    def setup(state, rng, sym):
        values.append(rng.getrandbits(32))
        return contracts.Setup(args=(values[-1], 0, 0, 0), returns_value=False)

    cfg = types.SimpleNamespace(symbol_values={})
    code = make_code(f)
    captured = []
    real = D.run_once

    def spy(uc, st, entry, setup_, *more):
        r = real(uc, st, entry, setup_, *more)
        if entry == D.TEST_ADDRESS:
            captured.append(bytes(r["ram"][FLAG - D.RAM_BASE : FLAG - D.RAM_BASE + 4]))
        return r

    with contract_of(LABEL, setup), patched(D, original_function=lambda cfg, n: (ORIGINAL, 8), run_once=spy):
        result = D.test_function(cfg, LABEL, code, 3, 1, made_up_ram(f), bytes(D.SCRATCH_SIZE))
    if result[:3] != (0, 3, 0):
        return f"result {result[:3]}"
    wanted = [struct.pack("<I", v) for v in values]
    return None if captured == wanted else f"a0 reached the build as {captured}, wanted {wanted}"


# ---------------------------------------------------------------------------
# Group C: main, with stand-ins


SEEN = {"folders": [], "contracts": [], "entries": []}  # what the last run_main handed to the stand-ins


def run_main(argv, *, names=("func_80000001",), results=None, build=b"\0\0\0\0" * 4, original_size=16,
             control=None, load=None, build_error=None, original_error=None, executed=None, register=True,
             build_size=None, build_entry=0, cli_names=None):
    """Run difftest.main with stand-ins. Returns (status, stdout, stderr, calls of test_function).

    `results` gives the first four values of test_function per name; `executed` the offsets of the fifth
    (default: every slot of the original). `register` puts a contract for each name in CONTRACTS.
    `build` is the code bytes of the stand-in's Build; its size defaults to their length.
    """
    calls = []
    results = results or {}
    SEEN["folders"], SEEN["contracts"], SEEN["entries"] = [], [], []
    before = set(contracts.CONTRACTS)

    def test_function(cfg, name, code, cases, seed, ram, scratch, entry=0):
        calls.append((name, code, cases, seed))
        SEEN["entries"].append(entry)
        SEEN["contracts"].append(contracts.CONTRACTS.get(name))
        full = set(range(0, original_size, 4)) if executed is None else executed
        return (*results.get(name, (0, cases, 0, [])), full)

    def build_function(cfg, name, directory, folder=None):
        SEEN["folders"].append(folder)
        if build_error:
            raise build_error
        return D.Build(build, len(build) if build_size is None else build_size, build_entry)

    def original_function(cfg, name):
        if original_error:
            raise original_error
        return 0x80000000, original_size

    def load_config(path):
        if load:
            raise load
        return types.SimpleNamespace(symbol_values={})

    out, err = io.StringIO(), io.StringIO()
    stack = contextlib.ExitStack()
    with stack:
        for name in names:
            if register:
                stack.enter_context(contract_of(name, plain_setup(False), control))
        stack.enter_context(patched(matchbuild, load_config=load_config))
        stack.enter_context(patched(D, build_function=build_function, initial_memory=lambda cfg, image: b"",
                                    original_function=original_function, test_function=test_function))
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            try:
                status = D.main(["--config", "build.toml", *argv, *(names if cli_names is None else cli_names)])
            finally:
                if not register:  # contracts that main took from files
                    for name in set(contracts.CONTRACTS) - before:
                        del contracts.CONTRACTS[name]
    return status, out.getvalue(), err.getvalue(), calls


def case_c_all_equal():
    status, out, _, _ = run_main(["--cases", "7"], results={"func_80000001": (2, 5, 0, [])})
    line = out.splitlines()[0] if out else ""
    ok = status == 0 and re.fullmatch(r"func_80000001: built 16 bytes, original 16 bytes; cases 7, discarded 2, equal 5, different 0", line)
    return None if ok else f"status {status}, output {out!r}"


def case_c_line_shape():
    status, out, _, _ = run_main(["--cases", "7"], build=b"\0" * 8, original_size=12,
                                 results={"func_80000001": (1, 4, 2, ["first difference: case 3, seed 1", "ram 0x1: x"])})
    want = ["func_80000001: built 8 bytes, original 12 bytes; cases 7, discarded 1, equal 4, different 2",
            "  first difference: case 3, seed 1", "  ram 0x1: x",
            "func_80000001 coverage: 3 of 3 instruction slots of the original executed"]
    return None if out.splitlines() == want else f"output {out.splitlines()}"


def case_c_difference():
    status, _, _, _ = run_main([], results={"func_80000001": (0, 9, 1, ["x"])})
    return None if status == 1 else f"status {status}"


def case_c_no_equal_case():
    status, _, _, _ = run_main([], results={"func_80000001": (10, 0, 0, [])})
    return None if status == 1 else f"status {status}"


def case_c_control_with_differences():
    status, out, _, _ = run_main(["--control", "--cases", "10"], results={"func_80000001": (0, 7, 3, [])},
                                 control=lambda words: (1, 0xDEADBEEF, "a word"))
    lines = out.splitlines()
    ok = status == 0 and lines and lines[0] == "func_80000001 control: different 3 of 10 (expected more than 0)"
    return None if ok else f"status {status}, output {out!r}"


def case_c_control_without_difference():
    status, out, _, _ = run_main(["--control"], results={"func_80000001": (0, 10, 0, [])},
                                 control=lambda words: (1, 0xDEADBEEF, "a word"))
    ok = status == 1 and "control: different 0 of" in out
    return None if ok else f"status {status}, output {out!r}"


def case_c_control_with_discards_only():
    status, _, _, _ = run_main(["--control"], results={"func_80000001": (10, 0, 0, [])},
                               control=lambda words: (1, 0xDEADBEEF, "a word"))
    return None if status == 1 else f"status {status}"


def case_c_control_alters_one_word():
    build = make_code([0x11111111, 0x22222222, 0x33333333, 0x44444444])
    seen_words = []

    def control(words):
        seen_words.append(words)
        return 2, 0xDEADBEEF, "third word"

    _, out, _, calls = run_main(["--control"], build=build, results={"func_80000001": (0, 0, 5, [])}, control=control)
    want = make_code([0x11111111, 0x22222222, 0xDEADBEEF, 0x44444444])
    if seen_words != [[0x11111111, 0x22222222, 0x33333333, 0x44444444]]:
        return f"control was given {seen_words}"
    if len(calls) != 1 or calls[0][1] != want:
        return f"test_function got {calls[0][1].hex() if calls else None}, wanted {want.hex()}"
    return None if "  altered: third word, instruction slot 2" in out.splitlines() else f"output {out!r}"


def case_c_control_first_and_last_word():
    build = make_code([1, 2, 3])
    for index in (0, 2):
        _, _, _, calls = run_main(["--control"], build=build, results={"func_80000001": (0, 0, 5, [])},
                                  control=lambda words, index=index: (index, 9, "w"))
        want = [1, 2, 3]
        want[index] = 9
        if calls[0][1] != make_code(want):
            return f"index {index}: got {calls[0][1].hex()}"
    return None


def case_c_no_control_leaves_code_alone():
    build = make_code([1, 2, 3, 4])
    _, _, _, calls = run_main([], build=build)
    return None if calls and calls[0][1] == build else "the build was altered without --control"


def case_c_arguments_reach_the_run():
    _, _, _, calls = run_main(["--cases", "13", "--seed", "9"])
    return None if calls and calls[0][2:] == (13, 9) else f"calls {calls}"


def case_c_no_contract():
    out, errb = io.StringIO(), io.StringIO()
    with patched(matchbuild, load_config=lambda p: types.SimpleNamespace(symbol_values={})), \
            contextlib.redirect_stdout(out), contextlib.redirect_stderr(errb):
        status = D.main(["--config", "build.toml", "func_80000009"])
    ok = status == 2 and "no contract for func_80000009" in errb.getvalue() and not out.getvalue()
    return None if ok else f"status {status}, stderr {errb.getvalue()!r}"


def case_c_second_function_differs():
    names = ("func_80000001", "func_80000002")
    status, out, _, calls = run_main([], names=names, results={"func_80000002": (0, 8, 2, ["x"])})
    firsts = [l for l in out.splitlines() if ": built " in l]
    ok = (status == 1 and len(firsts) == 2 and firsts[0].startswith("func_80000001:") and "different 0" in firsts[0]
          and firsts[1].startswith("func_80000002:") and "different 2" in firsts[1] and len(calls) == 2)
    return None if ok else f"status {status}, output {out!r}"


def case_c_first_function_differs():
    names = ("func_80000001", "func_80000002")
    status, out, _, calls = run_main([], names=names, results={"func_80000001": (0, 8, 2, ["x"])})
    firsts = [l for l in out.splitlines() if ": built " in l]
    ok = status == 1 and len(firsts) == 2 and len(calls) == 2
    return None if ok else f"status {status}, output {out!r}"


def case_c_config_error():
    status, _, err, _ = run_main([], load=matchbuild.ConfigError(["bad one", "bad two"]))
    ok = status == 2 and "CONFIG ERROR: bad one" in err and "CONFIG ERROR: bad two" in err
    return None if ok else f"status {status}, stderr {err!r}"


def case_c_config_input_error():
    status, _, err, _ = run_main([], load=D.InputError("missing"))
    return None if status == 2 and "INPUT ERROR: missing" in err else f"status {status}, stderr {err!r}"


def case_c_config_os_error():
    status, _, err, _ = run_main([], load=OSError("gone"))
    return None if status == 2 and "INPUT ERROR" in err else f"status {status}, stderr {err!r}"


def case_c_original_input_error():
    status, _, err, calls = run_main([], original_error=D.InputError("odd name"))
    return None if status == 2 and "INPUT ERROR: odd name" in err and not calls else f"status {status}, stderr {err!r}"


def case_c_build_errors():
    for error in (matchbuild.StepError("step"), matchbuild.EnvironmentFailure("env"), D.InputError("src")):
        status, out, err, calls = run_main([], build_error=error)
        if status != 3 or "BUILD ERROR" not in err or calls or out:
            return f"{type(error).__name__}: status {status}, stderr {err!r}"
    return None


# ---------------------------------------------------------------------------
# Group D: what difftest.py takes from matchbuild.py


def used_names() -> list[str]:
    tree = ast.parse((HERE / "difftest.py").read_text())
    return sorted({n.attr for n in ast.walk(tree)
                   if isinstance(n, ast.Attribute) and isinstance(n.value, ast.Name) and n.value.id == "matchbuild"})


def case_d_names_exist():
    names = used_names()
    missing = [n for n in names if not hasattr(matchbuild, n)]
    if len(names) < 8:
        return f"the scan found only {names}"
    return f"matchbuild lacks {missing}" if missing else None


def case_d_declarations():
    # As build_function makes them.
    function = matchbuild.FunctionDecl("f", D.TEST_ADDRESS, 0)
    unit = matchbuild.UnitDecl(name="f", source="f.c", flags=("-O2", "-G0"), functions=(function,))
    ok = unit.functions == (function,) and unit.name == "f" and unit.flags == ("-O2", "-G0") and function.address == D.TEST_ADDRESS
    return None if ok else "the declarations do not hold what build_function gives them"


def case_d_error_classes():
    try:
        raise matchbuild.ConfigError(["a"])
    except matchbuild.ConfigError as exc:
        if exc.errors != ["a"]:
            return "ConfigError.errors"
    return None


# ---------------------------------------------------------------------------
# Group E: coverage


def beq(rs, rt, at, target):
    """beq at byte offset `at` of the function to byte offset `target` (the delay slot is at + 4)."""
    return 0x10000000 | rs << 21 | rt << 16 | ((target - at - 4) // 4) & 0xFFFF


def jal(target): return 0x0C000000 | (target >> 2) & 0x03FFFFFF


def executed_of(original: list[int], build: list[int], inputs: list[int], size: int | None = None) -> tuple:
    """Run one case per entry of `inputs` (the word at FLAG); returns the result of test_function."""
    order = iter(inputs)

    def setup(state, rng, sym):
        state.w32(FLAG, next(order))
        return contracts.Setup(args=(), returns_value=False)

    cfg = types.SimpleNamespace(symbol_values={})
    with contract_of(LABEL, setup), patched(D, original_function=lambda cfg, name: (ORIGINAL, size or 4 * len(original))):
        return D.test_function(cfg, LABEL, make_code(build), len(inputs), 1, made_up_ram(original), bytes(D.SCRATCH_SIZE))


# Two arms. Offsets: 0 lui, 4 lw, 8 beq, c delay, 10 and 14 arm A, 18 beq to the end, 1c delay,
# 20 arm B (target of the first branch), 24 jr ra, 28 delay slot.
ARMS = [lui(T0, FLAG >> 16), lw(T1, T0, 0), beq(T1, ZERO, 0x8, 0x20), NOP,
        addiu(V0, ZERO, 1), addiu(V0, V0, 1), beq(ZERO, ZERO, 0x18, 0x24), NOP,
        addiu(V0, ZERO, 2), JR_RA, NOP]
ARM_A = {0x0, 0x4, 0x8, 0xC, 0x10, 0x14, 0x18, 0x1C, 0x24, 0x28}  # input 1: the branch at 8 is not taken
ARM_B = {0x0, 0x4, 0x8, 0xC, 0x20, 0x24, 0x28}  # input 0: taken


def case_e_straight():
    f = returning(*store(5))
    got = executed_of(f, f, [0, 0])[4]
    want = set(range(0, 4 * len(f), 4))
    return None if got == want else f"executed {sorted(got)}, wanted {sorted(want)}"


def case_e_branch_never_taken():
    got = executed_of(ARMS, ARMS, [0, 0, 0])[4]
    return None if got == ARM_B else f"executed {sorted(got)}, wanted {sorted(ARM_B)}"


def case_e_other_arm():
    got = executed_of(ARMS, ARMS, [1, 1])[4]
    return None if got == ARM_A else f"executed {sorted(got)}, wanted {sorted(ARM_A)}"


def case_e_union_of_arms():
    got = executed_of(ARMS, ARMS, [0, 1])[4]
    want = set(range(0, 4 * len(ARMS), 4))
    return None if got == want and got == ARM_A | ARM_B else f"executed {sorted(got)}, wanted {sorted(want)}"


def case_e_callee_and_build_not_counted():
    # The original calls a function 0x100 bytes after its start; the build is longer than the original.
    original = [addiu(SP, SP, -8), sw(RA, SP, 4), jal(ORIGINAL + 0x100), NOP, lw(RA, SP, 4), addiu(SP, SP, 8), JR_RA, NOP]
    ram = bytearray(made_up_ram(original))
    callee = make_code(returning(addiu(V0, ZERO, 3)))
    ram[0x20100 : 0x20100 + len(callee)] = callee
    cfg = types.SimpleNamespace(symbol_values={})
    build = original + [NOP] * 8
    with contract_of(LABEL, plain_setup(False)), patched(D, original_function=lambda cfg, name: (ORIGINAL, 32)):
        got = D.test_function(cfg, LABEL, make_code(build), 3, 1, bytes(ram), bytes(D.SCRATCH_SIZE))
    want = set(range(0, 32, 4))
    if got[:3] != (0, 3, 0):
        return f"result {got[:3]}"
    return None if got[4] == want else f"executed {sorted(got[4])}, wanted {sorted(want)}"


def case_e_discarded_run_adds_nothing():
    # 0 lui, 4 lw, 8 beq to 18, c delay, 10 lui hole, 14 lw from the hole (faults), 18 jr ra, 1c delay.
    f = [lui(T0, FLAG >> 16), lw(T1, T0, 0), beq(T1, ZERO, 0x8, 0x18), NOP,
         lui(9, HOLE >> 16), lw(9, 9, 0), JR_RA, NOP]
    result = executed_of(f, f, [0, 1])
    if result[:3] != (1, 1, 0):
        return f"result {result[:3]}"
    want = {0x0, 0x4, 0x8, 0xC, 0x18, 0x1C}
    return None if result[4] == want else f"executed {sorted(result[4])}, wanted {sorted(want)}"


def case_e_nothing_when_all_discarded():
    f = returning(lui(9, HOLE >> 16), lw(9, 9, 0))
    result = executed_of(f, f, [0, 0])
    return None if result[0] == 2 and result[4] == set() else f"result {result[:3]}, executed {sorted(result[4])}"


def case_e_slots_in():
    blocks = {(0x1000 - 8, 16), (0x100C, 16)}  # one ends inside, one starts inside
    if (got := D.slots_in(blocks, 0x1000, 16)) != {0, 4, 0xC}:
        return f"cut blocks gave {sorted(got)}"
    if (got := D.slots_in({(0x1000, 8), (0x1004, 8)}, 0x1000, 32)) != {0, 4, 8}:
        return f"overlapping blocks gave {sorted(got)}"
    if (got := D.slots_in({(0x0F00, 0x100), (0x1010, 8)}, 0x1000, 16)) != set():
        return f"blocks outside gave {sorted(got)}"
    if (got := D.slots_in({(0x1000, 4)}, 0x1000, 16)) != {0}:
        return f"one slot gave {sorted(got)}"
    if (got := D.slots_in({(0x0F00, 0x400)}, 0x1000, 8)) != {0, 4}:
        return f"a block around the function gave {sorted(got)}"
    return None


def case_e_ranges_text():
    wanted = [([], ""), ([8], "+0x8"), ([0, 4, 8], "+0x0..+0x8"), ([0, 4, 0x10, 0x14, 0x18], "+0x0..+0x4, +0x10..+0x18"),
              ([4, 0x40, 0x44], "+0x4, +0x40..+0x44"), ([0x10, 0x20], "+0x10, +0x20"), ([0, 4], "+0x0..+0x4")]
    for offsets, text in wanted:
        if D.ranges_text(offsets) != text:
            return f"{offsets} gave {D.ranges_text(offsets)!r}, wanted {text!r}"
    return None


# ---------------------------------------------------------------------------
# Group F: main with the coverage line, --uncovered, --folder and contract files


def case_f_coverage_line():
    _, out, _, _ = run_main([], original_size=48, executed={0, 4, 8})
    want = "func_80000001 coverage: 3 of 12 instruction slots of the original executed"
    lines = out.splitlines()
    return None if len(lines) == 2 and lines[1] == want else f"output {lines}"


def case_f_coverage_line_follows_the_report():
    _, out, _, _ = run_main([], original_size=8, executed={0},
                            results={"func_80000001": (0, 1, 1, ["first difference: case 0, seed 1", "ram 0x1: x"])})
    lines = out.splitlines()
    ok = len(lines) == 4 and lines[0].startswith("func_80000001: built") and lines[3].endswith("1 of 2 instruction slots of the original executed")
    return None if ok else f"output {lines}"


def case_f_uncovered():
    _, out, _, _ = run_main(["--uncovered"], original_size=16, executed={0, 8})
    lines = out.splitlines()
    return None if lines[1:] == ["func_80000001 coverage: 2 of 4 instruction slots of the original executed",
                                 "  not executed: +0x4, +0xc"] else f"output {lines}"


def case_f_uncovered_run():
    _, out, _, _ = run_main(["--uncovered"], original_size=32, executed={0, 0x1C})
    return None if out.splitlines()[-1] == "  not executed: +0x4..+0x18" else f"output {out!r}"


def case_f_uncovered_full():
    _, out, _, _ = run_main(["--uncovered"], original_size=16)
    return None if "not executed" not in out and "4 of 4" in out else f"output {out!r}"


def case_f_no_uncovered_without_flag():
    _, out, _, _ = run_main([], original_size=16, executed={0})
    return None if "not executed" not in out and "1 of 4" in out else f"output {out!r}"


def case_f_control_prints_no_coverage():
    _, out, _, _ = run_main(["--control", "--uncovered"], original_size=16, executed={0},
                            results={"func_80000001": (0, 0, 5, [])}, control=lambda words: (1, 9, "w"))
    return None if "coverage" not in out and "not executed" not in out else f"output {out!r}"


def case_f_low_coverage_keeps_status():
    status, _, _, _ = run_main(["--uncovered"], original_size=400, executed={0})
    zero, _, _, _ = run_main([], original_size=400, executed=set())
    return None if status == 0 and zero == 0 else f"status {status}, {zero}"


def case_f_low_coverage_does_not_hide_a_difference():
    status, _, _, _ = run_main([], original_size=400, executed={0}, results={"func_80000001": (0, 5, 1, ["x"])})
    return None if status == 1 else f"status {status}"


def case_f_default_folder():
    run_main([])
    return None if SEEN["folders"] == [HERE] else f"build got {SEEN['folders']}"


FILE_CONTRACT = """import contracts

CONTRACT = contracts.Contract(lambda state, rng, sym: contracts.Setup(args=(7,), returns_value=True),
                              lambda words: (0, 0, "from the file"))
"""


def with_folder(text: str | None, body):
    """Call body(folder) with a temporary folder that holds func_80000005.py when `text` is given."""
    with tempfile.TemporaryDirectory() as tmp:
        if text is not None:
            (Path(tmp) / "func_80000005.py").write_text(text)
        return body(Path(tmp).resolve())


def from_file(text: str | None, name: str = "func_80000005"):
    return with_folder(text, lambda folder: (folder, run_main(["--folder", str(folder)], names=(name,), register=False),
                                             list(SEEN["folders"]), list(SEEN["contracts"])))


def case_f_folder_reaches_the_build():
    folder, (status, _, err, _), folders, _ = from_file(FILE_CONTRACT)
    return None if folders == [folder] else f"folder {folder}, build got {folders}"


def case_f_contract_file_loaded_and_used():
    _, (status, _, err, _), _, used = from_file(FILE_CONTRACT)
    if status != 0 or len(used) != 1 or used[0] is None:
        return f"status {status}, stderr {err!r}, contracts {used}"
    setup = used[0].setup(None, None, None)
    ok = setup.args == (7,) and setup.returns_value is True and used[0].control([])[2] == "from the file"
    return None if ok else "the contract in use is not the file's"


def case_f_table_wins_over_file():
    def body(folder):
        with contract_of("func_80000005", plain_setup(False), lambda words: (0, 0, "table")):
            status = run_main(["--folder", str(folder)], names=("func_80000005",), register=False)[0]
            return status, [c.control([])[2] for c in SEEN["contracts"]]

    status, which = with_folder(FILE_CONTRACT, body)
    return None if status == 0 and which == ["table"] else f"status {status}, contracts {which}"


def case_f_file_without_contract():
    _, (status, _, err, calls), _, _ = from_file("X = 1\n")
    ok = status == 2 and "func_80000005.py does not define CONTRACT" in err and not calls
    return None if ok else f"status {status}, stderr {err!r}"


def case_f_file_with_wrong_contract():
    _, (status, _, err, _), _, _ = from_file("CONTRACT = (1, 2)\n")
    return None if status == 2 and "does not define CONTRACT" in err else f"status {status}, stderr {err!r}"


def case_f_file_that_raises():
    _, (status, _, err, calls), _, _ = from_file("raise RuntimeError('made up')\n")
    ok = status == 2 and "func_80000005.py cannot be loaded: RuntimeError: made up" in err and not calls
    return None if ok else f"status {status}, stderr {err!r}"


def case_f_file_with_a_syntax_error():
    _, (status, _, err, calls), _, _ = from_file("def (:\n")
    ok = status == 2 and "func_80000005.py cannot be loaded: SyntaxError" in err and not calls
    return None if ok else f"status {status}, stderr {err!r}"


def case_f_neither_table_nor_file():
    _, (status, _, err, calls), _, _ = from_file(None)
    ok = status == 2 and "no contract for func_80000005" in err and not calls
    return None if ok else f"status {status}, stderr {err!r}"


def case_f_build_reads_the_source_from_the_folder():
    name = "func_80000007"  # no such source in the tool's own folder
    cfg = types.SimpleNamespace()
    stop = lambda *a: (None, ["stopped here"])
    with tempfile.TemporaryDirectory() as tmp:
        folder = Path(tmp)
        try:
            D.build_function(cfg, name, folder, folder)
            return "no source in the folder was accepted"
        except D.InputError as exc:
            if f"no source {name}.c in {folder.name}" not in str(exc):
                return f"message {exc}"
        (folder / f"{name}.c").write_text("")
        with patched(matchbuild, prepare_pipeline=stop):
            try:
                D.build_function(cfg, name, folder, folder)
            except matchbuild.StepError as exc:
                return None if "stopped here" in str(exc) else f"message {exc}"
            except D.InputError as exc:
                return f"the source in the folder was not found: {exc}"
    return "the build went on past a failed pipeline"


def case_f_file_of_another_function_not_used():
    _, (status, _, err, _), _, _ = from_file(FILE_CONTRACT, name="func_80000006")  # the file is for func_80000005
    return None if status == 2 and "no contract for func_80000006" in err else f"status {status}, stderr {err!r}"


# ---------------------------------------------------------------------------
# Group G: recorders for callees (contracts.CallLog)

CALLEE = 0x80021000
CALLEE_B = 0x80021100
MARK_BODY = returning(*store(0x55))  # a callee that would write a marker at FLAG


def caller(calls, tail=()):
    """A caller with a 32-byte frame; `calls` lists (callee, argument values), the fifth and later on the stack."""
    words = [addiu(SP, SP, -32), sw(RA, SP, 28)]
    for callee, args in calls:
        for i, value in enumerate(args[:4]):
            words.append(addiu(4 + i, ZERO, value))
        for i, value in enumerate(args[4:]):
            words += [addiu(T0, ZERO, value), sw(T0, SP, 16 + 4 * i)]
        words += [jal(callee), NOP]
    return [*words, *tail, lw(RA, SP, 28), addiu(SP, SP, 32), JR_RA, NOP]


def ram_with_callees(words: list[int]) -> bytes:
    ram = bytearray(made_up_ram(words))
    for address in (CALLEE, CALLEE_B):
        body = make_code(MARK_BODY)
        ram[address - D.RAM_BASE : address - D.RAM_BASE + len(body)] = body
    return bytes(ram)


def log_setup(replaces, args=(), holder=None):
    def setup(state, rng, sym):
        log = contracts.CallLog(state)
        for replace in replaces:
            log.replace(*replace)
        if holder is not None:
            holder.append(log)
        return contracts.Setup(args=args, returns_value=False)

    return setup


def run_caller(words, replaces, args=()):
    """One call of the caller on a state with the recorders. Returns (log entries, final state, log, u32 reader)."""
    holder: list = []
    state = D.State(ram_with_callees(words), bytes(D.SCRATCH_SIZE))
    setup = log_setup(replaces, args, holder)(state, None, None)
    result = D.run_once(D.machine(make_code([NOP])), state, ORIGINAL, setup)
    if isinstance(result, str):
        raise RuntimeError(result)
    log = holder[0]

    def u32(address):
        return struct.unpack_from("<I", result["ram"], address - D.RAM_BASE)[0]

    end = u32(log.cursor)
    return [u32(a) for a in range(log.entries, end, 4)], result, log, u32


def pair_with_log(original, build, replaces, args=(), cases=3):
    cfg = types.SimpleNamespace(symbol_values={})
    with contract_of(LABEL, log_setup(replaces, args)), \
            patched(D, original_function=lambda cfg, name: (ORIGINAL, 4 * len(original))):
        return D.test_function(cfg, LABEL, make_code(build), cases, 1, ram_with_callees(original), bytes(D.SCRATCH_SIZE))


def case_g_four_arguments():
    entries, result, log, u32 = run_caller(caller([(CALLEE, [1, 2, 3, 4])]), [(CALLEE, 4, 0)])
    if entries != [CALLEE, 1, 2, 3, 4]:
        return f"log {[hex(e) for e in entries]}"
    if log.entries != log.cursor + 4:
        return "the entries do not follow the cursor word"
    return None if u32(log.cursor) - log.entries == 20 else "the cursor did not move by five words"


def case_g_argument_counts():
    for n in range(4):
        entries, _, log, u32 = run_caller(caller([(CALLEE, [1, 2, 3, 4])]), [(CALLEE, n, 0)])
        if entries != [CALLEE, *range(1, n + 1)] or u32(log.cursor) - log.entries != 4 * (1 + n):
            return f"{n} arguments: log {[hex(e) for e in entries]}"
    return None


def leftover_pair(n_recorded: int, index: int):
    """Original and build differ in argument register `index` only."""
    def code(value):
        values = [1, 2, 3, 4]
        values[index] = value
        return caller([(CALLEE, values)])

    return pair_with_log(code(5), code(6), [(CALLEE, n_recorded, 0)])


def case_g_unrecorded_argument_is_no_difference():
    for n in range(4):
        got = leftover_pair(n, n)[:3]  # register `n` is the first one that is not recorded
        if got != (0, 3, 0):
            return f"{n} recorded, register a{n} differs: {got}"
    return None


def case_g_recorded_argument_is_a_difference():
    for n in range(1, 5):
        got = leftover_pair(n, n - 1)[:3]  # register n - 1 is the last one that is recorded
        if got != (0, 0, 3):
            return f"{n} recorded, register a{n - 1} differs: {got}"
    return None


def case_g_stack_arguments():
    entries, _, _, _ = run_caller(caller([(CALLEE, [1, 2, 3, 4, 55, 66])]), [(CALLEE, 6, 0)])
    if entries != [CALLEE, 1, 2, 3, 4, 55, 66]:
        return f"log {[hex(e) for e in entries]}"
    entries, _, _, _ = run_caller(caller([(CALLEE, [1, 2, 3, 4, 55, 66])]), [(CALLEE, 5, 0)])
    return None if entries == [CALLEE, 1, 2, 3, 4, 55] else f"five arguments: log {[hex(e) for e in entries]}"


def case_g_stack_argument_difference():
    a = caller([(CALLEE, [1, 2, 3, 4, 55, 66])])
    b = caller([(CALLEE, [1, 2, 3, 4, 55, 67])])
    got = (pair_with_log(a, b, [(CALLEE, 6, 0)])[:3], pair_with_log(a, b, [(CALLEE, 5, 0)])[:3])
    return None if got == ((0, 0, 3), (0, 3, 0)) else f"results {got}"


def case_g_result_in_v0():
    for result in (0x12345678, 0xFFFFFFFF, 0, 0x80000001, 0x0000FFFF, 0xFFFF0000):
        _, final, _, _ = run_caller(caller([(CALLEE, [])]), [(CALLEE, 0, result)])
        if final["v0"] != result:
            return f"result {result:#x} came back as {final['v0']:#x}"
    return None


def case_g_order_and_values():
    original = caller([(CALLEE, [1]), (CALLEE_B, [2]), (CALLEE, [3])])
    replaces = [(CALLEE, 1, 0), (CALLEE_B, 1, 0)]
    entries = run_caller(original, replaces)[0]
    if entries != [CALLEE, 1, CALLEE_B, 2, CALLEE, 3]:
        return f"log {[hex(e) for e in entries]}"
    variants = {
        "the same": (original, (0, 3, 0)),
        "another order": (caller([(CALLEE_B, [2]), (CALLEE, [1]), (CALLEE, [3])]), (0, 0, 3)),
        "one call omitted": (caller([(CALLEE, [1]), (CALLEE, [3])]), (0, 0, 3)),
        "another value": (caller([(CALLEE, [1]), (CALLEE_B, [9]), (CALLEE, [3])]), (0, 0, 3)),
        "another callee": (caller([(CALLEE, [1]), (CALLEE, [2]), (CALLEE, [3])]), (0, 0, 3)),
        "an extra call": (caller([(CALLEE, [1]), (CALLEE_B, [2]), (CALLEE, [3]), (CALLEE, [3])]), (0, 0, 3)),
    }
    for what, (build, want) in variants.items():
        got = pair_with_log(original, build, replaces)[:3]
        if got != want:
            return f"{what}: {got}, wanted {want}"
    return None


def case_g_callee_body_not_run():
    _, final, _, u32 = run_caller(caller([(CALLEE, [])]), [(CALLEE, 0, 0)])
    # The marker would be written by the callee's own code.
    marker = u32(FLAG)
    plain = D.run_once(D.machine(make_code([NOP])), D.State(ram_with_callees(caller([(CALLEE, [])])), bytes(D.SCRATCH_SIZE)),
                       ORIGINAL, contracts.Setup(args=(), returns_value=False))
    unreplaced = struct.unpack_from("<I", plain["ram"], FLAG - D.RAM_BASE)[0]
    if unreplaced != 0x55:
        return f"without a recorder the callee wrote {unreplaced:#x}, so this control does not see it"
    return None if marker == 0 else f"the marker {marker:#x} was written"


def case_g_two_slot_callee():
    state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    state.w32(CALLEE, 0x11111111)
    state.w32(CALLEE + 4, 0x22222222)
    state.w32(CALLEE + 8, 0xDEADBEEF)
    state.w32(CALLEE - 4, 0xCAFEF00D)
    contracts.CallLog(state).replace(CALLEE, 2, 7)
    after, before = struct.unpack("<I", state.read(CALLEE + 8, 4))[0], struct.unpack("<I", state.read(CALLEE - 4, 4))[0]
    first = struct.unpack("<I", state.read(CALLEE, 4))[0]
    second = struct.unpack("<I", state.read(CALLEE + 4, 4))[0]
    if after != 0xDEADBEEF or before != 0xCAFEF00D:
        return f"neighbouring words became {before:#x} and {after:#x}"
    return None if first >> 26 == 2 and second == 0 else f"words {first:#x} {second:#x}"


def case_g_registers_left_alone():
    tail = [lui(T0, FLAG >> 16), sw(RA, T0, 0), sw(SP, T0, 4)]
    words = caller([(CALLEE, [1, 2, 3, 4, 5, 6])], tail)
    _, final, _, u32 = run_caller(words, [(CALLEE, 6, 9)])
    jal_at = ORIGINAL + 4 * (2 + 4 + 4 + 0)  # after the frame (2), four addiu and two stack arguments (4)
    if u32(FLAG) != jal_at + 8:
        return f"ra after the call {u32(FLAG):#x}, wanted {jal_at + 8:#x}"
    if u32(FLAG + 4) != D.STACK_TOP - 32:
        return f"sp after the call {u32(FLAG + 4):#x}"
    wanted = [0x5A5A0000 + i for i in range(len(D.SAVED))]
    return None if final["saved"] == wanted and final["sp"] == D.STACK_TOP else f"saved {final['saved']}"


def case_g_log_is_compared():
    # The log is in RAM, so a build that leaves it out differs only through it.
    a = caller([(CALLEE, [1])])
    b = caller([])
    got = pair_with_log(a, b, [(CALLEE, 1, 0)])[:3]
    return None if got == (0, 0, 3) else f"result {got}"


# ---------------------------------------------------------------------------
# Group H: translated code must not outlive the rewrite of RAM


def case_h_recorder_result_per_case():
    uc = D.machine(make_code([NOP]))
    words = caller([(CALLEE, [])])
    for result in (0x11111111, 0x22222222, 0xFFFFFFFF, 0):
        state = D.State(ram_with_callees(words), bytes(D.SCRATCH_SIZE))
        contracts.CallLog(state).replace(CALLEE, 0, result)
        final = D.run_once(uc, state, ORIGINAL, contracts.Setup(args=(), returns_value=True))
        if isinstance(final, str) or final["v0"] != result:
            return f"result {result:#x} came back as {final if isinstance(final, str) else hex(final['v0'])}"
    return None


# 0 addiu sp, 4 sw ra, 8 jal callee, c delay, 10 beq v0 to arm B, 14 delay, arm A 18..24 and a branch to the end
# at 28 (2c delay), arm B 30..3c, end 40 lw ra, 44 addiu sp, 48 jr ra, 4c delay.
def two_arm_caller(a: int, b: int) -> list[int]:
    return [addiu(SP, SP, -8), sw(RA, SP, 4), jal(CALLEE), NOP, beq(V0, ZERO, 0x10, 0x30), NOP,
            *store(a), beq(ZERO, ZERO, 0x28, 0x40), NOP, *store(b), lw(RA, SP, 4), addiu(SP, SP, 8), JR_RA, NOP]


def case_h_recorder_result_reaches_both_arms():
    def setup(state, rng, sym):
        contracts.CallLog(state).replace(CALLEE, 0, rng.getrandbits(1))
        return contracts.Setup(args=(), returns_value=False)

    original, build = two_arm_caller(1, 2), two_arm_caller(9, 2)  # the build is wrong in arm A only
    cfg = types.SimpleNamespace(symbol_values={})
    with contract_of(LABEL, setup), patched(D, original_function=lambda cfg, name: (ORIGINAL, 4 * len(original))):
        result = D.test_function(cfg, LABEL, make_code(build), 12, 1, ram_with_callees(original), bytes(D.SCRATCH_SIZE))
    discarded, equal, different, _, executed = result
    if executed != set(range(0, 4 * len(original), 4)):
        return f"coverage {len(executed)} of {len(original)}: one arm only was reached"
    return None if discarded == 0 and equal > 0 and different > 0 and equal + different == 12 else f"result {result[:3]}"


def case_h_code_written_by_setup_runs_as_written():
    at = 0x80050000
    uc = D.machine(make_code([NOP]))
    for k in (3, 7, 11, 0x1234):
        state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
        for i, word in enumerate(returning(addiu(V0, ZERO, k))):
            state.w32(at + 4 * i, word)
        final = D.run_once(uc, state, at, contracts.Setup(args=(), returns_value=True))
        if isinstance(final, str) or final["v0"] != k:
            return f"code for {k} ran as {final if isinstance(final, str) else hex(final['v0'])}"
    return None


# ---------------------------------------------------------------------------
# Group I: the build result (Build) and J: addresses


def case_i_main_uses_size_and_entry():
    code = make_code([1, 2, 3, 4, 5, 6])
    status, out, _, calls = run_main(["--cases", "5"], build=code, build_size=16, build_entry=8, original_size=16)
    line = out.splitlines()[0]
    ok = status == 0 and line.startswith("func_80000001: built 16 bytes, original 16 bytes;") \
        and SEEN["entries"] == [8] and calls[0][1] == code
    return None if ok else f"status {status}, line {line!r}, entries {SEEN['entries']}"


def case_i_entry_starts_the_build_there():
    helper = returning(*store(0x55))  # would leave a marker at FLAG
    real = returning(*store(5))
    original = returning(*store(5))
    cfg = types.SimpleNamespace(symbol_values={})
    code = make_code(helper + real)
    with contract_of(LABEL, plain_setup(False)), patched(D, original_function=lambda cfg, name: (ORIGINAL, 4 * len(original))):
        at_entry = D.test_function(cfg, LABEL, code, 4, 1, made_up_ram(original), bytes(D.SCRATCH_SIZE), 4 * len(helper))
        at_start = D.test_function(cfg, LABEL, code, 4, 1, made_up_ram(original), bytes(D.SCRATCH_SIZE))
    if at_entry[:3] != (0, 4, 0):
        return f"with the entry: {at_entry[:3]}"
    return None if at_start[:3] == (0, 0, 4) else f"without the entry: {at_start[:3]}"


def case_i_control_sees_the_code_only():
    code = make_code([0x11, 0x22, 0x33, 0x44, 0xAAAA, 0xBBBB])  # four words of code, two of read-only data
    seen = []

    def control(words):
        seen.append(list(words))
        return 3, 0xDEADBEEF, "last code word"

    _, out, _, calls = run_main(["--control"], build=code, build_size=16, control=control,
                                results={"func_80000001": (0, 0, 5, [])})
    want = make_code([0x11, 0x22, 0x33, 0xDEADBEEF, 0xAAAA, 0xBBBB])
    if seen != [[0x11, 0x22, 0x33, 0x44]]:
        return f"control saw {seen}"
    return None if calls and calls[0][1] == want else f"test_function got {calls[0][1].hex() if calls else None}"


def case_i_control_outside_the_code():
    code = make_code([0x11, 0x22, 0x33, 0x44, 0xAAAA, 0xBBBB])
    for index in (4, 5, -1):
        status, out, err, calls = run_main(["--control"], build=code, build_size=16,
                                           control=lambda words, index=index: (index, 1, "w"))
        if status != 2 or calls or "INPUT ERROR" not in err or out:
            return f"index {index}: status {status}, stderr {err!r}, calls {len(calls)}"
    return None


def elf_object(text: int, data: int, common: int) -> bytes:
    """A relocatable MIPS ELF object with .text and .sdata of the given sizes and one common symbol."""
    names = b"\0.text\0.sdata\0.symtab\0.strtab\0.shstrtab\0"
    where = {n: names.index(b"." + n.encode()) for n in ("text", "sdata", "symtab", "strtab", "shstrtab")}
    symbols = bytes(16) + struct.pack("<IIIBBH", 1, 4, common, 0x11, 0, 0xFFF2)
    bodies = [(b"", 0, 0, 0), (bytes(text), 1, 0, 0), (bytes(data), 1, 0, 0), (symbols, 2, 4, 16), (b"\0c\0", 3, 0, 0), (names, 3, 0, 0)]
    offset, parts, headers = 52, [], [bytes(40)]
    for index, (body, kind, link, entsize) in enumerate(bodies[1:], 1):
        key = ("text", "sdata", "symtab", "strtab", "shstrtab")[index - 1]
        headers.append(struct.pack("<IIIIIIIIII", where[key], kind, 0, 0, offset, len(body), link, 0, 4, entsize))
        parts.append(body)
        offset += len(body)
    header = b"\x7fELF" + bytes([1, 1, 1, 0]) + bytes(8) + struct.pack("<HHIIIIIHHHHHH", 1, 8, 1, 0, 0, offset, 0, 52, 0, 0, 40, len(headers), 5)
    return header + b"".join(parts) + b"".join(headers)


def case_i_section_sizes():
    with tempfile.TemporaryDirectory() as tmp:
        for text, data, common in ((16, 4, 0), (32, 0, 8), (0, 12, 4)):
            path = Path(tmp) / "unit.o"
            path.write_bytes(elf_object(text, data, common))
            got = D.section_sizes(path)
            want = {".text": text, ".sdata": data}
            if any(got.get(k) != v for k, v in want.items()) or got.get("COMMON", 0) != common:
                return f"({text}, {data}, {common}) gave {got}"
    return None


def case_j_addresses():
    function = lambda n, a: types.SimpleNamespace(name=n, address=a)
    cfg = types.SimpleNamespace(symbol_values={"sym_a": 1, "both": 2},
                                units=[types.SimpleNamespace(functions=(function("both", 3), function("fn_b", 4))),
                                       types.SimpleNamespace(functions=(function("fn_c", 5),))])
    want = {"sym_a": 1, "both": 2, "fn_b": 4, "fn_c": 5}
    return None if D.addresses(cfg) == want else f"{D.addresses(cfg)}"


def case_j_addresses_without_units():
    for cfg in (types.SimpleNamespace(symbol_values={"x": 7}), types.SimpleNamespace(symbol_values={"x": 7}, units=[])):
        if D.addresses(cfg) != {"x": 7}:
            return f"{D.addresses(cfg)}"
    return None


def case_j_setup_is_given_the_table():
    got = []

    def setup(state, rng, sym):
        got.append(dict(sym))
        return contracts.Setup(args=(), returns_value=False)

    f = returning()
    cfg = types.SimpleNamespace(symbol_values={"s": 1, "w": 2},
                                units=[types.SimpleNamespace(functions=(types.SimpleNamespace(name="w", address=9),
                                                                         types.SimpleNamespace(name="fn", address=8)))])
    with contract_of(LABEL, setup), patched(D, original_function=lambda cfg, name: (ORIGINAL, 4)):
        D.test_function(cfg, LABEL, make_code(f), 1, 1, made_up_ram(f), bytes(D.SCRATCH_SIZE))
    return None if got == [{"fn": 8, "w": 2, "s": 1}] else f"setup was given {got}"


# ---------------------------------------------------------------------------
# Group K: --all


ALL_NAMES = ("func_80000001", "func_80000002", "func_80000003")


def all_folder(body, sources=("func_80000003.c", "func_80000001.c", "helper.c", "func_x.py", "func_y.txt", "func_80000002.c")):
    with tempfile.TemporaryDirectory() as tmp:
        for source in sources:
            (Path(tmp) / source).write_text("")
        return body(Path(tmp).resolve())


def case_k_all_runs_each_source_once_in_order():
    status, out, _, calls = all_folder(lambda folder: run_main(["--folder", str(folder), "--all"], names=ALL_NAMES, cli_names=()))
    lines = [l for l in out.splitlines() if ": built " in l]
    ok = status == 0 and [c[0] for c in calls] == list(ALL_NAMES) and [l.split(":")[0] for l in lines] == list(ALL_NAMES)
    return None if ok else f"status {status}, ran {[c[0] for c in calls]}"


def case_k_all_with_a_name():
    status, out, _, calls = all_folder(lambda folder: run_main(["--folder", str(folder), "--all"], names=ALL_NAMES,
                                                              cli_names=("func_80000001",)))
    return None if status == 2 and not calls and not out else f"status {status}, calls {len(calls)}"


def case_k_neither_all_nor_name():
    status, out, err, calls = all_folder(lambda folder: run_main(["--folder", str(folder)], names=ALL_NAMES, cli_names=()))
    return None if status == 2 and not calls and not out and "INPUT ERROR" in err else f"status {status}, calls {len(calls)}"


def case_k_all_without_sources():
    status, out, err, calls = all_folder(lambda folder: run_main(["--folder", str(folder), "--all"], names=ALL_NAMES, cli_names=()),
                                         sources=("helper.c", "func_80000001.py", "func_80000002.txt", "xfunc_80000003.c"))
    return None if status == 2 and not calls and "INPUT ERROR" in err else f"status {status}, calls {len(calls)}"


def case_k_all_with_control():
    seen = []

    def control(words):
        seen.append(len(words))
        return 1, 9, "w"

    status, out, _, calls = all_folder(lambda folder: run_main(["--folder", str(folder), "--all", "--control"], names=ALL_NAMES,
                                                              cli_names=(), control=control, results={n: (0, 0, 5, []) for n in ALL_NAMES}))
    lines = [l for l in out.splitlines() if " control: " in l]
    ok = status == 0 and len(seen) == 3 and [c[0] for c in calls] == list(ALL_NAMES) and [l.split()[0] for l in lines] == list(ALL_NAMES)
    return None if ok else f"status {status}, controls {len(seen)}, ran {[c[0] for c in calls]}"


# ---------------------------------------------------------------------------
# Group L: the symbol file of the standalone link, and where the link put what the unit defines


SYMBOL_TEXT = (
    "/* header: func_801e9080 = 0x1; */\n"
    "func_801e9080 = 0x801e9080;\n"
    "other_name=0x80010000;\n"
    "   spaced   =   0x1;\n"
    "\n"
    "PROVIDE(provided = 0x2);\n"
    "PROVIDE ( prov2 = 0x3 );\n"
    "func_801e9080_slot06_00 = 0x801e9080;\n"
    "func_801e9 = 0x4;\n"
    "tabbed\\t=\\t0x5;\n"
    "# not an assignment: nothing = here\n"
).replace("\\t", "\t")


def lines_of(text: str) -> list[str]:
    return text.splitlines()


def case_l_drops_defined_names_in_all_forms():
    got = lines_of(D.without_assignments(SYMBOL_TEXT, {"func_801e9080", "spaced", "provided", "prov2", "tabbed"}))
    want = ["/* header: func_801e9080 = 0x1; */", "other_name=0x80010000;", "",
            "func_801e9080_slot06_00 = 0x801e9080;", "func_801e9 = 0x4;", "# not an assignment: nothing = here"]
    return None if got == want else f"kept {got}"


def case_l_keeps_names_that_only_begin_alike():
    got = lines_of(D.without_assignments(SYMBOL_TEXT, {"func_801e9080_slot06_00"}))
    if "func_801e9080 = 0x801e9080;" not in got or "func_801e9080_slot06_00 = 0x801e9080;" in got:
        return f"the longer name was not the one dropped: {got}"
    got = lines_of(D.without_assignments(SYMBOL_TEXT, {"func_801e9080"}))
    if "func_801e9080_slot06_00 = 0x801e9080;" not in got or "func_801e9080 = 0x801e9080;" in got:
        return f"the shorter name was not the one dropped: {got}"
    return None if len(got) == len(lines_of(SYMBOL_TEXT)) - 1 else f"{len(got)} lines kept"


def case_l_keeps_names_that_end_alike():
    got = D.without_assignments("xslab = 1;\nslab = 2;\nslabx = 3;\n", {"slab"})
    return None if got == "xslab = 1;\nslabx = 3;\n" else f"kept {got!r}"


def case_l_keeps_comments_blanks_and_the_rest():
    got = D.without_assignments(SYMBOL_TEXT, {"nothing", "func_801e9"})
    want = SYMBOL_TEXT.replace("func_801e9 = 0x4;\n", "")
    return None if got == want else f"kept {got!r}"


def case_l_no_names_no_change():
    for text in (SYMBOL_TEXT, SYMBOL_TEXT.replace("\n", "\r\n"), SYMBOL_TEXT.rstrip("\n"), ""):
        for names in (set(), {"absent"}):
            if D.without_assignments(text, names) != text:
                return f"text of {len(text)} characters changed for {names}"
    return None


def case_l_keeps_line_endings():
    text = "a = 1;\r\nb = 2;\r\nc = 3;"
    got = D.without_assignments(text, {"b"})
    return None if got == "a = 1;\r\nc = 3;" else f"got {got!r}"


def case_l_misplaced_borders():
    low, high = 0x80400000, 0x80400100
    linked = {"inside": low + 0x10, "first": low, "last": high - 4, "end": high, "below": low - 4, "far": 0x801E9080}
    got = D.misplaced(linked, {"inside", "first", "last", "end", "below", "far", "unknown"}, low, high)
    want = ["below at 0x803ffffc", "end at 0x80400100", "far at 0x801e9080", "unknown nowhere"]
    return None if got == want else f"got {got}"


def case_l_misplaced_nothing_to_report():
    low, high = 0x80400000, 0x80400100
    ok = D.misplaced({"a": low, "b": high - 1, "other": 5}, {"a", "b"}, low, high) == [] and D.misplaced({}, set(), low, high) == []
    return None if ok else "a name inside the range, or no name, was reported"


def case_l_misplaced_only_the_unit_names():
    # A name that the unit does not define may lie anywhere.
    return None if D.misplaced({"mine": 0x80400004, "theirs": 0x801E9080}, {"mine"}, 0x80400000, 0x80400100) == [] else "reported a foreign name"


def elf_with_symbols(symbols: list[tuple[str, int]] | None) -> bytes:
    """A linked-looking ELF (type EXEC) with .text, and a symbol table when `symbols` is given (None: no table).
    A symbol with an empty name stands for the unnamed entries."""
    shstr = b"\0.text\0.symtab\0.strtab\0.shstrtab\0"
    text = bytes(8)
    strtab, entries = b"\0", bytes(16)
    for name, value in symbols or []:
        at = len(strtab) if name else 0
        strtab += name.encode() + b"\0" if name else b""
        entries += struct.pack("<IIIBBH", at, value, 4, 0x12, 0, 1)
    bodies = [(text, 1, 0, 0, 1), (entries, 2, 3, 16, 7), (strtab, 3, 0, 0, 15), (shstr, 3, 0, 0, 23)]
    if symbols is None:
        shstr = b"\0.text\0.shstrtab\0"
        bodies = [bodies[0], (shstr, 3, 0, 0, 7)]
        shstr_index = 2
    else:
        shstr_index = 4
    offset, parts, headers = 52, [], [bytes(40)]
    for body, kind, link, entsize, name_at in bodies:
        headers.append(struct.pack("<IIIIIIIIII", name_at, kind, 0, 0, offset, len(body), link, 0, 4, entsize))
        parts.append(body)
        offset += len(body)
    header = b"\x7fELF" + bytes([1, 1, 1, 0]) + bytes(8) + struct.pack("<HHIIIIIHHHHHH", 2, 8, 1, 0, 0, offset, 0, 52, 0, 0, 40, len(headers), shstr_index)
    return header + b"".join(parts) + b"".join(headers)


def case_l_linked_addresses():
    with tempfile.TemporaryDirectory() as tmp:
        path = Path(tmp) / "image.elf"
        path.write_bytes(elf_with_symbols([("alpha", 0x80400000), ("", 0x1234), ("beta_gamma", 0x801E9080)]))
        got = D.linked_addresses(path)
        if got != {"alpha": 0x80400000, "beta_gamma": 0x801E9080}:
            return f"got {got}"
        path.write_bytes(elf_with_symbols(None))
        return None if D.linked_addresses(path) == {} else f"without a table: {D.linked_addresses(path)}"


# ---------------------------------------------------------------------------
# Groups N, O, P: pointees and watched blocks of CallLog

BUF = 0x80060000  # blocks that the made-up callers fill for a call
BUF2 = 0x80060100
WATCHED = 0x80030100
WATCHED2 = 0x80030200
RESULTS = 0x80030400


def setreg(reg, value):
    return [lui(reg, value >> 16), ori(reg, reg, value & 0xFFFF)]


def stw(address, value):
    return [*setreg(T0, address), *setreg(T1, value), sw(T1, T0, 0)]


def program(*ops):
    """A caller. ("st", address, value) stores a word; ("call", callee, [a0..a3 values], [stack argument values])."""
    words = [addiu(SP, SP, -32), sw(RA, SP, 28)]
    for op in ops:
        if op[0] == "st":
            words += stw(op[1], op[2])
        else:
            _, callee, registers, stack = op
            for i, value in enumerate(stack):
                words += [*setreg(T1, value), sw(T1, SP, 16 + 4 * i)]
            for i, value in enumerate(registers):
                words += setreg(4 + i, value)
            words += [jal(callee), NOP]
    return [*words, lw(RA, SP, 28), addiu(SP, SP, 32), JR_RA, NOP]


def fill_block(address, *values):
    return [("st", address + 4 * i, v) for i, v in enumerate(values)]


def logged(words, replaces, watch=()):
    """Entries of the log after one run of the caller `words`."""
    holder: list = []

    def setup(state, rng, sym):
        log = contracts.CallLog(state, watch=watch)
        for replace in replaces:
            replace(log) if callable(replace) else log.replace(*replace)
        holder.append(log)
        return contracts.Setup(args=(), returns_value=False)

    state = D.State(ram_with_callees(words), bytes(D.SCRATCH_SIZE))
    setup_ = setup(state, None, None)
    result = D.run_once(D.machine(make_code([NOP])), state, ORIGINAL, setup_)
    if isinstance(result, str):
        raise RuntimeError(result)
    log = holder[0]
    read = lambda address: struct.unpack_from("<I", result["ram"], address - D.RAM_BASE)[0]
    return [read(a) for a in range(log.entries, read(log.cursor), 4)], result, read


def pair_logged(original, build, replaces, watch=(), cases=3):
    def setup(state, rng, sym):
        log = contracts.CallLog(state, watch=watch)
        for replace in replaces:
            replace(log) if callable(replace) else log.replace(*replace)
        return contracts.Setup(args=(), returns_value=False)

    cfg = types.SimpleNamespace(symbol_values={})
    with contract_of(LABEL, setup), patched(D, original_function=lambda cfg, name: (ORIGINAL, 4 * len(original))):
        return D.test_function(cfg, LABEL, make_code(build), cases, 1, ram_with_callees(original), bytes(D.SCRATCH_SIZE))[:3]


def hexes(values):
    return [hex(v) for v in values]


def case_n_register_pointee():
    words = program(*fill_block(BUF, 0xA0, 0xA1, 0xA2), ("st", BUF + 12, 0xEEEE), ("call", CALLEE, [BUF], []),
                    *fill_block(BUF, 0xB0, 0xB1, 0xB2), ("call", CALLEE, [BUF], []))
    entries = logged(words, [(CALLEE, 1, 0, {0: 3})])[0]
    want = [CALLEE, BUF, 0xA0, 0xA1, 0xA2, CALLEE, BUF, 0xB0, 0xB1, 0xB2]
    return None if entries == want else f"log {hexes(entries)}"


def case_n_stack_pointee():
    words = program(*fill_block(BUF, 0xA0, 0xA1, 0xFFFF), ("call", CALLEE, [1, 2, 3, 4], [BUF]),
                    *fill_block(BUF, 0xB0, 0xB1, 0xFFFF), ("call", CALLEE, [1, 2, 3, 4], [BUF]))
    entries = logged(words, [(CALLEE, 5, 0, {4: 2})])[0]
    want = [CALLEE, 1, 2, 3, 4, BUF, 0xA0, 0xA1, CALLEE, 1, 2, 3, 4, BUF, 0xB0, 0xB1]
    return None if entries == want else f"log {hexes(entries)}"


def case_n_pointees_in_index_order():
    words = program(*fill_block(BUF, 0x10, 0x11), *fill_block(BUF2, 0x20, 0x21), *fill_block(BUF2 + 0x80, 0x30),
                    ("call", CALLEE, [BUF2, BUF], []), ("call", CALLEE_B, [BUF, 0, 0, 0], [BUF2 + 0x80]))
    entries = logged(words, [(CALLEE, 2, 0, {1: 2, 0: 2}), (CALLEE_B, 5, 0, {4: 1, 0: 1})])[0]
    want = [CALLEE, BUF2, BUF, 0x20, 0x21, 0x10, 0x11, CALLEE_B, BUF, 0, 0, 0, BUF2 + 0x80, 0x10, 0x30]
    return None if entries == want else f"log {hexes(entries)}"


def case_n_pointee_seen_through_test_function():
    original = program(*fill_block(BUF, 1), ("call", CALLEE, [BUF], []), *fill_block(BUF, 2), ("call", CALLEE, [BUF], []))
    build = program(*fill_block(BUF, 9), ("call", CALLEE, [BUF], []), *fill_block(BUF, 2), ("call", CALLEE, [BUF], []))
    with_pointee = pair_logged(original, build, [(CALLEE, 1, 0, {0: 1})])
    without = pair_logged(original, build, [(CALLEE, 1, 0)])
    if with_pointee != (0, 0, 3):
        return f"with the pointee: {with_pointee}"
    return None if without == (0, 3, 0) else f"without the pointee (the gap that the feature closes): {without}"


def case_n_refused_pointees():
    state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    log = contracts.CallLog(state)
    for arguments, pointees in ((1, {1: 1}), (1, {-1: 1}), (0, {0: 1}), (2, {2: 4}), (1, {0: 0}), (1, {0: -3})):
        try:
            log.replace(CALLEE, arguments, 0, pointees)
        except ValueError:
            continue
        return f"{arguments} arguments with pointees {pointees} was accepted"
    log.replace(CALLEE, 2, 0, {1: 1})  # the last argument with one word is fine
    return None


def case_o_watched_blocks_at_every_call():
    words = program(*fill_block(BUF, 0xC0), *fill_block(WATCHED, 7, 8), *fill_block(WATCHED2, 9),
                    ("call", CALLEE, [BUF], []), *fill_block(WATCHED, 70), ("call", CALLEE, [BUF], []))
    entries = logged(words, [(CALLEE, 1, 0, {0: 1})], watch=((WATCHED, 2), (WATCHED2, 1)))[0]
    want = [CALLEE, BUF, 0xC0, 7, 8, 9, CALLEE, BUF, 0xC0, 70, 8, 9]
    if entries != want:
        return f"log {hexes(entries)}"
    swapped = logged(words, [(CALLEE, 1, 0, {0: 1})], watch=((WATCHED2, 1), (WATCHED, 2)))[0]
    return None if swapped[:6] == [CALLEE, BUF, 0xC0, 9, 7, 8] else f"watched blocks in the other order: {hexes(swapped)}"


def case_o_order_of_store_and_call():
    original = program(*fill_block(WATCHED, 5), ("call", CALLEE, [], []))
    build = program(("call", CALLEE, [], []), *fill_block(WATCHED, 5))
    watched = pair_logged(original, build, [(CALLEE, 0, 0)], watch=((WATCHED, 1),))
    plain = pair_logged(original, build, [(CALLEE, 0, 0)])
    if watched != (0, 0, 3):
        return f"with the block watched: {watched}"
    return None if plain == (0, 3, 0) else f"without the watch (the gap that the feature closes): {plain}"


def case_o_refused_watches():
    state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    for watch in (((WATCHED + 2, 1),), ((WATCHED, 0),), ((WATCHED, -1),), ((WATCHED, 1), (WATCHED2 + 1, 2)), ((WATCHED + 1, 0),)):
        try:
            contracts.CallLog(state, watch=watch)
        except ValueError:
            continue
        return f"the watch {watch} was accepted"
    contracts.CallLog(state, watch=((WATCHED, 1), (WATCHED2, 3)))
    return None


def case_o_cursor_moves_by_the_whole_entry():
    words = program(*fill_block(BUF, 1, 2, 3), *fill_block(WATCHED, 4, 5),
                    ("call", CALLEE, [BUF, 7], []), ("call", CALLEE_B, [9], []), ("call", CALLEE, [BUF, 8], []))
    entries, _, _ = logged(words, [(CALLEE, 2, 0, {0: 3}), (CALLEE_B, 1, 0)], watch=((WATCHED, 2),))
    # A: address, two arguments, three pointee words, two watched words = 8; B: address, one argument, two watched = 4.
    want = [CALLEE, BUF, 7, 1, 2, 3, 4, 5, CALLEE_B, 9, 4, 5, CALLEE, BUF, 8, 1, 2, 3, 4, 5]
    return None if entries == want else f"log {hexes(entries)}"


def case_o_no_watch_no_extra_words():
    words = program(*fill_block(WATCHED, 4, 5), ("call", CALLEE, [3], []))
    entries = logged(words, [(CALLEE, 1, 0)])[0]
    return None if entries == [CALLEE, 3] else f"log {hexes(entries)}"


def case_p_recorders_footprint_not_a_convention():
    # t0 and t2 to t5 and v0 are changed by a recorder; the others named here are not. This is the
    # recorder's footprint, not a promise of the calling convention.
    return footprint(R(CALLEE, 5, 0x77, pointees={0: 2, 4: 1}), watch=((WATCHED, 1),), v0=0x77)


def fill_block_code(address, *values):
    return [w for i, v in enumerate(values) for w in stw(address + 4 * i, v)]


# ---------------------------------------------------------------------------
# Groups Q to U: masks, results in turn, runs that a recorder ends, stores and counts


def R(address, arguments, result=0, **options):
    """A recorder to put in place of a callee: a callable for `logged` and `pair_logged`."""
    return lambda log: log.replace(address, arguments, result, **options)


def program_v0(*ops):
    """Like `program`, with ("sv0", address): store the callee's result at address."""
    out = [addiu(SP, SP, -32), sw(RA, SP, 28)]
    for op in ops:
        if op[0] == "sv0":
            out += [*setreg(T0, op[1]), sw(V0, T0, 0)]
        elif op[0] == "st":
            out += stw(op[1], op[2])
        else:
            _, callee, registers, stack = op
            for i, value in enumerate(stack):
                out += [*setreg(T1, value), sw(T1, SP, 16 + 4 * i)]
            for i, value in enumerate(registers):
                out += setreg(4 + i, value)
            out += [jal(callee), NOP]
    return [*out, lw(RA, SP, 28), addiu(SP, SP, 32), JR_RA, NOP]


def endless(extra=(), store_value=None, callee=CALLEE):
    """A function that calls `callee` for ever; after each call it adds 1 to the word at FLAG.
    `extra` words run once before the loop; `store_value` (a word) is stored at FLAG2 first thing in the loop."""
    head = [addiu(SP, SP, -8), sw(RA, SP, 4), *extra]
    loop = len(head) * 4
    body = [jal(callee), NOP, *setreg(T0, FLAG), lw(T1, T0, 0), NOP, addiu(T1, T1, 1), sw(T1, T0, 0)]
    at = loop + 4 * len(body)
    return [*head, *body, beq(ZERO, ZERO, at, loop), NOP]


def calls(count, callee=CALLEE, registers=(), stack=()):
    return [("call", callee, list(registers), list(stack))] * count


def results_of(replaces, sequence):
    """v0 after each call of `sequence` (a list of callees), stored one word each."""
    ops = []
    for i, callee in enumerate(sequence):
        ops += [("call", callee, [], []), ("sv0", RESULTS + 4 * i)]
    entries, result, read = logged(program_v0(*ops), replaces)
    return [read(RESULTS + 4 * i) for i in range(len(sequence))]


# Q: masks


def case_q_argument_logged_under_its_mask():
    words = program(("call", CALLEE, [0x12345678, 0xFFFF, 0xDEADBEEF], [0x0F0F0F0F, 0xA1B2C3D4]))
    entries = logged(words, [R(CALLEE, 6, masks={0: 0xFF, 1: 0x0F0F, 4: 0xFFFF})])[0]
    # a3 holds a leftover (0); the sixth argument is not masked.
    want = [CALLEE, 0x78, 0x0F0F, 0xDEADBEEF, 0, 0x0F0F, 0xA1B2C3D4]
    return None if entries == want else f"log {hexes(entries)}"


def case_q_masked_out_bits_are_no_difference():
    def build(a0, a4):
        return program(("call", CALLEE, [a0], [a4, 0]))

    replaces = [R(CALLEE, 6, masks={0: 0xFFFF, 4: 0x00FF})]
    original = build(0x1234ABCD, 0x777700EE)
    same = pair_logged(original, build(0x5678ABCD, 0x999900EE), replaces)
    kept_bit = pair_logged(original, build(0x1234ABCC, 0x777700EE), replaces)
    kept_stack = pair_logged(original, build(0x1234ABCD, 0x777700EF), replaces)
    unmasked = pair_logged(original, build(0x5678ABCD, 0x777700EE), [R(CALLEE, 6)])
    if same != (0, 3, 0):
        return f"only masked-out bits differ: {same}"
    if kept_bit != (0, 0, 3) or kept_stack != (0, 0, 3):
        return f"a kept bit differs: {kept_bit}, {kept_stack}"
    return None if unmasked == (0, 0, 3) else f"without the mask: {unmasked}"


def case_q_bad_masks_refused():
    state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    log = contracts.CallLog(state)
    for arguments, masks in ((2, {0: 0x10000}), (2, {0: 0x1FFFF}), (2, {0: -1}), (2, {2: 0xFF}), (2, {-1: 0xFF}), (0, {0: 1})):
        try:
            log.replace(CALLEE, arguments, 0, masks=masks)
        except ValueError:
            continue
        return f"{arguments} arguments with masks {masks} was accepted"
    log.replace(CALLEE, 2, 0, masks={0: 1, 1: 0xFFFF})
    return None  # a mask of 0 is allowed: see group W


# R: results in turn


def case_r_three_results_then_the_last():
    got = results_of([R(CALLEE, 0, results=(11, 22, 33))], [CALLEE] * 5)
    return None if got == [11, 22, 33, 33, 33] else f"results {hexes(got)}"


def case_r_results_replace_result():
    got = results_of([R(CALLEE, 0, 99, results=(1, 2))], [CALLEE] * 3)
    return None if got == [1, 2, 2] else f"results {hexes(got)}"


def case_r_one_result_is_a_constant():
    got = results_of([R(CALLEE, 0, 5, results=(0xFFFFFFFF,))], [CALLEE] * 3)
    return None if got == [0xFFFFFFFF] * 3 else f"results {hexes(got)}"


def case_r_recorders_do_not_share_their_position():
    got = results_of([R(CALLEE, 0, results=(1, 2, 3)), R(CALLEE_B, 0, results=(10, 20))], [CALLEE, CALLEE_B] * 3)
    return None if got == [1, 10, 2, 20, 3, 20] else f"results {hexes(got)}"


def case_r_full_32_bit_results():
    got = results_of([R(CALLEE, 0, results=(0x12345678, 0, 0x80000001))], [CALLEE] * 3)
    return None if got == [0x12345678, 0, 0x80000001] else f"results {hexes(got)}"


def footprint(replace, watch=(), v0=None, at_changes=None):
    """A caller keeps values in the registers named here across a call to a recorder made with `replace`;
    this is the recorder's footprint, not a promise of the calling convention. Returns None or a text."""
    kept = {"t6": 14, "t7": 15, "t8": 24, "t9": 25, "v1": 3}
    head = [addiu(SP, SP, -32), sw(RA, SP, 28), *fill_block_code(BUF, 1, 2), *fill_block_code(BUF2, 3),
            *setreg(T1, BUF2), sw(T1, SP, 16), *setreg(1, 0x7777)]
    for number, value in zip(kept.values(), (0x1111, 0x2222, 0x3333, 0x4444, 0x5555)):
        head += setreg(number, value)
    for i, value in enumerate((BUF, 0x11, 0x12, 0x13)):
        head += setreg(4 + i, value)
    call_at = len(head)
    after = [*setreg(T0, RESULTS)]
    for slot, number in enumerate((*kept.values(), 4, 5, 6, 7, RA, SP, 1)):
        after.append(sw(number, T0, 4 * slot))
    words = [*head, jal(CALLEE), NOP, *after, lw(RA, SP, 28), addiu(SP, SP, 32), JR_RA, NOP]
    entries, result, read = logged(words, [replace], watch=watch)
    got = [read(RESULTS + 4 * i) for i in range(12)]
    want = [0x1111, 0x2222, 0x3333, 0x4444, 0x5555, BUF, 0x11, 0x12, 0x13, ORIGINAL + 4 * (call_at + 2), D.STACK_TOP - 32]
    if got[:11] != want:
        return f"after the call: {hexes(got[:11])}, wanted {hexes(want)}"
    if at_changes is not None and (got[11] != 0x7777) != at_changes:
        return f"at after the call: {got[11]:#x}"
    saved = [0x5A5A0000 + i for i in range(len(D.SAVED))]
    if result["saved"] != saved or result["sp"] != D.STACK_TOP:
        return f"saved {result['saved']}, sp {result['sp']:#x}"
    return None if v0 is None or result["v0"] == v0 else f"v0 {result['v0']:#x}"


def case_r_footprint_with_results_adds_only_at():
    return footprint(R(CALLEE, 5, results=(0x66, 0x67), pointees={0: 2, 4: 1}), watch=((WATCHED, 1),), v0=0x66, at_changes=True)


# S: a run that a recorder ends


def case_s_run_ended_at_the_nth_call():
    for n in (1, 2, 5):
        entries, result, read = logged(endless(), [R(CALLEE, 0, ends_run_at=n)])
        if len(entries) != n or read(FLAG) != n - 1:
            return f"N={n}: {len(entries)} calls logged, {read(FLAG)} stores made after a call"
    return None


def pair_endless(original, build, n=2, returns=True, extra_replace=(), cases=3):
    def setup(state, rng, sym):
        log = contracts.CallLog(state)
        log.replace(CALLEE, 0, 0, ends_run_at=n)
        return contracts.Setup(args=(), returns_value=False, returns=returns)

    cfg = types.SimpleNamespace(symbol_values={})
    with contract_of(LABEL, setup), patched(D, original_function=lambda cfg, name: (ORIGINAL, 4 * len(original))):
        return D.test_function(cfg, LABEL, make_code(build), cases, 1, ram_with_callees(original), bytes(D.SCRATCH_SIZE))


def case_s_ended_run_is_completed_not_discarded():
    f = endless()
    got = pair_endless(f, f)
    return None if got[:3] == (0, 3, 0) else f"result {got[:3]}"


def case_s_registers_not_compared_when_not_returning():
    original = endless()
    other_registers = endless(extra=[addiu(16, ZERO, 7), addiu(V0, ZERO, 3), addiu(30, ZERO, 1)])
    loose = pair_endless(original, other_registers, returns=False)
    strict = pair_endless(original, other_registers, returns=True)
    if loose[:3] != (0, 3, 0):
        return f"different registers with returns=False: {loose[:3]}"
    if strict[:3] != (0, 0, 3):
        return f"different registers with the default returns: {strict[:3]}"
    return None


def case_s_log_and_memory_still_compared_when_not_returning():
    original = endless()
    other_memory = endless(extra=stw(FLAG + 0x40, 5))
    fewer_calls = [addiu(SP, SP, -8), sw(RA, SP, 4), jal(CALLEE), NOP, *setreg(T0, FLAG), sw(ZERO, T0, 0), j(D.STOP_ADDRESS), NOP]
    results = (pair_endless(original, other_memory, returns=False)[:3], pair_endless(original, fewer_calls, returns=False)[:3])
    return None if results == ((0, 0, 3), (0, 0, 3)) else f"results {results}"


def case_s_recorder_without_ends_run_at_returns_every_time():
    words = program(*calls(3), ("st", FLAG, 5))
    for option in ({}, {"ends_run_at": 0}, {"ends_run_at": 9}):
        entries, result, read = logged(words, [R(CALLEE, 0, **option)])
        if len(entries) != 3 or read(FLAG) != 5:
            return f"{option}: {len(entries)} calls, FLAG {read(FLAG)}"
    return None


def case_s_negative_ends_run_at_refused():
    state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    log = contracts.CallLog(state)
    try:
        log.replace(CALLEE, 0, 0, ends_run_at=-1)
    except ValueError:
        log.replace(CALLEE, 0, 0, ends_run_at=0)
        return None
    return "ends_run_at of -1 was accepted"


def case_s_coverage_of_an_ended_run():
    f = endless()
    got = pair_endless(f, f, n=2)[4]
    want = set(range(0, 4 * len(f), 4))
    one = pair_endless(f, f, n=1)[4]
    if got != want:
        return f"N=2 executed {sorted(got)}, wanted {sorted(want)}"
    return None if one == {0, 4, 8, 12} else f"N=1 executed {sorted(one)}"


def case_s_state_stop_is_where_the_run_ends():
    state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    return None if state.stop == D.STOP_ADDRESS else f"stop {state.stop:#x}"


# T: differences without registers


def case_t_registers_skipped_when_not_returning():
    a, b = state(), state(v0=5, saved=[1] * len(D.SAVED_NAMES), sp=0)
    if D.differences(a, b, True, False) != []:
        return f"reported {D.differences(a, b, True, False)}"
    return None if len(D.differences(a, b, True, True)) == 1 + len(D.SAVED_NAMES) + 1 else f"with returns: {D.differences(a, b, True, True)}"


def case_t_memory_compared_when_not_returning():
    ram = D.differences(state(), with_byte("ram", 0x1234), False, False)
    scratch = D.differences(state(), with_byte("scratch", 0x10), False, False)
    low = D.STACK_LOW - D.RAM_BASE
    stack = D.differences(state(), with_byte("ram", low), False, False)
    first = D.differences(state(), with_byte("ram", low - 1), False, False)
    ok = len(ram) == 1 and ram[0].startswith("ram 0x80001234:") and len(scratch) == 1 and scratch[0].startswith("scratchpad 0x1f800010:") \
        and stack == [] and len(first) == 1
    return None if ok else f"ram {ram}, scratch {scratch}, stack {stack}, below {first}"


def case_t_returns_defaults_to_true():
    return None if D.differences(state(), state(sp=4), False) and D.differences(state(), state(v0=1), True) else "the default skips registers"


# U: stores and counts


def case_u_store_at_the_nth_call():
    words = program(*calls(3))
    entries = logged(words, [R(CALLEE, 0, stores=((2, WATCHED, 0xAA),))], watch=((WATCHED, 1),))
    got, _, read = entries
    if got != [CALLEE, 0, CALLEE, 0, CALLEE, 0xAA]:
        return f"log {hexes(got)}"
    return None if read(WATCHED) == 0xAA else f"final {read(WATCHED):#x}"


def case_u_no_store_when_fewer_calls():
    entries, _, read = logged(program(*calls(1)), [R(CALLEE, 0, stores=((2, WATCHED, 0xAA),))], watch=((WATCHED, 1),))
    return None if read(WATCHED) == 0 and entries == [CALLEE, 0] else f"final {read(WATCHED):#x}, log {hexes(entries)}"


def case_u_two_stores_at_different_calls():
    entries, _, read = logged(program(*calls(4)), [R(CALLEE, 0, stores=((1, WATCHED, 1), (3, WATCHED2, 3)))],
                              watch=((WATCHED, 1), (WATCHED2, 1)))
    want = [CALLEE, 0, 0, CALLEE, 1, 0, CALLEE, 1, 0, CALLEE, 1, 3]
    return None if entries == want and (read(WATCHED), read(WATCHED2)) == (1, 3) else f"log {hexes(entries)}"


def case_u_store_and_end_at_the_same_call():
    entries, _, read = logged(endless(), [R(CALLEE, 0, ends_run_at=2, stores=((2, WATCHED, 0xBB),))], watch=((WATCHED, 1),))
    want = [CALLEE, 0, CALLEE, 0]
    return None if entries == want and read(WATCHED) == 0xBB and read(FLAG) == 1 else f"log {hexes(entries)}, stored {read(WATCHED):#x}, FLAG {read(FLAG)}"


def case_u_bad_stores_and_counts_refused():
    state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    log = contracts.CallLog(state)
    for option in ({"stores": ((0, WATCHED, 1),)}, {"stores": ((-1, WATCHED, 1),)}, {"stores": ((1, WATCHED + 2, 1),)},
                   {"stores": ((1, WATCHED, 1), (2, WATCHED + 1, 1))}, {"counts": (WATCHED + 1,)}, {"counts": (WATCHED, WATCHED2 + 2)}):
        try:
            log.replace(CALLEE, 0, 0, **option)
        except ValueError:
            continue
        return f"{option} was accepted"
    log.replace(CALLEE, 0, 0, stores=((1, WATCHED, 1),), counts=(WATCHED2,))
    return None


def case_u_counts_go_up_after_the_entry():
    entries, _, read = logged(program(*calls(3)), [R(CALLEE, 0, counts=(WATCHED,))], watch=((WATCHED, 1),))
    return None if entries == [CALLEE, 0, CALLEE, 1, CALLEE, 2] and read(WATCHED) == 3 else f"log {hexes(entries)}, final {read(WATCHED)}"


def case_u_two_counted_words():
    entries, _, read = logged(program(*calls(2)), [R(CALLEE, 0, counts=(WATCHED, WATCHED2))], watch=((WATCHED, 1), (WATCHED2, 1)))
    ok = entries == [CALLEE, 0, 0, CALLEE, 1, 1] and (read(WATCHED), read(WATCHED2)) == (2, 2)
    return None if ok else f"log {hexes(entries)}"


def case_u_wait_loop_ends_by_itself():
    # call, then loop while the counted word is not 3; then store 1 at FLAG and return.
    loop = [jal(CALLEE), NOP, *setreg(T0, WATCHED), lw(T1, T0, 0), NOP, addiu(10, ZERO, 3)]
    at = 4 * (2 + len(loop))
    words = [addiu(SP, SP, -8), sw(RA, SP, 4), *loop, 0x14000000 | T1 << 21 | 10 << 16 | ((8 - at - 4) // 4 & 0xFFFF), NOP,
             *stw(FLAG, 1), lw(RA, SP, 4), addiu(SP, SP, 8), JR_RA, NOP]
    entries, _, read = logged(words, [R(CALLEE, 0, counts=(WATCHED,))])
    return None if len(entries) == 3 and read(FLAG) == 1 and read(WATCHED) == 3 else f"{len(entries)} calls, FLAG {read(FLAG)}, counted {read(WATCHED)}"


def case_u_footprint_with_every_option():
    return footprint(R(CALLEE, 5, 0x66, pointees={0: 2, 4: 1}, masks={1: 0xFF}, stores=((1, WATCHED2, 5),), counts=(WATCHED,), ends_run_at=9),
                     watch=((WATCHED, 1),), v0=0x66)


# ---------------------------------------------------------------------------
# Groups V and W: a tail of the contract's own, and a mask of 0

C = contracts  # the exported encoders and register numbers are C.lui, C.ori, C.lw, C.sw, C.addiu, C.JR_RA, C.AT ...
MARK = 0x80030300


def rtype(rs, rt, rd, funct):
    return rs << 21 | rt << 16 | rd << 11 | funct


def fill_tail(value=0xCAFE0001):
    """A model of a callee that stores `value` through its first argument and returns that argument."""
    return [C.lui(C.T2, value), C.ori(C.T2, C.T2, value), C.sw(C.T2, 0, C.A0), C.addiu(C.V0, C.A0, 0), C.JR_RA, NOP]


def copy_to_mark_tail(source=WATCHED):
    """A model that copies the word at `source` to MARK and returns 0."""
    return [C.lui(C.T0, source), C.ori(C.T0, C.T0, source), C.lw(C.T1, 0, C.T0), NOP,
            C.lui(C.T0, MARK), C.ori(C.T0, C.T0, MARK), C.sw(C.T1, 0, C.T0), C.JR_RA, C.addiu(C.V0, 0, 0)]


def case_v_tail_stores_through_the_argument_and_returns_it():
    words = program_v0(("st", BUF, 0), ("call", CALLEE, [BUF], []), ("sv0", RESULTS))
    for option in ({}, {"results": (1, 2)}):
        entries, result, read = logged(words, [R(CALLEE, 1, 0x55, tail=fill_tail(), **option)], watch=((BUF, 1),))
        if entries != [CALLEE, BUF, 0]:
            return f"{option}: entry {hexes(entries)}: it must show the word as it was before the tail"
        if read(RESULTS) != BUF or read(BUF) != 0xCAFE0001:
            return f"{option}: v0 {read(RESULTS):#x}, word {read(BUF):#x}"
    return None


def case_v_tail_with_a_counter_of_its_own():
    cell_holder = []

    def replace(log):
        cell = log.state.alloc(4)
        cell_holder.append(cell)
        tail = [C.lui(C.T0, cell), C.ori(C.T0, C.T0, cell), C.lw(C.T1, 0, C.T0), NOP, C.addiu(C.T2, 0, 2),
                rtype(C.T1, C.T2, C.AT, 0x2B),  # sltu at, t1, t2
                C.addiu(C.T1, C.T1, 1), C.sw(C.T1, 0, C.T0),
                rtype(0, C.AT, C.AT, 0x23),  # subu at, zero, at
                rtype(C.A0, C.AT, C.V0, 0x24),  # and v0, a0, at
                C.JR_RA, NOP]
        log.replace(CALLEE, 1, 0, tail=tail)

    # loop: a0 = BUF; call; bne v0, 0, loop
    words = [addiu(SP, SP, -32), sw(RA, SP, 28), *setreg(4, BUF), jal(CALLEE), NOP, beq(V0, ZERO, 8 + 4 * 4, 8) | 0x04000000, NOP,
             lw(RA, SP, 28), addiu(SP, SP, 32), JR_RA, NOP]
    entries = logged(words, [replace])[0]
    return None if entries == [CALLEE, BUF] * 3 else f"log {hexes(entries)}"


def case_v_stores_counts_and_end_act_before_the_tail():
    mark_tail = copy_to_mark_tail()
    # Ended at the first call: the store is made, the tail does not run.
    entries, _, read = logged(endless(), [R(CALLEE, 0, ends_run_at=1, stores=((1, WATCHED, 5),), counts=(WATCHED2,), tail=mark_tail)])
    if (read(WATCHED), read(WATCHED2), read(MARK)) != (5, 1, 0):
        return f"ended at call 1: store {read(WATCHED):#x}, count {read(WATCHED2)}, mark {read(MARK):#x}"
    # Ended at the second call: the tail ran the first time, and saw the store of that call.
    entries, _, read = logged(endless(), [R(CALLEE, 0, ends_run_at=2, stores=((1, WATCHED, 5),), counts=(WATCHED2,), tail=mark_tail)])
    if (read(WATCHED), read(WATCHED2), read(MARK), read(FLAG)) != (5, 2, 5, 1):
        return f"ended at call 2: store {read(WATCHED):#x}, count {read(WATCHED2)}, mark {read(MARK):#x}, FLAG {read(FLAG)}"
    return None


def case_v_masked_local_at_different_places_of_two_frames():
    def caller_with_local_at(offset):
        return [addiu(SP, SP, -32), sw(RA, SP, 28), *setreg(T1, 0x1234), sw(T1, SP, offset), addiu(4, SP, offset), jal(CALLEE), NOP,
                lw(T1, SP, offset), *setreg(T0, FLAG), sw(T1, T0, 0), lw(RA, SP, 28), addiu(SP, SP, 32), JR_RA, NOP]

    original, build = caller_with_local_at(16), caller_with_local_at(20)
    masked = pair_logged(original, build, [R(CALLEE, 1, 0, masks={0: 0}, pointees={0: 1}, tail=fill_tail())])
    unmasked = pair_logged(original, build, [R(CALLEE, 1, 0, pointees={0: 1}, tail=fill_tail())])
    if masked != (0, 3, 0):
        return f"masked to 0: {masked}"
    return None if unmasked == (0, 0, 3) else f"unmasked (why the mask exists): {unmasked}"


def case_v_exported_encoders_and_registers():
    for rt, imm in ((1, 0), (8, 0x1234), (29, 0xFFFF)):
        if C.lui(rt, imm << 16) != lui(rt, imm) or C.ori(rt, 9, imm) != ori(rt, 9, imm):
            return f"lui/ori({rt}, {imm:#x})"
    for rt, off, base in ((2, 0, 4), (8, 4, 29), (13, -4, 29), (9, 0x7FFC, 1)):
        if C.lw(rt, off, base) != lw(rt, base, off) or C.sw(rt, off, base) != sw(rt, base, off):
            return f"lw/sw({rt}, {off}, {base})"
    for rt, rs, imm in ((2, 4, 0), (8, 0, 2), (29, 29, -32), (10, 11, 0x7FFF)):
        if C.addiu(rt, rs, imm) != addiu(rt, rs, imm):
            return f"addiu({rt}, {rs}, {imm})"
    if C.JR_RA != JR_RA:
        return f"JR_RA {C.JR_RA:#x}"
    got = [C.AT, C.V0, C.A0, C.A1, C.A2, C.A3, C.T0, C.T1, C.T2, C.T3, C.T4, C.T5, C.SP]
    want = [1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 29]
    return None if got == want else f"registers {got}"


def case_w_mask_zero_logs_zero():
    words = program(("call", CALLEE, [0x1234, 0xFFFFFFFF], []), ("call", CALLEE, [0xFFFF, 0xFFFFFFFF], []))
    entries = logged(words, [R(CALLEE, 2, masks={0: 0})])[0]
    if entries != [CALLEE, 0, 0xFFFFFFFF] * 2:
        return f"log {hexes(entries)}"
    same = pair_logged(program(("call", CALLEE, [0x1234], [])), program(("call", CALLEE, [0x5678], [])), [R(CALLEE, 1, masks={0: 0})])
    other = pair_logged(program(("call", CALLEE, [0x1234], [])), program(("call", CALLEE, [0x5678], [])), [R(CALLEE, 1)])
    return None if same == (0, 3, 0) and other == (0, 0, 3) else f"masked {same}, unmasked {other}"


def case_w_still_refused_masks():
    state = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    log = contracts.CallLog(state)
    for arguments, masks in ((2, {0: 0x10000}), (2, {0: 0x1FFFF}), (2, {0: -1}), (2, {2: 0}), (2, {-1: 0}), (0, {0: 0})):
        try:
            log.replace(CALLEE, arguments, 0, masks=masks)
        except ValueError:
            continue
        return f"{arguments} arguments with masks {masks} was accepted"
    log.replace(CALLEE, 2, 0, masks={0: 0, 1: 0xFFFF})
    return None


# ---------------------------------------------------------------------------
# Group X: --writes, the audit of where the original writes


def sb(rt, base, off): return 0xA0000000 | base << 21 | rt << 16 | (off & 0xFFFF)
def beq_self(): return 0x1000FFFF  # beq zero, zero, -1: a branch to itself (the delay slot follows)


A0 = 4
ARENA = D.ARENA_BASE
BYTE_WRITER = returning(ori(T1, ZERO, 0x55), sb(T1, A0, 0))  # *(char *)a0 = 0x55
WORD_WRITER = returning(ori(T1, ZERO, 0x5555), sw(T1, A0, 0))  # *(int *)a0 = 0x5555: two bytes change
ZERO_WRITER = returning(sw(ZERO, A0, 0))  # *(int *)a0 = 0: no byte changes in zeroed memory


def audit(words, prepare, cases=1, budget=None, ram=None, sym=None):
    """D.audit_writes on a made-up original. `prepare(state, case)` makes the case's memory and gives the arguments."""
    counter = iter(range(cases))

    def setup(state, rng, table):
        return contracts.Setup(args=tuple(prepare(state, next(counter))), returns_value=False)

    cfg = types.SimpleNamespace(symbol_values=sym or {})
    with contract_of(LABEL, setup), \
            patched(D, original_function=lambda cfg, name: (ORIGINAL, 4 * len(words)), BUDGET=budget or D.BUDGET):
        return D.audit_writes(cfg, LABEL, cases, 1, made_up_ram(words) if ram is None else ram, bytes(D.SCRATCH_SIZE))


def block_of(size, extra=()):
    """prepare: allocate a block of `size`, then run the original on the address `extra[0]` bytes from its start."""
    def prepare(state, case):
        start = state.alloc(size)
        return (start + extra[0],)
    return prepare


def want(result, outside, largest, discarded=0, first=None):
    got = result[:3]
    if got != (discarded, outside, largest):
        return f"discarded/outside/largest {got}, wanted {(discarded, outside, largest)}"
    if first is not None and not result[3].startswith(first):
        return f"first {result[3]!r}, wanted it to start {first!r}"
    return None


def case_x_inside_a_block():
    for offset in (0, 3, 7):
        why = want(audit(BYTE_WRITER, block_of(8, (offset,))), 0, 0)
        if why:
            return f"offset {offset}: {why}"
    return None


def case_x_one_byte_past_the_block():
    return want(audit(BYTE_WRITER, block_of(8, (8,))), 1, 1, first=f"case 0; {ARENA + 8:#x}..{ARENA + 8:#x} (")


def case_x_one_byte_before_the_block():
    return want(audit(BYTE_WRITER, block_of(8, (-1,))), 1, 1, first=f"case 0; {ARENA - 1:#x}..{ARENA - 1:#x} (")


def case_x_global_written_by_the_setup():
    def prepare(state, case):
        state.w32(0x80030000, 0)
        return (0x80030000,)
    return want(audit(WORD_WRITER, prepare), 0, 0)


def case_x_global_not_written_by_the_setup():
    return want(audit(WORD_WRITER, lambda state, case: (0x80030000,)), 1, 2)


def case_x_global_after_owns():
    def prepare(state, case):
        before = state.read(0x80030000, 4)
        state.owns(0x80030000, 4)
        if state.read(0x80030000, 4) != before:
            raise AssertionError("owns wrote")
        return (0x80030000,)
    return want(audit(WORD_WRITER, prepare), 0, 0)


def case_x_owns_covers_only_its_range():
    def prepare(state, case):
        state.owns(0x80030000, 1)  # the word store changes bytes 0 and 1: byte 1 is not made
        return (0x80030000,)
    return want(audit(WORD_WRITER, prepare), 1, 1)


def case_x_owns_outside_memory_refused():
    st = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    for address, size in ((0x80200000, 1), (0x801FFFFF, 2), (0x1F801000, 1), (0x10, 1)):
        try:
            st.owns(address, size)
        except ValueError:
            continue
        return f"owns({address:#x}, {size}) was accepted"
    return None


def refused(call) -> bool:
    try:
        call()
    except ValueError:
        return True
    return False


def record_sizes(state):
    return len(state.made_ram), len(state.made_scratch)


def case_x_negative_owns_is_refused_and_the_record_keeps_its_size():
    # Only the last byte of RAM is made. A negative size must not shrink the record and move that mark to
    # offset 0: the store to the first byte of RAM is then still a store outside.
    def prepare(state, case):
        state.owns(D.RAM_BASE + D.RAM_SIZE - 1, 1)
        if not refused(lambda: state.owns(D.RAM_BASE, -1)):
            raise AssertionError("owns(RAM_BASE, -1) was accepted")
        if record_sizes(state) != (D.RAM_SIZE, D.SCRATCH_SIZE) or state.made_ram[0] or not state.made_ram[-1]:
            raise AssertionError("the record changed")
        return (D.RAM_BASE,)
    return want(audit(BYTE_WRITER, prepare), 1, 1)


def case_x_negative_owns_in_the_scratchpad_likewise():
    def prepare(state, case):
        state.owns(D.SCRATCH_BASE + D.SCRATCH_SIZE - 1, 1)
        if not refused(lambda: state.owns(D.SCRATCH_BASE, -1)):
            raise AssertionError("owns(SCRATCH_BASE, -1) was accepted")
        if record_sizes(state) != (D.RAM_SIZE, D.SCRATCH_SIZE) or state.made_scratch[0] or not state.made_scratch[-1]:
            raise AssertionError("the record changed")
        return (D.SCRATCH_BASE,)
    return want(audit(BYTE_WRITER, prepare), 1, 1)


def case_x_negative_sizes_are_refused_before_anything_changes():
    st = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    arena = st.arena
    for name, call in (("owns in RAM", lambda: st.owns(0x80030000, -4)), ("owns at the end of RAM", lambda: st.owns(D.RAM_BASE + D.RAM_SIZE, -1)),
                       ("owns in the scratchpad", lambda: st.owns(D.SCRATCH_BASE + 8, -8)), ("alloc", lambda: st.alloc(-1)),
                       ("alloc of a whole word", lambda: st.alloc(-4)), ("read", lambda: st.read(0x80030000, -1))):
        if not refused(call):
            return f"{name}: a negative size was accepted"
        if record_sizes(st) != (D.RAM_SIZE, D.SCRATCH_SIZE) or any(st.made_ram) or any(st.made_scratch) or st.arena != arena:
            return f"{name}: the refusal changed the state"
    return None


def case_x_alloc_that_does_not_fit_leaves_the_arena():
    st = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    first = st.alloc(8)
    arena = st.arena
    if not refused(lambda: st.alloc(D.ARENA_END - D.ARENA_BASE)):
        return "a block larger than the arena's rest was handed out"
    if st.arena != arena or st.alloc(4) != first + 8:
        return "the refused block moved the arena"
    return None


def case_x_empty_spans_are_valid_and_make_nothing():
    st = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    for address in (0x80030000, D.RAM_BASE, D.RAM_BASE + D.RAM_SIZE, D.SCRATCH_BASE, D.SCRATCH_BASE + D.SCRATCH_SIZE):
        st.owns(address, 0)
        st.write(address, b"")
    block = st.alloc(0)
    if any(st.made_ram) or any(st.made_scratch) or record_sizes(st) != (D.RAM_SIZE, D.SCRATCH_SIZE):
        return "an empty span made memory or changed the record"
    return None if st.alloc(0) == block else "an empty block moved the arena"


def case_x_a_record_of_another_size_is_refused_by_the_audit():
    st = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    final = {"ram": bytes(D.RAM_SIZE), "scratch": bytes(D.SCRATCH_SIZE)}
    for attribute in ("made_ram", "made_scratch"):
        keep = getattr(st, attribute)
        setattr(st, attribute, keep[:1])
        try:
            D.outside_addresses(st, final)
        except D.InputError:
            setattr(st, attribute, keep)
            continue
        return f"a shortened {attribute} was accepted"
    return None if D.outside_addresses(st, final) == [] else "the unchanged state reports addresses"


def case_x_same_value_not_seen():
    # The stated limit: a store of the value that is already there changes no byte.
    why = want(audit(ZERO_WRITER, lambda state, case: (0x80030000,)), 0, 0)
    if why:
        return why
    # The same store over content that differs is seen.
    ram = bytearray(made_up_ram(ZERO_WRITER))
    ram[0x30000] = 9
    return want(audit(ZERO_WRITER, lambda state, case: (0x80030000,), ram=bytes(ram)), 1, 1)


def case_x_stack_region():
    for address in (D.STACK_LOW, STACK_WORD, D.STACK_TOP - 1, D.STACK_TOP, D.STACK_TOP + 15):
        why = want(audit(BYTE_WRITER, lambda state, case, a=address: (a,)), 0, 0)
        if why:
            return f"{address:#x}: {why}"
    return None


def case_x_stack_borders():
    for address in (D.STACK_LOW - 1, D.STACK_TOP + D.HOME_AREA):
        why = want(audit(BYTE_WRITER, lambda state, case, a=address: (a,)), 1, 1)
        if why:
            return f"{address:#x}: {why}"
    return None


def case_x_stack_borders_as_the_comparator_has_them():
    # Below and inside the region the audit counts what the comparator compares; the 16 bytes above are the
    # difference: compared, not counted.
    for address in (D.STACK_LOW - 1, D.STACK_LOW, D.STACK_TOP - 1):
        a, b = state(), state()
        b["ram"][address - D.RAM_BASE] = 0x55
        compared = bool(D.differences(a, b, False))
        counted = audit(BYTE_WRITER, lambda state, case, x=address: (x,))[1] == 1
        if compared != counted:
            return f"{address:#x}: comparator {compared}, audit {counted}"
    for address in (D.STACK_TOP, D.STACK_TOP + 15):
        a, b = state(), state()
        b["ram"][address - D.RAM_BASE] = 0x55
        if not D.differences(a, b, False):
            return f"{address:#x} is not compared"
    return None


def case_x_home_area():
    # sp + 0 and sp + 15 are the function's own; sp + 16 is not. sp is the tool's initial stack pointer.
    for offset, outside in ((0, 0), (4, 0), (5, 0), (15, 0), (16, 1)):
        why = want(audit(BYTE_WRITER, lambda state, case, o=offset: (D.STACK_TOP + o,)), outside, outside)
        if why:
            return f"sp + {offset}: {why}"
    # The function stores through its own sp, as a spill of a1 does.
    spill = returning(ori(T1, ZERO, 0x55), sw(T1, SP, 4), sb(T1, SP, 15), sb(T1, SP, 16))
    return want(audit(spill, lambda state, case: ()), 1, 1, first=f"case 0; {D.STACK_TOP + 16:#x}..")


def case_x_home_area_made_stays_made():
    def prepare(state, case):
        state.w8(D.STACK_TOP + 16, 0)
        return (D.STACK_TOP + 16,)
    return want(audit(BYTE_WRITER, prepare), 0, 0)


def case_x_home_area_is_compared():
    # Only the audit exempts it: a difference there is still a difference of the comparison.
    original = returning(*store(0x55, D.STACK_TOP + 4))
    return expect(run_pair(original, returning(NOP)), 0, 0, 10)


def case_x_scratchpad_unmade():
    return want(audit(BYTE_WRITER, lambda state, case: (D.SCRATCH_BASE + 0x10,)), 1, 1,
                first=f"case 0; {D.SCRATCH_BASE + 0x10:#x}..{D.SCRATCH_BASE + 0x10:#x} (")


def case_x_scratchpad_first_and_last_bytes():
    for address in (D.SCRATCH_BASE, D.SCRATCH_BASE + 15, D.SCRATCH_BASE + D.SCRATCH_SIZE - 1):
        why = want(audit(BYTE_WRITER, lambda state, case, a=address: (a,)), 1, 1)
        if why:
            return f"{address:#x}: {why}"
    return None


def case_x_scratchpad_made():
    def by_write(state, case):
        state.w8(D.SCRATCH_BASE + 0x10, 0)
        return (D.SCRATCH_BASE + 0x10,)

    def by_owns(state, case):
        state.owns(D.SCRATCH_BASE + 0x10, 1)
        return (D.SCRATCH_BASE + 0x10,)

    return want(audit(BYTE_WRITER, by_write), 0, 0) or want(audit(BYTE_WRITER, by_owns), 0, 0)


def case_x_made_ram_does_not_cover_scratchpad():
    def prepare(state, case):
        state.w8(D.RAM_BASE + 0x10, 0)  # the same offset in RAM
        state.owns(D.RAM_BASE + 0x10, 1)
        return (D.SCRATCH_BASE + 0x10,)
    return want(audit(BYTE_WRITER, prepare), 1, 1)


def case_x_made_scratchpad_does_not_cover_ram():
    def prepare(state, case):
        state.w8(D.SCRATCH_BASE + 0x30000 % D.SCRATCH_SIZE, 0)
        return (0x80030000,)
    return want(audit(BYTE_WRITER, prepare), 1, 1)


def case_x_made_by_each_writer():
    st = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    base = 0x80030000
    st.w8(base, 1)
    st.w16(base + 8, 1)
    st.w32(base + 16, 1)
    st.write(base + 32, b"abc")
    st.write(base + 40, b"")
    marked = [i for i, m in enumerate(st.made_ram) if m]
    wanted = [0x30000, 0x30008, 0x30009, *range(0x30010, 0x30014), 0x30020, 0x30021, 0x30022]
    if marked != wanted:
        return f"made bytes {[hex(i) for i in marked]}"
    if any(st.made_scratch):
        return "a write to RAM marked the scratchpad"
    st.w8(D.SCRATCH_BASE + 5, 1)
    return None if [i for i, m in enumerate(st.made_scratch) if m] == [5] else "the scratchpad write is not marked alone"


def case_x_alloc_marks_the_whole_block():
    st = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    a = st.alloc(12)
    b = st.alloc(1)
    marked = [i + D.RAM_BASE for i, m in enumerate(st.made_ram) if m]
    wanted = [*range(a, a + 12), b]
    return None if marked == wanted else f"made {[hex(m) for m in marked]}, wanted {[hex(m) for m in wanted]}"


def case_x_alloc_padding_is_not_made():
    # alloc(5) makes bytes 0 to 4; the three padding bytes up to the next block are not made, the next block is.
    def prepare(state, case):
        start = state.alloc(5)
        state.alloc(4)
        return (start + (4, 5, 7, 8)[case],)
    return want(audit(BYTE_WRITER, prepare, cases=4), 2, 1, first="case 1; ")


def case_x_alloc_of_a_multiple_of_four_has_no_padding():
    def prepare(state, case):
        start = state.alloc(8)
        state.alloc(4)
        return (start + 8,)  # the next block's first byte
    return want(audit(BYTE_WRITER, prepare), 0, 0)


def case_x_largest_and_first_over_the_cases():
    # Case 1 changes two bytes outside, case 3 one: the largest is 2, the first is 1; the other cases stay inside.
    words = returning(ori(T1, ZERO, 0x55), sb(T1, A0, 0), sb(T1, A0 + 1, 0))  # stores to a0 and to a1

    def prepare(state, case):
        block = state.alloc(8)
        if case == 1:
            return (block + 8, block + 9)
        if case == 3:
            return (block + 8, block)
        return (block, block + 1)

    return want(audit(words, prepare, cases=5), 2, 2, first="case 1; ")


def case_x_discarded_not_counted_as_outside():
    fault = [*store(0x55), lui(T0, HOLE >> 16), lw(T1, T0, 0), JR_RA, NOP]
    loop = [*store(0x55), beq_self(), NOP]
    nothing = lambda state, case: ()
    why = want(audit(fault, nothing, cases=3), 0, 0, discarded=3)
    if why:
        return f"fault: {why}"
    why = want(audit(loop, nothing, cases=2, budget=200), 0, 0, discarded=2)
    if why:
        return f"budget: {why}"
    # The same store in a run that ends is outside.
    return want(audit(returning(*store(0x55)), nothing, cases=3), 3, 1, first="case 0; ")


def case_x_discarded_and_outside_in_one_run():
    # The store of FLAG comes first; a0 = 1 then faults, a0 = 0 returns.
    words = [*store(0x55), 0x10000000 | A0 << 21 | 3, NOP, lui(T0, HOLE >> 16), lw(T1, T0, 0), JR_RA, NOP]
    return want(audit(words, lambda state, case: (case % 2,), cases=4), 2, 1, discarded=2, first="case 0; ")


def case_x_recorder_log_and_code_not_counted():
    replaces = [(CALLEE, 2, 7), (CALLEE_B, 0, 0)]
    words = caller([(CALLEE, [1, 2]), (CALLEE_B, [])])
    # The recorders did write: code at both callees and entries in the log.
    entries, result, log, u32 = run_caller(words, replaces)
    if len(entries) != 4 or u32(CALLEE) >> 26 != 2 or u32(CALLEE_B) >> 26 != 2:
        return f"the recorders wrote nothing ({len(entries)} entries)"

    def prepare(state, case):
        log = contracts.CallLog(state)
        for replace in replaces:
            log.replace(*replace)
        return ()

    return want(audit(words, prepare, ram=ram_with_callees(words)), 0, 0)


def case_x_recorder_does_not_cover_a_store_of_the_function():
    words = caller([(CALLEE, [1, 2])], tail=store(0x55))  # the function writes FLAG after the call

    def prepare(state, case):
        contracts.CallLog(state).replace(CALLEE, 2, 7)
        return ()

    return want(audit(words, prepare, ram=ram_with_callees(words)), 1, 1)


def case_x_callee_body_would_write_outside():
    # Without the recorder the callee's body runs and writes FLAG: the audit sees it.
    words = caller([(CALLEE, [1, 2])])
    return want(audit(words, lambda state, case: (), ram=ram_with_callees(words)), 1, 1)


def case_x_state_is_new_for_each_case():
    # A byte made in case 0 is not made in case 1.
    def prepare(state, case):
        if case == 0:
            state.w8(0x80030000, 0)
        return (0x80030000,)
    return want(audit(BYTE_WRITER, prepare, cases=2), 1, 1, first="case 1; ")


def case_x_cases_use_the_seed_and_case_number():
    drawn = []

    def setup(state, rng, table):
        drawn.append(rng.getrandbits(32))
        return contracts.Setup(args=(), returns_value=False)

    cfg = types.SimpleNamespace(symbol_values={})
    with contract_of(LABEL, setup), patched(D, original_function=lambda cfg, name: (ORIGINAL, 8)):
        D.audit_writes(cfg, LABEL, 3, 7, made_up_ram(returning()), bytes(D.SCRATCH_SIZE))
    wanted = [random.Random(f"7:{LABEL}:{case}").getrandbits(32) for case in range(3)]
    return None if drawn == wanted else f"drawn {drawn}, wanted {wanted}"


def case_x_zero_cases():
    return want(audit(BYTE_WRITER, lambda state, case: (0x80030000,), cases=0), 0, 0, first="")


def case_x_audit_builds_nothing():
    with patched(D, build_function=lambda *a, **k: (_ for _ in ()).throw(AssertionError("build"))):
        return want(audit(BYTE_WRITER, block_of(8, (0,))), 0, 0)


# runs_text

def case_x_runs_join_within_sixteen_bytes():
    sym = {"data_a": 0x80030000}
    text = D.runs_text([0x80030000, 0x80030010, 0x80030011, 0x80030022], sym)
    want_ = "0x80030000..0x80030011 (data_a+0x0); 0x80030022..0x80030022 (data_a+0x22)"
    return None if text == want_ else text


def case_x_runs_sixteen_and_seventeen():
    sym = {"s": 0x80030000}
    joined = D.runs_text([0x80030000, 0x80030010], sym)
    split = D.runs_text([0x80030000, 0x80030011], sym)
    ok = joined == "0x80030000..0x80030010 (s+0x0)" and split == "0x80030000..0x80030000 (s+0x0); 0x80030011..0x80030011 (s+0x11)"
    return None if ok else f"{joined!r} / {split!r}"


def case_x_runs_six_then_more():
    found = [0x80030000 + 0x100 * i for i in range(9)]
    parts = D.runs_text(found, {"s": 0x80030000}).split("; ")
    ok = len(parts) == 7 and parts[0] == "0x80030000..0x80030000 (s+0x0)" and parts[5].startswith("0x80030500") and parts[6] == "and 3 more"
    exact = D.runs_text(found[:6], {"s": 0x80030000}).split("; ")
    return None if ok and len(exact) == 6 and "more" not in exact[-1] else f"{parts} / {exact}"


def case_x_runs_nearest_name():
    sym = {"low": 0x80010000, "mid": 0x80020000, "high": 0x80030000, "text": "x", "scr": 0x1F800100}
    text = D.runs_text([0x80020004, 0x80029000, 0x8002FFFF], sym)
    want_ = "0x80020004..0x80020004 (mid+0x4); 0x80029000..0x80029000 (mid+0x9000); 0x8002ffff..0x8002ffff (mid+0xffff)"
    return None if text == want_ else text


def case_x_runs_name_must_share_the_region():
    sym = {"ram_name": 0x80010000, "scr_name": 0x1F800100}
    scratch_below = D.runs_text([0x1F8000F0], sym)  # no scratchpad name at or below: not the RAM name
    scratch_above = D.runs_text([0x1F800104], sym)
    ram_below = D.runs_text([0x8000F000], sym)  # no RAM name at or below
    ok = scratch_below.endswith("(?)") and scratch_above.endswith("(scr_name+0x4)") and ram_below.endswith("(?)")
    return None if ok else f"{scratch_below!r} {scratch_above!r} {ram_below!r}"


def case_x_runs_name_at_the_address_itself():
    text = D.runs_text([0x80030000], {"a": 0x80020000, "b": 0x80030000, "c": 0x80040000})
    return None if text == "0x80030000..0x80030000 (b+0x0)" else text


def case_x_runs_tie_takes_the_last_name():
    text = D.runs_text([0x80030004], {"b": 0x80030000, "a": 0x80030000})
    return None if text == "0x80030004..0x80030004 (b+0x4)" else text


def case_x_runs_ram_before_scratchpad():
    text = D.runs_text([0x80030000, 0x1F800010], {})
    return None if text == "0x80030000..0x80030000 (?); 0x1f800010..0x1f800010 (?)" else text


# main

OUT = "func_80000001"


def writes_main(argv, results, **kwargs):
    """main with stand-ins and an audit_writes stand-in that gives `results[name]` and records its calls."""
    seen = []

    def audit_writes(cfg, name, cases, seed, ram, scratch):
        seen.append((name, cases, seed))
        return results.get(name, (0, 0, 0, ""))

    with patched(D, audit_writes=audit_writes):
        status, out, err, calls = run_main(["--writes", *argv], **kwargs)
    return status, out, err, calls, seen


def case_x_main_line():
    status, out, _, calls, seen = writes_main(["--cases", "9", "--seed", "4"], {OUT: (2, 3, 17, "case 5; 0x80030000..0x80030003 (s+0x0)")})
    ok = out == f"{OUT} writes: cases 9, discarded 2, outside 3 (largest 17 bytes)\n" and status == 1
    return None if ok and seen == [(OUT, 9, 4)] else f"status {status}, out {out!r}, seen {seen}"


def case_x_main_line_when_clean():
    status, out, _, _, _ = writes_main(["--uncovered"], {OUT: (4, 0, 0, "")}, )
    return None if status == 0 and out == f"{OUT} writes: cases 1000, discarded 4, outside 0 (largest 0 bytes)\n" else f"{status} {out!r}"


def case_x_main_second_line():
    first = "case 5; 0x80030000..0x80030003 (s+0x0); and 2 more"
    status, out, _, _, _ = writes_main(["--uncovered"], {OUT: (0, 3, 6, first)})
    want_ = f"{OUT} writes: cases 1000, discarded 0, outside 3 (largest 6 bytes)\n  first: {first}\n"
    return None if status == 1 and out == want_ else f"{status} {out!r}"


def case_x_main_no_second_line_without_uncovered():
    _, out, _, _, _ = writes_main([], {OUT: (0, 3, 6, "case 5; x")})
    return None if out.count("\n") == 1 else out


def case_x_main_status():
    s0 = writes_main([], {OUT: (0, 0, 0, "")})[0]
    s1 = writes_main([], {OUT: (0, 1, 1, "case 0; x")})[0]
    s2 = writes_main([], {OUT: (5, 0, 0, "")})[0]
    return None if (s0, s1, s2) == (0, 1, 0) else f"statuses {(s0, s1, s2)}"


def case_x_main_control_is_an_error():
    status, out, err, calls, seen = writes_main(["--control"], {})
    ok = status == 2 and out == "" and "INPUT ERROR" in err and "--writes" in err and "--control" in err and not seen and not calls
    return None if ok else f"status {status}, out {out!r}, err {err!r}"


def case_x_main_no_build():
    status, out, err, calls, seen = writes_main(["--uncovered"], {OUT: (0, 0, 0, "")})
    return None if SEEN["folders"] == [] and calls == [] and seen == [(OUT, 1000, 1)] else f"builds {SEEN['folders']}, runs {calls}"


def case_x_main_build_that_would_fail_is_not_tried():
    status, out, err, _, _ = writes_main([], {OUT: (0, 0, 0, "")}, build_error=matchbuild.StepError("no"))
    return None if status == 0 and err == "" else f"status {status}, err {err!r}"


def case_x_main_all_in_order_and_all_run():
    results = {n: (0, 0, 0, "") for n in ALL_NAMES}
    results["func_80000002"] = (1, 2, 3, "case 4; r")
    seen_all = []

    def body(folder):
        status, out, err, calls, seen = writes_main(["--folder", str(folder), "--all", "--uncovered"], results, names=ALL_NAMES, cli_names=())
        seen_all.append(seen)
        return status, out

    status, out = all_folder(body)
    lines = out.splitlines()
    ok = (status == 1 and [l.split()[0] for l in lines if " writes: " in l] == list(ALL_NAMES)
          and lines[2] == "  first: case 4; r" and len(lines) == 4 and [s[0] for s in seen_all[0]] == list(ALL_NAMES))
    return None if ok else f"status {status}, out {lines}"


def case_x_main_no_contract():
    status, out, err, _, seen = writes_main([], {}, register=False)
    return None if status == 2 and out == "" and "no contract" in err and not seen else f"{status} {out!r} {err!r}"


def case_x_main_config_error():
    status, out, err, _, seen = writes_main([], {}, load=matchbuild.ConfigError(["bad"]))
    return None if status == 2 and out == "" and "CONFIG ERROR" in err and not seen else f"{status} {out!r} {err!r}"


def case_x_main_original_input_error():
    status, out, err, _, seen = writes_main([], {}, original_error=D.InputError("nope"))
    return None if status == 2 and out == "" and "INPUT ERROR: nope" in err and not seen else f"{status} {out!r} {err!r}"


def case_x_default_output_unchanged():
    def fail(*a, **k):
        raise AssertionError("audit_writes ran in the default mode")

    with patched(D, audit_writes=fail):
        _, out, _, _ = run_main(["--cases", "7"], build=b"\0" * 8, original_size=12, executed={0, 8},
                                results={OUT: (1, 4, 2, ["first difference: case 3, seed 1", "ram 0x1: x"])})
        _, out_u, _, _ = run_main(["--cases", "7", "--uncovered"], build=b"\0" * 8, original_size=12, executed={0, 8},
                                  results={OUT: (1, 4, 2, ["first difference: case 3, seed 1", "ram 0x1: x"])})
        _, out_c, _, _ = run_main(["--cases", "7", "--control"], build=b"\0" * 8, original_size=12, control=lambda words: (1, 9, "w"),
                                  results={OUT: (0, 0, 5, [])})
    stored = ("func_80000001: built 8 bytes, original 12 bytes; cases 7, discarded 1, equal 4, different 2\n"
              "  first difference: case 3, seed 1\n  ram 0x1: x\n"
              "func_80000001 coverage: 2 of 3 instruction slots of the original executed\n")
    stored_u = stored + "  not executed: +0x4\n"
    stored_c = "func_80000001 control: different 5 of 7 (expected more than 0)\n  altered: w, instruction slot 1\n"
    for got, wanted in ((out, stored), (out_u, stored_u), (out_c, stored_c)):
        if got != wanted:
            return f"{got!r} != {wanted!r}"
    return None


def case_x_state_default_behaviour_unchanged():
    st = D.State(bytes(D.RAM_SIZE), bytes(D.SCRATCH_SIZE))
    block = st.alloc(5)
    st.w32(block, 0x01020304)
    st.w8(D.SCRATCH_BASE + 3, 9)
    ok = (block == D.ARENA_BASE and st.arena == D.ARENA_BASE + 8 and st.read(block, 5) == b"\4\3\2\1\0"
          and st.read(D.SCRATCH_BASE + 3, 1) == b"\x09" and st.stop == D.STOP_ADDRESS)
    try:
        st.arena = D.ARENA_END - 4
        st.alloc(8)
        return "alloc past the arena was accepted"
    except ValueError:
        pass
    return None if ok else "State behaves differently"


def inventory_lookup(game_rows, library_rows, name):
    """`original_function` on made-up inventory tables: (result, error text)."""
    import tempfile
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp) / "ps1"
        (root / "inventory").mkdir(parents=True)
        (root / "src" / "tool").mkdir(parents=True)
        (root / "inventory" / "game.tsv").write_text("".join(f"{a}\t{n}\tx\n" for a, n in game_rows))
        (root / "inventory" / "library.tsv").write_text("".join(f"{a}\t{n}\tx\n" for a, n in library_rows))
        with patched(D, HERE=root / "src" / "tool"):
            try:
                return D.original_function(None, name), None
            except D.InputError as exc:
                return None, str(exc)


def case_y_size_from_game_table():
    got, err = inventory_lookup([("80100000", 12)], [("80200000", 99)], "func_80100000")
    return None if got == (0x80100000, 12) and err is None else f"{got} {err}"


def case_y_size_from_library_table():
    got, err = inventory_lookup([("80100000", 12)], [("80200000", 99)], "func_80200000")
    return None if got == (0x80200000, 99) and err is None else f"{got} {err}"


def case_y_in_neither_table_is_an_error():
    got, err = inventory_lookup([("80100000", 12)], [("80200000", 99)], "func_80300000")
    return None if got is None and err and "lists 0 functions" in err else f"{got} {err}"


def case_y_in_both_tables_is_an_error():
    got, err = inventory_lookup([("80100000", 12)], [("80100000", 99)], "func_80100000")
    return None if got is None and err and "lists 2 functions" in err else f"{got} {err}"


CASES = [
    ("a-equal-states-give-no-line", case_a_equal),
    ("a-ram-byte-reported-with-console-address", case_a_ram_byte),
    ("a-scratchpad-byte-reported-with-console-address", case_a_scratch_byte),
    ("a-stack-region-borders", case_a_stack_borders),
    ("a-first-and-last-byte-of-ram-and-scratchpad", case_a_first_and_last_bytes),
    ("a-each-saved-register-reported-by-name", case_a_saved_registers),
    ("a-sp-reported", case_a_sp),
    ("a-v0-reported-only-when-contract-returns-value", case_a_v0_only_when_returned),
    ("a-two-pages-both-reported", case_a_two_pages),
    ("a-two-bytes-in-one-page-both-reported", case_a_same_page_two_bytes),
    ("a-ram-and-scratchpad-both-reported", case_a_scratch_two_bytes_and_ram_together),
    ("a-stack-bytes-in-a-page-do-not-hide-others", case_a_page_straddling_stack),
    ("b-same-store-equal", case_b_same_store),
    ("b-other-store-different", case_b_other_store),
    ("b-stack-content-only-equal", case_b_stack_content_only),
    ("b-other-s0-different", case_b_other_s0),
    ("b-other-sp-different", case_b_other_sp),
    ("b-other-fp-different", case_b_other_fp),
    ("b-register-copy-is-seen", case_b_register_copy_is_seen),
    ("b-report-is-capped", case_b_report_is_capped),
    ("b-calls-start-with-known-registers", case_b_initial_registers),
    ("b-original-never-returns-discarded", case_b_original_never_returns),
    ("b-original-unmapped-read-discarded", case_b_original_unmapped_read),
    ("b-build-unmapped-read-different", case_b_build_unmapped_read),
    ("b-build-never-returns-different", case_b_build_never_returns),
    ("b-v0-ignored-when-contract-returns-nothing", case_b_v0_ignored_when_no_value),
    ("b-v0-compared-when-contract-returns-value", case_b_v0_compared_when_value),
    ("b-v0-equal-when-contract-returns-value", case_b_v0_equal_when_value),
    ("b-report-of-first-difference", case_b_report_of_first_difference),
    ("b-seed-fixes-the-random-numbers", case_b_seed),
    ("b-argument-registers-reach-the-code", case_b_argument_registers),
    ("c-all-equal-status-0-and-line", case_c_all_equal),
    ("c-line-and-report-shape", case_c_line_shape),
    ("c-difference-status-1", case_c_difference),
    ("c-no-equal-case-status-1", case_c_no_equal_case),
    ("c-control-with-differences-status-0-and-line", case_c_control_with_differences),
    ("c-control-without-difference-status-1", case_c_control_without_difference),
    ("c-control-with-discards-only-status-1", case_c_control_with_discards_only),
    ("c-control-alters-exactly-the-named-word", case_c_control_alters_one_word),
    ("c-control-first-and-last-word", case_c_control_first_and_last_word),
    ("c-no-control-leaves-the-code-alone", case_c_no_control_leaves_code_alone),
    ("c-cases-and-seed-reach-the-run", case_c_arguments_reach_the_run),
    ("c-no-contract-status-2", case_c_no_contract),
    ("c-second-function-differs-status-1-both-lines", case_c_second_function_differs),
    ("c-first-function-differs-status-1-both-lines", case_c_first_function_differs),
    ("c-config-error-status-2", case_c_config_error),
    ("c-config-input-error-status-2", case_c_config_input_error),
    ("c-config-os-error-status-2", case_c_config_os_error),
    ("c-original-input-error-status-2", case_c_original_input_error),
    ("c-build-errors-status-3", case_c_build_errors),
    ("d-every-matchbuild-name-used-exists", case_d_names_exist),
    ("d-declarations-can-be-made-as-build_function-does", case_d_declarations),
    ("d-config-error-carries-its-errors", case_d_error_classes),
    ("e-straight-function-all-slots-executed", case_e_straight),
    ("e-branch-never-taken-slots-behind-it-missing", case_e_branch_never_taken),
    ("e-other-arm-covers-the-others", case_e_other_arm),
    ("e-two-arms-union-covers-all", case_e_union_of_arms),
    ("e-callee-and-build-slots-not-counted", case_e_callee_and_build_not_counted),
    ("e-discarded-run-adds-nothing", case_e_discarded_run_adds_nothing),
    ("e-all-discarded-covers-nothing", case_e_nothing_when_all_discarded),
    ("e-slots-in-cuts-and-merges-blocks", case_e_slots_in),
    ("e-ranges-text", case_e_ranges_text),
    ("f-coverage-line-shape-and-size-in-slots", case_f_coverage_line),
    ("f-coverage-line-follows-the-report", case_f_coverage_line_follows_the_report),
    ("f-uncovered-lists-missing-offsets", case_f_uncovered),
    ("f-uncovered-joins-runs", case_f_uncovered_run),
    ("f-uncovered-silent-at-full-coverage", case_f_uncovered_full),
    ("f-no-uncovered-line-without-the-flag", case_f_no_uncovered_without_flag),
    ("f-control-prints-no-coverage", case_f_control_prints_no_coverage),
    ("f-low-coverage-alone-keeps-status-0", case_f_low_coverage_keeps_status),
    ("f-low-coverage-does-not-hide-a-difference", case_f_low_coverage_does_not_hide_a_difference),
    ("f-default-folder-is-the-tools-own", case_f_default_folder),
    ("f-folder-reaches-the-build", case_f_folder_reaches_the_build),
    ("f-contract-file-loaded-and-used", case_f_contract_file_loaded_and_used),
    ("f-table-entry-wins-over-file", case_f_table_wins_over_file),
    ("f-file-without-contract-status-2", case_f_file_without_contract),
    ("f-file-with-wrong-contract-status-2", case_f_file_with_wrong_contract),
    ("f-file-that-raises-status-2", case_f_file_that_raises),
    ("f-file-with-a-syntax-error-status-2", case_f_file_with_a_syntax_error),
    ("f-neither-table-nor-file-status-2", case_f_neither_table_nor_file),
    ("f-build-reads-the-source-from-the-folder", case_f_build_reads_the_source_from_the_folder),
    ("f-file-of-another-function-not-used", case_f_file_of_another_function_not_used),
    ("g-four-arguments-logged-in-order", case_g_four_arguments),
    ("g-argument-counts-0-to-3", case_g_argument_counts),
    ("g-unrecorded-argument-is-no-difference", case_g_unrecorded_argument_is_no_difference),
    ("g-recorded-argument-is-a-difference", case_g_recorded_argument_is_a_difference),
    ("g-fifth-and-sixth-argument-from-the-stack", case_g_stack_arguments),
    ("g-stack-argument-difference", case_g_stack_argument_difference),
    ("g-result-in-v0-in-full", case_g_result_in_v0),
    ("g-order-omission-and-value-are-differences", case_g_order_and_values),
    ("g-callee-body-not-run", case_g_callee_body_not_run),
    ("g-two-slot-callee-replaced-alone", case_g_two_slot_callee),
    ("g-ra-sp-and-saved-registers-left-alone", case_g_registers_left_alone),
    ("g-missing-call-is-seen-in-the-log", case_g_log_is_compared),
    ("h-recorder-result-differs-per-case", case_h_recorder_result_per_case),
    ("h-both-arms-reached-and-one-wrong-arm-seen", case_h_recorder_result_reaches_both_arms),
    ("h-code-written-by-setup-runs-as-written", case_h_code_written_by_setup_runs_as_written),
    ("i-main-uses-build-size-and-entry", case_i_main_uses_size_and_entry),
    ("i-entry-starts-the-build-there", case_i_entry_starts_the_build_there),
    ("i-control-sees-the-code-words-only", case_i_control_sees_the_code_only),
    ("i-control-outside-the-code-status-2", case_i_control_outside_the_code),
    ("i-section-sizes", case_i_section_sizes),
    ("j-addresses-symbols-win-over-functions", case_j_addresses),
    ("j-addresses-without-units", case_j_addresses_without_units),
    ("j-setup-is-given-the-table", case_j_setup_is_given_the_table),
    ("k-all-runs-each-source-once-in-sorted-order", case_k_all_runs_each_source_once_in_order),
    ("k-all-with-a-name-status-2", case_k_all_with_a_name),
    ("k-neither-all-nor-name-status-2", case_k_neither_all_nor_name),
    ("k-all-without-sources-status-2", case_k_all_without_sources),
    ("k-all-with-control-runs-each-control", case_k_all_with_control),
    ("l-drops-defined-names-in-all-forms", case_l_drops_defined_names_in_all_forms),
    ("l-keeps-names-that-only-begin-alike", case_l_keeps_names_that_only_begin_alike),
    ("l-keeps-names-that-end-alike", case_l_keeps_names_that_end_alike),
    ("l-keeps-comments-blanks-and-the-rest", case_l_keeps_comments_blanks_and_the_rest),
    ("l-no-names-no-change", case_l_no_names_no_change),
    ("l-keeps-line-endings", case_l_keeps_line_endings),
    ("l-misplaced-borders-and-unknown-names", case_l_misplaced_borders),
    ("l-misplaced-nothing-to-report", case_l_misplaced_nothing_to_report),
    ("l-misplaced-only-the-unit-names", case_l_misplaced_only_the_unit_names),
    ("l-linked-addresses", case_l_linked_addresses),
    ("n-register-pointee-follows-the-arguments", case_n_register_pointee),
    ("n-stack-pointee-fifth-argument", case_n_stack_pointee),
    ("n-pointees-in-index-order", case_n_pointees_in_index_order),
    ("n-pointee-difference-and-the-gap-without-it", case_n_pointee_seen_through_test_function),
    ("n-bad-pointee-index-or-count-refused", case_n_refused_pointees),
    ("o-watched-blocks-copied-at-every-call", case_o_watched_blocks_at_every_call),
    ("o-store-before-or-after-call-with-and-without-watch", case_o_order_of_store_and_call),
    ("o-bad-watch-address-or-count-refused", case_o_refused_watches),
    ("o-cursor-moves-by-the-whole-entry", case_o_cursor_moves_by_the_whole_entry),
    ("o-no-watch-no-extra-words", case_o_no_watch_no_extra_words),
    ("p-recorders-footprint-not-a-convention", case_p_recorders_footprint_not_a_convention),
    ("q-argument-logged-under-its-mask", case_q_argument_logged_under_its_mask),
    ("q-masked-out-bits-no-difference-kept-bits-difference", case_q_masked_out_bits_are_no_difference),
    ("q-bad-masks-refused", case_q_bad_masks_refused),
    ("r-three-results-then-the-last-again", case_r_three_results_then_the_last),
    ("r-results-replace-the-result-argument", case_r_results_replace_result),
    ("r-one-result-is-a-constant", case_r_one_result_is_a_constant),
    ("r-recorders-do-not-share-their-position", case_r_recorders_do_not_share_their_position),
    ("r-results-in-full-32-bits", case_r_full_32_bit_results),
    ("r-recorders-footprint-with-results-adds-only-at", case_r_footprint_with_results_adds_only_at),
    ("s-run-ended-at-the-nth-call", case_s_run_ended_at_the_nth_call),
    ("s-ended-run-is-completed-not-discarded", case_s_ended_run_is_completed_not_discarded),
    ("s-registers-not-compared-when-not-returning", case_s_registers_not_compared_when_not_returning),
    ("s-log-and-memory-still-compared-when-not-returning", case_s_log_and_memory_still_compared_when_not_returning),
    ("s-recorder-without-ends-run-at-returns-every-time", case_s_recorder_without_ends_run_at_returns_every_time),
    ("s-negative-ends-run-at-refused", case_s_negative_ends_run_at_refused),
    ("s-coverage-of-an-ended-run", case_s_coverage_of_an_ended_run),
    ("s-state-stop-is-the-default-stop-address", case_s_state_stop_is_where_the_run_ends),
    ("t-registers-skipped-when-not-returning", case_t_registers_skipped_when_not_returning),
    ("t-memory-compared-when-not-returning", case_t_memory_compared_when_not_returning),
    ("t-returns-defaults-to-true", case_t_returns_defaults_to_true),
    ("u-store-at-the-nth-call", case_u_store_at_the_nth_call),
    ("u-no-store-when-fewer-calls", case_u_no_store_when_fewer_calls),
    ("u-two-stores-at-different-calls", case_u_two_stores_at_different_calls),
    ("u-store-and-end-at-the-same-call", case_u_store_and_end_at_the_same_call),
    ("u-bad-stores-and-counts-refused", case_u_bad_stores_and_counts_refused),
    ("u-counts-go-up-after-the-entry", case_u_counts_go_up_after_the_entry),
    ("u-two-counted-words", case_u_two_counted_words),
    ("u-wait-loop-ends-by-itself", case_u_wait_loop_ends_by_itself),
    ("u-footprint-with-every-option", case_u_footprint_with_every_option),
    ("v-tail-stores-through-the-argument-and-returns-it", case_v_tail_stores_through_the_argument_and_returns_it),
    ("v-tail-with-a-counter-of-its-own", case_v_tail_with_a_counter_of_its_own),
    ("v-stores-counts-and-end-act-before-the-tail", case_v_stores_counts_and_end_act_before_the_tail),
    ("v-masked-local-at-two-places-equal-unmasked-different", case_v_masked_local_at_different_places_of_two_frames),
    ("v-exported-encoders-and-registers", case_v_exported_encoders_and_registers),
    ("w-mask-zero-logs-zero", case_w_mask_zero_logs_zero),
    ("w-other-bad-masks-still-refused", case_w_still_refused_masks),
    ("x-inside-a-block", case_x_inside_a_block),
    ("x-one-byte-past-the-block", case_x_one_byte_past_the_block),
    ("x-one-byte-before-the-block", case_x_one_byte_before_the_block),
    ("x-global-written-by-the-setup", case_x_global_written_by_the_setup),
    ("x-global-not-written-by-the-setup", case_x_global_not_written_by_the_setup),
    ("x-global-after-owns", case_x_global_after_owns),
    ("x-owns-covers-only-its-range", case_x_owns_covers_only_its_range),
    ("x-owns-outside-memory-refused", case_x_owns_outside_memory_refused),
    ("x-negative-owns-is-refused-and-the-record-keeps-its-size", case_x_negative_owns_is_refused_and_the_record_keeps_its_size),
    ("x-negative-owns-in-the-scratchpad-likewise", case_x_negative_owns_in_the_scratchpad_likewise),
    ("x-negative-sizes-are-refused-before-anything-changes", case_x_negative_sizes_are_refused_before_anything_changes),
    ("x-alloc-that-does-not-fit-leaves-the-arena", case_x_alloc_that_does_not_fit_leaves_the_arena),
    ("x-empty-spans-are-valid-and-make-nothing", case_x_empty_spans_are_valid_and_make_nothing),
    ("x-a-record-of-another-size-is-refused-by-the-audit", case_x_a_record_of_another_size_is_refused_by_the_audit),
    ("x-same-value-not-seen", case_x_same_value_not_seen),
    ("x-stack-region", case_x_stack_region),
    ("x-stack-borders", case_x_stack_borders),
    ("x-stack-borders-as-the-comparator-has-them", case_x_stack_borders_as_the_comparator_has_them),
    ("x-home-area", case_x_home_area),
    ("x-home-area-made-stays-made", case_x_home_area_made_stays_made),
    ("x-home-area-is-compared", case_x_home_area_is_compared),
    ("x-scratchpad-unmade", case_x_scratchpad_unmade),
    ("x-scratchpad-first-and-last-bytes", case_x_scratchpad_first_and_last_bytes),
    ("x-scratchpad-made", case_x_scratchpad_made),
    ("x-made-ram-does-not-cover-scratchpad", case_x_made_ram_does_not_cover_scratchpad),
    ("x-made-scratchpad-does-not-cover-ram", case_x_made_scratchpad_does_not_cover_ram),
    ("x-made-by-each-writer", case_x_made_by_each_writer),
    ("x-alloc-marks-the-whole-block", case_x_alloc_marks_the_whole_block),
    ("x-alloc-padding-is-not-made", case_x_alloc_padding_is_not_made),
    ("x-alloc-of-a-multiple-of-four-has-no-padding", case_x_alloc_of_a_multiple_of_four_has_no_padding),
    ("x-largest-and-first-over-the-cases", case_x_largest_and_first_over_the_cases),
    ("x-discarded-not-counted-as-outside", case_x_discarded_not_counted_as_outside),
    ("x-discarded-and-outside-in-one-run", case_x_discarded_and_outside_in_one_run),
    ("x-recorder-log-and-code-not-counted", case_x_recorder_log_and_code_not_counted),
    ("x-recorder-does-not-cover-a-store-of-the-function", case_x_recorder_does_not_cover_a_store_of_the_function),
    ("x-callee-body-would-write-outside", case_x_callee_body_would_write_outside),
    ("x-state-is-new-for-each-case", case_x_state_is_new_for_each_case),
    ("x-cases-use-the-seed-and-case-number", case_x_cases_use_the_seed_and_case_number),
    ("x-zero-cases", case_x_zero_cases),
    ("x-audit-builds-nothing", case_x_audit_builds_nothing),
    ("x-runs-join-within-sixteen-bytes", case_x_runs_join_within_sixteen_bytes),
    ("x-runs-sixteen-and-seventeen", case_x_runs_sixteen_and_seventeen),
    ("x-runs-six-then-more", case_x_runs_six_then_more),
    ("x-runs-nearest-name", case_x_runs_nearest_name),
    ("x-runs-name-must-share-the-region", case_x_runs_name_must_share_the_region),
    ("x-runs-name-at-the-address-itself", case_x_runs_name_at_the_address_itself),
    ("x-runs-tie-takes-the-last-name", case_x_runs_tie_takes_the_last_name),
    ("x-runs-ram-before-scratchpad", case_x_runs_ram_before_scratchpad),
    ("x-main-line", case_x_main_line),
    ("x-main-line-when-clean", case_x_main_line_when_clean),
    ("x-main-second-line", case_x_main_second_line),
    ("x-main-no-second-line-without-uncovered", case_x_main_no_second_line_without_uncovered),
    ("x-main-status", case_x_main_status),
    ("x-main-control-is-an-error", case_x_main_control_is_an_error),
    ("x-main-no-build", case_x_main_no_build),
    ("x-main-build-that-would-fail-is-not-tried", case_x_main_build_that_would_fail_is_not_tried),
    ("x-main-all-in-order-and-all-run", case_x_main_all_in_order_and_all_run),
    ("x-main-no-contract", case_x_main_no_contract),
    ("x-main-config-error", case_x_main_config_error),
    ("x-main-original-input-error", case_x_main_original_input_error),
    ("x-default-output-unchanged", case_x_default_output_unchanged),
    ("x-state-default-behaviour-unchanged", case_x_state_default_behaviour_unchanged),
    ("y-size-from-game-table", case_y_size_from_game_table),
    ("y-size-from-library-table", case_y_size_from_library_table),
    ("y-in-neither-table-is-an-error", case_y_in_neither_table_is_an_error),
    ("y-in-both-tables-is-an-error", case_y_in_both_tables_is_an_error),
]


def main() -> int:
    failed = 0
    for name, case in CASES:
        try:
            why = case()
        except Exception as exc:  # a case that raises is a case that fails
            why = f"raised {type(exc).__name__}: {exc}"
        if why:
            failed += 1
            print(f"FAIL {name}: {why}")
        else:
            print(f"ok {name}")
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
