"""Subsecond synthetic controls for intro animation search; no private inputs."""
from pathlib import Path
import hashlib
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/intro'))
import animation as a


def packed(steps, layout):
    return b''.join(struct.pack(a.FORMATS[layout], s.duration, s.flags, s.artwork,
                               s.branch, s.auxiliary) for s in steps)


REF = (a.Step(7, 0, 8, 0, 0), a.Step(11, 2, 20, 0, 0), a.Step(9, 0x8000, 8, -2, 0))
PS1 = (a.Step(7, 0, 0x123456, 99, 8), a.Step(11, 2, 0x123498, -99, 9),
       a.Step(9, 0x8000, 0x123456, -2, 10))


class IntroAnimation(unittest.TestCase):
    def test_endianness_and_field_relocation(self):
        self.assertEqual(a.sequence(packed(REF, 'saturn'), 0, 'saturn'), REF)
        self.assertEqual(a.sequence(packed(PS1, 'ps1'), 0, 'ps1'), PS1)
        self.assertEqual(a.schedule(REF), a.schedule(PS1))
        self.assertNotEqual(REF[0].artwork, PS1[0].artwork)
        self.assertEqual(a.artwork_aliases(REF), (0, 1, 0))

    def test_ignores_unused_branch_artwork_addresses_and_auxiliary(self):
        hits = a.candidates(b'xx' + packed(PS1, 'ps1') + b'xx', REF)
        self.assertEqual([h['offset'] for h in hits], [2])
        self.assertTrue(hits[0]['artwork_aliases_equal'])

    def test_artwork_aliases_are_reported_not_assumed(self):
        steps = PS1[:-1] + (a.Step(9, 0x8000, 0xabcdef, -2, 10),)
        hit, = a.candidates(packed(steps, 'ps1'), REF)
        self.assertFalse(hit['artwork_aliases_equal'])

    def test_alignment_and_adjacent_candidates(self):
        data = packed(PS1, 'ps1')
        self.assertEqual(a.candidates(b'x' + data, REF), [])
        self.assertEqual([h['offset'] for h in a.candidates(data * 2, REF)], [0, len(data)])
        with self.assertRaises(a.InputError):
            a.candidates(data * 2, REF, limit=1)

    def test_mutations_of_every_control_field_rejected(self):
        original = packed(PS1, 'ps1')
        # Each byte of duration/flags and the active branch is significant.
        for at in [i * 12 + j for i in range(3) for j in range(4)] + [32, 33]:
            with self.subTest(at=at):
                changed = bytearray(original)
                changed[at] ^= 1
                self.assertEqual(a.candidates(bytes(changed), REF), [])

    def test_truncated_target_has_no_match(self):
        self.assertEqual(a.candidates(packed(PS1, 'ps1')[:-1], REF), [])

    def test_invalid_reference_bounds_and_termination(self):
        raw = packed(REF, 'saturn')
        for data, offset, layout, limit in [(raw[:-1], 0, 'saturn', 256),
                (raw, -2, 'saturn', 256), (raw, 1, 'saturn', 256),
                (raw, 0, 'other', 256), (raw, 0, 'saturn', 0),
                (raw, 0, 'saturn', 2), (raw, 0, 'saturn', 257)]:
            with self.subTest(offset=offset, layout=layout, limit=limit):
                with self.assertRaises(a.InputError):
                    a.sequence(data, offset, layout, limit)
        for duration, branch in [(0, 0), (0x8000, 0), (1, -1), (1, 1)]:
            with self.assertRaises(a.InputError):
                a.sequence(packed((a.Step(duration, 0x8000, 0, branch, 0),), 'saturn'), 0, 'saturn')

    def test_search_rejects_degenerate_reference(self):
        terminal = (a.Step(1, 0x8000, 0, 0, 0),)
        for ref in [(), terminal, terminal + REF]:
            with self.assertRaises(a.InputError):
                a.candidates(bytes(100), ref)

    def test_manifest_hashes_scope_and_offsets(self):
        data = packed(PS1, 'ps1')
        entry = dict(path='PAC/MADEUP.PAC', size=len(data), sha256=hashlib.sha256(data).hexdigest())
        called = []
        def read(name):
            called.append(name)
            return data
        refs = [dict(variant=0, address=123, steps=REF)]
        report = a.scan([entry, dict(path='MOVIE.STR'), dict(path='SOUND.XA')], read, refs)
        self.assertEqual(called, [entry['path']])
        self.assertEqual(report['excluded'], ['MOVIE.STR', 'SOUND.XA'])
        self.assertEqual(report['candidates'][0]['file'], entry['path'])
        self.assertEqual(report['candidates'][0]['offset'], 0)
        with self.assertRaises(a.InputError):
            a.scan([entry], lambda _: data[:-1], refs)
        with self.assertRaises(a.InputError):
            a.scan([entry], lambda _: bytes(len(data)), refs)
        for path in ['../escape', '/absolute', 'PAC\\bad', '']:
            with self.assertRaises(a.InputError):
                a.scan([dict(entry, path=path)], read, refs)
        with self.assertRaises(a.InputError):
            a.scan([entry, entry], read, refs)
        with self.assertRaises(a.InputError):
            a.scan([], read, refs)
        with self.assertRaises(FileNotFoundError):
            a.scan([entry], lambda _: (_ for _ in ()).throw(FileNotFoundError()), refs)

    def test_unpinned_reference_refused(self):
        with self.assertRaises(a.InputError):
            a.references(bytes(32))

    def test_palette_rgb_and_control_bits(self):
        colors = tuple((i * 0x431 + 0x5b5b) & 0x7fff for i in range(16))
        changed = (0x1234,) + tuple(c | (0x8000 if i % 2 else 0)
                                   for i, c in enumerate(colors[1:]))
        data = struct.pack('<16H', *changed)
        self.assertEqual(a.palette_candidates(b'xx' + data, colors), [2])
        self.assertEqual(a.palette_candidates(b'x' + data, colors), [])
        for at in range(2, 32):
            bad = bytearray(data)
            bad[at] ^= 1  # change an RGB component, not bit 15
            self.assertEqual(a.palette_candidates(bytes(bad), colors), [])
        self.assertEqual(a.palette_candidates(data[:-1], colors), [])

    def test_palette_refusals_and_limit(self):
        colors = tuple(range(16))
        data = struct.pack('<16H', *colors)
        for bad in [colors[:-1], colors + (0,), (0x10000,) + colors[1:]]:
            with self.assertRaises(a.InputError):
                a.palette_candidates(data, bad)
        with self.assertRaises(a.InputError):
            a.palette_candidates(data * 2, colors, limit=1)

    def test_output_restricted_and_never_overwritten(self):
        with tempfile.TemporaryDirectory() as tmp, patch.object(a, 'ROOT', Path(tmp)):
            root = Path(tmp)
            local = root / 'port/local'
            local.mkdir(parents=True)
            target = local / 'evidence.json'
            self.assertEqual(a.output_path(target), target)
            for invalid in [root / 'outside.json', local / '../../escape.json']:
                with self.assertRaises(a.InputError):
                    a.output_path(invalid)
            target.write_text('keep')
            with self.assertRaises(a.InputError):
                a.output_path(target)
            self.assertEqual(target.read_text(), 'keep')
            link = local / 'escape'
            link.symlink_to(root, target_is_directory=True)
            with self.assertRaises(a.InputError):
                a.output_path(link / 'out.json')


if __name__ == '__main__':
    unittest.main()
