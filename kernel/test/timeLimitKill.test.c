#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <wait.h>
#include <errno.h>

const long TIME_MS = 700;

void child() {
	const long time_limit_ms = TIME_MS;
	struct rlimit rlim = {
		.rlim_cur = time_limit_ms,
		.rlim_max = time_limit_ms		
	};

	if (setrlimit(16, &rlim) != 0) // RLIMIT_MCPU
		exit(-1);

	clock_t now, start = clock();
	while (1) {
		now = clock();
		if (1.0 * (now - start) / CLOCKS_PER_SEC > 2) break;
	}

	exit(-1);
}

void parent(pid_t pid) {
	int status;
	if (wait(&status) != pid)
		exit(-1);

	status &= 0xffff;
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) exit(-1);

	struct rusage usage;
	if (getrusage(RUSAGE_CHILDREN, &usage) != 0)
		exit(-1);

	struct timeval u = usage.ru_utime;
	struct timeval s = usage.ru_stime;

	long t = (u.tv_sec + s.tv_sec) * 1000000 + (u.tv_usec + s.tv_usec);

	if (abs(t - TIME_MS * 1000) > 10000) exit(-1);
}

int main() {
	pid_t pid = fork();
	if (pid < 0) {
		return -1;
	} else if (pid == 0) {
		child();
	} else {
		parent(pid);
	}
	return 0;
}