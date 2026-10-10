#!/usr/bin/env python3
"""Build and run ONE function of the game's C, as compiled for the PC, on described cases.

The first half of the native replay of fixtures: this module compiles a C file that defines one function with the
flags and the rename that `hostbuild.py` uses for a unit, links it with the generic harness `port/replay/harness.c`
so that every name the object refers to is at its PS1 address, and runs cases in that program. It reports what the
function returned, which stand-ins it called with which arguments, and which bytes of the PS1's memory it changed.
Nothing of the game is read but the C file, the build configuration and the symbol file next to it; no disc image,
no PS1 compiler. `hostbuild.py` is imported and not changed. Python 3.11 or later, standard library only.

    build_program(source, name, config, build, cc, timeout)  ->  BUILD/NAME.exe
    run_cases(program, cases, timeout)                       ->  one result per case

`port/replay/README.md` describes a case, a result and the two text formats. The formats are written and read here
(`format_cases`, `parse_cases`, `format_result`, `parse_results`) and by the harness.

usage:
  hostreplay.py --config BUILD_TOML --cc CC --build DIR --source FILE.c --name NAME --cases FILE.json [--timeout SECONDS]
                [--run PREFIX]

The cases file is a JSON list of cases; in it `bytes` are hexadecimal text, `memory` entries are lists, numbers are
integers. The results are printed as JSON, bytes as hexadecimal text and the keys of `changes` as hexadecimal text.
"""

from __future__ import annotations

import argparse
import json
import os
import queue
import re
import subprocess
import sys
import threading
import time
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
REPO = TOOLS.parents[1]
HARNESS = REPO / "port" / "replay" / "harness.c"
sys.path.insert(0, str(TOOLS))

import hostbuild as hb  # noqa: E402

RAM = (0x80000000, 0x200000)
SCRATCH = (0x1F800000, 0x1000)
MAX_ARGS = 8
FILL = 0xA5


class ReplayError(Exception):
    """The build or the use is wrong; the text names the first thing that is."""


# ---------------------------------------------------------------------------------------------------------------
# Cases and results as text.


def _word(value, what: str) -> int:
    if not isinstance(value, int) or isinstance(value, bool) or not -0x80000000 <= value <= 0xFFFFFFFF:
        raise ValueError(f"{what} must be a 32-bit integer, not {value!r}")
    return value & 0xFFFFFFFF


def normalize_case(case: dict) -> dict:
    """The case with every field present and every number a plain unsigned integer; ValueError when it is not one."""
    unknown = sorted(set(case) - {"memory", "args", "stubs", "watch"})
    if unknown:
        raise ValueError(f"a case has no field `{unknown[0]}`")
    memory = []
    for item in case.get("memory", []):
        kind = item[0] if item else None
        if kind == "fill" and len(item) == 4:
            byte = _word(item[3], "a fill byte")
            if byte > 255:
                raise ValueError(f"a fill byte is at most 0xff, not {byte:#x}")
            memory.append(("fill", _word(item[1], "an address"), _word(item[2], "a length"), byte))
        elif kind == "bytes" and len(item) == 3:
            data = bytes(item[2])
            memory.append(("bytes", _word(item[1], "an address"), data))
        else:
            raise ValueError(f"a memory entry is ('fill', address, length, byte) or ('bytes', address, bytes), not {item!r}")
    args = [_word(a, "an argument") for a in case.get("args", [])]
    if len(args) > MAX_ARGS:
        raise ValueError(f"at most {MAX_ARGS} arguments, not {len(args)}")
    stubs = []
    for stub in case.get("stubs", []):
        extra = sorted(set(stub) - {"at", "arguments", "masks", "pointees", "returns", "stores", "counts", "ends_at"})
        if extra:
            raise ValueError(f"a stub has no field `{extra[0]}`")
        if "at" not in stub or "arguments" not in stub:
            raise ValueError("a stub needs `at` and `arguments`")
        arguments = _word(stub["arguments"], "`arguments`")
        masks = {int(k): _word(v, "a mask") for k, v in stub.get("masks", {}).items()}
        pointees = {int(k): _word(v, "a pointee length") for k, v in stub.get("pointees", {}).items()}
        for index in [*masks, *pointees]:
            if not 0 <= index < arguments:
                raise ValueError(f"argument {index} of {arguments} has a mask or a pointee")
        stubs.append({
            "at": _word(stub["at"], "`at`"), "arguments": arguments, "masks": dict(sorted(masks.items())),
            "pointees": dict(sorted(pointees.items())), "returns": [_word(v, "a returned value") for v in stub.get("returns", [])],
            "stores": [(_word(n, "a call number"), _word(a, "an address"), _word(v, "a value")) for n, a, v in stub.get("stores", [])],
            "counts": [_word(a, "an address") for a in stub.get("counts", [])], "ends_at": _word(stub.get("ends_at", 0), "`ends_at`"),
        })
    watch = [(_word(a, "an address"), _word(n, "a word count")) for a, n in case.get("watch", [])]
    return {"memory": memory, "args": args, "stubs": stubs, "watch": watch}


def format_cases(cases: list[dict]) -> str:
    """The cases as the text the harness reads (numbers in hexadecimal, one directive a line)."""
    out = []
    for case in map(normalize_case, cases):
        out.append("case")
        for item in case["memory"]:
            if item[0] == "fill":
                out.append(f"fill {item[1]:x} {item[2]:x} {item[3]:x}")
            else:
                out.append(f"bytes {item[1]:x} {item[2].hex()}")
        if case["args"]:
            out.append("args " + " ".join(f"{a:x}" for a in case["args"]))
        for stub in case["stubs"]:
            out.append(f"stub {stub['at']:x} {stub['arguments']:x} {stub['ends_at']:x}")
            out += [f"mask {i:x} {m:x}" for i, m in stub["masks"].items()]
            out += [f"pointee {i:x} {n:x}" for i, n in stub["pointees"].items()]
            out += [f"return {v:x}" for v in stub["returns"]]
            out += [f"store {n:x} {a:x} {v:x}" for n, a, v in stub["stores"]]
            out += [f"count {a:x}" for a in stub["counts"]]
        out += [f"watch {a:x} {n:x}" for a, n in case["watch"]]
        out.append("go")
    return "".join(x + "\n" for x in out)


def parse_cases(text: str) -> list[dict]:
    """The inverse of `format_cases`: the normalized cases of a text."""
    cases: list[dict] = []
    case: dict | None = None
    for number, line in enumerate(text.splitlines(), 1):
        parts = line.split()
        if not parts:
            continue
        key, rest = parts[0], parts[1:]
        try:
            if key == "case":
                case = {"memory": [], "args": [], "stubs": [], "watch": []}
            elif case is None:
                raise ValueError("a line outside a case")
            elif key == "fill":
                case["memory"].append(("fill", *(int(x, 16) for x in rest)))
            elif key == "bytes":
                case["memory"].append(("bytes", int(rest[0], 16), bytes.fromhex(rest[1] if len(rest) > 1 else "")))
            elif key == "args":
                case["args"] = [int(x, 16) for x in rest]
            elif key == "stub":
                at, arguments, ends_at = (int(x, 16) for x in rest)
                case["stubs"].append({"at": at, "arguments": arguments, "masks": {}, "pointees": {}, "returns": [], "stores": [], "counts": [], "ends_at": ends_at})
            elif key in ("mask", "pointee", "return", "store", "count"):
                stub = case["stubs"][-1]
                values = [int(x, 16) for x in rest]
                if key == "mask":
                    stub["masks"][values[0]] = values[1]
                elif key == "pointee":
                    stub["pointees"][values[0]] = values[1]
                elif key == "return":
                    stub["returns"].append(values[0])
                elif key == "store":
                    stub["stores"].append(tuple(values))
                else:
                    stub["counts"].append(values[0])
            elif key == "watch":
                case["watch"].append(tuple(int(x, 16) for x in rest))
            elif key == "go":
                cases.append(case)
                case = None
            else:
                raise ValueError(f"unknown directive {key}")
        except (ValueError, IndexError) as err:
            raise ReplayError(f"cases line {number}: {err}")
    if case is not None:
        raise ReplayError("the last case has no `go`")
    return cases


def _runs(changes: dict[int, int]) -> list[tuple[int, bytes]]:
    runs: list[tuple[int, bytearray]] = []
    for address in sorted(changes, key=lambda a: (a < RAM[0], a)):  # RAM first, then the scratchpad, as the harness writes them
        if runs and runs[-1][0] + len(runs[-1][1]) == address:
            runs[-1][1].append(changes[address])
        else:
            runs.append((address, bytearray([changes[address]])))
    return [(a, bytes(b)) for a, b in runs]


def format_result(result: dict) -> str:
    """One result as the text the harness writes (the ends `returned`, `stub` and `fault` only)."""
    out = ["begin", f"end {result['end']}"]
    if result["end"] == "returned":
        out.append(f"result {result['result']:x}")
    if result["end"] == "fault":
        f = result["fault"]
        out.append(f"fault {f['kind']} {f['address']:x} {f['pc']:x}")
    for call in result["calls"]:
        out.append(f"call {call['at']:x} {len(call['args'])}" + "".join(f" {a:x}" for a in call["args"]))
        out += [f"stack {i}" for i in call["stack_args"]]
        out += [f"pointee {i} {data.hex()}" for i, data in sorted(call["pointees"].items())]
        out += [f"watched {data.hex()}" for data in call["watched"]]
    out += [f"change {a:x} {len(b)} {b.hex()}" for a, b in _runs(result["changes"])]
    out.append("done")
    return "".join(x + "\n" for x in out)


def parse_results(text: str) -> tuple[list[dict], str]:
    """(the complete results in the text, the error text when the program wrote `error TEXT`, else an empty string).

    A result that has begun and has no `done` is not returned."""
    results: list[dict] = []
    current: dict | None = None
    call: dict | None = None
    for line in text.splitlines():
        parts = line.split()
        if not parts:
            continue
        key, rest = parts[0], parts[1:]
        if key == "error":
            return results, line[len("error"):].strip()
        if key == "begin":
            current, call = {"end": "", "calls": [], "changes": {}}, None
        elif current is None:
            raise ReplayError(f"the program wrote a line outside a result: {line!r}")
        elif key == "end":
            current["end"] = rest[0]
        elif key == "result":
            current["result"] = int(rest[0], 16)
        elif key == "fault":
            current["fault"] = {"kind": rest[0], "address": int(rest[1], 16), "pc": int(rest[2], 16)}
        elif key == "call":
            call = {"at": int(rest[0], 16), "args": [int(x, 16) for x in rest[2:2 + int(rest[1])]], "stack_args": [], "pointees": {}, "watched": []}
            current["calls"].append(call)
        elif key == "stack" and call is not None:
            call["stack_args"].append(int(rest[0]))
        elif key == "pointee" and call is not None:
            call["pointees"][int(rest[0])] = bytes.fromhex(rest[1])
        elif key == "watched" and call is not None:
            call["watched"].append(bytes.fromhex(rest[0] if rest else ""))
        elif key == "change":
            address, data = int(rest[0], 16), bytes.fromhex(rest[2] if len(rest) > 2 else "")
            if len(data) != int(rest[1]):
                raise ReplayError(f"a change of {rest[1]} bytes has {len(data)}")
            for k, b in enumerate(data):
                current["changes"][address + k] = b
        elif key == "done":
            results.append(current)
            current = None
        else:
            raise ReplayError(f"the program wrote a line that is not understood: {line!r}")
    return results, ""


# ---------------------------------------------------------------------------------------------------------------
# Building the program.


def _read(path: Path) -> str:
    try:
        return path.read_text()
    except OSError as err:
        raise ReplayError(f"cannot read {path}: {err.strerror}")


def _write_if_changed(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    if not path.is_file() or path.read_text() != text:
        path.write_text(text)


def _headers(source: Path, cc: str, gen: Path, timeout: int) -> list[Path] | None:
    """The files the compiler reads for `source` besides it, from its own dependency listing. None when the
    listing failed or did not end: what the source depends on is then not known, which is not the same as
    nothing (a header that was deleted makes the listing fail)."""
    proc = hb.hostcheck.compile_run([cc, *hb.COMPILE_FLAGS, "-M", "-I", str(gen), str(source)], timeout)
    if proc is None or proc.returncode != 0:
        return None
    text = proc.stdout.replace("\\\n", " ").replace("\\ ", "\0")
    return [Path(x.replace("\0", " ")) for x in text.split()[2:] if x not in ("\\", ":")]


def _up_to_date(exe: Path, inputs: list[Path]) -> bool:
    """Whether the program is newer than every input. An input of the same time is not older: it is built again."""
    if not exe.is_file():
        return False
    newest = exe.stat().st_mtime_ns
    try:
        return all(p.stat().st_mtime_ns < newest for p in inputs)
    except OSError:
        return False


def _check(proc, what: str, timeout: int) -> None:
    if proc is None:
        raise ReplayError(f"{what}: no end after {timeout} seconds")
    if proc.returncode != 0:
        raise ReplayError(f"{what}: {hb.first_error(proc.stderr)}")


_CONFIGURATIONS: dict[tuple, tuple] = {}
_CONFIGURATIONS_LOCK = threading.Lock()


def _configuration(config: Path) -> tuple[dict[str, int], Path | None, str, str]:
    """What a build takes from the configuration: (the names with their PS1 addresses, the fields file or None,
    the name of the generated header, its text). Reading it costs more than a build of one function, and it is
    the same for every function, so it is read once for each state of its files: the configuration, the symbol
    file and the fields file, each by its path, time and size."""
    def state(path: Path) -> tuple:
        try:
            st = path.stat()
        except OSError as err:
            raise ReplayError(f"cannot read {path}: {err.strerror}")
        return (str(path.resolve()), st.st_mtime_ns, st.st_size)

    key = (state(config), state(config.parent / "symbols.ld"))
    with _CONFIGURATIONS_LOCK:
        found = _CONFIGURATIONS.get(key)
        if found is not None and (found[1] is None or state(found[1]) == found[4]):
            return found[:4]
        cfg = hb.hostcheck.read_config(config)
        selection = hb.select_units(cfg, config)
        symbols = hb.read_symbols(_read(config.parent / "symbols.ld"))
        names = hb.merge_names(symbols + [(f.name, f.address, f"unit {f.unit}") for f in selection.declared])
        fields_path, header, header_text, fields_state = None, "", "", None
        if "types" in cfg:
            fields_path, header = hb.hostcheck.read_types(cfg, config)
            fields_state = state(fields_path)
            header_text = hb.structgen.generate_header(hb.hostcheck.read_model(fields_path), Path(header).name)
        _CONFIGURATIONS[key] = (names, fields_path, header, header_text, fields_state)
        return names, fields_path, header, header_text


def build_program(source, name: str, config, build, cc: str, timeout: int = 300) -> Path:
    """Compile `source`, which defines the function `name`, and link it with the harness into BUILD/NAME.exe.

    The unit is compiled and renamed as `hostbuild` does it, so its definition of NAME is `impl_NAME` and its
    references are plain names; every name the object refers to that the symbol file or the functions of the
    configuration give an address is `ps1_NAME` at that address, and the references are renamed to match. A name
    that the object defines and refers to and that has no address is the host copy, as in `hostbuild`; any other
    name left is the host's (the C library, the compiler's helpers), and one that nothing resolves is an error that
    names it. A program newer than its inputs (the source, every file the compiler lists as read for it, the
    configuration, the symbol file, the fields file, the harness, this driver and `hostbuild.py`) is not built
    again; when that listing fails or does not end the program is built, whatever is on disk. A program made from
    another source file, another configuration or another compiler than the ones given is built too, whatever the
    files' times (a note beside the program says what it was made from). The result is its path."""
    source, config, build = Path(source), Path(config), Path(build)
    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name):
        raise ReplayError(f"{name!r} is not a C name")
    try:
        names, fields_path, header, header_text = _configuration(config)
        gen, exe = build / "gen", build / f"{name}.exe"
        inputs = [source, config, config.parent / "symbols.ld", HARNESS, Path(__file__), Path(hb.__file__)]
        if fields_path is not None:
            _write_if_changed(gen / header, header_text)
            inputs.append(fields_path)
        if not source.is_file():
            raise ReplayError(f"{source} does not exist")
        # The program on disk is handed back only when everything it was built from is known and older. When
        # the listing of what the source reads fails or does not end, the build is made, and it says what is wrong.
        # A program is also what it was made FROM: another source file for the same name, another configuration or
        # another compiler make another program, whatever the times of the files say. The build notes the three
        # beside the program when it has succeeded, and a program without that note, or with another, is built.
        made_from = "".join(f"{text}\n" for text in (source.resolve(), config.resolve(), cc))
        note = gen / f"{name}.from"
        same_origin = exe.is_file() and note.is_file() and note.read_text() == made_from
        headers = _headers(source, cc, gen, timeout) if same_origin else None
        if headers is not None and _up_to_date(exe, inputs + headers):
            return exe
        note.unlink(missing_ok=True)
        for sub in ("asm", "obj", "rt", "probe"):
            (build / sub).mkdir(parents=True, exist_ok=True)
        underscore = hb.detect_underscore(cc, build / "probe", timeout)
        outcome = hb.build_unit(hb.Job(name, source, False, [], None), cc, build, underscore, timeout)
        if not outcome.ok:
            raise ReplayError(f"{source}: {outcome.reason}")
        if name not in outcome.defined:
            raise ReplayError(f"{source} does not define {name}")
        listing = hb.hostcheck.compile_run([hb.default_nm(cc), str(build / "obj" / f"{name}.o")], timeout)
        _check(listing, "nm", timeout)
        refs = set()
        for line in listing.stdout.splitlines():
            parts = line.split()
            if len(parts) == 2 and parts[0] == "U":
                refs.add(hb.c_name(parts[1], underscore))
        given = {r: names[r] for r in refs if r in names}
        aliases = sorted(r for r in refs if r not in names and r in outcome.defined)
        _write_if_changed(gen / f"{name}.names.ld", hb.render_names(given, aliases, underscore))
        _write_if_changed(gen / f"{name}.redefine.txt", hb.render_redefine(given, aliases, underscore))
        reason = hb.redefine(name, hb.default_objcopy(cc), build, timeout, f"{name}.redefine.txt")
        if reason:
            raise ReplayError(reason)
        target = gen / f"{name}.target.c"
        _write_if_changed(target, f"extern void impl_{name}(void);\nvoid *replay_target = (void *)impl_{name};\n")
        objects = {"target": build / "obj" / f"{name}.target.o", "harness": build / "rt" / "harness.o", "hole": build / "rt" / "hole.o"}
        _write_if_changed(gen / "hole.s", hb.hole_source())
        for key, command in (
            ("target", [cc, "-O1", "-Wall", "-Wextra", "-c", "-o", str(objects["target"]), str(target)]),
            ("harness", [cc, "-O1", "-Wall", "-Wextra", "-c", "-o", str(objects["harness"]), str(HARNESS)]),
            ("hole", [cc, "-c", "-o", str(objects["hole"]), str(gen / "hole.s")]),
        ):
            objects[key].unlink(missing_ok=True)
            _check(hb.hostcheck.compile_run(command, timeout), key, timeout)
        exe.unlink(missing_ok=True)
        link = hb.hostcheck.compile_run(
            [cc, str(objects["harness"]), str(objects["hole"]), str(objects["target"]), str(build / "obj" / f"{name}.o"),
             str(gen / f"{name}.names.ld"), *hb.LINK_FLAGS, "-o", str(exe)], timeout)
    except hb.Problem as err:
        raise ReplayError(" ".join(str(err).split()))
    if link is None:
        raise ReplayError(f"link: no end after {timeout} seconds")
    if link.returncode != 0 or not exe.is_file():
        missing = sorted(set(re.findall(r"undefined reference to `([^']*)'", link.stderr)))
        if missing:
            shown = ", ".join(hb.c_name(m, underscore) for m in missing)
            raise ReplayError(f"link: no PS1 address, and nothing in the link provides: {shown}")
        raise ReplayError(f"link: {hb.link_errors(link.stderr)}")
    note.write_text(made_from)
    return exe


# ---------------------------------------------------------------------------------------------------------------
# Running cases.


class _Process:
    """One run of the program on a list of cases: the complete results, and how it ended."""

    def __init__(self, program: Path, prefix, cases: list[dict], timeout: float):
        self.results: list[dict] = []
        self.timed_out = False
        self.status: int | None = None
        self.error = ""
        text = format_cases(cases).encode()
        proc = subprocess.Popen([*prefix, str(program), "--limit", str(int(timeout) + 10)], stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        lines: queue.Queue = queue.Queue()

        def feed():
            try:
                proc.stdin.write(text)
                proc.stdin.close()
            except OSError:
                pass

        def drain():
            for raw in iter(proc.stdout.readline, b""):
                lines.put(raw)
            lines.put(None)

        threading.Thread(target=feed, daemon=True).start()
        threading.Thread(target=drain, daemon=True).start()
        block: list[str] = []
        deadline = time.monotonic() + timeout
        while True:
            try:
                raw = lines.get(timeout=max(0.0, deadline - time.monotonic()))
            except queue.Empty:
                self.timed_out = True
                proc.kill()  # by its PID
                break
            if raw is None:
                break
            line = raw.decode("ascii", "replace")
            block.append(line)
            if line.startswith("error"):
                self.error = line[5:].strip()
                break
            if line.strip() == "done":
                done, _ = parse_results("".join(block))
                self.results += done
                block = []
                deadline = time.monotonic() + timeout
        try:
            self.status = proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            proc.kill()
            self.status = proc.wait(timeout=10)


def run_cases(program, cases: list[dict], timeout: float = 10, prefix=()) -> list[dict]:
    """One result per case. The cases run in one process; a case that ends the process is `died` (with the exit
    status), one that does not answer within `timeout` seconds is `timeout` (the process is stopped by its PID), and
    the cases after either are run in a new process."""
    program = Path(program)
    results: list[dict] = []
    while len(results) < len(cases):
        run = _Process(program, prefix, cases[len(results):], timeout)
        if run.error:
            raise ReplayError(f"the program refused the cases: {run.error}")
        results += run.results
        if len(results) == len(cases):
            break
        if run.timed_out:
            results.append({"end": "timeout", "calls": [], "changes": {}})
        else:
            results.append({"end": "died", "status": run.status, "calls": [], "changes": {}})
    return results


# ---------------------------------------------------------------------------------------------------------------
# The command line.


def _cases_from_json(items: list) -> list[dict]:
    cases = []
    for item in items:
        case = dict(item)
        case["memory"] = [(m[0], m[1], bytes.fromhex(m[2])) if m[0] == "bytes" else tuple(m) for m in item.get("memory", [])]
        cases.append(case)
    return cases


def _result_to_json(result: dict) -> dict:
    out = dict(result)
    out["calls"] = [{**c, "pointees": {str(k): v.hex() for k, v in c["pointees"].items()}, "watched": [w.hex() for w in c["watched"]]} for c in result["calls"]]
    out["changes"] = {f"{a:x}": b for a, b in sorted(result["changes"].items())}
    return out


def main() -> int:
    parser = argparse.ArgumentParser(description="Build and run one function of the game's C on cases.")
    parser.add_argument("--config", type=Path, required=True)
    parser.add_argument("--cc", default="i686-w64-mingw32-gcc")
    parser.add_argument("--build", type=Path, required=True)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--name", required=True)
    parser.add_argument("--cases", type=Path, required=True)
    parser.add_argument("--timeout", type=int, default=10)
    parser.add_argument("--run", default="", help="command prefix that starts a Windows program")
    args = parser.parse_args()
    try:
        cases = _cases_from_json(json.loads(args.cases.read_text()))
        exe = build_program(args.source, args.name, args.config, args.build, args.cc)
        results = run_cases(exe, cases, args.timeout, args.run.split())
    except (ReplayError, ValueError, OSError) as err:
        print(f"hostreplay.py: {err}", file=sys.stderr)
        return 2
    print(json.dumps([_result_to_json(r) for r in results], indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
