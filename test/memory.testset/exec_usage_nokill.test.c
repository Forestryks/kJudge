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
 * exec_usage_nokill.test.c
 *
 * Test whether data and BSS segment are taken into account when
 * calculating vm size
 */

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

#include <simple_test.h>
#include <kjudge.h>

#define SIZE		(long)(50 * 1024) /* 50 MB */
#define MEMLIMIT_KB (long)(60 * 1024) /* 60 MB */
#define EXEC_TARGET "./exec_target.test"

void child() {
	struct rlimit rlim = {
		.rlim_cur = MEMLIMIT_KB * 1024,
		.rlim_max = MEMLIMIT_KB * 1024
	};

	ASSERT(setrlimit(RLIMIT_AS, &rlim) == 0);
	ASSERT(prctl(PR_ISOLATE, PR_MEMLIMITATION, 0, 0, 0) == 0);

	ASSERT(execl(EXEC_TARGET, EXEC_TARGET, NULL) == 0); /* will not return on success */
}

void parent(pid_t pid) {
	int status;
	struct rusage usage;

	ASSERT(wait(&status) == pid);

	ASSERT(!W_WASMEMLIMIT_EXEC(status));
	W_CLEARBITS(status);
	ASSERT(WIFEXITED(status) && WEXITSTATUS(status) == 0);

	ASSERT(getrusage(RUSAGE_CHILDREN, &usage) == 0);
	LOG("Memory usage: %ld", usage.ru_maxvm);
	ASSERT(usage.ru_maxvm >= SIZE);

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
