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

#include <linux/aio_abi.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>
#include <errno.h>

#include <simple_test.h>
#include <kjudge.h>

void child() {
    aio_context_t dummy;

    ASSERT(prctl(PR_ISOLATE, PR_SAFEMODE, 0, 0, 0) == 0);
    ASSERT(syscall(__NR_io_cancel, dummy, NULL, NULL) < 0);
    ASSERT(errno == EPERM);

    exit(0);
}

void parent(pid_t pid) {
    int status;

    ASSERT(wait(&status) == pid);

    ASSERT(!W_WAS_SECURITY_VIOLATION(status));
    W_CLEARBITS(status);
    ASSERT(WIFEXITED(status) && WEXITSTATUS(status) == 0);

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