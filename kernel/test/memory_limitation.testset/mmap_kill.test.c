#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <kjudge.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>

#define VA_ARGS(...)    , ##__VA_ARGS__
#define LOG(s, ...)     printf("[%s:%d] " s "\n", __FILE__, __LINE__ VA_ARGS(__VA_ARGS__))
#define FAIL(s, ...)    printf("[%s:%d] Epic fail! " s "\n", __FILE__, __LINE__ VA_ARGS(__VA_ARGS__)); exit(-1);
#define ASSERT(x)       do { if (!(x)) {FAIL("Assertation \"" #x "\" failed");} } while (0);

#define FILENAME        ".mmap_tmp0fsdcsd"

const long MEMLIMIT_KB = 1024 * 50;     // 50 MB
const long MMAP_FILE_SIZE = 55;         // 55 MB
char cmd[100];

void child() {
    int fd;
    char *addr;
    struct stat sb;

    struct rlimit rlim = {
        .rlim_cur = MEMLIMIT_KB * 1024,
        .rlim_max = MEMLIMIT_KB * 1024
    };

    ASSERT(setrlimit(RLIMIT_AS, &rlim) == 0);
    ASSERT(kj_isolate(IMEMLIMITATION) == 0);
    
    sprintf(cmd, "dd if=/dev/urandom of=" FILENAME " bs=1M count=%ld >/dev/null 2>&1", MMAP_FILE_SIZE);
    ASSERT(system(cmd) == 0);

    fd = open(FILENAME, O_RDONLY);
    ASSERT(fd != -1);

    ASSERT(fstat(fd, &sb) != -1);
    ASSERT(sb.st_size == MMAP_FILE_SIZE * 1024 * 1024);

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr != MAP_FAILED) {
        munmap(addr, sb.st_size);
        close(fd);
        remove(FILENAME);
        ASSERT(addr == MAP_FAILED);
    }

    close(fd);
    ASSERT(remove(FILENAME) == 0);
}

void parent(pid_t pid) {
    int status;
    struct rusage usage;

    ASSERT(wait(&status) == pid);

    printf("status = ");
    for (int i = 31; i >= 0; --i) {
        printf("%d", (status >> i) & 1);
    }
    printf("\n");

    ASSERT(W_WASMEMLIMIT(status));
    W_CLEARBITS(status);
    ASSERT(WIFEXITED(status));

    ASSERT(getrusage(RUSAGE_CHILDREN, &usage) == 0);
    LOG("Memory limit: %ld KB", MEMLIMIT_KB);
    LOG("Memory usage: %ld KB", usage.ru_maxrss);
    ASSERT(usage.ru_maxrss <= MEMLIMIT_KB);
}

int main() {
    pid_t pid = fork();
    ASSERT(pid >= 0);
    
    if (pid == 0) {
        child();
    } else {
        parent(pid);
    }
    exit(0);
}