#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <wait.h>
#include <errno.h>

long sum = 0;

void child() {
	const long time_limit_ms = 1000;
	struct rlimit rlim = {
		.rlim_cur = time_limit_ms,
		.rlim_max = time_limit_ms		
	};

	if (setrlimit(16, &rlim) < 0) {
		printf("setrlimit error %ld\n", (long)errno);
		exit(0);
	}

	while (1) {};
}

void parent(pid_t pid) {
	int status;
	pid_t wt = wait(&status);
	if (wt != pid) {
		printf("error: wait() != pid\n");
		exit(-1);
	}

//	printf("child exited\n");

	struct rusage usage;
	int err = getrusage(RUSAGE_CHILDREN, &usage);

	if (err != 0) {
		printf("getrusage error %d\n", errno);
		exit(-1);
	}

	struct timeval u = usage.ru_utime;
	struct timeval s = usage.ru_stime;

	long t = (u.tv_sec + s.tv_sec) * 1000000 + (u.tv_usec + s.tv_usec) - sum;
	sum += t;

	printf("%ld microseconds\n", t);
}

void run() {
//	printf("parent start\n");
	pid_t pid = fork();
	if (pid < 0) {
		printf("error: fork() returned %d", pid);
		exit(-1);
	} else if (pid == 0) {
		child();
		exit(0);
	} else {
		parent(pid);
	}
}

int main() {
	for (int i = 0; i < 100; ++i) {
		run();
	}
}
