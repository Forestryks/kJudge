import os
import re
import sys
from utils import Colors, find_block


# goes through files in current directory
def process_syscalls(handler, dir_prefix="."):
    for subdir, dirs, files in os.walk(dir_prefix):
        reldir = os.path.relpath(subdir, dir_prefix)
        for file in files:
            path = os.path.join(subdir, file)
            relpath = os.path.join(reldir, file)
            if path.endswith(".kjudge"):
                continue
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

            entries = [(entry, find_block(text, entry, "{}")[1] + 1) for entry in entries]
            entries = [entry for entry in entries
                       if len(re.findall(pattern, text[entry[0]:entry[1]], re.MULTILINE)) == 1]

            splitted = []
            for i in range(len(entries)):
                if i == 0:
                    splitted.append(text[:entries[i][0]])
                else:
                    splitted.append(text[entries[i - 1][1]:entries[i][0]])
                splitted.append(handler(text[entries[i][0]:entries[i][1]], relpath))
                if i + 1 == len(entries):
                    splitted.append(text[entries[i][1]:])

            result = "".join(splitted)
            with open(path + ".kjudge", "w") as output_file:
                try:
                    output_file.write(result)
                except Exception as exception:
                    print(Colors.fail("Write to {0} failed with unknown error".format(path)), file=sys.stderr)
                    raise exception
