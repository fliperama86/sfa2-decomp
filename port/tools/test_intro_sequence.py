#!/usr/bin/env python3
"""Isolated native-C intro reader controls, <1 s, no game/private data/emulator."""
import ctypes
import importlib.util
import struct
import unittest

from intro_sequence_support import INTRO, State, fixture_rom, fixtures, invoke, native


class IntroSequence(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = native()
        cls.dll = cls.build.__enter__()

    @classmethod
    def tearDownClass(cls):
        cls.build.__exit__(None, None, None)

    def test_recorded_synthetic_fixtures(self):
        for case in fixtures():
            with self.subTest(case=case['name']):
                result, after = invoke(self.dll, case['op'], fixture_rom(case),
                                       case['before'], case.get('table', 0), case.get('index', 0))
                self.assertEqual((result, after), (0, case['after']))

    def test_select_refusals_are_atomic(self):
        rom = bytearray(64)
        before = [18, 24, 32]
        for table, index in [(1,0),(64,0),(0,32),(0xffffffff,0)]:
            with self.subTest(table=table,index=index):
                self.assertEqual(invoke(self.dll, 'select', rom, before, table, index), (-1,before))
        for target in (1, 50, 64, 0xfffe):
            struct.pack_into('>H', rom, 0, target)
            self.assertEqual(invoke(self.dll, 'select', rom, before), (-1,before))
        self.assertEqual(invoke(self.dll, 'select', None, before), (-1,before))

    def test_tick_refusals_are_atomic(self):
        rom = bytearray(64)
        for record, flags in [(1,0),(64,0),(0xfffffff0,0),(50,0),(48,0),(48,0x8000)]:
            before = [record,1,flags]
            self.assertEqual(invoke(self.dll, 'tick', rom, before), (-1,before))
        for target in (1,50,64,0xffffffff):
            struct.pack_into('>I',rom,32,target)
            before = [16,1,0x8000]
            self.assertEqual(invoke(self.dll, 'tick', rom, before), (-1,before))
        self.assertEqual(invoke(self.dll, 'tick', None, [0,1,0]), (-1,[0,1,0]))

    def test_null_state_and_oversize_span(self):
        self.assertEqual(self.dll.intro_sequence_tick(None,0,None), -1)
        self.assertEqual(self.dll.intro_sequence_select(None,0,0,0,None), -1)
        tiny = ctypes.create_string_buffer(bytes(16),16)
        state = State(0,1,0)
        self.assertEqual(self.dll.intro_sequence_select(tiny,0x1000001,0,0,ctypes.byref(state)), -1)
        self.assertEqual(self.dll.intro_sequence_tick(tiny,0x1000001,ctypes.byref(state)), -1)
        self.assertEqual([state.record,state.remaining,state.flags], [0,1,0])

    def test_zero_duration_is_not_a_terminator(self):
        rom = bytearray(64)
        result, state = invoke(self.dll, 'select',rom,[0,0,0])
        self.assertEqual((result,state),(0,[0,0,0]))
        self.assertEqual(invoke(self.dll, 'tick',rom,state),(0,[0,65535,0]))

    def test_verifier_refuses_unpinned_inputs_and_public_outputs(self):
        spec = importlib.util.spec_from_file_location('verify_sequence', INTRO / 'verify_sequence.py')
        verifier = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(verifier)
        with self.assertRaises(ValueError):
            verifier.checked_opcodes(bytes(16))
        with self.assertRaises(ValueError):
            verifier.checked_opcodes(bytes(0x300000))
        with self.assertRaises(ValueError):
            verifier.output_path(INTRO / 'public.json')
        with self.assertRaises(ValueError):
            verifier.output_path(INTRO.parent / 'local' / '..' / 'intro' / 'escape.json')

    def test_mutation_is_detected(self):
        case = next(c for c in fixtures() if c['name']=='advance-loads-full-flags')
        with native(control=True) as mutant:
            self.assertNotEqual(invoke(mutant,'tick',fixture_rom(case),case['before']), (0,case['after']))


if __name__ == '__main__':
    unittest.main()
