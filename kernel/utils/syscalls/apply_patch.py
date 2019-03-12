#!/usr/bin/python3
import re
import os
import sys
import json
import hashlib
import datetime
from collections import OrderedDict
from color_utils import Colors
from parse_utils import find_block
from process_syscalls import process_syscalls
from sys import argv


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
        print(Colors.italic("Skipping syscall {0}:{1}".format(path, name)), file=sys.stderr)
        return source

    if name not in database["syscalls"]:
        database["syscalls"][name] = {
            "generic": {
                "action": "none"
            },
            "entries": []
        }

    database["syscalls"][name]["entries"].append({
        "path": path,
        "signature": signature,
        "code_hash": code_hash
    })
    return source


def main():
    global database
    if len(argv) != 2:
        print("Usage: python3 find_syscalls.py [kernel_dir]")
        exit(-1)
    kernel_dir = argv[1]
    database = {
        "kernel_version": os.path.basename(kernel_dir),
        "creation_time": str(datetime.datetime.now()),
        "syscalls": OrderedDict()
    }
    process_syscalls(handler, kernel_dir)

    json_output = json.dumps(database, indent=4)
    print(json_output)
    print(Colors.good("Successfully parsed {0} syscalls".format(str(len(database["syscalls"])))), file=sys.stderr)


if __name__ == "__main__":
    main()
