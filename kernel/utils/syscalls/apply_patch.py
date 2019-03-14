#!/usr/bin/python3
import os
import json
import hashlib
from utils import Colors, find_block, ask_yes_no
from process_syscalls import process_syscalls
from sys import argv
from actions import actions


database = {}


def handler(source, path):
    global database
    args_begin, args_end = find_block(source, 0, "()")
    code_begin, code_end = find_block(source, 0, "{}")

    args = list(map(lambda x: x.strip(), source[args_begin + 1:args_end].split(',')))
    name = args[0]
    signature = source[0:args_begin] + " ".join(source[args_begin:args_end + 1].split())
    code_hash = hashlib.md5(source[code_begin:code_end + 1].encode()).hexdigest()

    if "arch" in path and "x86" not in path:
        print(Colors.italic("Skipping syscall {0}:{1}".format(path, name)))
        return source

    if name not in database["syscalls"]:
        print(Colors.fail("Syscall {0} not occur in database".format(name)))
        exit(-1)

    item = None
    value = -1
    multiple = False
    for entry in database["syscalls"][name]["entries"]:
        current = -1
        if entry["path"] == path:
            current = 1
            if entry["signature"] == signature:
                current = 2
                if entry["code_hash"] == code_hash:
                    current = 3

            if current == value:
                multiple = True
            elif current > value:
                value = current
                multiple = False
                item = entry

    if multiple:
        print(Colors.fail("Multiple occurrences of {0}:{1} in database".format(path, name)))
        exit(-1)

    if item is None:
        print(Colors.fail("Syscall {0}:{1} not occur in database".format(path, name)))
        exit(-1)

    if signature != item["signature"]:
        print(Colors.warn("Signature mismatch for {0}:{1}".format(path, name)))
    if code_hash != item["code_hash"]:
        print(Colors.warn("Hash mismatch for {0}:{1}".format(path, name)))

    if "action" in item:
        return actions[item["action"]](source, path)
    else:
        return actions[database["syscalls"][name]["generic"]["action"]](source, path)


def apply_patch(dir_prefix):
    for subdir, dirs, files in os.walk(dir_prefix):
        for file in files:
            path = os.path.join(subdir, file)
            ext = ".kjudge"
            if not path.endswith(ext):
                continue
            os.rename(path, path[:-len(ext)])


def main():
    global database
    if len(argv) != 3:
        print("Usage: python3 apply_patch.py [patch] [kernel_dir]")
        exit(-1)
    patch_file = argv[1]
    kernel_dir = argv[2]

    kernel_name = os.path.basename(os.path.abspath(kernel_dir))

    file = open(patch_file)
    database = json.load(file)
    file.close()

    print("Applying kJudge syscalls patch created at {0} for kernel version {1}".format(
        Colors.italic(database["creation_time"]),
        Colors.italic(database["kernel_version"]))
    )

    if kernel_name != database["kernel_version"]:
        if not ask_yes_no("Your kernel version ({0}) differ from patch kernel version ({1}). Continue?".format(
            kernel_name,
            database["kernel_version"]
        ), "no"):
            exit(-1)

    process_syscalls(handler, kernel_dir)
    if ask_yes_no("Syscalls patched successfully! Apply patch to kernel source?"):
        apply_patch(kernel_dir)


if __name__ == "__main__":
    main()
