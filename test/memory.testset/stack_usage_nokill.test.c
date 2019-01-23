/* 
 * Copyright (C) 2019 Andrei Odintsov <forestryks1@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * stack_usage_nokill.test.c
 *
 * Check whether stack is taken into account when calculating vm size
 */

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#include <simple_test.h>
#include <kjudge.h>

#define MEMLIMIT_KB     (long)(500 * 1024) // 500 MB
#define MIN_MEMUSAGE_KB (long)(15 * 1024)  // 15 MB
#define REC_DEPTH       (long)(MIN_MEMUSAGE_KB * 2 * 1024 / 2 / sizeof(long))

void rec(long x) {
    if (x == REC_DEPTH) return;
    rec(x + 1);
}

void child() {
    struct rlimit rlim = {
        .rlim_cur = MEMLIMIT_KB * 1024,
        .rlim_max = MEMLIMIT_KB * 1024
    };

    ASSERT(setrlimit(RLIMIT_AS, &rlim) == 0);
    if (setrlimit(RLIMIT_STACK, &rlim) != 0) {
        if (errno == EPERM) {
            FAIL("This test must be run as root");
        } else {
            FAIL("setrlimit(RLIMIT_STACK, &rlim) failed");
        }
    }
    ASSERT(prctl(PR_ISOLATE, PR_MEMLIMITATION, 0, 0, 0) == 0);

    rec(0);
    exit(0);
}

void parent(pid_t pid) {
    int status;
    struct rusage usage;

    ASSERT(wait(&status) == pid);
    ASSERT(!W_WASMEMLIMIT_STACK(status));
    W_CLEARBITS(status);
    ASSERT(WIFEXITED(status) && WEXITSTATUS(status) == 0);

    ASSERT(getrusage(RUSAGE_CHILDREN, &usage) == 0);
    LOG("Memory usage: %ld", usage.ru_maxvm);
    ASSERT(usage.ru_maxvm >= MIN_MEMUSAGE_KB);

    EXIT_SUCC();
}

int main() {
    pid_t pid = fork();
    ASSERT(pid >= 0);
    if (pid == 0) {
        child();
    } else {
        parent(pid);
    }
}
