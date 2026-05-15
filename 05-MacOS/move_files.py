import os
import shutil
from pathlib import Path

TARGET_NAMES = {
    "Window.app",
    "build.log",
    "build.sh",
    "Window.m",
    "OGL.app",
    "Log.txt",
    "OGL.m",
    "OGL.mm",
    "vmath.h",
    "Stone.bmp",
    "Kundali.bmp",
    "Kundali.png",
    "Smiley.bmp",
    "SmileyTexture.png",
    "Sphere.h",
    "Sphere.lib",
}

def move_target_files_to_code(root_path):
    root = Path(root_path)
    if not root.exists() or not root.is_dir():
        raise ValueError(f"Path not found or not directory: {root_path}")

    moved = []
    for dirpath, dirnames, filenames in os.walk(root):
        dirp = Path(dirpath)
        want = set(filenames) & TARGET_NAMES
        if not want:
            continue

        code_dir = dirp / "Code"
        code_dir.mkdir(exist_ok=True)

        for name in sorted(want):
            src = dirp / name
            dst = code_dir / name
            if dst.exists():
                print(f"Skip: target already exists in Code: {dst}")
                continue

            try:
                # For directories like Window.app/OGL.app, use move
                shutil.move(str(src), str(dst))
                moved.append((src, dst))
                print(f"Moved: {src} -> {dst}")
            except Exception as e:
                print(f"ERROR moving {src} -> {dst}: {e}")

    return moved


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(
        description="Move matching build/source files into a Code folder next to each file."
    )
    parser.add_argument("root", help="Root path to scan")
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show what would be moved without making changes",
    )
    args = parser.parse_args()

    if args.dry_run:
        print("Dry run mode: no modifications performed.")
        for dirpath, dirnames, filenames in os.walk(args.root):
            want = set(filenames) & TARGET_NAMES
            if want:
                print(f"[DRY] {dirpath}: {sorted(want)}")
    else:
        move_target_files_to_code(args.root)