#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

uint32_t hash(const char *s);

int main(void)
{
  char     buf[1024];
  uint64_t lbuf;

  printf("Enter password: ");
  scanf("%s", buf);
  lbuf = hash(buf);
  if (lbuf == 0x5cb9e)
  {
    printf("well done!\n");

    return EXIT_SUCCESS;
  }
  else
  {
    printf("You didn't say the magic word!\n");
    sleep(1);

    return EXIT_FAILURE;
  }
}
