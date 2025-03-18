import subprocess
import os
import shutil
from typing import Set
import argparse

import subprocess
import json
import os
pwd = os.path.dirname(os.path.abspath(__file__))
#
# 命令行输出不要有中文
#
def get_dll_dependencies(source_path: str, disable_cache: bool = False):

    # 执行objdump
    output = subprocess.check_output(
        ["objdump", "-p", source_path], text=True)
    dep_dll_names = []

    # 解析objdump输出
    for line in output.split('\n'):
        if "DLL Name" in line:
            dep_dll_name = line.split(":")[1].strip()
            dep_dll_names.append(dep_dll_name)

    return dep_dll_names


def find_and_copy_dlls(dll_name: str, source_dir: str, target_dir: str, copied_dlls: Set[str], is_entry: bool = False) -> None:
    """
    递归地找到并复制dll及其依赖到指定目录。

    :param dll_name: 要复制的dll文件名。
    :param source_dir: dll文件源目录。
    :param target_dir: 目标目录，将dll及其依赖复制到此目录。
    :param copied_dlls: 已复制的dll文件集合，避免重复复制。
    """
    if not is_entry:
        if dll_name in copied_dlls:
            return
        copied_dlls.add(dll_name)

        source_path = os.path.join(source_dir, dll_name)
        target_path = os.path.join(target_dir, dll_name)
        if not os.path.exists(source_path):
            print(f"DLL Not Found:\t {source_path}")
            return

        if os.path.exists(target_path):
            print(f"DLL Exists:\t {dll_name}")
        else:
            shutil.copy(source_path, target_path)
            print(f"DLL Copied:\t {dll_name}")
    else:
        source_path = dll_name
    if not os.path.exists(source_path):
        print(f"DLL Ingored:\t {source_path}")
        return
    try:

        # output = subprocess.check_output(["objdump", "-p", source_path], text=True)
        # for line in output.split('\n'):
        #     if "DLL Name" in line:
        #         dep_dll_name = line.split(":")[1].strip()
        #         find_and_copy_dlls(dep_dll_name, source_dir, target_dir, copied_dlls)
        for dep_dll_name in get_dll_dependencies(source_path, is_entry):
            find_and_copy_dlls(dep_dll_name, source_dir,
                               target_dir, copied_dlls)
    except subprocess.CalledProcessError as e:
        print(f"Error processing {dll_name}: {e}")


def main():
    parser = argparse.ArgumentParser(
        description="Recursively copy DLLs and their dependencies.")
    parser.add_argument("--source_dir", type=str,
                        default="C:/msys2/msys64/mingw64/bin", help="Source directory of DLL files.")
    parser.add_argument("--target_dir", type=str, default="./dlls",
                        help="Target directory to copy DLL files to.")
    parser.add_argument("--initial_dlls", nargs='+', default=[
                        "example.cp310-win_amd64.pyd"], help="Initial list of DLL files to copy.")

    args = parser.parse_args()

    source_dir = args.source_dir
    if not os.path.exists(source_dir):
        print(f"Not found: {source_dir}")
        return
    target_dir = args.target_dir
    initial_dlls = args.initial_dlls

    os.makedirs(target_dir, exist_ok=True)

    for dll in initial_dlls:
        find_and_copy_dlls(dll, source_dir, target_dir, set(), True)


# python script.py --source_dir /path/to/source --target_dir /path/to/target --initial_dlls dll1.dll dll2.dll
# python find_copy_dlls.py --target_dir sample_build --initial_dlls sample_build/libadd.dll
if __name__ == "__main__":
    main()
