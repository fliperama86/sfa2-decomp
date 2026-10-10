"""Bounded context and metadata-only function/task indexes."""
from __future__ import annotations

from bisect import bisect_left, bisect_right
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys
import tomllib


class Problem(Exception):
    pass


def revision(root: Path, ref: str = 'HEAD') -> str:
    return subprocess.check_output(['git', 'rev-parse', '--verify', ref + '^{commit}'], cwd=root, text=True).strip()


def sections(text: str) -> list[dict]:
    lines = text.splitlines()
    starts = [i for i, line in enumerate(lines) if re.match(r'^## ', line)]
    return [{'title': lines[a][3:], 'line': a + 1, 'text': '\n'.join(lines[a:b])}
            for a, b in zip(starts, starts[1:] + [len(lines)])]


RECORD_NAME = re.compile(r'^\d{4}-\d{2}-\d{2}-[a-z0-9]+(-[a-z0-9]+)*\.md$')
RECORD_HEADING = re.compile(r'^## .+ \(\d{4}-\d{2}-\d{2}\)$')


def record_files(root: Path) -> list[Path]:
    """The files of docs/records/ except its README, in order of file name."""
    folder = root / 'docs/records'
    return sorted((p for p in folder.glob('*.md') if p.name != 'README.md'), key=lambda p: p.name)


def check_records(root: Path) -> list[str]:
    """Name every record file whose name or first line has not the required form."""
    problems = []
    for path in record_files(root):
        if not RECORD_NAME.match(path.name):
            problems.append(f'{path.name}: the name is not YYYY-MM-DD-short-name.md')
        lines = path.read_text().splitlines()
        if not lines or not RECORD_HEADING.match(lines[0]):
            problems.append(f'{path.name}: the first line is not a heading "## Title (YYYY-MM-DD)"')
    return problems


def context(root: Path, query: str = '', limit: int = 120) -> str:
    """Active state, or the sections and records that match every word of the query.

    Order when both match: the history's sections first, in file order, capped at
    `limit` lines exactly as before; then the matching records of docs/records/,
    newest first (reverse order of file name), capped at `limit` lines of their
    own. The total can reach twice the limit: separate budgets keep a record from
    being hidden by old history sections.
    """
    if limit < 1 or limit > 500:
        raise Problem('context limit must be 1..500 lines')
    active = (root / 'docs/project-memory.md').read_text()
    if not query:
        return '\n'.join(active.splitlines()[:limit])
    history = root / 'docs/project-memory-history.md'
    text = history.read_text() if history.exists() else active
    terms = query.casefold().split()
    found = [s for s in sections(text) if all(t in s['text'].casefold() for t in terms)]
    out = []
    for s in found:
        out.extend([f"[{history.name if history.exists() else 'project-memory.md'}:{s['line']}]", s['text'], ''])
    recs = []
    for path in reversed(record_files(root)):
        body = path.read_text()
        if all(t in body.casefold() for t in terms):
            recs.extend([f'[records/{path.name}]', body.rstrip('\n'), ''])
    lines = '\n'.join(out).splitlines()[:limit] + '\n'.join(recs).splitlines()[:limit]
    return '\n'.join(lines) or 'No matching historical section.'


def history_index(path: Path) -> dict:
    data = path.read_bytes()
    return {'file': path.name, 'sha256': hashlib.sha256(data).hexdigest(),
            'sections': [{k: s[k] for k in ('title', 'line')} for s in sections(data.decode())]}


def load_coverage(root: Path):
    spec = importlib.util.spec_from_file_location('ai_coverage', root / 'ps1/tools/coveragemap.py')
    if spec is None or spec.loader is None:
        raise Problem('coverage tool unavailable')
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def ledger(root: Path, evidence: Path | None = None) -> dict:
    """Use the canonical sweep classification; never infer tests from file presence."""
    config = tomllib.loads((root / 'ps1/src/build.toml').read_text())
    coverage = load_coverage(root)
    panels = coverage.build_panels(root / 'ps1/inventory', config)
    images = {i['name']: i for i in config.get('image', [])}
    owners = {}
    for unit in config.get('unit', []):
        image = unit.get('image', coverage.RESIDENT)
        for fn in unit.get('functions', []):
            key = (image, fn['address'])
            if key in owners:
                raise Problem('duplicate declared ownership')
            owners[key] = (unit, fn)
    for name, image in images.items():
        if 'like' not in image:
            continue
        base = images[image['like']]
        shift = image['address'] - base['address']
        for (old_image, address), (unit, fn) in list(owners.items()):
            if old_image == base['name'] and unit['name'] not in image.get('leave_out', []):
                owners[(name, address + shift)] = (unit, fn)
    grouped = {}
    for (image, address), (unit, fn) in owners.items():
        grouped.setdefault(image, []).append((address, unit, fn))
    grouped = {image: sorted(items, key=lambda i: i[0]) for image, items in grouped.items()}
    starts = {image: [a for a, _, _ in items] for image, items in grouped.items()}
    ends = {image: [a + fn['size'] for a, _, fn in items] for image, items in grouped.items()}
    candidates = {}
    for folder in sorted((root / 'ps1/src').glob('*_nonmatching')):
        for source in sorted(folder.glob('func_*.c')):
            match = re.fullmatch(r'func_([0-9a-f]{8})(?:_(.+))?', source.stem)
            if not match:
                raise Problem(f'invalid nonmatching name: {source.name}')
            key = (match[2] or coverage.RESIDENT, int(match[1], 16))
            if key in candidates or key in owners:
                raise Problem(f'duplicate C ownership: {source.name}')
            candidates[key] = source
    rows = []
    for panel in panels:
        for block in panel.blocks:
            image = coverage.RESIDENT if panel.name == coverage.RESIDENT else block.image
            for fn in block.functions:
                key = (image, fn.address)
                lo = bisect_right(ends.get(image, []), fn.address)
                hi = bisect_left(starts.get(image, []), fn.address + fn.size)
                overlapping = [(u, f) for _, u, f in grouped.get(image, [])[lo:hi]]
                owner = overlapping[0] if len(overlapping) == 1 else None
                candidate = candidates.get(key)
                # Second placements of nonmatching C use the same source.
                if not candidate and image in images and 'like' in images[image]:
                    base = images[images[image]['like']]
                    candidate = candidates.get((base['name'], fn.address - images[image]['address'] + base['address']))
                kind = {coverage.C: 'exact_c', coverage.ASM: 'assembly', coverage.PARTIAL: 'partial'}.get(fn.state, 'raw')
                name = owner[1]['name'] if owner else (candidate.stem if candidate else f'func_{fn.address:08x}')
                source = 'ps1/src/' + owner[0]['source'] if owner else (candidate.relative_to(root).as_posix() if candidate else None)
                contract = candidate.with_suffix('.py') if candidate else None
                row = {'id': f'{image or block.key}:{fn.address:08x}', 'image': image,
                       'content': block.key, 'address': fn.address, 'size_estimate': fn.size,
                       'name': name, 'name_origin': 'project_declared_or_inferred_not_original_symbol_proof', 'status': 'nonmatching_unvalidated' if candidate else kind,
                       'source': source, 'contract': contract.relative_to(root).as_posix() if contract and contract.is_file() else None,
                       'owner_unit': owner[0]['name'] if owner else None,
                       'owners': [{'unit': u['name'], 'function': f['name'], 'source': 'ps1/src/' + u['source']} for u, f in overlapping],
                       'coverage_status': kind,
                       'second_placement': block.second_link,
                       'sdk': bool(source and source.startswith('ps1/src/sdk/')),
                       'evidence': ['ps1/docs/matching-build.md'] if kind in ('exact_c', 'assembly') else ([str(candidate.parent.relative_to(root) / 'README.md')] if candidate else []),
                       'validation': 'declared_exact_owner_not_fresh_verification' if kind in ('exact_c', 'assembly') else 'no_machine_verified_result_imported',
                       'blocker': 'missing_c' if (kind != 'exact_c' or not overlapping) and not candidate else None}
                if candidate and block.second_link:
                    row.update(validation='candidate_from_first_placement_unvalidated_here', blocker='second_placement_needs_scoped_contract')
                if kind in ('exact_c', 'assembly') and not overlapping:
                    row.update(status='raw', validation='canonical_coverage_has_no_applicable_owner', blocker='missing_c')
                rows.append(row)
    if evidence:
        import_evidence(root, rows, evidence)
    dirty = bool(subprocess.check_output(['git', 'status', '--porcelain', '--', 'ps1/src', 'ps1/inventory'], cwd=root, text=True).strip())
    return {'schema': 1, 'commit': revision(root), 'metadata_worktree_dirty': dirty, 'inventory_is_estimate': True,
            'overall': coverage.overall(panels), 'data_rows': sum(p.data_rows for p in panels),
            'functions': rows, 'limits': 'Sweep boundaries can span multiple declarations; owners lists preserve them. Contracts alone never establish tested nonmatching C. Exact owner declarations are not a new build result. No original bytes or private paths.'}


TASK_SECTIONS = ('Objective', 'Pinned contract', 'Scope', 'Acceptance commands', 'Evidence', 'Decisions and escalation', 'Completion')


def validate_task(text: str) -> None:
    found = {s['title']: s['text'].split('\n', 1)[-1].strip() for s in sections(text)}
    missing = [s for s in TASK_SECTIONS if not found.get(s)]
    if missing:
        raise Problem('missing task sections: ' + ', '.join(missing))
    if re.search(r'<[A-Za-z][^>]*>', text) or 'TODO' in text:
        raise Problem('task still contains placeholders')
    if 'STOP' not in found['Decisions and escalation']:
        raise Problem('task lacks the decision-fork STOP rule')


def import_evidence(root: Path, rows: list[dict], path: Path) -> None:
    # A report is local evidence, not an authenticated GitHub approval.
    from review import Check, check_output, digest
    report = json.loads(path.read_text())
    if report.get('schema') != 1 or report.get('head') != revision(root) or report.get('status') != 'checks_passed':
        raise Problem('evidence report must be a complete check pass at the current commit')
    if not report.get('checks'):
        raise Problem('empty report is not function evidence')
    verified = {}
    for record in report['checks']:
        if record.get('complete') is not True or record.get('exit_code') != 0 or record.get('current_head', record.get('head')) != report['head']:
            raise Problem('incomplete or wrong-head evidence')
        log = Path(record['evidence'])
        if log.is_absolute() or not (path.parent / log).resolve().is_relative_to(path.parent.resolve()):
            raise Problem('evidence path escapes its report')
        log = path.parent / log
        if digest(log) != record['evidence_sha256']:
            raise Problem('evidence log hash differs')
        check = Check(record['name'], tuple(record['argv']), record['scope'], tuple(record['functions']), record['mode'])
        check_output(check, log.read_text())
        if check.mode == 'exit':
            continue
        source_files = record.get('source_files')
        if not source_files:
            raise Problem('function evidence lacks dependency hashes')
        if source_files != __import__('review').source_files(root, check.scope):
            raise Problem('function evidence dependency manifest differs')
        seed = check.argv[check.argv.index('--seed') + 1] if '--seed' in check.argv else None
        for name in check.functions:
            verified.setdefault(name, set()).add((check.mode, seed))
    for row in rows:
        if not row['contract'] or row.get('second_placement'):
            continue
        modes = verified.get(Path(row['source']).stem, set())
        seeds = {s for mode, s in modes if mode == 'differential'} & {s for mode, s in modes if mode == 'writes'}
        if len(seeds) >= 2 and ('control', None) in modes:
            row.update(status='tested_nonmatching', validation='scoped_differential_two_seeds_controls_and_original_writes', evidence=[path.name])
