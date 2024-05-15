#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#include <linux/futex.h>
#include <sys/syscall.h>


int futex(int* uaddr, int futex_op, int val, const struct timespec* timeout,
          int* uaddr2, int val3) {
  return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

void wake_futex_blocking(int* futex_addr) {
  while (1) {
    int futex_rc = futex(futex_addr, FUTEX_WAKE, 1, NULL, NULL, 0);
    if (futex_rc == -1) {
      perror("futex wake");
      exit(1);
    } else if (futex_rc > 0) {
      return;
    }
  }
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

  int ret = ftruncate(s, sizeof(sem_t));
  if (0 != ret)
  printf("ftruncate failed: %s\n", strerror(errno));

  void *m = mmap(0,  sizeof(int) , PROT_READ | PROT_WRITE, MAP_SHARED, s, 0);
  if (MAP_FAILED == m)
  {
    printf("mmap failed\n");
    return EXIT_FAILURE;
  }

  int value = 0;
  struct timespec ts;
  while (1)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ++value;
    *((int*)m) = value;
    wake_futex_blocking((int*)m);
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);
    usleep(1000000);
  }
  return EXIT_SUCCESS;
}
