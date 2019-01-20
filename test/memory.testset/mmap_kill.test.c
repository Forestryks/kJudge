/* Copyright (C) 2019 Andrei Odintsov <forestryks1@gmail.com>
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

/* mmap_kill.test.c
 *
 * Check whether creating lagre memory mapping via mmap() fails with
 * memory limit error.
 */

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#include <simple_test.h>
#include <kjudge.h>

#define MEMLIMIT_KB  (long)(50 * 1024) /* 50 MB */
#define MMAP_SIZE_KB (long)(60 * 1024) /* 60 MB */

void child() {
    struct rlimit rlim = {
        .rlim_cur = MEMLIMIT_KB * 1024,
        .rlim_max = MEMLIMIT_KB * 1024
    };

    ASSERT(setrlimit(RLIMIT_AS, &rlim) == 0);
    ASSERT(kj_isolate(IMEMLIMITATION) == 0);

    ASSERT(mmap(NULL, MMAP_SIZE_KB * 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0) == MAP_FAILED);
    ASSERT(errno == ENOMEM);

    exit(0);
}

void parent(pid_t pid) {
    int status;
    struct rusage usage;

    ASSERT(wait(&status) == pid);

    ASSERT(W_WASMEMLIMIT(status));
    W_CLEARBITS(status);
    ASSERT(WIFEXITED(status) && WEXITSTATUS(status) == 0);

    ASSERT(getrusage(RUSAGE_CHILDREN, &usage) == 0);
    LOG("Memory usage: %ld", usage.ru_maxvm);
    ASSERT(usage.ru_maxvm != 0);

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
