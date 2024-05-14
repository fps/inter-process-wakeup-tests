#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "shm_sync.h"

int main(int argc, char *argv[])
{
  int s = shm_open("/s", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (-1 == s) 
  {
    printf("Failed to create shared memory object: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }
  
  int ret = ftruncate(s, sizeof(struct shm_sync));
  if (0 != ret)
  printf("ftruncate failed: %s\n", strerror(errno));

  void *m = mmap(0,  sizeof(struct shm_sync) , PROT_READ | PROT_WRITE, MAP_SHARED, s, 0);
  if (MAP_FAILED == m)
  {
    printf("mmap failed\n");
    return EXIT_FAILURE;
  }

  pthread_condattr_t cond_attr;
  pthread_condattr_init(&cond_attr);
  pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
  pthread_cond_init(&(((struct shm_sync*)m)->condition), &cond_attr);

  pthread_mutexattr_t mutex_attr;
  pthread_mutexattr_init(&mutex_attr);
  pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&(((struct shm_sync*)m)->mutex), &mutex_attr);

  struct timespec ts;
  
  while(1)
  {
    ret = pthread_mutex_lock(&(((struct shm_sync*)m)->mutex));
    if (0 != ret)
    {
      printf("Failed to lock mutex: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    ret = pthread_cond_wait(&(((struct shm_sync*)m)->condition), &(((struct shm_sync*)m)->mutex));
    if (0 != ret)
    {
      printf("Failed to wait on condition: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("secs: %ld, nsecs: %ld\n", ts.tv_sec, ts.tv_nsec);

    ret = pthread_mutex_unlock(&(((struct shm_sync*)m)->mutex));
    if (0 != ret)
    {
      printf("Failed to unlock mutex: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
  }
}
