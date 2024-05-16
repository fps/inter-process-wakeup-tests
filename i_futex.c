#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <sched.h>

// Adapted from https://eli.thegreenplace.net/2018/basics-of-futexes/

inline int futex(int* uaddr, int futex_op, int val, const struct timespec* timeout,
          int* uaddr2, int val3) {
  return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

inline void wait_on_futex_value(int* futex_addr, int val) {
  //struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000000 };
  // while (1) {
    // sched_yield();
    int futex_rc = futex(futex_addr, FUTEX_WAIT, val, NULL, NULL, 0);
    if (futex_rc == -1) {
      if (errno != EAGAIN) {
        perror("futex");
        exit(1);
      }
      // printf(".");
    } else if (futex_rc == 0) {
      if (*futex_addr != val) {
        // This is a real wakeup.
        return;
      }
    } else {
      abort();
    }
  //}
}


int main(int argc, char *argv[])
{
  mlockall(MCL_FUTURE);

  int s = shm_open("/s", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (-1 == s)
  {
    printf("Failed to create shared memory object: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  int ret = ftruncate(s, sizeof(int));
  if (0 != ret)
  printf("ftruncate failed: %s\n", strerror(errno));

  void * volatile m = mmap(0,  sizeof(int) , PROT_READ | PROT_WRITE, MAP_SHARED, s, 0);
  if (MAP_FAILED == m)
  {
    printf("mmap failed\n");
    return EXIT_FAILURE;
  }

  *(int*)m = 0;

  struct timespec ts;
  int value = 0;
  for (int index = 0; index < 10; ++index)
  {
    wait_on_futex_value((int*)m, value);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);
    ++value;
  }
  return EXIT_SUCCESS;
}
