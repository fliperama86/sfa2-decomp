"""Synthetic parser controls, under a second, no original input."""
import struct
import unittest

from formats import InputError, Reader, checked, pac_chunks, signed16


class Controls(unittest.TestCase):
    def test_checked(self):
        self.assertEqual(checked(b'abcd', 1, 2), b'bc')
        for offset, size in [(-1, 1), (0, -1), (3, 2)]:
            with self.assertRaises(InputError):
                checked(b'abcd', offset, size)

    def test_signed16(self):
        self.assertEqual(signed16(65505), -31)
        self.assertEqual(signed16(65536), 0)
        self.assertEqual(signed16(-32769), 32767)

    def test_pac_controls(self):
        b = bytearray(4096)
        struct.pack_into('<I', b, 0, 1)
        struct.pack_into('<HHII', b, 32, 4, 0, 4, 42)
        struct.pack_into('<I', b, 2048, 42)
        self.assertEqual(pac_chunks(bytes(b))[0]['data'], struct.pack('<I', 42))
        for changed in [bytes(b[:-1]), bytes(b) + b'x']:
            with self.assertRaises(InputError):
                pac_chunks(changed)
        struct.pack_into('<I', b, 2048, 43)
        with self.assertRaises(InputError):
            pac_chunks(bytes(b))
        struct.pack_into('<I', b, 0, 64)
        with self.assertRaises(InputError):
            pac_chunks(bytes(b))

    def fixture(self, steps):
        b = bytearray(256)
        struct.pack_into('<I', b, 0, 0x1040)
        for i, step in enumerate(steps):
            struct.pack_into('<hhIhH', b, 64 + 12 * i, *step)
        return Reader(bytes(b), 0x1000)

    def test_sequence_loop(self):
        r = self.fixture([(3, 0, 10, 0, 1), (4, -32768, 20, -1, 1)])
        q = r.sequence(0x1000, 0)
        self.assertEqual((q['duration_ticks'], q['timed_steps'], q['loop_target']), (7, 2, 0))
        self.assertEqual(q['unique_artwork_offsets'], [10, 20])
        self.assertEqual(q['collision_frames'], [1])
        self.assertFalse(q['terminal_hold'])

    def test_sequence_terminal(self):
        q = self.fixture([(2, 0, 10, 0, 1), (4095, -32768, 0, 0, 2)]).sequence(0x1000, 0)
        self.assertEqual(q['duration_ticks'], 2)
        self.assertEqual(q['unique_artwork_offsets'], [10])
        self.assertTrue(q['terminal_hold'])

    def test_sequence_negative_controls(self):
        for steps in [[(0, -32768, 0, 0, 0)], [(3, -32768, 0, -1, 0)],
                      [(3, 0, 0, 0, 0)]]:
            with self.assertRaises(InputError):
                self.fixture(steps).sequence(0x1000, 0, limit=1)
        r = self.fixture([(3, -32768, 0, 0, 0)])
        with self.assertRaises(InputError):
            r.sequence(0x1000, -1)
        with self.assertRaises(InputError):
            r.sequence(0x1000, 256)
        with self.assertRaises(InputError):
            r.sequence(0x2000, 0)

    def pose_fixture(self, mode, body, count=2):
        b = bytearray(256)
        struct.pack_into('<5H', b, 0, count, mode, 3, 0, 1)
        struct.pack_into('<' + 'H' * len(body), b, 10, *body)
        struct.pack_into('<3H', b, 128, 65505, 65500, 1)
        return Reader(bytes(b), 0x1000)

    def test_pose_modes(self):
        for mode, body in [(0, [10, 11]), (2, [10, 3, 11, 3]),
                           (4, [10, 1]), (6, [10, 3, 1])]:
            with self.subTest(mode=mode):
                p = self.pose_fixture(mode, body).pose(0x1000, 0x1080, 0)
                self.assertEqual([c['tile'] for c in p['cells']], [10, 11])
                self.assertEqual([c['x'] for c in p['cells']], [-31, -15])
                self.assertEqual(p['tile_bounds']['width'], 32)
                self.assertEqual(p['tile_bounds']['height'], 16)

    def test_pose_flags(self):
        p = self.pose_fixture(6, [10, 0x63, 1]).pose(0x1000, 0x1080, 0)
        self.assertTrue(all(c['flip_x'] and c['flip_y'] for c in p['cells']))
        self.assertEqual([c['palette'] for c in p['cells']], [3, 3])

    def test_pose_negative_controls(self):
        for mode, body, count in [(5, [10, 11], 2), (4, [10, 2], 2),
                                  (0, [10], 1), (0, [], 1025)]:
            with self.assertRaises(InputError):
                self.pose_fixture(mode, body, count).pose(0x1000, 0x1080, 0)
        for art in [-2, 1, 1000]:
            with self.assertRaises(InputError):
                self.pose_fixture(0, [10, 11]).pose(0x1000, 0x1080, art)


if __name__ == '__main__':
    unittest.main()
