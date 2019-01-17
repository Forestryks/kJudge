#!/usr/bin/python3

# Copyright (C) 2019 Andrei Odintsov <forestryks1@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

import os
import subprocess
from sys import stderr, argv


# ext : (compilation, run, cleanup)
extList = {
    'c': ('cc {0}.c -o {0}', './{0}', '{0}'),
    'cpp': ('g++ {0}.cpp -o {0}', './{0}', '{0}'),
    'py': (None, 'python3 {0}.py', None)
}


class tcol:
    FAIL = '\033[31;1m'
    OK = '\033[92m'
    WARN = '\033[33;1m'
    END = '\033[0m'

    @staticmethod
    def col(s, c):
        return c + s + tcol.END


def get_files(path, prefix):
    files = []
    for file in os.listdir(path):
        if os.path.isfile(path + '/' + file):
            sp = file.split('.')
            if len(sp) >= 3 and sp[-2] == 'test' and sp[-1] in extList:
                files.append(prefix + file)
        elif os.path.isdir(path + '/' + file):
            sp = file.split('.')
            if len(sp) >= 2 and sp[-1] == 'testset':
                files += get_files(path + '/' + file, prefix + file + '/')
    return files


def build_all(files):
    print('Building tests...')
    for file in files:
        pos = file.rfind('.')
        ext = file[pos + 1:]
        if extList[ext][0] is None:
            continue
        cmd = extList[ext][0].format(file[:pos])
        print(cmd)
        try:
            if os.system(cmd) != 0:
                raise Exception
        except:
            print(tcol.FAIL + 'Compilation failed ({0})'.format(file) + tcol.END)
            exit(-1)

    print('Build finished successfully')


class progress:
    LEN = 12
    cnt = -1

    def next(self):
        self.cnt += 1
        LEN = self.LEN
        cnt = self.cnt
        r = ' ' * LEN
        cnt %= (LEN - 3) * 2
        if cnt < LEN - 3:
            r = r[:cnt] + '.' * 3 + r[cnt + 3:]
        else:
            cnt -= LEN - 3
            r = r[:cnt] + '.' * 3 + r[cnt + 3:]
            r = r[::-1]
        r = '[' + r + ']'
        return r + ' ' + self.file

    def verdict(self, s):
        LEN = self.LEN
        pos = (LEN - len(s) + 1) // 2
        r = '[' + ' ' * pos + s + ' ' * (LEN - pos - len(s)) + ']'
        return r

    def passed(self):
        return tcol.col(self.verdict('PASSED'), tcol.OK) + ' ' + self.file

    def failed(self):
        return tcol.col(self.verdict('FAILED'), tcol.FAIL) + ' ' + self.file

    def __init__(self, file):
        self.file = file


def run_all(files):
    print('Running tests...')
    print('=' * 70)

    all = len(files)
    passed = 0
    failed = 0
    # skipped = 0

    log = open('.log', 'w')

    for file in files:
        fname = file.replace('.testset/', '::')
        bar = progress(fname)
        print(bar.next(), end='')

        log.write('=' * 70 + '\n')
        log.write('Running ' + fname + '\n')
        log.write('=' * 70 + '\n')
        try:
            pos = file.rfind('.')
            ext = file[pos + 1:]
            if extList[ext][1] is None:
                continue
            cmd = extList[ext][1].format(file[:pos])
            proc = subprocess.Popen(cmd.split(), stdout=log)
            while True:
                try:
                    res = proc.wait(timeout=0.1)
                    if res == 0:
                        print('\r' + bar.passed())
                        passed += 1
                    else:
                        print('\r' + bar.failed())
                        failed += 1
                    break
                except:
                    print('\r' + bar.next(), end='')
        except:
            print('\r' + bar.failed())
            failed += 1
        log.write('=' * 70 + '\n\n\n')
        del bar
    log.close()

    print('=' * 70)
    print('Summary:')
    print(tcol.col('Passed: {0}'.format(passed), tcol.OK))
    print(tcol.col('Failed: {0}'.format(failed), tcol.FAIL))


def clean_all(files):
    for file in files:
        pos = file.rfind('.')
        ext = file[pos + 1:]
        if extList[ext][2] is None:
            continue
        cmd = extList[ext][2].format(file[:pos])
        try:
            os.remove(cmd)
        except:
            print(tcol.WARN + 'Failed to remove {0}'.format(cmd) + tcol.END, file=stderr)


def run_tests():
    files = get_files(os.getcwd(), '')
    files.sort()

    build_all(files)
    run_all(files)
    clean_all(files)


if __name__ == "__main__":
    run_tests()
