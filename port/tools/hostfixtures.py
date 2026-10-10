#!/usr/bin/env python3
"""Replay the published fixtures of the game's functions on their C as compiled for the PC.

A `FUNC.fixtures.json` (format 1, written by `ps1/src/slot06_nonmatching/difftest.py --record`) holds, for one
function of the game, a few inputs with what the original code did on them: the calls it made, the memory it left,
the result. This tool builds the function's C with `hostreplay.py` (the cross compiler only; no PS1 compiler, no disc
image, no emulator, nothing private) and runs it on each fixture in the harness of `port/replay/`, then says whether
the PC build does what the original did on the recorded inputs. This is the only unit test of what the PC program
runs: a failure here is a finding about the port (an argument that the console's code leaves in a register, a result
of a function declared `void`, a read near address 0), not about the fixture.

It does not import `difftest.py` (that needs the emulator's package). `difftest.py`'s header is the definition of the
format, and the reader and the comparison here are written on their own. `port/replay/README.md`, "Replaying
fixtures", says what is compared, in which order, and what is not.

    hostfixtures.py --config BUILD_TOML --cc CC [--build DIR] [--jobs N] [--folder DIR ...] [--overrides DIR]
                    [--timeout SECONDS] [FUNC ...]

Without FUNC: every function that has a `*.fixtures.json` in the folders. Without --folder: the `*_nonmatching`
folders beside the configuration, but `library_nonmatching` (the PC build does not compile Sony's library C), and
`port/overrides`. A function of a folder that has a file `FUNC.c` in the overrides folder (default `port/overrides`) is
replayed with the override's C, which is what the PC program runs. Output: per function

    FUNC native replay: fixtures K, passed P, failed F[, stack arguments not compared S]
      fixture N (case C, NOTE):            one per failed fixture, then at most three lines of what differed first
        LINE
    FUNC native replay: not built: FIRST ERROR LINE
    FUNC native replay: not replayed: its fixtures hold calls of original callees (format 2)

and a last line with the counts. Status: 0 when nothing failed and everything was built, 1 otherwise, 2 for a file
that cannot be read or a name that no folder has (nothing is built then).

Cost: building one function's program takes about a second, a run well under a second; `--jobs N` (default 4)
builds and runs N functions at a time and prints the lines in the order of the names. Each function is built in its
own folder of `--build` (a temporary folder, removed at the end, without it). Python 3.11 or later, standard library
only; the programs are Windows programs and start directly.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import dataclasses
import json
import re
import shutil
import sys
import tempfile
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
REPO = TOOLS.parents[1]
sys.path.insert(0, str(TOOLS))

import hostreplay as hr  # noqa: E402

FORMAT = 1
RAM_BASE, RAM_END = 0x80000000, 0x80200000
SCRATCH_BASE, SCRATCH_END = 0x1F800000, 0x1F801000
STACK_LOW, STACK_HIGH = 0x801FE000, 0x801FF010  # the PS1's stack region and the 16 bytes above its top
POISON = 0xA5
FIXTURE_FIELDS = ("args", "calls", "case", "ends_in_callee", "note", "reads", "recorders", "result", "same", "seed", "watch", "writes")
CALL_FIELDS = ("args", "callee", "pointees", "returned", "watched")
FORMAT2_LINE = "not replayed: its fixtures hold calls of original callees (format 2)"


class FixtureError(Exception):
    """A fixtures file cannot be read, or a name has none; the text names the file."""


# ---------------------------------------------------------------------------------------------------------------
# Reading a file.


@dataclasses.dataclass
class Recorder:
    at: int
    callee: str
    arguments: int
    masks: dict[int, int]
    pointees: dict[int, int]  # argument index -> words behind it
    stores: list[tuple[int, int, int]]
    counts: list[int]
    ends_at: int


@dataclasses.dataclass
class Call:
    callee: str
    args: list[int]
    returned: int
    pointees: dict[int, list[tuple[int, bytes]]]  # argument index -> changes against the input state
    watched: list[list[tuple[int, bytes]]]  # one list of changes per watched block


@dataclasses.dataclass
class Fixture:
    number: int
    case: object
    note: str
    args: list[int]
    reads: dict[int, int]
    same: dict[int, int]
    writes: dict[int, int]
    recorders: list[Recorder]
    watch: list[tuple[int, int]]
    calls: list[Call]
    result: int | None
    ends_in_callee: bool


@dataclasses.dataclass
class Doc:
    name: str
    path: Path
    format: int
    fixtures: list[Fixture]


def _in_memory(address: int, size: int) -> bool:
    return (RAM_BASE <= address and address + size <= RAM_END) or (SCRATCH_BASE <= address and address + size <= SCRATCH_END)


class _Reader:
    """Checks the shape of one file, naming it in every error."""

    def __init__(self, path: Path):
        self.file = path.name
        self.where = ""

    def fail(self, text: str):
        raise FixtureError(f"{self.file}: {self.where}{text}")

    def word(self, value, what: str) -> int:
        if not isinstance(value, str) or not re.fullmatch(r"0x[0-9a-f]{1,8}", value):
            self.fail(f"{what} must be a hexadecimal word like \"0x80000000\", not {value!r}")
        return int(value, 16)

    def count(self, value, what: str, low: int = 0) -> int:
        if not isinstance(value, int) or isinstance(value, bool) or value < low:
            self.fail(f"{what} must be an integer of at least {low}, not {value!r}")
        return value

    def data(self, value, what: str) -> bytes:
        if not isinstance(value, str) or not re.fullmatch(r"(?:[0-9a-f]{2})*", value):
            self.fail(f"{what} must be hexadecimal text of whole bytes, not {value!r}")
        return bytes.fromhex(value)

    def listing(self, value, what: str) -> list:
        if not isinstance(value, list):
            self.fail(f"{what} must be a list")
        return value

    def runs(self, value, what: str) -> dict[int, int]:
        out: dict[int, int] = {}
        for item in self.listing(value, what):
            if not isinstance(item, list) or len(item) != 2:
                self.fail(f"an entry of {what} must be [address, bytes], not {item!r}")
            address, data = self.word(item[0], f"an address in {what}"), self.data(item[1], f"the bytes in {what}")
            if not data or not _in_memory(address, len(data)):
                self.fail(f"the run of {what} at {address:#x} is empty or outside RAM and the scratchpad")
            for offset, byte in enumerate(data):
                out[address + offset] = byte
        return out

    def changes(self, value, size: int, what: str) -> list[tuple[int, bytes]]:
        out = []
        for item in self.listing(value, what):
            if not isinstance(item, list) or len(item) != 2:
                self.fail(f"a change in {what} must be [offset, bytes], not {item!r}")
            offset, data = self.count(item[0], f"an offset in {what}"), self.data(item[1], f"the bytes in {what}")
            if not data or offset + len(data) > size:
                self.fail(f"a change in {what} at offset {offset} of {len(data)} bytes does not lie inside the {size} bytes it is against")
            out.append((offset, data))
        return out

    def recorder(self, raw) -> Recorder:
        if not isinstance(raw, dict):
            self.fail("a recorder must be an object")
        for field in ("at", "callee", "arguments"):
            if field not in raw:
                self.fail(f"a recorder lacks {field}")
        if not isinstance(raw["callee"], str) or not raw["callee"]:
            self.fail("the callee of a recorder must be a name")
        arguments = self.count(raw["arguments"], "the arguments of a recorder")
        at = self.word(raw["at"], "the address of a recorder")
        if not _in_memory(at, 5):
            self.fail(f"the recorder at {at:#x} lies outside RAM and the scratchpad")

        def keyed(field, convert):
            found = raw.get(field, {})
            if not isinstance(found, dict):
                self.fail(f"{field} of a recorder must be an object")
            out = {}
            for key, value in found.items():
                if not (isinstance(key, str) and key.isdigit() and int(key) < arguments):
                    self.fail(f"{field} of a recorder names argument {key!r} of {arguments}")
                out[int(key)] = convert(value)
            return out

        masks = keyed("masks", lambda v: self.count(v, "a mask"))
        pointees = keyed("pointees", lambda v: self.count(v, "a pointee length"))
        stores = []
        for item in self.listing(raw.get("stores", []), "stores of a recorder"):
            if not isinstance(item, list) or len(item) != 3:
                self.fail(f"a store of a recorder must be [call, address, value], not {item!r}")
            stores.append((self.count(item[0], "the call of a store", 1), self.word(item[1], "the address of a store"), self.word(item[2], "the value of a store")))
        counts = [self.word(x, "the address of a count") for x in self.listing(raw.get("counts", []), "counts of a recorder")]
        return Recorder(at, raw["callee"], arguments, masks, pointees, stores, counts, self.count(raw.get("ends_run_at", 0), "ends_run_at"))

    def call(self, raw, recorders: dict[str, Recorder], watch: list[tuple[int, int]]) -> Call:
        if not isinstance(raw, dict):
            self.fail("a call must be an object")
        missing = [f for f in CALL_FIELDS if f not in raw]
        if missing:
            self.fail(f"a call lacks {', '.join(missing)}")
        if "original" in raw:
            self.fail("a call is marked `original`; that is format 2, and this file says format 1")
        callee = raw["callee"]
        if callee not in recorders:
            self.fail(f"a call to {callee!r}, which no recorder stands for")
        recorder = recorders[callee]
        args = [self.word(x, "an argument of a call") for x in self.listing(raw["args"], "the arguments of a call")]
        if not isinstance(raw["pointees"], dict):
            self.fail("the pointees of a call must be an object")
        pointees = {}
        for key, spans in raw["pointees"].items():
            if not (isinstance(key, str) and key.isdigit() and int(key) in recorder.pointees and int(key) < len(args)):
                self.fail(f"a call to {callee} has pointee {key!r}, which its recorder does not read")
            pointees[int(key)] = self.changes(spans, 4 * recorder.pointees[int(key)], f"pointee {key} of a call to {callee}")
        blocks = self.listing(raw["watched"], "the watched blocks of a call")
        if len(blocks) != len(watch):
            self.fail(f"a call to {callee} has {len(blocks)} watched blocks, the fixture watches {len(watch)}")
        watched = [self.changes(b, 4 * n, f"watched block {k + 1} of a call to {callee}") for k, (b, (_a, n)) in enumerate(zip(blocks, watch))]
        return Call(callee, args, self.word(raw["returned"], "the returned value of a call"), pointees, watched)

    def fixture(self, number: int, raw) -> Fixture:
        self.where = f"fixture {number}: "
        if not isinstance(raw, dict):
            self.fail("it is not an object")
        if "needs_image" in raw:
            self.fail("it has the field needs_image; fixtures do not stand in for callees that run as original code")
        missing = [f for f in FIXTURE_FIELDS if f not in raw]
        if missing:
            self.fail(f"it lacks {', '.join(missing)}")
        recorders = [self.recorder(r) for r in self.listing(raw["recorders"], "recorders")]
        by_name = {r.callee: r for r in recorders}
        watch = []
        for item in self.listing(raw["watch"], "watch"):
            if not isinstance(item, list) or len(item) != 2:
                self.fail(f"a watch entry must be [address, words], not {item!r}")
            address, words = self.word(item[0], "a watched address"), self.count(item[1], "a watched word count", 1)
            if not _in_memory(address, 4 * words):
                self.fail(f"the watched block at {address:#x} lies outside RAM and the scratchpad")
            watch.append((address, words))
        calls = [self.call(c, by_name, watch) for c in self.listing(raw["calls"], "calls")]
        args = [self.word(x, "an argument") for x in self.listing(raw["args"], "args")]
        if len(args) > hr.MAX_ARGS:
            self.fail(f"it has {len(args)} arguments, the harness takes {hr.MAX_ARGS}")
        result = raw["result"]
        if result is not None:
            result = self.word(result, "the result")
        if not isinstance(raw["ends_in_callee"], bool):
            self.fail("ends_in_callee must be true or false")
        if not isinstance(raw["note"], str):
            self.fail("the note must be text")
        return Fixture(number, raw["case"], raw["note"], args, self.runs(raw["reads"], "reads"), self.runs(raw["same"], "same"),
                       self.runs(raw["writes"], "writes"), recorders, watch, calls, result, raw["ends_in_callee"])


def load_fixtures(path: Path, name: str) -> Doc:
    """The fixtures file `path` of function `name`, checked for its shape. Raises FixtureError, naming the file.

    A file of format 2 is returned with no fixtures read (this tool does not replay it)."""
    try:
        doc = json.loads(path.read_text())
    except OSError as err:
        raise FixtureError(f"cannot read {path.name}: {err.strerror or err}")
    except ValueError as err:
        raise FixtureError(f"{path.name} is not JSON: {err}")
    if not isinstance(doc, dict):
        raise FixtureError(f"{path.name}: the file is not an object")
    if doc.get("format") not in (1, 2) or isinstance(doc.get("format"), bool):
        raise FixtureError(f"{path.name}: format {doc.get('format')!r}, this tool reads format {FORMAT}")
    if doc.get("function") != name:
        raise FixtureError(f"{path.name}: the file is for {doc.get('function')!r}, not {name}")
    if not isinstance(doc.get("fixtures"), list) or not doc["fixtures"]:
        raise FixtureError(f"{path.name}: no fixtures")
    if doc["format"] == 2:
        return Doc(name, path, 2, [])
    reader = _Reader(path)
    return Doc(name, path, 1, [reader.fixture(n, raw) for n, raw in enumerate(doc["fixtures"])])


# ---------------------------------------------------------------------------------------------------------------
# The return type.


@dataclasses.dataclass(frozen=True)
class ReturnType:
    text: str
    kind: str  # "void", "bits" or "word"
    bits: int = 32


_QUALIFIERS = {"static", "extern", "inline", "__inline", "__inline__", "const", "volatile", "register"}


def return_type(source: str, name: str) -> ReturnType:
    """The type before the name on the definition line of `name` in the text of its file. Raises ValueError."""
    text = re.sub(r"/\*.*?\*/", " ", source, flags=re.S)
    text = re.sub(r"//[^\n]*", " ", text)
    text = re.sub(r"(?m)^[ \t]*#(?:[^\n\\]|\\\n|\\.)*$", " ", text)
    for match in re.finditer(r"\b" + re.escape(name) + r"\s*\([^;{]*?\)\s*\{", text):
        before = text[: match.start()]
        typ = before[max(before.rfind(";"), before.rfind("}")) + 1:].strip()
        if not typ or re.search(r"[^A-Za-z0-9_\s*]", typ):
            continue
        tokens = [t for t in re.findall(r"[A-Za-z_]\w*|\*", typ) if t not in _QUALIFIERS]
        # A pointer (a `*` among the tokens) is in none of the narrow sets below, so it is a whole word.
        words = set(tokens)
        if tokens == ["void"]:
            return ReturnType(typ, "void")
        if words & {"u8", "s8", "char"} and words <= {"u8", "s8", "char", "unsigned", "signed"}:
            return ReturnType(typ, "bits", 8)
        if words & {"u16", "s16", "short"} and words <= {"u16", "s16", "short", "unsigned", "signed", "int"}:
            return ReturnType(typ, "bits", 16)
        return ReturnType(typ, "word")
    raise ValueError(f"no definition of {name} with a return type in its file")


# ---------------------------------------------------------------------------------------------------------------
# From a fixture to a case.


def input_memory(fx: Fixture) -> dict[int, int]:
    """The bytes the case states: `reads`, then `same`, then each byte of `writes` that neither names, as its
    expected value with every bit flipped. Every other byte is the harness's filler."""
    memory = dict(fx.reads)
    memory.update(fx.same)
    for address, byte in fx.writes.items():
        if address not in fx.reads and address not in fx.same:
            memory[address] = byte ^ 0xFF
    return memory


def _runs(memory: dict[int, int]) -> list[tuple[str, int, bytes]]:
    runs: list[tuple[int, bytearray]] = []
    for address in sorted(memory):
        if runs and runs[-1][0] + len(runs[-1][1]) == address:
            runs[-1][1].append(memory[address])
        else:
            runs.append((address, bytearray([memory[address]])))
    return [("bytes", a, bytes(b)) for a, b in runs]


def build_case(fx: Fixture) -> dict:
    """The case of the harness for one fixture (`port/replay/README.md`)."""
    returned: dict[str, list[int]] = {}
    for call in fx.calls:
        returned.setdefault(call.callee, []).append(call.returned)
    stubs = [{"at": r.at, "arguments": r.arguments, "masks": dict(r.masks), "pointees": dict(r.pointees), "returns": returned.get(r.callee, []),
              "stores": list(r.stores), "counts": list(r.counts), "ends_at": r.ends_at} for r in fx.recorders]
    return {"memory": _runs(input_memory(fx)), "args": list(fx.args), "stubs": stubs, "watch": list(fx.watch)}


# ---------------------------------------------------------------------------------------------------------------
# Comparing a result with its fixture.


def hex8(value: int) -> str:
    return f"{value & 0xFFFFFFFF:#010x}"


def _input_bytes(memory: dict[int, int], address: int, size: int) -> bytes | None:
    """`size` bytes at `address` of the case's input state; None when they do not lie inside RAM or the scratchpad."""
    if not _in_memory(address, size):
        return None
    return bytes(memory.get(address + k, POISON) for k in range(size))


def _with_changes(base: bytes, spans: list[tuple[int, bytes]]) -> bytes:
    block = bytearray(base)
    for offset, data in spans:
        block[offset: offset + len(data)] = data
    return bytes(block)


def _fault_line(fault: dict) -> str:
    kind, address, pc = fault["kind"], fault["address"], fault["pc"]
    if kind in ("read", "write"):
        return f"the PC build faults: {kind} at {hex8(address)} from the instruction at {hex8(pc)}"
    if kind == "exec":
        return f"the PC build faults: it called {hex8(address)}, where no stand-in stands"
    if kind == "calls":
        return "the PC build faults: it called the stand-ins more than 100000 times in one case"
    return f"the PC build faults: exception {hex8(address)} at the instruction {hex8(pc)}"


def _end_line(fx: Fixture, res: dict) -> str | None:
    end = res["end"]
    if end == "fault":
        return _fault_line(res["fault"])
    if end == "died":
        return f"the PC build dies: the process ended with status {res.get('status')}"
    if end == "timeout":
        return "the PC build does not return: no answer within the time allowed"
    if end == "returned" and fx.ends_in_callee:
        return "the PC build returned, the original ended inside a callee that does not return"
    if end == "stub" and not fx.ends_in_callee:
        return "the PC build ended inside a callee that stops the run, the original returned"
    if end not in ("returned", "stub"):
        return f"the PC build ended in a way this tool does not know: {end!r}"
    return None


def _call_line(fx: Fixture, res: dict, label: dict[int, str]) -> tuple[str | None, int]:
    """(the first line in which the calls differ, or None; the arguments taken as equal because they are PC stack addresses)."""
    got, stack_equal = res["calls"], 0
    for number in range(1, max(len(got), len(fx.calls)) + 1):
        want = fx.calls[number - 1] if number <= len(fx.calls) else None
        seen = got[number - 1] if number <= len(got) else None
        if seen is None:
            return f"call {number}: missing, the original called {want.callee}", stack_equal
        name = label.get(seen["at"], hex8(seen["at"]))
        if want is None:
            return f"call {number}: extra, the PC build called {name}", stack_equal
        if name != want.callee:
            return f"call {number}: the PC build called {name}, the original called {want.callee}", stack_equal
        if len(seen["args"]) != len(want.args):
            return f"call {number} to {name}: the PC build logged {len(seen['args'])} arguments, the original {len(want.args)}", stack_equal
        for index, (original, native) in enumerate(zip(want.args, seen["args"])):
            if index in seen["stack_args"]:
                if STACK_LOW <= original < STACK_HIGH:
                    stack_equal += 1
                    continue
                return f"call {number} to {name}: argument {index + 1} is an address of the PC's stack, the original gave {hex8(original)}", stack_equal
            if original != native:
                return f"call {number} to {name}: argument {index + 1} is {hex8(native)}, the original gave {hex8(original)}", stack_equal
    return None, stack_equal


def _block_line(number: int, callee: str, place: str, expected: bytes, seen: bytes, only: list[tuple[int, bytes]] | None = None) -> str | None:
    """A line when a block at a call is not what the fixture says. `only` limits the check to the bytes of those changes."""
    if only is None:
        if len(seen) != len(expected):
            return f"call {number} to {callee}: {place} has {len(seen)} bytes in the PC build, {len(expected)} are expected"
        indices = range(len(expected))
    else:
        indices = [offset + k for offset, data in only for k in range(len(data))]
    for at in indices:
        if at >= len(seen) or seen[at] != expected[at]:
            have = f"{seen[at]:#04x}" if at < len(seen) else "nothing"
            return (f"call {number} to {callee}: {place} differs from byte {at} (the PC build has {have} there at this call, "
                    f"the original had {expected[at]:#04x})")
    return None


def _block_lines(fx: Fixture, res: dict, memory: dict[int, int]) -> str | None:
    for number, (want, seen) in enumerate(zip(fx.calls, res["calls"]), 1):
        places = []
        for index in sorted(want.pointees):
            address, length = want.args[index], 4 * next(r for r in fx.recorders if r.callee == want.callee).pointees[index]
            on_stack = index in seen["stack_args"]
            places.append((f"argument {index + 1}", address, length, want.pointees[index], seen["pointees"].get(index), on_stack))
        for k, (block, words) in enumerate(fx.watch):
            places.append((f"watched block {k + 1}", block, 4 * words, want.watched[k], seen["watched"][k] if k < len(seen["watched"]) else None, False))
        for place, address, length, spans, got, on_stack in places:
            if got is None:
                return f"call {number} to {want.callee}: the PC build logged nothing for {place}"
            if on_stack:
                # What a local held before the call is the PC's own; only the bytes the original changed are compared.
                expected = _with_changes(bytes(length), spans)
                line = _block_line(number, want.callee, place, expected, got, only=spans)
            else:
                base = _input_bytes(memory, address, length)
                if base is None:
                    return f"call {number} to {want.callee}: {place} lies at {hex8(address)}, outside RAM and the scratchpad"
                line = _block_line(number, want.callee, place, _with_changes(base, spans), got)
            if line:
                return line
    return None


def _result_line(fx: Fixture, res: dict, ret: ReturnType) -> str | None:
    if fx.result is None or fx.ends_in_callee:
        return None
    if ret.kind == "void":
        return f"the C returns nothing, the original's result was {hex8(fx.result)}"
    mask = 0xFFFFFFFF if ret.kind == "word" else (1 << ret.bits) - 1
    if res["result"] & mask == fx.result & mask:
        return None
    part = "" if ret.kind == "word" else f" (compared in the low {ret.bits} bits, the type is `{ret.text}`)"
    return f"result: the PC build gives {hex8(res['result'])}, the original gave {hex8(fx.result)}{part}"


def _write_lines(fx: Fixture, res: dict, memory: dict[int, int]) -> list[str]:
    lines = []
    changes = res["changes"]
    for address in sorted(set(fx.writes) | set(changes)):
        want = fx.writes.get(address)
        if want is None:
            lines.append(f"extra write at {hex8(address)}: the PC build left {changes[address]:#04x}, the original wrote nothing there")
        elif address not in changes:
            lines.append(f"missing write at {hex8(address)}: the PC build leaves {memory.get(address, POISON):#04x}, the original left {want:#04x}")
        elif changes[address] != want:
            lines.append(f"wrong value at {hex8(address)}: the PC build leaves {changes[address]:#04x}, the original left {want:#04x}")
    return lines


def compare(fx: Fixture, res: dict, ret: ReturnType) -> tuple[list[str], int]:
    """(the lines that say how the PC build differs from the fixture, empty when it passes; the arguments that were
    PC stack addresses and so taken as equal). Order: how it ended, the calls, the blocks at the calls, the result,
    the writes. How it ended, the calls and the blocks give their first difference only."""
    memory = input_memory(fx)
    line = _end_line(fx, res)
    if line:
        return [line], 0
    label = {r.at: r.callee for r in fx.recorders}
    lines = []
    line, stack_equal = _call_line(fx, res, label)
    if line:
        lines.append(line)
    else:
        block = _block_lines(fx, res, memory)
        if block:
            lines.append(block)
    line = _result_line(fx, res, ret)
    if line:
        lines.append(line)
    lines += _write_lines(fx, res, memory)
    return lines, stack_equal


# ---------------------------------------------------------------------------------------------------------------
# The functions.


@dataclasses.dataclass
class Job:
    name: str
    doc_path: Path
    source: Path  # the C that is built: the override's when the function has one
    override: Path | None = None  # set when `source` is an override that stands in place of the function's own C
    folder: Path | None = None


@dataclasses.dataclass
class Outcome:
    name: str
    lines: list[str]
    kind: str  # "passed", "failed", "not built", "not replayed"
    fixtures: int = 0
    passed: int = 0
    stack: int = 0


def _first_line(text: str) -> str:
    return next((x.strip() for x in str(text).splitlines() if x.strip()), "no message")


def replay_job(job: Job, doc: Doc, config: Path, build: Path, cc: str, timeout: float = 10, build_timeout: int = 300) -> Outcome:
    """Build the function's C and run its fixtures. Every way to end is an Outcome; nothing raises for a bad C."""
    head = f"{job.name} native replay: "
    lines = [head + f"the PC program runs the override {job.source.name} of {job.source.parent.name}; it is replayed in place of the nonmatching C"] if job.override else []
    if doc.format == 2:
        return Outcome(job.name, lines + [head + FORMAT2_LINE], "not replayed")
    try:
        ret = return_type(job.source.read_text(), job.name)
    except OSError as err:
        return Outcome(job.name, lines + [head + f"not built: cannot read {job.source.name}: {err.strerror or err}"], "not built")
    except ValueError as err:
        return Outcome(job.name, lines + [head + f"not built: {job.source.name}: {err}; its return type cannot be read"], "not built")
    try:
        # An override has a folder of its own: hostreplay reuses a program that is newer than its inputs, and a program built
        # from the nonmatching C is not one for the override, whose path is not among its inputs.
        exe = hr.build_program(job.source, job.name, config, build / (job.name + (".override" if job.override else "")), cc, build_timeout)
    except hr.ReplayError as err:
        return Outcome(job.name, lines + [head + f"not built: {_first_line(err)}"], "not built")
    try:
        results = hr.run_cases(exe, [build_case(fx) for fx in doc.fixtures], timeout)
    except (hr.ReplayError, ValueError) as err:
        return Outcome(job.name, lines + [head + f"not built: the harness refuses the cases: {_first_line(err)}"], "not built")
    if len(results) != len(doc.fixtures):  # a missing result is missing evidence, not a pass
        return Outcome(job.name, lines + [head + f"not built: the harness gave {len(results)} results for {len(doc.fixtures)} cases"], "not built")
    report, passed, stack = [], 0, 0
    for fx, res in zip(doc.fixtures, results):
        found, equal = compare(fx, res, ret)
        stack += equal
        if found:
            note = fx.note if len(fx.note) <= 70 else fx.note[:67] + "..."
            report.append(f"  fixture {fx.number} (case {fx.case}, {note}):")
            report.extend(f"    {x}" for x in found[:3])
        else:
            passed += 1
    total = len(doc.fixtures)
    summary = head + f"fixtures {total}, passed {passed}, failed {total - passed}" + (f", stack arguments not compared {stack}" if stack else "")
    return Outcome(job.name, lines + [summary, *report], "failed" if passed < total else "passed", total, passed, stack)


def default_folders(config: Path) -> list[Path]:
    folders = [p for p in sorted(config.parent.glob("*_nonmatching")) if p.is_dir() and p.name != "library_nonmatching"]
    return folders + [REPO / "port" / "overrides"]


def discover(folders: list[Path], overrides: Path, names: list[str]) -> list[Job]:
    """The functions to replay, sorted by name. Raises FixtureError for a name that no folder has, or that two do."""
    found: dict[str, list[Job]] = {}
    for folder in folders:
        for path in sorted(folder.glob("*.fixtures.json")):
            name = path.name[: -len(".fixtures.json")]
            source = folder / f"{name}.c"
            job = Job(name, path, source, folder=folder)
            if folder.resolve() != overrides.resolve() and (overrides / f"{name}.c").is_file():
                job.source, job.override = overrides / f"{name}.c", overrides / f"{name}.c"
            found.setdefault(name, []).append(job)
    for name in names:
        if name not in found:
            raise FixtureError(f"{name}: no fixtures file {name}.fixtures.json in {', '.join(f.name for f in folders)}")
    for name, jobs in found.items():
        if len({j.doc_path.resolve() for j in jobs}) > 1 and (not names or name in names):
            raise FixtureError(f"{name}: fixtures files in two folders: {jobs[0].doc_path.parent.name} and {jobs[1].doc_path.parent.name}")
    if not found:
        raise FixtureError(f"no fixtures file in {', '.join(f.name for f in folders)}")
    chosen = names or sorted(found)
    return [found[name][0] for name in sorted(set(chosen))]


def run(config: Path, cc: str, build: Path, jobs: int, folders: list[Path], overrides: Path, names: list[str], timeout: float, out, err) -> int:
    """The whole command; the lines go to `out`, the input errors to `err`. Returns the status."""
    if not config.is_file():
        print(f"INPUT ERROR: {config} is not a file", file=err)
        return 2
    try:
        selected = discover(folders, overrides, names)
        docs = [load_fixtures(job.doc_path, job.name) for job in selected]
    except FixtureError as exc:
        print(f"INPUT ERROR: {exc}", file=err)
        return 2
    outcomes: list[Outcome] = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, jobs)) as pool:
        futures = [pool.submit(replay_job, job, doc, config, build, cc, timeout) for job, doc in zip(selected, docs)]
        for future in futures:  # in the order of the names, whatever the jobs
            outcome = future.result()
            outcomes.append(outcome)
            for line in outcome.lines:
                print(line, file=out)
            out.flush()
    count = {k: sum(1 for o in outcomes if o.kind == k) for k in ("passed", "failed", "not built", "not replayed")}
    print(f"native replay: functions replayed {count['passed'] + count['failed']}, all passed {count['passed']}, with a failure {count['failed']}, "
          f"not built {count['not built']}, not replayed {count['not replayed']}", file=out)
    return 1 if count["failed"] or count["not built"] else 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Replay the published fixtures on the functions as compiled for the PC.")
    parser.add_argument("--config", type=Path, required=True)
    parser.add_argument("--cc", default="i686-w64-mingw32-gcc")
    parser.add_argument("--build", type=Path, default=None, help="the build folder (default: a temporary one, removed at the end)")
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--folder", type=Path, action="append", default=None, help="a folder of fixtures files and C (repeat; default: the *_nonmatching folders but library_nonmatching, and port/overrides)")
    parser.add_argument("--overrides", type=Path, default=REPO / "port" / "overrides")
    parser.add_argument("--timeout", type=float, default=10, help="seconds a case may take")
    parser.add_argument("functions", nargs="*", metavar="FUNC")
    args = parser.parse_args(argv)
    if args.jobs < 1:
        print("INPUT ERROR: --jobs needs 1 or more", file=sys.stderr)
        return 2
    folders = args.folder if args.folder is not None else default_folders(args.config)
    temporary = args.build is None
    build = Path(tempfile.mkdtemp(prefix="hostfixtures-")) if temporary else args.build
    try:
        return run(args.config, args.cc, build, args.jobs, folders, args.overrides, args.functions, args.timeout, sys.stdout, sys.stderr)
    finally:
        if temporary:
            shutil.rmtree(build, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
