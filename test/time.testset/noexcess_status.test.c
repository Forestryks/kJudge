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
 * noexcess_status.test.c
 *
 * Checks whether execution is not killed when time not exceed limit and
 * time limit flag is not set in status
 */

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h> /* for abs() */
#include <time.h>

#include <simple_test.h>
#include <kjudge.h>

#define TIME_LIMIT_MSEC (long)(500) /* 500ms == 1s */

void work() {
	const int MX = 10000000;
	int v1 = 0, v2 = 1, i;
	for (i = 0; i < MX; ++i) {
		v1 = v2 + v1;
		v1 ^= v2;
		v2 ^= v1;
		v1 ^= v2;
	}
}

void child() {
	struct rlimit rlim = {
		.rlim_cur = TIME_LIMIT_MSEC,
		.rlim_max = TIME_LIMIT_MSEC
	};

	ASSERT(setrlimit(RLIMIT_MCPU, &rlim) == 0);
	ASSERT(prctl(PR_ISOLATE, PR_TIMELIMITATION, 0, 0, 0) == 0);

	clock_t now, start = clock();
	for (;;) {
		work();
		now = clock();
		if (1.0 * (now - start) / CLOCKS_PER_SEC * 1000 * 2 > TIME_LIMIT_MSEC) break;
	}
	exit(0);
}

void parent(pid_t pid) {
	int status;
	long time_usec;
	struct rusage usage;

	ASSERT(wait(&status) == pid);

	ASSERT(!W_WAS_TIMELIMIT(status));
	W_CLEARBITS(status);
	ASSERT(WIFEXITED(status) && WEXITSTATUS(status) == 0);

	ASSERT(getrusage(RUSAGE_CHILDREN, &usage) == 0);

	time_usec = (usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) * 1000000 +
		      (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec);

    LOG("Time limit: %9ldus", TIME_LIMIT_MSEC * 1000);
    LOG("Time usage: %9ldus", time_usec);

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
