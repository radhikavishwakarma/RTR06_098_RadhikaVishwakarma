import os
from pathlib import Path

def create_code_output_in_leaf_dirs(root_path):
    root = Path(root_path)
    if not root.exists() or not root.is_dir():
        raise ValueError(f"Path not found or not directory: {root_path}")

    for current_dir, subdirs, files in os.walk(root):
        # only create in directories containing build.sh
        if "build.sh" in files:
            code_dir = Path(current_dir) / "Code"
            output_dir = Path(current_dir) / "Output"
            code_dir.mkdir(exist_ok=True)
            output_dir.mkdir(exist_ok=True)
            print(f"Created in: {current_dir}")

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Make Code and Output dirs in leaf dirs.")
    parser.add_argument("root", help="Root path to scan")
    args = parser.parse_args()
    create_code_output_in_leaf_dirs(args.root)