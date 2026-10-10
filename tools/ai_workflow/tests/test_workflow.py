"""Controls use synthetic source/inputs; never require a disc or compiler install."""
from __future__ import annotations

import dataclasses
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import model
import review
import workflow

REPO = Path(__file__).resolve().parents[3]
CFG = '''[baseline]
executable = "../audit/files/TEST"
[toolchain]
cpp = "cc"
maspsx = "wrapper.py"
binutils_prefix = "target-"
include_dirs = ["sdk/include"]
[toolchain.cc1]
kind = "local"
path = "../toolchain/cc1"
[[unit]]
name = "one"
source = "one.c"
functions = [{name="one",address=2148532224,size=16}]
'''


class Fixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name) / 'repo'
        self.root.mkdir()
        self.put('ps1/src/build.toml', CFG)
        self.put('ps1/src/one.c', 'int one(void) { return 1; }')
        self.put('ps1/src/slot06_nonmatching/func_80110000_slot06_00.c', 'int f(void) {return 2;}')
        self.put('ps1/src/slot06_nonmatching/func_80110000_slot06_00.py', 'CONTRACT = 1')
        self.put('.gitignore', 'local/\n')
        self.git('init', '-q')
        self.git('add', '.')
        self.git('-c', 'user.name=Test', '-c', 'user.email=test@example.invalid', 'commit', '-qm', 'fixture')
        self.check = review.Check('diff', ('python', '--cases', '2', '--seed', '1'), 'ps1', ('f',), 'differential')
        self.executions = 0
        self.result_output = 'f: built 12 bytes; cases 2, discarded 0, equal 2, different 0\n'
        self.rc = 0

    def put(self, name, text):
        p = self.root / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)
        return p

    def git(self, *args):
        return subprocess.check_output(['git', *args], cwd=self.root, text=True).strip()

    def identity(self, root, check):
        return {'files': review.source_files(root, 'ps1'), 'check': dataclasses.asdict(check),
                'private': {n: review.digest(root / n) for n in ('baseline', 'compiler') if (root / n).exists()}}

    def execute(self, argv, root, stream):
        self.executions += 1
        stream.write(self.result_output)
        return self.rc

    def run_job(self, name, **kw):
        return review.run_check(self.root, kw.pop('check', self.check), self.root / 'local' / name,
                                self.root / 'local/cache', 'head', identity=self.identity, execute=self.execute, **kw)


class Selection(Fixture):
    def selected(self, changed):
        return review.select_checks(self.root, changed, 2, (1, 7), 'cc32')

    def test_documentation_has_no_game_prerequisite(self):
        self.assertEqual(self.selected(['docs/example.md', 'port/README.md']), [])

    def test_owned_c_selects_matching_and_native(self):
        self.assertEqual([c.name for c in self.selected(['ps1/src/one.c'])], ['matching', 'native-build'])

    def test_candidate_selects_both_seeds_writes_controls_and_native(self):
        checks = self.selected(['ps1/src/slot06_nonmatching/func_80110000_slot06_00.c'])
        self.assertEqual([c.mode for c in checks], ['differential', 'writes', 'differential', 'writes', 'control', 'exit'])
        self.assertEqual(len({c.argv[c.argv.index('--seed')+1] for c in checks if '--seed' in c.argv}), 2)

    FB = 'ps1/src/slot06_nonmatching/func_80110004_slot06_00'

    def with_fixtures(self):
        """A second function of the folder, which has a fixtures file; the first has none."""
        for ext in ('.c', '.py', '.fixtures.json'):
            self.put(self.FB + ext, '')
        return 'func_80110004_slot06_00', 'func_80110000_slot06_00'

    def shape(self, changed):
        return [(c.name, c.mode, c.functions) for c in self.selected(changed) if c.mode != 'exit']

    def test_binding_header_config_tools_broaden(self):
        # What can change any function's build: the matching build, a replay of every function that has fixtures,
        # and one comparison of every function that has none. No write audit and no control: they were the
        # author's step for each function.
        with_file, without = self.with_fixtures()
        for p in ('ps1/src/symbols.ld', 'ps1/src/build.toml', 'ps1/src/types.fields', 'ps1/src/shared.h', 'ps1/tools/new.py'):
            with self.subTest(path=p):
                self.assertIn('matching', [c.name for c in self.selected([p])])
                self.assertEqual(self.shape([p]), [('slot06_nonmatching-replay', 'replay', (with_file,)),
                                                   ('slot06_nonmatching-diff-once-1', 'differential', (without,))])

    def test_the_test_tool_of_nonmatching_c_selects_its_tests_and_every_function_but_no_matching_build(self):
        with_file, without = self.with_fixtures()
        self.put('ps1/tools/test_example.py', '')
        for name in ('difftest.py', 'contracts.py', 'test_difftest.py', 'test_difftest_build.py'):
            path = 'ps1/src/slot06_nonmatching/' + name
            self.put(path, '')
            with self.subTest(path=path):
                names = [c.name for c in self.selected([path])]
                self.assertEqual(names, ['slot06_nonmatching-replay', 'slot06_nonmatching-diff-once-1', 'difftest-tools',
                                         'difftest-build-tools'])
        # Another file of a folder that is neither a function's nor the tool's still broadens.
        self.put('ps1/src/slot06_nonmatching/helper.py', '')
        self.assertIn('matching', [c.name for c in self.selected(['ps1/src/slot06_nonmatching/helper.py'])])

    def test_a_function_with_fixtures_is_replayed_and_not_compared_again(self):
        with_file, _ = self.with_fixtures()
        replay = [('slot06_nonmatching-replay', 'replay', (with_file,))]
        for changed in ([self.FB + '.c'], [self.FB + '.fixtures.json'], [self.FB + '.c', self.FB + '.fixtures.json'],
                        [self.FB + '.py', self.FB + '.fixtures.json'], [self.FB + '.c', self.FB + '.py', self.FB + '.fixtures.json']):
            with self.subTest(changed=changed):
                self.assertEqual(self.shape(changed), replay)
        check = [c for c in self.selected([self.FB + '.c']) if c.mode == 'replay'][0]
        self.assertEqual(check.argv[1:], ('ps1/src/slot06_nonmatching/difftest.py', '--config', 'ps1/src/build.toml', '--folder',
                                          'ps1/src/slot06_nonmatching', '--replay', with_file))
        # A fixtures file alone changes no C: nothing is built for it, and it is no reason for the matching build.
        self.assertEqual([c.name for c in self.selected([self.FB + '.fixtures.json'])], ['slot06_nonmatching-replay'])
        # A function of the same name in another folder is another function.
        for ext in ('.c', '.py'):
            self.put('ps1/src/other_nonmatching/' + with_file + ext, '')
        self.assertEqual(self.shape([self.FB + '.c']), replay)

    def test_a_changed_contract_without_new_fixtures_gets_the_wide_runs_too(self):
        with_file, _ = self.with_fixtures()
        for changed in ([self.FB + '.py'], [self.FB + '.py', self.FB + '.c']):
            with self.subTest(changed=changed):
                self.assertEqual([(n, m) for n, m, f in self.shape(changed)], [
                    ('slot06_nonmatching-replay', 'replay'), ('slot06_nonmatching-diff-1', 'differential'),
                    ('slot06_nonmatching-writes-1', 'writes'), ('slot06_nonmatching-diff-7', 'differential'),
                    ('slot06_nonmatching-writes-7', 'writes'), ('slot06_nonmatching-controls', 'control')])
                self.assertEqual({f for n, m, f in self.shape(changed)}, {(with_file,)})

    PG = 'ps1/src/programs/'
    PA, PB = 'func_80110004_logo', 'func_80110000_logo'

    def with_programs(self):
        """The folder of the stand-alone programs: the declaration, the two tools and their tests, and the folder `logo`
        with a function that has a fixtures file (PA) and one that has none (PB)."""
        for name in ('programs.toml', 'programs.py', 'discfile.py', 'test_programs.py', 'test_discfile.py'):
            self.put(self.PG + name, '')
        for ext in ('.c', '.py', '.fixtures.json'):
            self.put(self.PG + 'logo/' + self.PA + ext, '')
        for ext in ('.c', '.py'):
            self.put(self.PG + 'logo/' + self.PB + ext, '')

    def test_a_function_of_a_program_is_selected_like_a_nonmatching_one(self):
        self.with_programs()
        # With fixtures: the replay alone. Not the native build (the PC build does not see the folder) and not the matching build.
        for ext in ('.c', '.fixtures.json'):
            checks = self.selected([self.PG + 'logo/' + self.PA + ext])
            self.assertEqual([(c.name, c.mode, c.functions) for c in checks], [('programs-logo-replay', 'replay', (self.PA,))])
        self.assertEqual(checks[0].argv[1:], ('ps1/src/slot06_nonmatching/difftest.py', '--config', 'ps1/src/build.toml', '--folder',
                                              'ps1/src/programs/logo', '--replay', self.PA))
        # Without: both seeds, the audit and the control.
        checks = self.selected([self.PG + 'logo/' + self.PB + '.c'])
        self.assertEqual([c.name for c in checks], ['programs-logo-diff-1', 'programs-logo-writes-1', 'programs-logo-diff-7',
                                                    'programs-logo-writes-7', 'programs-logo-controls'])
        self.assertEqual({c.functions for c in checks}, {(self.PB,)})
        self.assertEqual(checks[0].argv[checks[0].argv.index('--folder') + 1], 'ps1/src/programs/logo')
        # A second program has a label of its own.
        for ext in ('.c', '.py'):
            self.put(self.PG + 'zed/func_80120000_zed' + ext, '')
        self.assertEqual([c.name for c in self.selected([self.PG + 'zed/func_80120000_zed.c'])],
                         ['programs-zed-diff-1', 'programs-zed-writes-1', 'programs-zed-diff-7', 'programs-zed-writes-7',
                          'programs-zed-controls'])

    def test_the_declaration_and_the_tools_of_the_programs_select_every_function_and_their_tests(self):
        self.with_programs()
        for name in ('programs.toml', 'programs.py', 'discfile.py'):
            with self.subTest(name=name):
                self.assertEqual([c.name for c in self.selected([self.PG + name])],
                                 ['programs-logo-replay', 'programs-logo-diff-once-1', 'test_programs', 'test_discfile'])
        # The tests alone select the tests alone.
        for name in ('test_programs.py', 'test_discfile.py'):
            with self.subTest(name=name):
                self.assertEqual([c.name for c in self.selected([self.PG + name])], ['test_programs', 'test_discfile'])
        checks = self.selected([self.PG + 'programs.py'])
        self.assertEqual(checks[2].argv[1:], ('ps1/src/programs/test_programs.py',))
        self.assertEqual(checks[3].argv[1:], ('ps1/src/programs/test_discfile.py',))

    def test_the_test_tool_changes_reach_the_functions_of_the_programs(self):
        self.with_programs()
        self.put('ps1/src/slot06_nonmatching/difftest.py', '')
        self.assertEqual([c.name for c in self.selected(['ps1/src/slot06_nonmatching/difftest.py'])],
                         ['slot06_nonmatching-diff-once-1', 'programs-logo-replay', 'programs-logo-diff-once-1', 'difftest-tools',
                          'difftest-build-tools'])

    def test_a_file_of_the_programs_that_carries_no_check_is_unmapped(self):
        self.with_programs()
        self.put(self.PG + 'README.md', '')
        self.put(self.PG + 'logo/README.md', '')
        self.assertEqual(self.selected([self.PG + 'README.md', self.PG + 'logo/README.md']), [])
        for changed in ([self.PG + 'stray.txt'], [self.PG + 'logo/notes.txt'], [self.PG + 'logo/sub/' + self.PB + '.c'],
                        [self.PG + self.PB + '.c'], [self.PG + 'logo/' + self.PB + '.json'], [self.PG + 'logo/programs.py']):
            self.put(changed[0], '')  # the file exists: it is refused as unmapped, not as a removed file
            with self.subTest(changed=changed), self.assertRaises(model.Problem) as raised:
                self.selected(changed)
            self.assertIn('unmapped changes', str(raised.exception))

    def test_replay_evidence_is_one_complete_line_for_each_function(self):
        check = review.Check('r', ('python', 'difftest.py', '--replay', 'fa', 'fb'), 'ps1', ('fa', 'fb'), 'replay')
        good = 'fa replay: fixtures 3, passed 3, failed 0\nfb replay: fixtures 12, passed 12, failed 0\n'
        review.check_output(check, good)
        bad = {'a failed fixture': good.replace('passed 3, failed 0', 'passed 2, failed 1'),
               'a fixture neither passed nor failed': good.replace('passed 3', 'passed 2'),
               'a line that says failed with every fixture passed': good.replace('passed 3, failed 0', 'passed 3, failed 1'),
               'no fixture': good.replace('fixtures 3, passed 3', 'fixtures 0, passed 0'),
               'a function without a line': good.splitlines()[0] + '\n',
               'a line twice': good + good.splitlines()[1] + '\n',
               'a line that goes on': good.replace('fb replay: fixtures 12, passed 12, failed 0', 'fb replay: fixtures 12, passed 12, failed 0 of 13')}
        for why, output in bad.items():
            with self.subTest(why=why), self.assertRaises(model.Problem):
                review.check_output(check, output)
        with self.assertRaises(model.Problem):
            review.check_output(review.Check('r', ('python', 'difftest.py', '--replay'), 'ps1', (), 'replay'), '')

    def test_a_second_summary_of_a_function_is_never_hidden_by_a_passing_one(self):
        # Every line that begins like a function's summary counts, also one that does not look like a pass: a pass
        # beside a failure, beside a line that goes on, beside a claim of no fixture, or beside its own copy.
        one = review.Check('r', ('python', 'difftest.py', '--replay', 'f'), 'ps1', ('f',), 'replay')
        good = 'f replay: fixtures 3, passed 3, failed 0'
        review.check_output(one, good + '\n')
        review.check_output(one, 'fa replay: fixtures 1, passed 0, failed 1\n' + good + '\n  fixture 0 (case 4, note):\n')
        others = {'a failure': 'f replay: fixtures 3, passed 2, failed 1', 'a line that goes on': good + ' of 4',
                  'no fixture': 'f replay: fixtures 0, passed 0, failed 0', 'its own copy': good,
                  'something else': 'f replay: could not be read'}
        for why, other in others.items():
            for output in (good + '\n' + other + '\n', other + '\n' + good + '\n'):
                with self.subTest(why=why, first=output.splitlines()[0]), self.assertRaises(model.Problem):
                    review.check_output(one, output)
        # The same for the three modes of the wide runs.
        wide = {'differential': ('f: built 12 bytes; cases 2, discarded 0, equal 2, different 0',
                                 ('f: built 12 bytes; cases 2, discarded 0, equal 1, different 1', 'f: no contract')),
                'writes': ('f writes: cases 2, discarded 0, outside 0 (largest 0 bytes)',
                           ('f writes: cases 2, discarded 0, outside 1 (largest 4 bytes)',)),
                'control': ('f control: different 1 of 2 (expected more than 0)',
                            ('f control: different 0 of 2 (expected more than 0)',))}
        for mode, (good, bad) in wide.items():
            check = dataclasses.replace(self.check, mode=mode)
            review.check_output(check, good + '\nf coverage: 3 of 3 instruction slots of the original executed\n')
            with self.subTest(mode=mode, lone='a line that goes on'), self.assertRaises(model.Problem):
                review.check_output(check, good + ' and more')
            for other in (*bad, good, good + ' and more'):
                for output in (good + '\n' + other, other + '\n' + good):
                    with self.subTest(mode=mode, other=other, first=output.splitlines()[0]), self.assertRaises(model.Problem):
                        review.check_output(check, output)

    def test_contradictory_replay_evidence_of_a_zero_status_run_is_not_a_pass_and_is_not_cached(self):
        check = review.Check('r', ('python', 'difftest.py', '--replay', 'f'), 'ps1', ('f',), 'replay')
        self.result_output = 'f replay: fixtures 3, passed 3, failed 0\nf replay: fixtures 3, passed 2, failed 1\n'
        result = self.run_job('contradiction', check=check)
        self.assertFalse(result['complete'])
        self.assertNotEqual(result['status'], 'fresh_pass')
        self.assertFalse((self.root / 'local/cache').exists())
        self.result_output = 'f replay: fixtures 3, passed 3, failed 0\n'
        self.assertEqual(self.run_job('plain', check=check)['status'], 'fresh_pass')

    def test_unknown_removed_and_missing_native_refuse(self):
        for changed in (['mystery.py'], ['requirements.txt'], ['ps1/src/slot06_nonmatching/func_80110001_slot06_00.c']):
            with self.subTest(changed=changed), self.assertRaises(model.Problem):
                self.selected(changed)
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['ps1/src/one.c'], 2, (1,7))

    def test_validation_tool_changes_select_control_suites(self):
        self.put('ps1/tools/test_example.py','')
        self.assertIn('test_example',[c.name for c in self.selected(['ps1/tools/example.py'])])

    def test_port_runtime_controls_get_the_compiler_or_refuse(self):
        for name in ('test_hostlaunch.py', 'test_hostmirror.py', 'test_hostbuild.py', 'test_hostreplay.py', 'test_hostfixtures.py', 'test_hostmdec.py'):
            self.put('port/tools/' + name, '')
        self.put('port/src/example.c', '')
        argv = {c.name: c.argv for c in self.selected(['port/src/example.c'])}
        for name in ('test_hostlaunch', 'test_hostmirror', 'test_hostbuild', 'test_hostreplay', 'test_hostfixtures', 'test_hostmdec'):
            self.assertEqual(argv[name][-2:], ('--cc', 'cc32'))
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7))

    def test_sound_chip_controls_get_the_compiler_and_psyz_or_refuse(self):
        self.put('port/tools/test_hostiopage.py', '')
        self.put('port/src/example.c', '')
        argv = {c.name: c.argv for c in review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7), 'cc32', 'psyzdir')}
        self.assertEqual(argv['test_hostiopage'][-4:], ('--cc', 'cc32', '--psyz-build', 'psyzdir'))
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7), 'cc32')

    def test_sound_out_controls_get_the_compiler_and_psyz_or_refuse(self):
        self.put('port/tools/test_hostaudio.py', '')
        self.put('port/src/example.c', '')
        argv = {c.name: c.argv for c in review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7), 'cc32', 'psyzdir')}
        self.assertEqual(argv['test_hostaudio'][-4:], ('--cc', 'cc32', '--psyz-build', 'psyzdir'))
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7), 'cc32')

    def test_a_change_under_port_overrides_gets_the_checks_of_a_change_of_the_build_tool(self):
        for name in ('test_hostlaunch.py', 'test_hostbuild.py', 'test_hostrun.py'):
            self.put('port/tools/' + name, '')
        self.put('port/tools/hostbuild.py', '')
        for name in ('fa.c', 'fa.py'):
            self.put('port/overrides/' + name, '')
        self.put('port/overrides/fb.c', '')
        self.put('port/overrides/fb.py', '')
        self.put('port/overrides/README.md', '')
        tool = [(c.name, c.argv) for c in self.selected(['port/tools/hostbuild.py'])]
        self.assertFalse([n for n, _ in tool if n.startswith('port-overrides-')])
        for changed in (['port/overrides/fa.c'], ['port/overrides/fa.py'], ['port/overrides/fa.c', 'port/overrides/fa.py']):
            checks = self.selected(changed)
            # The differential test of the changed override on both seeds, its write audit and its control come first.
            own = [c for c in checks if c.name.startswith('port-overrides-')]
            self.assertEqual([(c.name, c.mode, c.functions) for c in own], [
                ('port-overrides-diff-1', 'differential', ('fa',)), ('port-overrides-writes-1', 'writes', ('fa',)),
                ('port-overrides-diff-7', 'differential', ('fa',)), ('port-overrides-writes-7', 'writes', ('fa',)),
                ('port-overrides-controls', 'control', ('fa',))])
            # Their scope names the files their results depend on: the game's tree and the overrides.
            self.assertEqual({c.scope for c in own}, {'overrides'})
            for c in own:
                self.assertEqual(c.argv[c.argv.index('--folder') + 1], 'port/overrides')
                self.assertEqual(c.argv[-1], 'fa')
            self.assertEqual([(c.name, c.argv) for c in checks if c not in own], tool)
        # A header of the game, which every override includes, selects every override: one comparison of each,
        # and a replay of the one that has fixtures.
        broad = [c for c in self.selected(['ps1/src/shared.h']) if c.name.startswith('port-overrides-')]
        self.assertEqual([(c.name, c.mode, c.functions) for c in broad], [('port-overrides-diff-once-1', 'differential', ('fa', 'fb'))])
        self.put('port/overrides/fb.fixtures.json', '')
        broad = [c for c in self.selected(['ps1/src/shared.h']) if c.name.startswith('port-overrides-')]
        self.assertEqual([(c.name, c.mode, c.functions, c.scope) for c in broad], [
            ('port-overrides-replay', 'replay', ('fb',), 'overrides'), ('port-overrides-diff-once-1', 'differential', ('fa',), 'overrides')])
        own = [c for c in self.selected(['port/overrides/fb.c']) if c.name.startswith('port-overrides-')]
        self.assertEqual([(c.name, c.argv[-4:]) for c in own], [('port-overrides-replay', ('--folder', 'port/overrides', '--replay', 'fb'))])
        # The folder's page alone selects nothing.
        self.assertEqual(self.selected(['port/overrides/README.md']), [])
        names = [n for n, _ in tool]
        self.assertIn('native-build', names)
        self.assertIn('test_hostbuild', names)
        self.assertIn('test_hostrun', names)
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['port/overrides/fa.c'], 2, (1, 7))

    def test_port_pad_controls_get_the_compiler_and_psyz_or_refuse(self):
        self.put('port/tools/test_hostpads.py', '')
        self.put('port/src/example.c', '')
        argv = {c.name: c.argv for c in review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7), 'cc32', 'psyzdir')}
        self.assertEqual(argv['test_hostpads'][-4:], ('--cc', 'cc32', '--psyz-build', 'psyzdir'))
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7), 'cc32')

    def test_sdkshim_control_gets_the_compiler_and_psyz_or_refuses(self):
        self.put('port/tools/test_sdkshim.py', '')
        self.put('port/src/example.c', '')
        argv = {c.name: c.argv for c in review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7), 'cc32', 'psyzdir')}
        self.assertEqual(argv['test_sdkshim'][-4:], ('--cc', 'cc32', '--psyz-build', 'psyzdir'))
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7), 'cc32')
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['port/src/example.c'], 2, (1, 7))

    def test_bad_seeds_and_zero_cases_refuse(self):
        for cases, seeds in ((0,(1,7)),(2,(1,1))):
            with self.assertRaises(model.Problem):
                review.select_checks(self.root, [], cases, seeds)


class Results(Fixture):
    def test_complete_hit_and_force_fresh(self):
        first = self.run_job('first')
        self.assertEqual(first['status'], 'fresh_pass')
        self.assertEqual(self.run_job('second')['status'], 'cached')
        self.assertTrue((self.root / 'local/second/result.json').is_file())
        self.assertEqual(self.executions, 1)
        self.assertEqual(self.run_job('third', fresh=True)['status'], 'fresh_pass')
        self.assertEqual(self.executions, 2)

    def test_a_changed_override_is_never_answered_from_an_old_result(self):
        for name in ('port/overrides/fa.c', 'port/overrides/fa.py', 'port/src/example.c'):
            self.put(name, 'before')
            self.git('add', '-f', name)
        check = review.Check('port-overrides-diff-1', ('python', '--cases', '2', '--seed', '1'), 'overrides', ('f',), 'differential')
        # The key as the tool builds it: from the files of the check's own scope.
        scoped = lambda root, c: {'files': review.source_files(root, c.scope), 'check': dataclasses.asdict(c)}
        run = lambda name, c=check: review.run_check(self.root, c, self.root / 'local' / name, self.root / 'local/cache', 'head',
                                                     identity=scoped, execute=self.execute)
        self.assertEqual(run('o1')['status'], 'fresh_pass')
        self.assertEqual(run('o2')['status'], 'cached')
        for i, name in enumerate(('port/overrides/fa.c', 'port/overrides/fa.py', 'ps1/src/one.c')):
            self.put(name, 'after ' + str(i))
            with self.subTest(name=name):
                self.assertEqual(run('o3' + str(i))['status'], 'fresh_pass')
        # A file of the port outside the folder is no input of these checks.
        self.put('port/src/example.c', 'after')
        self.assertEqual(run('o4')['status'], 'cached')
        self.assertEqual(self.executions, 4)
        self.assertIn('port/overrides/fa.c', review.source_files(self.root, 'overrides'))
        self.assertNotIn('port/src/example.c', review.source_files(self.root, 'overrides'))
        # The scope of the game's own checks does not hold the folder: a check of an override keyed by it is
        # answered from the old result after the override changed. That was the fault of the first version.
        wrong = dataclasses.replace(check, scope='ps1')
        self.assertEqual(run('w1', wrong)['status'], 'fresh_pass')
        self.put('port/overrides/fa.c', 'changed again')
        self.assertEqual(run('w2', wrong)['status'], 'cached')

    def test_every_dependency_class_invalidates(self):
        for name in ('ps1/src/one.c', 'ps1/src/shared.h', 'ps1/src/symbols.ld', 'ps1/src/slot06_nonmatching/func_80110000_slot06_00.py', 'baseline', 'compiler'):
            self.put(name, 'before')
            self.git('add', '-f', name)
        self.run_job('original')
        for i, name in enumerate(('ps1/src/one.c', 'ps1/src/shared.h', 'ps1/src/symbols.ld', 'ps1/src/slot06_nonmatching/func_80110000_slot06_00.py', 'baseline', 'compiler')):
            self.put(name, 'after')
            with self.subTest(name=name):
                self.assertEqual(self.run_job('change'+str(i))['status'], 'fresh_pass')
        changed = dataclasses.replace(self.check, argv=('python', '--cases', '2', '--seed', '7'))
        self.assertEqual(self.run_job('arguments', check=changed)['status'], 'fresh_pass')
        self.assertEqual(self.executions, 8)

    def test_failed_partial_and_zero_cases_never_cached(self):
        for i, (rc, output) in enumerate(((1,self.result_output),(0,''),(0,'f: built 1 bytes; cases 0, discarded 0, equal 0, different 0'),(0,self.result_output*2))):
            self.rc, self.result_output = rc, output
            result = self.run_job('bad'+str(i))
            self.assertFalse(result['complete'])
            self.assertFalse((self.root / 'local/cache').exists())

    def test_missing_tampered_and_malformed_evidence_rerun(self):
        first = self.run_job('first')
        entry = self.root / 'local/cache' / first['key']
        for i, corrupt in enumerate(('delete','tamper','json','inputs','wrong_head_fields')):
            if corrupt == 'delete':
                (entry / 'output.log').unlink()
            elif corrupt == 'tamper':
                (entry / 'output.log').write_text('different evidence')
            elif corrupt == 'json':
                (entry / 'result.json').write_text('{')
            elif corrupt == 'inputs':
                (entry / 'inputs.json').write_text('{}')
            else:
                r=json.loads((entry / 'result.json').read_text()); r['complete']=False
                (entry / 'result.json').write_text(json.dumps(r))
            self.assertEqual(self.run_job('repair'+str(i))['status'], 'fresh_pass')
        self.assertEqual(self.run_job('good')['status'], 'cached')
        self.assertEqual(self.executions, 6)

    def test_unresolved_closure_runs_fresh_without_weak_cache(self):
        def unavailable(*args):
            raise model.Problem('unresolved')
        for i in range(2):
            r=review.run_check(self.root,self.check,self.root/f'local/no{i}',self.root/'local/cache','head',identity=unavailable,execute=self.execute)
            self.assertEqual(r['status'],'fresh_pass'); self.assertIsNone(r['key'])
        self.assertEqual(self.executions,2)

    def test_dependency_changes_during_run_refuse(self):
        def mutate(argv, root, stream):
            stream.write(self.result_output)
            self.put('ps1/src/one.c','changed during check')
            return 0
        r=review.run_check(self.root,self.check,self.root/'local/mutate',self.root/'local/cache','head',identity=self.identity,execute=mutate)
        self.assertFalse(r['complete']); self.assertIn('dependencies changed',r['error'])

    def test_actual_child_exit_and_timeout(self):
        for i, argv in enumerate(((sys.executable,'-c','raise SystemExit(17)'),(sys.executable,'-c','import time; time.sleep(10)'))):
            check=review.Check('child',argv,'workflow')
            r=review.run_check(self.root,check,self.root/f'local/child{i}',self.root/'local/cache','head',identity=self.identity,timeout=1)
            self.assertFalse(r['complete']); self.assertNotEqual(r['exit_code'],0)

    def test_imported_evidence_needs_complete_modes_head_hashes_and_sources(self):
        name='func_80110000_slot06_00'
        report_dir=self.root/'local/evidence'
        report_dir.mkdir(parents=True)
        records=[]
        modes=(('differential',1),('writes',1),('differential',7),('writes',7),('control',None))
        for i,(mode,seed) in enumerate(modes):
            argv=['python','--cases','2']+(['--seed',str(seed)] if seed is not None else [])
            output={'differential':f'{name}: built 12 bytes; cases 2, discarded 0, equal 2, different 0',
                    'writes':f'{name} writes: cases 2, discarded 0, outside 0 (largest 0 bytes)',
                    'control':f'{name} control: different 1 of 2 (expected more than 0)'}[mode]
            log=report_dir/f'{i}.log'; log.write_text(output)
            records.append({'name':str(i),'head':model.revision(self.root),'scope':'ps1','mode':mode,'argv':argv,
                'functions':[name],'complete':True,'exit_code':0,'evidence':log.name,'evidence_sha256':review.digest(log),
                'source_files':review.source_files(self.root,'ps1')})
        report={'schema':1,'head':model.revision(self.root),'status':'checks_passed','checks':records}
        path=report_dir/'report.json'; path.write_text(json.dumps(report))
        row={'source':'ps1/src/slot06_nonmatching/'+name+'.c','contract':'ps1/src/slot06_nonmatching/'+name+'.py','status':'nonmatching_unvalidated'}
        model.import_evidence(self.root,[row],path)
        self.assertEqual(row['status'],'tested_nonmatching')
        second={**row,'second_placement':True,'status':'nonmatching_unvalidated'}
        model.import_evidence(self.root,[second],path)
        self.assertEqual(second['status'],'nonmatching_unvalidated')
        incomplete={**report,'checks':records[:-1]}; path.write_text(json.dumps(incomplete))
        row['status']='nonmatching_unvalidated'; model.import_evidence(self.root,[row],path)
        self.assertEqual(row['status'],'nonmatching_unvalidated')
        # A replay is evidence for the recorded inputs only: a report with nothing else is read and leaves the status.
        log=report_dir/'r.log'; log.write_text(f'{name} replay: fixtures 2, passed 2, failed 0')
        replay={'name':'r','head':model.revision(self.root),'scope':'ps1','mode':'replay','argv':['python','--replay',name],
                'functions':[name],'complete':True,'exit_code':0,'evidence':log.name,'evidence_sha256':review.digest(log),
                'source_files':review.source_files(self.root,'ps1')}
        path.write_text(json.dumps({**report,'checks':[replay]})); model.import_evidence(self.root,[row],path)
        self.assertEqual(row['status'],'nonmatching_unvalidated')
        path.write_text(json.dumps({**report,'head':'wrong'}))
        with self.assertRaises(model.Problem): model.import_evidence(self.root,[row],path)
        path.write_text(json.dumps(report)); self.put('ps1/src/one.c','stale evidence')
        with self.assertRaises(model.Problem): model.import_evidence(self.root,[row],path)
        self.put('ps1/src/one.c','int one(void) { return 1; }')
        (report_dir/'0.log').write_text('tampered')
        with self.assertRaises(model.Problem): model.import_evidence(self.root,[row],path)

    def test_unittest_controls_must_run_not_skip_every_case(self):
        check=review.Check('test_example',('python',),'workflow')
        review.check_output(check,'Ran 2 tests in 1s\nOK\n')
        with self.assertRaises(model.Problem):review.check_output(check,'Ran 2 tests in 1s\nOK (skipped=2)\n')

    def test_write_and_negative_control_counts(self):
        for mode, good in (('writes','f writes: cases 2, discarded 0, outside 0 (largest 0 bytes)'),('control','f control: different 1 of 2 (expected more than 0)')):
            check=dataclasses.replace(self.check,mode=mode)
            review.check_output(check,good)
            with self.assertRaises(model.Problem): review.check_output(check,good.replace('cases 2','cases 1').replace('of 2','of 1'))
        with self.assertRaises(model.Problem): review.check_output(dataclasses.replace(self.check,mode='control'),'f control: different 0 of 2 (expected more than 0)')


class StagingAndContext(Fixture):
    def test_actual_config_paths_and_original_unchanged(self):
        data=Path(self.tmp.name)/'inputs'
        for p in review.config_inputs(self.root):
            original=data/p.relative_to(self.root)
            original.parent.mkdir(parents=True,exist_ok=True)
            if p.name=='include': original.mkdir()
            else: original.write_text('private fixture')
        inputs=review.stage_inputs(self.root,data)
        self.assertTrue(all(p.exists() for p in inputs))
        self.assertTrue(all(p.is_symlink() for p in inputs))
        self.assertEqual((data/'ps1/audit/files/TEST').read_text(),'private fixture')

    def test_gitlink_is_not_a_missing_regular_file(self):
        sha=model.revision(self.root)
        self.git('update-index','--add','--cacheinfo','160000,'+sha+',port/external/example')
        files=review.source_files(self.root,'port')
        self.assertTrue(files['port/external/example'].startswith('gitlink:'+sha+':'))

    def test_missing_and_escape_refuse(self):
        with self.assertRaises(model.Problem): review.stage_inputs(self.root,self.root/'absent')
        self.put('ps1/src/build.toml',CFG.replace('../audit/files/TEST','../../../outside'))
        with self.assertRaises(model.Problem): review.config_inputs(self.root)

    def test_frozen_commit_ignores_dirty_worktree(self):
        sha=model.revision(self.root)
        self.put('ps1/src/one.c','uncommitted')
        dest=self.root/'local/snapshot'
        review.freeze(self.root,sha,dest)
        self.assertNotEqual((dest/'ps1/src/one.c').read_text(),'uncommitted')
        with self.assertRaises(model.Problem): review.freeze(self.root,sha,dest)

    def test_bounded_history_and_hash_index(self):
        self.put('docs/project-memory.md','# Active\nshort\n')
        data='intro\n## Before\nold mistake\n## After\ncompiler correction\n'
        history=self.put('docs/project-memory-history.md',data)
        self.assertEqual(model.context(self.root,limit=1),'# Active')
        self.assertIn('compiler correction',model.context(self.root,'compiler correction'))
        self.assertEqual(len(model.context(self.root,'compiler',limit=2).splitlines()),2)
        self.assertEqual(model.history_index(history)['sha256'],hashlib.sha256(data.encode()).hexdigest())
        self.assertEqual(model.history_index(history)['sections'][1]['line'],4)
        with self.assertRaises(model.Problem): model.context(self.root,limit=0)

    def test_task_missing_sections_and_placeholders(self):
        template=(REPO/'docs/tasks/template.md').read_text()
        with self.assertRaises(model.Problem): model.validate_task(template)
        valid=template.replace('<short product change>','small change')
        model.validate_task(valid)
        with self.assertRaises(model.Problem): model.validate_task(valid.replace('## Scope','## Wrong'))
        with self.assertRaises(model.Problem): model.validate_task(valid.replace('STOP','Proceed'))

    def test_private_output_and_empty_report_refuse(self):
        with self.assertRaises(model.Problem): workflow.private_path(self.root,self.root/'docs/report.json')
        self.assertEqual(workflow.private_path(self.root,self.root/'local/report'),self.root/'local/report')
        report=self.put('local/empty.json',json.dumps({'schema':1,'head':model.revision(self.root),'status':'checks_passed','checks':[]}))
        with self.assertRaises(model.Problem): model.import_evidence(self.root,[],report)


class LedgerFixture(Fixture):
    def test_categories_data_and_partial_ranges(self):
        config=CFG+'\n[[unit]]\nname="data"\nsource="data.c"\nrodata={address=2148532240,size=8}\n[[unit]]\nname="assembly"\nsource="entry.s"\nkind="asm"\nfunctions=[{name="entry",address=2148532256,size=8}]\n[[unit]]\nname="partial"\nsource="part.c"\nfunctions=[{name="part",address=2148532276,size=4}]\n[[unit]]\nname="sdk"\nsource="sdk/lib.c"\nfunctions=[{name="sdk",address=2148532288,size=8}]\n'
        self.put('ps1/src/build.toml',config)
        self.put('ps1/inventory/game.tsv','80100000\t16\t-\n80100010\t8\t-\n80100018\t8\t-\n80100020\t8\t-\n80100030\t16\t-\n80100040\t8\t-\n')
        self.put('ps1/inventory/library.tsv','')
        self.put('ps1/inventory/modules.tsv','')
        self.put('ps1/src/slot06_nonmatching/func_80100018.c','int candidate(void){return 1;}')
        self.put('ps1/src/slot06_nonmatching/func_80100018.py','CONTRACT=1')
        with patch('model.load_coverage',return_value=model.load_coverage(REPO)):
            result=model.ledger(self.root)
        self.assertEqual(result['data_rows'],1)
        self.assertEqual([f['status'] for f in result['functions']],['exact_c','nonmatching_unvalidated','assembly','partial','exact_c'])
        self.assertTrue(result['functions'][-1]['sdk'])
        self.assertEqual(result['functions'][3]['owners'][0]['function'],'part')


class RepositoryControls(unittest.TestCase):
    def test_ledger_matches_canonical_inventory(self):
        root=REPO
        result=model.ledger(root)
        coverage=model.load_coverage(root)
        config=__import__('tomllib').loads((root/'ps1/src/build.toml').read_text())
        self.assertEqual(result['overall'],coverage.overall(coverage.build_panels(root/'ps1/inventory',config)))
        self.assertEqual(len(result['functions']),result['overall']['placements_total'])
        self.assertGreaterEqual(result['data_rows'],0)
        self.assertTrue(any(f['status']=='assembly' for f in result['functions']))
        self.assertTrue(any(f['sdk'] for f in result['functions']))
        self.assertTrue(any(f['second_placement'] for f in result['functions']))
        self.assertTrue(any(f['status']=='nonmatching_unvalidated' for f in result['functions']))
        self.assertFalse(any(f['status']=='tested_nonmatching' for f in result['functions']))
        self.assertTrue(any(f['blocker']=='missing_c' for f in result['functions']))
        self.assertEqual(len({f['id'] for f in result['functions']}),len(result['functions']))

    def test_archive_index_and_active_context_budget(self):
        history=REPO/'docs/project-memory-history.md'
        if not history.exists(): self.skipTest('history split not installed yet')
        self.assertEqual(json.loads((REPO/'docs/project-memory-index.json').read_text()),model.history_index(history))
        self.assertLessEqual(len((REPO/'docs/project-memory.md').read_text().splitlines()),120)


class Records(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.put('docs/project-memory.md', '# Active\n')
        self.put('docs/project-memory-history.md', '# History\n\n## Old widget work (2026-01-01)\nwidget in history')

    def put(self, name, text):
        p = self.root / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)

    def test_record_is_found_and_history_still_first(self):
        self.put('docs/records/2026-02-02-new-widget.md', '## New widget (2026-02-02)\nwidget in record\n')
        out = model.context(self.root, 'widget')
        self.assertIn('[project-memory-history.md:3]', out)
        self.assertIn('[records/2026-02-02-new-widget.md]', out)
        self.assertLess(out.index('widget in history'), out.index('widget in record'))
        self.assertEqual(model.context(self.root, 'history'), '[project-memory-history.md:3]\n## Old widget work (2026-01-01)\nwidget in history')

    def test_record_not_hidden_by_long_history(self):
        self.put('docs/project-memory-history.md', '## Big (2026-01-01)\n' + 'gizmo\n' * 50)
        self.put('docs/records/2026-02-02-gizmo.md', '## Gizmo (2026-02-02)\ngizmo record\n')
        out = model.context(self.root, 'gizmo', 10)
        self.assertIn('[records/2026-02-02-gizmo.md]', out)
        self.assertEqual(len(out.splitlines()), 10 + 3)

    def test_readme_is_not_a_record(self):
        self.put('docs/records/README.md', 'widget readme text\n')
        self.assertNotIn('records/README.md', model.context(self.root, 'widget'))
        self.assertEqual(model.check_records(self.root), [])

    def test_records_come_newest_first(self):
        self.put('docs/records/2026-03-03-b.md', '## B (2026-03-03)\ngadget\n')
        self.put('docs/records/2026-03-01-a.md', '## A (2026-03-01)\ngadget\n')
        out = model.context(self.root, 'gadget')
        self.assertLess(out.index('2026-03-03-b.md'), out.index('2026-03-01-a.md'))

    def test_bad_name_and_bad_first_line_are_refused_with_the_file_name(self):
        self.put('docs/records/2026-03-01-good.md', '## Good (2026-03-01)\ntext\n')
        self.put('docs/records/notes.md', '## Notes (2026-03-01)\n')
        self.put('docs/records/2026-03-01-Upper.md', '## Upper (2026-03-01)\n')
        self.put('docs/records/2026-03-02-nohead.md', 'no heading\n')
        self.put('docs/records/2026-03-04-nodate.md', '## No date\n')
        problems = '\n'.join(model.check_records(self.root))
        for name in ('notes.md', '2026-03-01-Upper.md', '2026-03-02-nohead.md', '2026-03-04-nodate.md'):
            self.assertIn(name, problems)
        self.assertNotIn('2026-03-01-good.md', problems)

    def test_empty_and_missing_folder_are_fine(self):
        self.assertEqual(model.check_records(self.root), [])
        self.assertIn('widget in history', model.context(self.root, 'widget'))
        (self.root / 'docs/records').mkdir()
        self.assertEqual(model.check_records(self.root), [])
        self.assertIn('widget in history', model.context(self.root, 'widget'))
        self.assertEqual(model.context(self.root, 'absent'), 'No matching historical section.')

    def test_repository_records_have_the_form(self):
        self.assertEqual(model.check_records(REPO), [])


if __name__=='__main__':
    unittest.main()
