#!/usr/bin/env python3
"""Recursively find and run 'bash build.sh' in directories.

Usage:
    python3 run_builds.py [path] [--dry-run] [--no-log]

By default, logs are appended to 'build.log' in each directory where a build runs.
"""
from pathlib import Path
import subprocess
import argparse
import sys


def find_build_scripts(root: Path):
    return [p for p in root.rglob('build.sh') if p.is_file()]


def run_build(script_path: Path, write_log: bool = True):
    cwd = script_path.parent
    cmd = ['bash', 'build.sh']
    proc = subprocess.run(cmd, cwd=str(cwd), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    out = proc.stdout or ''
    if write_log:
        log_file = cwd / 'build.log'
        with log_file.open('a', encoding='utf-8') as f:
            f.write(f'=== Running in {cwd} ===\n')
            f.write(out)
            f.write(f'\n=== Exit code: {proc.returncode} ===\n\n')
    return proc.returncode, out


def main():
    parser = argparse.ArgumentParser(description="Recursively find and run 'bash build.sh' in directories.")
    parser.add_argument('path', nargs='?', default='.', help='Root path to search (default: current directory)')
    parser.add_argument('--dry-run', action='store_true', help="Show where commands would run but don't execute them")
    parser.add_argument('--no-log', action='store_true', help='Do not write build.log files (only print output)')
    args = parser.parse_args()

    root = Path(args.path).expanduser().resolve()
    if not root.exists():
        print(f'Path not found: {root}', file=sys.stderr)
        sys.exit(2)

    scripts = find_build_scripts(root)
    if not scripts:
        print('No build.sh files found.')
        return

    for s in scripts:
        print(f'Found: {s} -> running in {s.parent}')
        if args.dry_run:
            continue
        code, out = run_build(s, write_log=(not args.no_log))
        if out:
            print(out, end='')
        if code != 0:
            print(f'Command failed with exit code {code} in {s.parent}', file=sys.stderr)


if __name__ == '__main__':
    main()
