#!/usr/bin/env python3
"""Search first-scene sprite schedules in the user's PS1 disc, without running it.

Reads the pinned Saturn OPENING.Z as an analysis reference, and hashes/scans
manifest-listed PS1 files except STR/XA streams. Seconds to a minute for the
examined disc. Synthetic controls need neither game nor compiler. Timing-only
candidates are not evidence of equal artwork or a retained intro controller.
"""
from __future__ import annotations

import argparse
from dataclasses import asdict, dataclass
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import struct
import subprocess
import sys

from assets import BASE, InputError, ROOT, checked, pinned

sys.path.insert(0, str(ROOT / 'ps1/src/programs'))
import discfile

FORMATS = {'saturn': '>HHHhI', 'ps1': '<HHIhH'}


@dataclass(frozen=True)
class Step:
    duration: int
    flags: int
    artwork: int
    branch: int
    auxiliary: int


def sequence(data: bytes, offset: int, layout: str, limit: int = 256) -> tuple[Step, ...]:
    if layout not in FORMATS or offset < 0 or offset % 2 or not 1 <= limit <= 256:
        raise InputError('invalid sequence layout, alignment or limit')
    steps = []
    for index in range(limit):
        step = Step(*struct.unpack(FORMATS[layout], checked(data, offset + index * 12, 12)))
        if not 1 <= step.duration <= 0x7fff:
            raise InputError('duration outside this positive-countdown profile')
        steps.append(step)
        if step.flags & 0x8000:
            if not 0 <= index + step.branch <= index:
                raise InputError('branch leaves this bounded sequence')
            return tuple(steps)
    raise InputError('no terminal branch within sequence limit')


def schedule(steps: tuple[Step, ...]) -> tuple:
    return tuple((s.duration, s.flags, s.branch if s.flags & 0x8000 else None) for s in steps)


def artwork_aliases(steps: tuple[Step, ...]) -> tuple[int, ...]:
    indices = {}
    return tuple(indices.setdefault(s.artwork, len(indices)) for s in steps)


def candidates(data: bytes, reference: tuple[Step, ...], limit: int = 10000) -> list[dict]:
    """Find PS1 control-field matches; ignore artwork addresses and auxiliary data."""
    if not 2 <= len(reference) <= 256 or limit < 1:
        raise InputError('need a bounded multi-record reference and positive hit limit')
    # Validate the supplied objects too, so malformed hand-built references fail.
    packed = b''.join(struct.pack(FORMATS['saturn'], *asdict(s).values()) for s in reference)
    if sequence(packed, 0, 'saturn') != reference:
        raise InputError('reference contains records after its terminal branch')
    pattern = b''
    for step in reference:
        pattern += re.escape(struct.pack('<HH', step.duration, step.flags)) + b'.{4}'
        pattern += re.escape(struct.pack('<h', step.branch)) if step.flags & 0x8000 else b'.{2}'
        pattern += b'.{2}'
    result = []
    for match in re.finditer(b'(?=' + pattern + b')', data, re.DOTALL):
        offset = match.start()
        if offset % 2:
            continue
        if len(result) == limit:
            raise InputError('candidate limit exceeded; refusing a truncated report')
        found = sequence(data, offset, 'ps1', len(reference))
        result.append(dict(offset=offset, steps=[asdict(s) for s in found],
                           artwork_aliases_equal=artwork_aliases(found) == artwork_aliases(reference)))
    return result


def references(code: bytes) -> list[dict]:
    pinned(code, 'OPENING.Z')
    result = []
    # Five variants read by the first scene's actor, not all opening animation.
    for variant in range(5):
        address = struct.unpack('>I', checked(code, 0x060a09c0 - BASE + variant * 4, 4))[0]
        steps = sequence(code, address - BASE, 'saturn')
        result.append(dict(variant=variant, address=address, steps=steps))
    return result


def palette_candidates(data: bytes, colors: tuple[int, ...], limit: int = 10000) -> list[int]:
    """Find a 16-color RGB555 palette, ignoring transparent pen 0 and bit 15.

    This compares color components only, not transparency/control-bit behavior.
    Input words are decoded integers; searched words use PS1 little-endian order.
    """
    if len(colors) != 16 or any(not 0 <= c <= 0xffff for c in colors) or limit < 1:
        raise InputError('expected sixteen color words and positive hit limit')
    pattern = b'.{2}'
    for color in colors[1:]:
        low, high = color & 255, (color >> 8) & 127
        pattern += re.escape(bytes([low])) + b'[' + re.escape(bytes([high, high | 128])) + b']'
    hits = []
    for match in re.finditer(b'(?=' + pattern + b')', data, re.DOTALL):
        if match.start() % 2 == 0:
            if len(hits) == limit:
                raise InputError('palette candidate limit exceeded')
            hits.append(match.start())
    return hits


def verify_file(data: bytes, entry: dict) -> None:
    if len(data) != entry['size'] or hashlib.sha256(data).hexdigest() != entry['sha256']:
        raise InputError(f"baseline mismatch: {entry['path']}")


def scan(entries: list[dict], read, refs: list[dict]) -> dict:
    searched, excluded, matches = [], [], []
    names = set()
    for entry in entries:
        path = entry['path']
        if (not isinstance(path, str) or not path or path in names or '\\' in path
                or PurePosixPath(path).is_absolute() or '..' in PurePosixPath(path).parts):
            raise InputError('invalid or duplicate manifest path')
        names.add(path)
        if PurePosixPath(path).suffix.upper() in ('.STR', '.XA'):
            excluded.append(path)
            continue
        data = read(path)
        verify_file(data, entry)
        searched.append(dict(path=path, size=len(data), sha256=entry['sha256']))
        cache = {}
        for ref in refs:
            key = (schedule(ref['steps']), artwork_aliases(ref['steps']))
            if key not in cache:
                cache[key] = candidates(data, ref['steps'])
            matches.extend(dict(file=path, variant=ref['variant'], **hit) for hit in cache[key])
    if not searched or not refs:
        raise InputError('empty search scope or reference set')
    return dict(schema=1, evidence='timing-only candidates, not scene identity',
                limits=['STR/XA streams excluded', 'no decompression or sector-slack search',
                        'same record count/timing/flags/branch required; retimed tables may not match',
                        'background maps and palette cycling are separate from sprite schedules'],
                searched=searched, excluded=excluded,
                references=[dict(variant=r['variant'], address=r['address'],
                                 steps=[asdict(s) for s in r['steps']]) for r in refs],
                candidates=matches)


def output_path(path: Path) -> Path:
    path = path.resolve()
    if not path.is_relative_to((ROOT / 'port/local').resolve()) or path.exists():
        raise InputError('output must be new and inside this checkout\'s port/local/')
    return path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--opening-z', type=Path, required=True)
    parser.add_argument('--disc', type=Path, required=True)
    parser.add_argument('--manifest', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    try:
        out = output_path(args.output)
        code = args.opening_z.read_bytes()
        refs = references(code)
        manifest = args.manifest.read_bytes()
        image = discfile.Image(discfile.image_path(args.disc))
        try:
            report = scan(json.loads(manifest)['files'], lambda p: discfile.read_file(image, p), refs)
        finally:
            image.close()
        report['reference_sha256'] = hashlib.sha256(code).hexdigest()
        report['manifest_sha256'] = hashlib.sha256(manifest).hexdigest()
        report['source_commit'] = subprocess.check_output(
            ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
        report['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
        out.parent.mkdir(parents=True, exist_ok=True)
        with out.open('x') as stream:
            json.dump(report, stream, indent=2)
            stream.write('\n')
        print(f"{len(report['searched'])} files searched; {len(report['candidates'])} timing-only candidates")
        return 0
    except (InputError, discfile.DiscError, OSError, ValueError, KeyError, TypeError) as exc:
        print(f'animation probe: {exc}', file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
