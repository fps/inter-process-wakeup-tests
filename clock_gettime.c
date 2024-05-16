#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  struct timespec ts1;
  clock_gettime(CLOCK_MONOTONIC, &ts1);

  struct timespec ts2;
  for (int index = 0; index < 1000000; ++index)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts2);
  }
  
  printf("secs: %ld, nsecs: %ld\n", ts1.tv_sec, ts1.tv_nsec);
  printf("secs: %ld, nsecs: %ld\n", ts2.tv_sec, ts2.tv_nsec);
}
