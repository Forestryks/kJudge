#!/usr/bin/python3
import re
import os
import sys
import json
import hashlib
import datetime
from collections import OrderedDict


VERSION = "4.19.16"
kernel_dir = "linux-{0}-kjudge".format(VERSION)


class Colors:
    @staticmethod
    def warn(text):
        return "\x1b[3;33m" + text + "\x1b[0m"

    @staticmethod
    def fail(text):
        return "\x1b[1;31m" + text + "\x1b[0m"

    @staticmethod
    def good(text):
        return "\x1b[1;32m" + text + "\x1b[0m"

    @staticmethod
    def italic(text):
        return "\x1b[3m" + text + "\x1b[0m"


def find_block(text, pos, parentheses):
    opened = False
    balance = 0

    begin = pos
    for i in range(pos, len(text)):
        if text[i] == parentheses[0]:
            balance += 1
            if not opened:
                opened = True
                begin = i
        elif text[i] == parentheses[1]:
            balance -= 1
        if opened and balance == 0:
            return begin, i
        if balance < 0:
            raise ValueError
    raise ValueError


def process_syscall(text, database, path, pos):
    args_begin, args_end = find_block(text, pos, "()")
    code_begin, code_end = find_block(text, pos, "{}")

    args = list(map(lambda x: x.strip(), text[args_begin + 1:args_end].split(',')))
    name = args[0]
    signature = text[pos:args_begin] + " ".join(text[args_begin:args_end + 1].split())
    code_hash = hashlib.md5(text[code_begin:code_end + 1].encode()).hexdigest()

    if "arch" in path and "x86" not in path:
        print(Colors.italic("Skipping syscall {0}:{1}".format(path, name)), file=sys.stderr)
        return

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


def main():
    os.chdir(kernel_dir)
    database = {
        "kernel_version": VERSION,
        "creation_time": str(datetime.datetime.now()),
        "syscalls": OrderedDict()
    }

    for subdir, dirs, files in os.walk("."):
        subdir = os.path.normpath(subdir)
        for file in files:
            path = os.path.join(subdir, file)
            with open(path) as input_file:
                try:
                    text = input_file.read()
                except UnicodeDecodeError:
                    print(Colors.warn("Reading unicode characters from {0} failed".format(path)), file=sys.stderr)
                    continue
                except Exception as exception:
                    print(Colors.fail("Read from {0} failed with unknown error".format(path)), file=sys.stderr)
                    raise exception

            pattern = "^(COMPAT_)?SYSCALL_DEFINE\\d\\("
            entries = [m.start() for m in re.finditer(pattern, text, re.MULTILINE)]
            if len(entries) == 0:
                continue
            for entry in entries:
                process_syscall(text, database, path, entry)

    json_output = json.dumps(database, indent=4)
    print(json_output)
    print(Colors.good("Successfully parsed {0} syscalls".format(str(len(database["syscalls"])))), file=sys.stderr)


if __name__ == "__main__":
    main()
