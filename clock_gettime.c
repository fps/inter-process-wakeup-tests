#include <time.h>

int main(int argc, char *argv[])
{
  struct timespec ts;
  for (int index = 0; index < 1000000; ++index)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts);
  }
}
