import struct
import unittest

from formats import InputError, png, texture


def fixture(header, words):
    return struct.pack('<I', 4) + struct.pack('<H', header) + struct.pack('<' + 'H' * len(words), *words)


class Controls(unittest.TestCase):
    def test_literal(self):
        self.assertEqual(texture(fixture(0, [0x4321] * 64), 0), bytes([1, 2, 3, 4]) * 64)

    def test_sparse(self):
        b = fixture(0x8000, [1, 0, 0, 0, 0x4321])
        self.assertEqual(texture(b, 0), bytes([1, 2, 3, 4]) + bytes(252))

    def test_items(self):
        b = fixture(64, [0, 0, 0, 0] + [0x4321] * 64)
        self.assertEqual(texture(b, 0), bytes([1, 2, 3, 4]) * 64)

    def test_reference_is_to_original_input(self):
        # The block is at offset 128, with one original word before it.
        # First of 60 items copies that word plus four following original words.
        b = bytearray(300)
        struct.pack_into('<I', b, 0, 128)
        struct.pack_into('<H', b, 126, 0x4321)
        struct.pack_into('<5H', b, 128, 60, 1, 0, 0, 0)
        struct.pack_into('<60H', b, 138, 7, *([0x5555] * 59))
        result = texture(bytes(b), 0)
        self.assertEqual(result[:4], bytes([1, 2, 3, 4]))
        self.assertEqual(result[20:], bytes([5] * 236))

    def test_negative_controls(self):
        for b in [fixture(0, [0] * 63), fixture(0x1000, []), fixture(65, []),
                  fixture(1, [0, 0]), fixture(1, [1, 0xffff]), fixture(64, [0xffff] * 4 + [0] * 64)]:
            with self.assertRaises(InputError):
                texture(b, 0)
        for index in [-1, 0x4000]:
            with self.assertRaises(InputError):
                texture(fixture(0, [0] * 64), index)

    def test_png(self):
        result = png(1, 1, b'\x01')
        self.assertEqual(result[:8], b'\x89PNG\r\n\x1a\n')
        self.assertEqual(struct.unpack_from('>II', result, 16), (1, 1))
        with self.assertRaises(InputError):
            png(2, 1, b'\x01')


if __name__ == '__main__':
    unittest.main()
