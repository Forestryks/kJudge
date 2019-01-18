#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <wait.h>
#include <errno.h>

int main() {
	const long time_limit_ms = 1000;
	struct rlimit rlim = {
		.rlim_cur = time_limit_ms,
		.rlim_max = time_limit_ms		
	};

	if (setrlimit(16, &rlim) < 0) // RLIMIT_MCPU
		return -1;
	return 0;
}