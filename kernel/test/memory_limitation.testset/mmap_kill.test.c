#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <wait.h>
#include <errno.h>
#include <kjudge.h>

#define VA_ARGS(...) , ##__VA_ARGS__
#define LOG(s, ...) printf("[%s:%d] " s "\n", __FILE__, __LINE__ VA_ARGS(__VA_ARGS__))
#define FAIL(s, ...) printf("[%s:%d] Epic fail! " s "\n", __FILE__, __LINE__ VA_ARGS(__VA_ARGS__)); exit(-1);
#define ASSERT(x) do { if (!(x)) {FAIL("Assertation \"" #x "\" failed");} } while (0);

#define MEMLIMIT_KB 	(long)1024 * 50
#define SUBPROC_NAME 	"mmap_subproc"

void child() {
	struct rlimit rlim = {
		.rlim_cur = MEMLIMIT_KB * 1024,
		.rlim_max = MEMLIMIT_KB * 1024
	};

	ASSERT(setrlimit(RLIMIT_AS, &rlim) == 0);
	ASSERT(kj_isolate(IMEMLIMITATION) == 0);
	ASSERT(execl("./" SUBPROC_NAME, "./" SUBPROC_NAME, NULL) != -1);
}

void parent(pid_t pid) {
	int status;
	struct rusage usage;

	ASSERT(wait(&status) == pid);
	ASSERT(W_WASMEMLIMIT(status));
	W_CLEARBITS(status);
	ASSERT(WIFEXITED(status));

	ASSERT(getrusage(RUSAGE_CHILDREN, &usage) == 0);
	LOG("Memory limit: %ld KB", MEMLIMIT_KB);
	LOG("Memory usage: %ld KB", usage.ru_maxrss);
	ASSERT(usage.ru_maxrss <= MEMLIMIT_KB);
}

const char *cmd = "cc " SUBPROC_NAME ".c -o " SUBPROC_NAME;

int main() {
	ASSERT(system(cmd) == 0);

	pid_t pid = fork();
	ASSERT(pid >= 0);
	
	if (pid == 0) {
		child();
	} else {
		parent(pid);
	}
	exit(0);
}