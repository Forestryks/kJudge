#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VA_ARGS(...) , ##__VA_ARGS__
#define LOG(s, ...) printf("[%s:%d] " s "\n", __FILE__, __LINE__ VA_ARGS(__VA_ARGS__))
#define FAIL(s, ...) printf("[%s:%d] Epic fail! " s "\n", __FILE__, __LINE__ VA_ARGS(__VA_ARGS__)); exit(-1);
#define ASSERT(x) do { if (!(x)) {FAIL("Assertation \"" #x "\" failed");} } while (0);

#define FILENAME ".mmap_tmp0fsdcsd"
const long SIZE = 55; // 55 MB
char cmd[100];

int main() {
    int fd;
    char *addr;
    struct stat sb;

    sprintf(cmd, "dd if=/dev/urandom of=" FILENAME " bs=1M count=%ld >/dev/null 2>&1", SIZE);
    ASSERT(system(cmd) == 0);

    fd = open(FILENAME, O_RDONLY);
    ASSERT(fd != -1);

    ASSERT(fstat(fd, &sb) != -1);

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    ASSERT(addr != MAP_FAILED);

    munmap(addr, sb.st_size);
    close(fd);

    ASSERT(remove(FILENAME) == 0);
    exit(0);
}
