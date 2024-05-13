#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage: io input-file-name output-file-name\n");
    return EXIT_FAILURE;
  }

  FILE *inp = fopen(argv[1], "r");
  FILE *out = fopen(argv[2], "w");

  while (1)
  {
    int value = fgetc(inp);
    if (EOF == value)
    {
      break;
    }
    fputc(value, out);
  }
  return EXIT_SUCCESS;
}
