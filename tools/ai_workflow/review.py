"""Frozen-head check selection and conservative, integrity-checked result reuse."""
from __future__ import annotations

import dataclasses
import hashlib
import importlib.metadata
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import time
import tomllib

from model import Problem


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def inside(root: Path, path: Path) -> Path:
    # Check lexical paths before creating links, not their external targets.
    normalized = Path(os.path.abspath(path))
    if not normalized.is_relative_to(root.resolve()):
        raise Problem('configured staging path escapes the frozen checkout')
    return normalized


def config_inputs(root: Path) -> list[Path]:
    directory = root / 'ps1/src'
    cfg = tomllib.loads((directory / 'build.toml').read_text())
    refs = [cfg['baseline']['executable'], *[i['archive'] for i in cfg.get('image', [])],
            *cfg['toolchain'].get('include_dirs', [])]
    cc = cfg['toolchain']['cc1']
    if cc.get('kind') != 'local':
        raise Problem('automatic staging supports the pinned local compiler only')
    refs.append(cc['path'])
    if cfg.get('overlays', {}).get('directory'):
        refs.append(cfg['overlays']['directory'])
    for ref in refs:
        if Path(ref).is_absolute() or '~' in ref:
            raise Problem('private input must use a repository-relative configuration path')
    return sorted(set(inside(root, directory / ref) for ref in refs))


def stage_inputs(snapshot: Path, data_root: Path) -> list[Path]:
    inputs = config_inputs(snapshot)
    for target in inputs:
        original = data_root / target.relative_to(snapshot)
        if target.exists():
            # Tracked includes need no link; never replace a tracked file.
            continue
        if not original.exists():
            raise Problem('private prerequisite missing: ' + target.relative_to(snapshot).as_posix())
        target.parent.mkdir(parents=True, exist_ok=True)
        target.symlink_to(original.resolve(), target_is_directory=original.is_dir())
    return inputs


@dataclasses.dataclass(frozen=True)
class Check:
    name: str
    argv: tuple[str, ...]
    scope: str
    functions: tuple[str, ...] = ()
    mode: str = 'exit'


# The scopes whose checks build with the game's toolchain and read its private inputs.
GAME_SCOPES = ('ps1', 'overrides')


def select_checks(root: Path, changed: list[str], cases: int, seeds: tuple[int, ...], cc: str | None = None,
                  psyz: str | None = None) -> list[Check]:
    if cases < 1 or len(set(seeds)) < 2:
        raise Problem('require positive cases and at least two distinct seeds')
    src = root / 'ps1/src'
    cfg = tomllib.loads((src / 'build.toml').read_text())
    owned = {'ps1/src/' + u['source'] for u in cfg.get('unit', [])}
    folders = sorted(p for p in src.glob('*_nonmatching') if p.is_dir())
    broad = any(p.startswith('ps1/tools/') or p in ('ps1/src/build.toml', 'ps1/src/symbols.ld', 'ps1/src/types.fields')
                or p.startswith('ps1/src/') and p.endswith('.h')
                or any(p.startswith('ps1/src/' + f.name + '/') and Path(p).name not in ('README.md',)
                       and not re.fullmatch(r'func_[0-9a-f]{8}(?:_[a-z0-9_]+)?\.(?:c|py)', Path(p).name) for f in folders)
                for p in changed)
    checks = []
    if broad or any(p in owned or p.startswith('ps1/inventory/') for p in changed):
        checks.append(Check('matching', (sys.executable, 'ps1/tools/matchbuild.py', '--config', 'ps1/src/build.toml', '--tag', 'ai-review'), 'ps1'))
    if any(p.startswith('ps1/tools/') and not p.endswith('.md') for p in changed):
        for script in sorted((root / 'ps1/tools').glob('test_*.py')):
            checks.append(Check(script.stem, (sys.executable, script.relative_to(root).as_posix()), 'ps1'))
    for folder in folders:
        names = sorted(p.stem for p in folder.glob('func_*.c'))
        selected = names if broad else [n for n in names if any('ps1/src/' + folder.name + '/' + n + ext in changed for ext in ('.c', '.py'))]
        if not selected:
            continue
        rel = folder.relative_to(root).as_posix()
        base = (sys.executable, 'ps1/src/slot06_nonmatching/difftest.py', '--config', 'ps1/src/build.toml', '--folder', rel, '--cases', str(cases))
        for seed in seeds:
            checks.append(Check(folder.name + f'-diff-{seed}', base + ('--seed', str(seed), *selected), 'ps1', tuple(selected), 'differential'))
            checks.append(Check(folder.name + f'-writes-{seed}', base + ('--writes', '--seed', str(seed), *selected), 'ps1', tuple(selected), 'writes'))
        checks.append(Check(folder.name + '-controls', base + ('--control', *selected), 'ps1', tuple(selected), 'control'))
    # The port's overrides in C are tested like nonmatching C: by the differential test of each one's contract.
    overrides = root / 'port/overrides'
    if overrides.is_dir():
        names = sorted(p.stem for p in overrides.glob('*.c'))
        selected = names if broad else [n for n in names if any('port/overrides/' + n + ext in changed for ext in ('.c', '.py'))]
        if selected:
            base = (sys.executable, 'ps1/src/slot06_nonmatching/difftest.py', '--config', 'ps1/src/build.toml', '--folder', 'port/overrides', '--cases', str(cases))
            # Their scope is their own: the files a result depends on are those of `ps1` and the overrides themselves.
            for seed in seeds:
                checks.append(Check(f'port-overrides-diff-{seed}', base + ('--seed', str(seed), *selected), 'overrides', tuple(selected), 'differential'))
                checks.append(Check(f'port-overrides-writes-{seed}', base + ('--writes', '--seed', str(seed), *selected), 'overrides', tuple(selected), 'writes'))
            checks.append(Check('port-overrides-controls', base + ('--control', *selected), 'overrides', tuple(selected), 'control'))
    if any(p.startswith('ps1/src/slot06_nonmatching/') and Path(p).name in ('difftest.py', 'contracts.py', 'test_difftest.py', 'test_difftest_build.py') for p in changed):
        # Harness changes exercise every contract folder, not just changed stems.
        if not broad:
            return select_checks(root, changed + ['ps1/tools/'], cases, seeds, cc, psyz)
        checks.extend([Check('difftest-tools', (sys.executable, 'ps1/src/slot06_nonmatching/test_difftest.py'), 'ps1'),
                       Check('difftest-build-tools', (sys.executable, 'ps1/src/slot06_nonmatching/test_difftest_build.py', '--config', 'ps1/src/build.toml'), 'ps1')])
    if any(p.startswith('port/') and not p.endswith('.md') for p in changed):
        for script in sorted((root / 'port/tools').glob('test_*.py')):
            argv = (sys.executable, script.relative_to(root).as_posix())
            if script.name in ('test_hostlaunch.py', 'test_hostmodules.py', 'test_hostgpu.py', 'test_hostmirror.py', 'test_hostpads.py', 'test_hostbuild.py'):
                if not cc:
                    raise Problem('port runtime controls require --cc, not a silent skip')
                argv += ('--cc', cc)
            if script.name in ('test_hostgpu.py', 'test_hostpads.py'):
                if not psyz:
                    raise Problem('graphics and pad controls require --psyz-build, not a silent skip')
                argv += ('--psyz-build', psyz)
            checks.append(Check(script.stem, argv, 'port'))
        if not checks:
            raise Problem('port code has no discovered controls')
    game_changed = broad or any(p in owned or any(p.startswith('ps1/src/' + f.name + '/') and p.endswith('.c') for f in folders) for p in changed)
    if game_changed or any(p.startswith('port/') and not p.endswith('.md') for p in changed):
        if not cc:
            raise Problem('native integration requires explicit --cc (32-bit compiler), not a silent skip')
        checks.append(Check('native-build', (sys.executable, 'port/tools/hostbuild.py', '--config', 'ps1/src/build.toml', '--cc', cc, '--build', 'local/ai-native-build'), 'port'))
    if any(p.startswith('tools/ai_workflow/') for p in changed):
        checks.append(Check('workflow-controls', (sys.executable, '-m', 'unittest', 'discover', '-s', 'tools/ai_workflow/tests', '-v'), 'workflow'))
    understood = lambda p: p.startswith(('docs/', 'ps1/docs/')) or p.endswith('.md') or p in owned or p.startswith(('ps1/tools/', 'ps1/inventory/', 'port/', 'tools/ai_workflow/')) or p in ('ps1/src/build.toml', 'ps1/src/symbols.ld', 'ps1/src/types.fields') or p.startswith('ps1/src/') and (p.endswith('.h') or any(p.startswith('ps1/src/' + f.name + '/') for f in folders))
    unknown = [p for p in changed if not understood(p)]
    if unknown:
        raise Problem('unmapped changes require explicit review scope: ' + ', '.join(unknown))
    # Removal/rename must not silently disappear from the new source list.
    absent = [p for p in changed if p.endswith(('.c', '.py', '.s')) and not (root / p).exists()]
    if absent and not any(c.name == 'matching' for c in checks):
        raise Problem('removed code requires an explicit matching/ownership review')
    return checks


def check_output(check: Check, output: str) -> None:
    if check.mode == 'exit':
        if check.name == 'workflow-controls' and not re.search(r'Ran [1-9][0-9]* tests', output):
            raise Problem('workflow test discovery ran no tests')
        if check.name.startswith('test_'):
            bespoke = 'all cases behaved as required' in output and re.search(r'^ok\s+', output, re.M)
            standard = re.search(r'Ran ([1-9][0-9]*) tests?', output)
            finished = re.search(r'^OK(?: \(skipped=([0-9]+)\))?$', output, re.M)
            tested = standard and finished and int(finished[1] or 0) < int(standard[1])
            if not (bespoke or tested):
                raise Problem('control suite lacks executed cases and a completion marker')
        return
    if '--cases' not in check.argv:
        raise Problem('function check has no requested case count')
    cases = int(check.argv[check.argv.index('--cases') + 1])
    if not check.functions or cases < 1:
        raise Problem('empty function check')
    for name in check.functions:
        if check.mode == 'differential':
            pattern = rf'^{re.escape(name)}: built .*; cases ({cases}), discarded 0, equal ({cases}), different 0$'
        elif check.mode == 'writes':
            pattern = rf'^{re.escape(name)} writes: cases {cases}, discarded 0, outside 0 \(largest 0 bytes\)$'
        elif check.mode == 'control':
            pattern = rf'^{re.escape(name)} control: different ([1-9][0-9]*) of {cases} \(expected more than 0\)$'
        else:
            raise Problem('unknown check mode')
        matches = list(re.finditer(pattern, output, re.M))
        if len(matches) != 1:
            raise Problem('missing/duplicate/incomplete evidence for ' + name)
        if check.mode == 'control' and int(matches[0][1]) > cases:
            raise Problem('control count exceeds requested cases')


def hash_tree(path: Path) -> list[tuple[str, str]]:
    if path.is_file():
        return [(str(path.resolve()), digest(path))]
    if not path.is_dir():
        raise Problem('dependency is missing')
    return [(str(p.resolve()), digest(p)) for p in sorted(path.rglob('*')) if p.is_file() and
            not any(part in ('.git', '__pycache__') for part in p.relative_to(path).parts)]


def library_closure(path: Path, optional: bool = False) -> list[tuple[str, str]]:
    if sys.platform != 'linux':
        raise Problem('result cache environment closure currently supported on Linux only')
    result = subprocess.run(['ldd', str(path)], text=True, capture_output=True, timeout=20)
    output = result.stdout + result.stderr
    missing = re.findall(r'^\s*(\S+) => not found$', output, re.M)
    if missing and not optional:
        raise Problem('required library dependency missing: ' + path.name)
    if result.returncode and not any(t in output for t in ('not a dynamic executable', 'statically linked')):
        raise Problem('tool library closure unavailable')
    libs = [(f'missing:{name}', 'unavailable') for name in sorted(missing)]
    for library in sorted(set(re.findall(r'(/[^\s()]+)', output))):
        candidate = Path(library)
        if not candidate.is_file():
            raise Problem('tool library dependency unavailable')
        libs.append((str(candidate.resolve()), digest(candidate)))
    return libs


def tool_identity(command: str) -> dict:
    path = Path(shutil.which(command) or command).resolve()
    if not path.is_file():
        raise Problem('tool missing: ' + command)
    version = subprocess.run([str(path), '--version'], text=True, capture_output=True, timeout=20)
    if version.returncode:
        raise Problem('tool identity unavailable: ' + command)
    return {'file': str(path), 'sha256': digest(path), 'version': version.stdout + version.stderr, 'libraries': library_closure(path)}


def source_files(root: Path, scope: str) -> dict[str, str]:
    entries = subprocess.check_output(['git', 'ls-files', '-s', '-z'], cwd=root).decode().split('\0')
    # What a result of each scope depends on. `overrides`: the differential test of an override of the port reads the
    # game's tree and the override's own two files; a result keyed without `port/overrides/` would be reused after an
    # override changed.
    prefixes = {'ps1': ('ps1/', 'requirements.txt'),
                'overrides': ('ps1/', 'port/overrides/', 'requirements.txt')}.get(scope, ('ps1/', 'port/', 'requirements.txt'))
    result = {}
    for entry in entries:
        if not entry:
            continue
        info, name = entry.split('\t', 1)
        if scope != 'workflow' and not name.startswith(prefixes):
            continue
        mode, oid, stage = info.split()
        if stage != '0':
            raise Problem('unmerged dependency')
        path = root / name
        if mode == '160000':
            # A Git link is a pinned dependency, not a missing regular file.
            contents = hash_tree(path) if path.exists() else []
            result[name] = 'gitlink:' + oid + ':' + key_of({'contents': contents})
        elif mode == '120000' or not path.is_file():
            raise Problem('tracked dependency is missing or a link: ' + name)
        else:
            result[name] = digest(path)
    return dict(sorted(result.items()))


def fingerprint(root: Path, check: Check) -> dict:
    """Conservative scope: over-invalidation is preferable to false reuse."""
    if check.scope == 'port':
        raise Problem('native/PsyZ subprocess and platform closure is not resolved; run fresh')
    if any(os.environ.get(k) for k in ('LD_PRELOAD', 'LD_LIBRARY_PATH', 'PYTHONPATH')):
        raise Problem('external loader/import overrides disable result reuse')
    files = list(source_files(root, check.scope).items())
    implementation = hash_tree(Path(__file__).parent)
    tools = [tool_identity(sys.executable), tool_identity('git')]
    if check.scope in GAME_SCOPES:
        files += [(path, sha) for p in config_inputs(root) for path, sha in hash_tree(p)]
        cfg = tomllib.loads((root / 'ps1/src/build.toml').read_text())['toolchain']
        for command in (cfg['cpp'], *[cfg['binutils_prefix'] + suffix for suffix in ('as', 'ld', 'objcopy', 'nm')]):
            tools.append(tool_identity(command))
        files += library_closure(root / 'ps1/src' / cfg['cc1']['path'])
        wrapper = Path(os.path.expanduser(cfg['maspsx']))
        files += hash_tree(wrapper.parent)
        wrapper_head = subprocess.check_output(['git', '-C', str(wrapper.parent), 'rev-parse', 'HEAD'], text=True).strip()
        wrapper_dirty = subprocess.check_output(['git', '-C', str(wrapper.parent), 'status', '--porcelain', '--untracked-files=all'], text=True)
        tools.append({'maspsx_head': wrapper_head, 'maspsx_worktree_status': wrapper_dirty})
        pinned = subprocess.check_output(['git', '-C', str(wrapper.parent), 'archive', cfg['maspsx_commit']])
        tools.append({'maspsx_pinned_export_sha256': hashlib.sha256(pinned).hexdigest()})
        if subprocess.check_output(['git', '-C', str(wrapper.parent), 'replace', '--list'], text=True).strip():
            raise Problem('Git replacement objects leave wrapper closure unresolved')
        for package in ('capstone', 'pyelftools', 'unicorn'):
            dist = importlib.metadata.distribution(package)
            for file in dist.files or ():
                p = Path(dist.locate_file(file))
                if p.is_file() and p.suffix != '.pyc':
                    files.append((str(p.resolve()), digest(p)))
                    if '.so' in p.name:
                        files += library_closure(p)
    # Hash Python's standard library without recursively pulling unrelated packages.
    import sysconfig
    stdlib = Path(sysconfig.get_path('stdlib'))
    for path in sorted(stdlib.rglob('*')):
        if path.is_file() and not any(p in ('site-packages', '__pycache__') for p in path.relative_to(stdlib).parts) and path.suffix in ('.py', '.so', '.zip'):
            files.append((str(path.resolve()), digest(path)))
            if '.so' in path.name:
                files += library_closure(path, optional=True)
    # Python startup can import from .pth/sitecustomize, not only named packages.
    # Reject executable/path-extension .pth files rather than guess their closure.
    import site
    for directory in site.getsitepackages() + [site.getusersitepackages()]:
        directory = Path(directory)
        if not directory.exists():
            continue
        for pth in directory.glob('*.pth'):
            if any(line.strip() and not line.lstrip().startswith('#') for line in pth.read_text().splitlines()):
                raise Problem('Python .pth startup closure unresolved; run fresh')
        files += hash_tree(directory)
        for extension in directory.rglob('*.so*'):
            if extension.is_file():
                files += library_closure(extension, optional=True)
    for prefix in {Path(sys.prefix), Path(sys.base_prefix)}:
        config = prefix / 'pyvenv.cfg'
        if config.exists():
            files.append((str(config.resolve()), digest(config)))
    environment = hashlib.sha256(json.dumps(dict(os.environ), sort_keys=True).encode()).hexdigest()
    return {'schema': 1, 'check': dataclasses.asdict(check), 'files': sorted(set(files)),
            'implementation': implementation, 'tools': tools, 'python': sys.version, 'python_path': sys.path, 'environment_digest': environment}


def key_of(inputs: dict) -> str:
    return hashlib.sha256(json.dumps(inputs, sort_keys=True).encode()).hexdigest()


def cached(entry: Path, key: str, check: Check) -> dict | None:
    try:
        record = json.loads((entry / 'result.json').read_text())
        manifest = entry / 'inputs.json'
        if digest(manifest) != record['inputs_sha256'] or key_of(json.loads(manifest.read_text())) != key:
            return None
        evidence = entry / 'output.log'
        if (record['key'] != key or record['exit_code'] != 0 or record['complete'] is not True
                or record['name'] != check.name or record['argv'] != list(check.argv)
                or record['functions'] != list(check.functions) or record['mode'] != check.mode
                or record['scope'] != check.scope or digest(evidence) != record['evidence_sha256']):
            return None
        check_output(check, evidence.read_text())
        return record
    except (OSError, ValueError, KeyError, TypeError, Problem):
        return None


def run_check(root: Path, check: Check, out: Path, cache: Path, head: str, fresh: bool = False,
              identity=fingerprint, execute=None, timeout: int = 1800) -> dict:
    out.mkdir(parents=True, exist_ok=False)
    reason = None
    errors = (Problem, OSError, subprocess.SubprocessError, importlib.metadata.PackageNotFoundError)
    sources = source_files(root, check.scope)
    try:
        inputs = identity(root, check)
        key = key_of(inputs)
    except errors as exc:
        inputs, key, reason = None, None, str(exc)
    if inputs is not None:
        (out / 'inputs.json').write_text(json.dumps(inputs, sort_keys=True) + '\n')
    input_sha = digest(out / 'inputs.json') if inputs is not None else None
    entry = cache / key if key else None
    old = cached(entry, key, check) if entry and not fresh else None
    if old:
        shutil.copyfile(entry / 'output.log', out / 'output.log')
        try:
            if digest(out / 'output.log') != old['evidence_sha256'] or identity(root, check) != inputs:
                raise Problem('cached evidence or dependencies changed during reuse')
            record = {**old, 'status': 'cached', 'current_head': head, 'origin_head': old['head'], 'source_files': sources, 'inputs_sha256': input_sha}
            (out / 'result.json').write_text(json.dumps(record, indent=2) + '\n')
            return record
        except errors:
            # Fall through to a real run, not a weak-key retry.
            old = None
    started = time.monotonic()
    error = None
    with (out / 'output.log').open('w') as stream:
        try:
            if execute:
                rc = execute(check.argv, root, stream)
            else:
                # Kill the entire group on timeout, not just the command wrapper.
                import signal
                proc = subprocess.Popen(check.argv, cwd=root, stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
                try:
                    rc = proc.wait(timeout=timeout)
                except (subprocess.TimeoutExpired, KeyboardInterrupt):
                    os.killpg(proc.pid, signal.SIGKILL)
                    proc.wait()
                    raise Problem('check timed out or was interrupted')
        except (Problem, OSError, subprocess.SubprocessError) as exc:
            rc, error = 2, str(exc)
    record = {'name': check.name, 'head': head, 'argv': list(check.argv), 'functions': list(check.functions), 'mode': check.mode,
              'scope': check.scope, 'exit_code': rc, 'complete': False, 'status': 'failed', 'source_files': sources,
              'elapsed_seconds': time.monotonic() - started, 'key': key, 'cache_disabled_reason': reason,
              'evidence_sha256': digest(out / 'output.log'), 'inputs_sha256': input_sha}
    if error:
        record['error'] = error
    if rc == 0:
        try:
            check_output(check, (out / 'output.log').read_text())
            if source_files(root, check.scope) != sources or inputs is not None and identity(root, check) != inputs:
                raise Problem('dependencies changed during the check')
            record.update(complete=True, status='fresh_pass')
        except errors as exc:
            record['error'] = str(exc)
    (out / 'result.json').write_text(json.dumps(record, indent=2) + '\n')
    if entry and record['complete']:
        cache.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=cache) as temp:
            stage = Path(temp)
            shutil.copyfile(out / 'output.log', stage / 'output.log')
            shutil.copyfile(out / 'inputs.json', stage / 'inputs.json')
            (stage / 'result.json').write_text(json.dumps(record, indent=2) + '\n')
            if entry.exists():
                # Repair a corrupt entry; concurrent writers remain integrity-checked.
                if cached(entry, key, check) is None:
                    shutil.rmtree(entry)
                else:
                    return record
            try:
                os.rename(stage, entry)
            except FileExistsError:
                pass
    return record


def freeze(root: Path, head: str, destination: Path) -> None:
    if destination.exists():
        raise Problem('snapshot already exists; choose a new report directory')
    destination.mkdir(parents=True)
    archive = subprocess.run(['git', 'archive', head], cwd=root, stdout=subprocess.PIPE, check=True).stdout
    import io, tarfile
    with tarfile.open(fileobj=io.BytesIO(archive)) as tar:
        # filter=data is available in supported Python 3.12; explicit path checks
        # keep the Python 3.11 path equally fail-closed.
        for member in tar.getmembers():
            inside(destination, destination / member.name)
            if not (member.isfile() or member.isdir()):
                raise Problem('tracked links need explicit snapshot review')
        tar.extractall(destination, filter='data') if sys.version_info >= (3, 12) else tar.extractall(destination)
    # Keep the real commit and tracked index, without sharing mutable objects.
    subprocess.run(['git', 'init', '-q', str(destination)], check=True)
    subprocess.run(['git', '-c', 'protocol.file.allow=always', 'fetch', '-q', '--depth=1', str(root.resolve()), head], cwd=destination, check=True)
    subprocess.run(['git', 'update-ref', 'refs/heads/frozen', head], cwd=destination, check=True)
    subprocess.run(['git', 'symbolic-ref', 'HEAD', 'refs/heads/frozen'], cwd=destination, check=True)
    subprocess.run(['git', 'reset', '-q'], cwd=destination, check=True)
    result = subprocess.run(['git', 'diff', '--quiet', '--no-ext-diff'], cwd=destination)
    if result.returncode:
        raise Problem('archive differs from the requested commit (attributes or missing files)')
