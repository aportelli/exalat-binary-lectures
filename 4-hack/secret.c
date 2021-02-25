#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char *argv[])
{
  char buf[1024];
  char s[] = 
{
  0x5c, 0x7a, 0x9, 0xe0, 0x18, 0x2e, 0x2c, 0xac, 
  0xf4, 0xf3, 0x1b, 0xfc, 0x56, 0x2b, 0x8d
};

  strcpy(buf, argv[1]);
  for (unsigned int m = 0; m < sizeof(s); ++m)
  {
    unsigned char c = s[m];
    c ^= 0xea;
    c = -c;
    c = ~c;
    c -= 0x3;
    c = ~c;
    c = -c;
    c -= 0x1f;
    c ^= 0x3e;
    c = ~c;
    c = (c >> 0x6) | (c << 0x2);
    c ^= m;
    c -= m;
    c = -c;
    c ^= 0x40;
    c = (c >> 0x6) | (c << 0x2);
    c += 0x47;
    c = -c;
    c += 0x10;
    c = (c >> 0x6) | (c << 0x2);
    c = ~c;
    c = (c >> 0x7) | (c << 0x1);
    c ^= 0x81;
    c -= m;
    c ^= 0xa8;
    c -= 0xf4;
    c ^= m;
    c -= m;
    c = ~c;
    c += 0x3f;
    c = ~c;
    c -= 0x1c;
    c ^= m;
    c = (c >> 0x1) | (c << 0x7);
    c ^= 0xeb;
    c += 0x56;
    c ^= m;
    c = -c;
    c = (c >> 0x5) | (c << 0x3);
    c = -c;
    c ^= 0x59;
    s[m] = c;
  }
  if (strcmp(s, buf) == 0)
  {
    printf("well done!\n");

    return EXIT_SUCCESS;
  }
  else
  {
    printf("You didn't say the magic word!\n");

    return EXIT_FAILURE;
  }

  printf("%s\n", s);
}