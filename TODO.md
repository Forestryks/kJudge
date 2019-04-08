 - Rewrite testing framefork
 - Do something with tile limit detection (possible remove due to obsolescence of this method)

# Tests
## Generic tests
 - Always passes
 - prctl(KJ_ISOLATE) test

## Security violation tests
 - Second exec forbiddance in safemode
 - Firbiddance of path with ".." in it
 - kj_secure_path fail with allowed path if kjduge-control disabled
 - kj_secure_path with allowed path
 - kj_secure_path with allowed and later forbidded path
 - kj_user_secure_path_at with wrong dfd != AT_FDCWD
 - Syscalls (?)
 - Inability to enter safemode without kjudge-control module
 - Some tests on kjudge-control module
 - Syscalls tests (fork, forbid, forbid_path, ...)

## Memory limit tests
 - Memory limit on brk()
 - No memory limit on brk()
 - Memory limit on mmap()
 - No memory limit on mmap()
 - Empty program usage
 - Stack usage
 - Memory limit on stack overflow
 - Memory limit on exec
 - No memory limit on exec


## Time limit tests
 - setrlimit(RLIMIT_MCPU)
 - Time limit on excess
 - Empty program usage
 - Precision tests (?)
