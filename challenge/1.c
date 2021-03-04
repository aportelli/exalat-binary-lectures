#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char secret[] = "manners.maketh.man";

int main(void)
{
  char buf[1024];

  printf("Enter password: ");
  scanf("%s", buf);
  if (strcmp(secret, buf) == 0)
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
