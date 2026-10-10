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

    def test_binding_header_config_tools_broaden(self):
        for p in ('ps1/src/symbols.ld', 'ps1/src/build.toml', 'ps1/src/types.fields', 'ps1/src/shared.h', 'ps1/tools/new.py', 'ps1/src/slot06_nonmatching/contracts.py'):
            with self.subTest(path=p):
                checks = self.selected([p])
                self.assertIn('matching', [c.name for c in checks])
                self.assertIn('control', [c.mode for c in checks])

    def test_unknown_removed_and_missing_native_refuse(self):
        for changed in (['mystery.py'], ['requirements.txt'], ['ps1/src/slot06_nonmatching/func_80110001_slot06_00.c']):
            with self.subTest(changed=changed), self.assertRaises(model.Problem):
                self.selected(changed)
        with self.assertRaises(model.Problem):
            review.select_checks(self.root, ['ps1/src/one.c'], 2, (1,7))

    def test_validation_tool_changes_select_control_suites(self):
        self.put('ps1/tools/test_example.py','')
        self.assertIn('test_example',[c.name for c in self.selected(['ps1/tools/example.py'])])

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


if __name__=='__main__':
    unittest.main()
