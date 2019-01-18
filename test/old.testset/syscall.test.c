#include <unistd.h>

int main() {
	long ret = syscall(333, 0);
	if (ret == 0) return 0;
	return -1;
}