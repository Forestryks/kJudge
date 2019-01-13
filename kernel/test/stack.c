#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <wait.h>
#include <errno.h>

long sum = 0;

int rec(int v) {
	rec(v + 1);
}

void child() {
	rec(0);
}

void parent(pid_t pid) {
	int status;
	pid_t wt = wait(&status);
	if (wt != pid) {
		printf("error: wait() != pid\n");
		exit(-1);
	}

	printf("child exited\n");

	for (int i = 31; i >= 0; --i) {
		printf("%d", (status >> i) & 1);
	}
	printf("\n");
	return;
}

void run() {
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
	run();
	// for (int i = 0; i < 100; ++i) {
	// 	run();
	// }
}
