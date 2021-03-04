#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
  char buf[1024];
  long lbuf;

  printf("Enter password: ");
  scanf("%s", buf);
  lbuf = atol(buf);
  if (((lbuf + 16 * 64578) % (17 * 842)) == 0)
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
