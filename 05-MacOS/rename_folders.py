import os
import sys

def rename_folders(root_path):
    """
    Recursively traverse the directory tree starting from root_path
    and rename any folders that contain underscores (_) by replacing them with hyphens (-).
    """
    for dirpath, dirnames, filenames in os.walk(root_path, topdown=False):
        for dirname in dirnames:
            if '_' in dirname:
                old_path = os.path.join(dirpath, dirname)
                new_dirname = dirname.replace('_', '-')
                new_path = os.path.join(dirpath, new_dirname)
                try:
                    os.rename(old_path, new_path)
                    print(f"Renamed: {old_path} -> {new_path}")
                except OSError as e:
                    print(f"Error renaming {old_path}: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python rename_folders.py <folder_path>")
        sys.exit(1)
    
    root_path = sys.argv[1]
    if not os.path.isdir(root_path):
        print(f"Error: {root_path} is not a valid directory")
        sys.exit(1)
    
    rename_folders(root_path)
    print("Folder renaming completed.")