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
 * empty_program_usage.test.c
 *
 * Determines how much time empty program consumes
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
	exit(0);
}

void parent(pid_t pid) {
	int status;
	long time_usec;
	struct rusage usage;

	ASSERT(wait(&status) == pid);

	W_CLEARBITS(status);
	ASSERT(WIFEXITED(status) && WEXITSTATUS(status) == 0);

	ASSERT(getrusage(RUSAGE_CHILDREN, &usage) == 0);

	time_usec = (usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) * 1000000 +
		      (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec);

    LOG("Time usage: %9ldns", time_usec);

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