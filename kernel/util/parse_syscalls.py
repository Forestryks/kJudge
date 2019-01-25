#!/usr/bin/python3
import subprocess
import re

VERSION = "4.19.16"
pattern = re.compile("#define\\s+__NR_([\\w\\d]+)\\s+([\\d]+)")
unistd = "linux-{0}-kjudge/include/uapi/asm-generic/unistd.h".format(VERSION)

syscalls = []
with open(unistd, "r") as fin:
    for line in fin.readlines():
        entry = pattern.search(line)
        if entry is None:
            continue

        syscalls.append((
            entry.group(1),
            entry.group(2)
        ))

for syscall in syscalls:
    proc = subprocess.Popen(
        [
            "grep",
            "-rne",
            "SYSCALL_DEFINE[0-5]({0}".format(syscall[0]),
            "linux-{0}-kjudge/".format(VERSION)
        ],
        stdout=subprocess.PIPE
    )

    files = []
    for line in proc.stdout:
        line = line.decode()
        pos = line.find(":")
        if pos is None:
            continue
        pos = line.find(":", pos + 1)
        if pos is None:
            continue
        files.append(line[:pos])

    proc.wait()

    print("{id}\t{name}\t{files}".format(
        id=syscall[1],
        name=syscall[0],
        files="\t".join(files)
    ))
