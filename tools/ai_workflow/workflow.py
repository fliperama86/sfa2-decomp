#!/usr/bin/env python3
"""Bounded context, linked inventory and fail-closed local review orchestration."""
from __future__ import annotations

import argparse
import dataclasses
import datetime
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

from model import Problem, context, history_index, ledger, revision, validate_task
from review import GAME_SCOPES, config_inputs, freeze, run_check, select_checks, stage_inputs


def git(root: Path, *args: str) -> str:
    return subprocess.check_output(['git', *args], cwd=root, text=True).strip()


def private_path(root: Path, path: Path) -> Path:
    # Both a separate worktree's local/ and the shared checkout's local/ are OK.
    path = path.absolute()
    common = Path(git(root, 'rev-parse', '--path-format=absolute', '--git-common-dir')).parent
    if not any(path.resolve().is_relative_to((p / 'local').resolve()) for p in (root, common)):
        raise Problem('generated reports/cache must stay inside an ignored local/ directory')
    return path


def doctor(root: Path) -> dict:
    cfg = __import__('tomllib').loads((root / 'ps1/src/build.toml').read_text())
    tc = cfg['toolchain']
    items = [{'name': p.relative_to(root).as_posix(), 'available': p.exists()} for p in config_inputs(root)]
    commands = [sys.executable, tc['cpp'], *(tc['binutils_prefix'] + s for s in ('as', 'ld', 'objcopy', 'nm'))]
    items += [{'name': Path(c).name, 'available': bool(shutil.which(c))} for c in commands]
    items.append({'name': 'configured maspsx wrapper', 'available': Path(os.path.expanduser(tc['maspsx'])).is_file()})
    for package in ('capstone', 'elftools', 'unicorn'):
        import importlib.util
        items.append({'name': package, 'available': importlib.util.find_spec(package) is not None})
    return {'schema': 1, 'status': 'available' if all(x['available'] for x in items) else 'missing_prerequisites',
            'checks': items, 'limits': 'Presence only, not pin verification or a build. No private inputs modified.'}


def review(args, root: Path) -> int:
    if args.pr:
        if args.base or args.head:
            raise Problem('--pr and explicit base/head are mutually exclusive')
        data = json.loads(subprocess.check_output(['gh', 'pr', 'view', str(args.pr), '--json', 'baseRefOid,headRefOid'], cwd=root, text=True))
        for sha in (data['baseRefOid'], data['headRefOid']):
            subprocess.run(['git', 'fetch', 'origin', sha], cwd=root, check=True, stdout=sys.stderr)
        base, head = revision(root, data['baseRefOid']), revision(root, data['headRefOid'])
    else:
        if not args.base:
            raise Problem('provide --base and optional --head, or --pr')
        base, head = revision(root, args.base), revision(root, args.head or 'HEAD')
    # Inspect exactly the named head, not the mutable working directory.
    stamp = datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    output = private_path(root, args.output or root / 'local/ai-workflow/reports' / (head[:8] + '-' + stamp))
    if output.exists():
        raise Problem('report directory already exists; preserve old evidence')
    output.mkdir(parents=True)
    report = {'schema': 1, 'base': base, 'head': head, 'status': 'incomplete', 'checks': [],
              'limits': 'Check results only, not a review verdict. No whole-game or gameplay claim.'}
    report_path = output / 'report.json'
    rc = 2
    try:
        snapshot = output / 'repo'
        freeze(root, head, snapshot)
        changed = git(root, 'diff', '--name-only', base, head).splitlines()
        cache = private_path(root, args.cache or root / 'local/ai-workflow/cache')
        checks = select_checks(snapshot, changed, args.cases, tuple(args.seeds), args.cc, args.psyz_build)
        # Share the authoritative builder's dependency-keyed object cache across
        # frozen reviews; still relink and compare complete claimed output.
        checks = [dataclasses.replace(c, argv=c.argv + ('--cache', str(cache / 'matching-objects'))) if c.name == 'matching' else c for c in checks]
        report.update(changed=changed, plan=[dataclasses.asdict(c) for c in checks])
        if args.plan:
            report['status'] = 'planned_not_run'
            rc = 0
        else:
            if any(c.scope in GAME_SCOPES for c in checks):
                common = Path(git(root, 'rev-parse', '--path-format=absolute', '--git-common-dir')).parent
                stage_inputs(snapshot, (args.data_root or common).resolve())
            for check in checks:
                print('Checking ' + check.name + ' ...', flush=True)
                check_out = output / 'checks' / check.name
                result = run_check(snapshot, check, check_out, cache, head, args.fresh, timeout=args.timeout)
                result['evidence'] = (check_out / 'output.log').relative_to(output).as_posix()
                report['checks'].append(result)
                report_path.write_text(json.dumps(report, indent=2) + '\n')
                print(check.name + ': ' + result['status'], flush=True)
                if not result['complete']:
                    raise Problem('check failed: ' + check.name)
            report['status'] = 'checks_passed' if checks else 'documentation_only_no_code_checks'
            rc = 0
    except (Problem, OSError, ValueError, subprocess.SubprocessError, KeyboardInterrupt) as exc:
        report.update(status='failed_or_incomplete', error=str(exc))
        print(str(exc), file=sys.stderr)
    finally:
        report_path.write_text(json.dumps(report, indent=2) + '\n')
        print('Report: ' + str(report_path), flush=True)
    return rc


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[2])
    sub = p.add_subparsers(dest='command', required=True)
    doc = sub.add_parser('doctor')
    doc.add_argument('--verbose', action='store_true')
    ctx = sub.add_parser('context')
    ctx.add_argument('--query', default='')
    ctx.add_argument('--reindex', action='store_true', help='regenerate the preserved memory heading/hash index')
    ctx.add_argument('--limit', type=int, default=120)
    led = sub.add_parser('ledger')
    led.add_argument('--output', type=Path)
    led.add_argument('--evidence', type=Path)
    rev = sub.add_parser('review')
    rev.add_argument('--base')
    rev.add_argument('--head')
    rev.add_argument('--pr', type=int)
    rev.add_argument('--data-root', type=Path)
    rev.add_argument('--output', type=Path)
    rev.add_argument('--cache', type=Path)
    rev.add_argument('--cc', help='32-bit compiler for native integration')
    rev.add_argument('--psyz-build')
    rev.add_argument('--cases', type=int, default=2000)
    rev.add_argument('--seeds', nargs='+', type=int, default=[1, 7])
    rev.add_argument('--fresh', action='store_true')
    rev.add_argument('--plan', action='store_true')
    rev.add_argument('--timeout', type=int, default=1800)
    task = sub.add_parser('task')
    task.add_argument('--check', type=Path)
    task.add_argument('--name', default='bounded product change')
    return p


def main() -> int:
    args = parser().parse_args()
    root = args.root.resolve()
    try:
        if args.command == 'review':
            if args.timeout < 1:
                raise Problem('timeout must be positive')
            return review(args, root)
        if args.command == 'context':
            if args.reindex:
                index = history_index(root / 'docs/project-memory-history.md')
                (root / 'docs/project-memory-index.json').write_text(json.dumps(index, indent=2) + '\n')
            print(context(root, args.query, args.limit))
        elif args.command == 'doctor':
            result = doctor(root)
            if not args.verbose:
                items = result.pop('checks')
                result.update(available=sum(i['available'] for i in items), total=len(items), missing=[i['name'] for i in items if not i['available']])
            print(json.dumps(result, indent=2))
            return 0 if result['status'] == 'available' else 2
        elif args.command == 'ledger':
            result = ledger(root, args.evidence)
            output = private_path(root, args.output or root / 'local/ai-workflow/ledger.json')
            output.parent.mkdir(parents=True, exist_ok=True)
            output.write_text(json.dumps(result, indent=2) + '\n')
            print(json.dumps({'output': str(output), 'overall': result['overall'], 'rows': len(result['functions'])}, indent=2))
        elif args.command == 'task':
            if args.check:
                validate_task(args.check.read_text())
                print('Task contract complete (acceptance commands have not been run).')
            else:
                print((root / 'docs/tasks/template.md').read_text().replace('<short product change>', args.name), end='')
        return 0
    except (Problem, OSError, ValueError, subprocess.SubprocessError) as exc:
        print('Refused: ' + str(exc), file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
