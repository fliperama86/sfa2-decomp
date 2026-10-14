"""Subsecond synthetic controls for port/intro/assets.py. No private input or game."""
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch
import zlib

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/intro'))
import assets


def pack(indices):
    return bytes((indices[i] << 4) | indices[i + 1] for i in range(0, len(indices), 2))


class IntroAssets(unittest.TestCase):
    def test_nibbles_and_cell_rows(self):
        indices = bytes((x + y * 3) % 16 for y in range(8) for x in range(8))
        self.assertEqual(assets.pattern(pack(indices), 0x1000, 8), indices)

    def test_four_cell_quadrants(self):
        packed = b''.join(bytes([v * 17]) * 32 for v in (1, 2, 3, 4))
        result = assets.pattern(packed, 0x1000, 16)
        expected = bytes(1 + x // 8 + 2 * (y // 8) for y in range(16) for x in range(16))
        self.assertEqual(result, expected)
        changed = bytearray(packed)
        changed[0] ^= 0x10
        self.assertNotEqual(assets.digest(result), assets.digest(assets.pattern(changed, 0x1000, 16)))

    def test_pattern_refusals(self):
        for character, size, count in [(0x1000, 4, 128), (0x0fff, 8, 128),
                                       (0x1001, 16, 128), (0x1000, 16, 127),
                                       (0x8000, 8, 128)]:
            with self.subTest(character=character, size=size, count=count):
                with self.assertRaises(assets.InputError):
                    assets.pattern(bytes(count), character, size)

    def test_page_flips_palette_and_transparency(self):
        indices = bytearray(64)
        indices[0], indices[7], indices[56], indices[63] = 1, 2, 3, 4
        data = pack(indices) + bytes(32)
        colors = [0] * 2048
        colors[16:21] = [0xffff, 0x801f, 0x83e0, 0xfc00, 0xffff]
        corners = [(255, 0, 0, 255), (0, 255, 0, 255),
                   (0, 0, 255, 255), (255, 255, 255, 255)]
        for fx, fy in [(False, False), (True, False), (False, True), (True, True)]:
            flags = 1 | (0x4000 if fx else 0) | (0x8000 if fy else 0)
            code = struct.pack('>HH', flags, 0x1000) + struct.pack('>HH', 0, 0x1001) * 1023
            rgba, cells = assets.page(code, data, colors, assets.BASE, 8)
            for i, (x, y) in enumerate([(0, 0), (7, 0), (0, 7), (7, 7)]):
                pos = (y * 256 + x) * 4
                self.assertEqual(tuple(rgba[pos:pos + 4]), corners[i ^ int(fx) ^ (2 * int(fy))])
            self.assertEqual(rgba[4:8], bytes(4))  # palette's white pen zero stays transparent
            self.assertEqual(rgba[32:36], bytes(4))
            self.assertEqual(cells[0]['flip_x'], fx)
            self.assertEqual(cells[0]['flip_y'], fy)
            self.assertEqual(cells[0]['file_offset'], 0)
            self.assertEqual(cells[0]['pixels_sha256'], assets.digest(indices))

    def test_map_bounds_and_unsupported_attributes(self):
        data = bytes(32)
        valid = struct.pack('>HH', 0, 0x1000) * 1024
        for code, address in [(valid[:-1], assets.BASE), (valid, assets.BASE - 4),
                              (struct.pack('>HH', 0x2000, 0x1000) + valid[4:], assets.BASE),
                              (struct.pack('>HH', 0, 0x8000) + valid[4:], assets.BASE)]:
            with self.assertRaises(assets.InputError):
                assets.page(code, data, [0] * 2048, address, 8)

    def test_palette_terminator_and_bank(self):
        with patch.object(assets, 'PALETTE_LOADS', ((assets.BASE, 32),)):
            colors = assets.palettes(struct.pack('>3H', 0x1234, 0xabcd, 0xffff))
            self.assertEqual(colors[31:35], [0, 0x1234, 0xabcd, 0])
            with self.assertRaises(assets.InputError):
                assets.palettes(bytes(2))
        with patch.object(assets, 'PALETTE_LOADS', ((assets.BASE, 2047),)):
            with self.assertRaises(assets.InputError):
                assets.palettes(bytes(4))

    def test_revision_and_mutation(self):
        data = b'made up input'
        with patch.object(assets, 'PINS', {'fixture': (len(data), assets.digest(data))}):
            assets.pinned(data, 'fixture')
            for changed in (data[:-1], data[:-1] + b'!'):
                with self.assertRaises(assets.InputError):
                    assets.pinned(changed, 'fixture')
        with self.assertRaises(assets.InputError):
            assets.export(b'not game data', b'not game data')

    def test_png_round_trip_and_crc(self):
        rgba = bytes((x % 256 for x in range(256 * 256 * 4)))
        png = assets.png(rgba)
        self.assertEqual(png[:8], b'\x89PNG\r\n\x1a\n')
        pos, kinds = 8, []
        while pos < len(png):
            size = struct.unpack_from('>I', png, pos)[0]
            kind, body = png[pos + 4:pos + 8], png[pos + 8:pos + 8 + size]
            crc = struct.unpack_from('>I', png, pos + 8 + size)[0]
            self.assertEqual(crc, zlib.crc32(kind + body))
            kinds.append(kind)
            if kind == b'IDAT':
                raw = zlib.decompress(body)
                self.assertEqual(raw[::1025], bytes(256))
                self.assertEqual(b''.join(raw[y * 1025 + 1:(y + 1) * 1025] for y in range(256)), rgba)
            pos += size + 12
        self.assertEqual(kinds, [b'IHDR', b'IDAT', b'IEND'])
        with self.assertRaises(assets.InputError):
            assets.png(bytes(3))

    def test_private_output_and_preservation(self):
        with tempfile.TemporaryDirectory() as tmp, patch.object(assets, 'ROOT', Path(tmp)):
            folder = Path(tmp) / 'port/local/intro-asset-probe'
            report = assets.write_export(folder, 'fixture', {'schema': 1}, {'test.png': b'fixture'})
            before = report.read_bytes()
            with self.assertRaises(assets.InputError):
                assets.write_export(folder, 'fixture', {}, {'test.png': b'changed'})
            self.assertEqual(report.read_bytes(), before)
            self.assertEqual((folder / 'fixture-test.png').read_bytes(), b'fixture')
            for path, prefix in [(Path(tmp) / 'public', 'fixture'), (folder, '../escape')]:
                with self.assertRaises(assets.InputError):
                    assets.write_export(path, prefix, {}, {})


if __name__ == '__main__':
    unittest.main()
