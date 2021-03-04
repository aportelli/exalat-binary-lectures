#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int inc(char *str, const size_t length)
{
  size_t i    = 0;
  int    more = 1;

  while (1)
  {
    if (str[i] == 'Z')
    {
      if (i == length - 1)
      {
        more = 0;

        break;
      }
      else
      {
        str[i] = 'a';
        ++i;
      }
      
      continue;
    }
    else
    {
      if (str[i] == 'z')
      {
        str[i] = 'A';
      }
      else
      {
        str[i]++;
      }

      break;
    }
  }
  
  return more;
}

__attribute__((constructor)) void ctor(void)
{
    // address of hash in crackme.3
    uint64_t address = 0x0000000000401150;

    unsigned int (*hash)(char *) = (unsigned int (*)(char *))(address);

    const size_t maxLength = 16;
    char         pwd[maxLength + 1];
    size_t       length, i;
    uint32_t     h = 0x5cb9e;

    printf("Looking for hash 0x%x\n", h);
    for (length = 1; length <= maxLength; ++length)
    {
        printf("Trying length %zu passwords...\n", length);
        for (i = 0; i < length; ++i)
        {
            pwd[i] = 'a';
        }
        pwd[length] = '\0';
        do
        {
            uint32_t ph = hash(pwd);
            if (ph == h)
            {
                printf("found: %s\n", pwd);

                break;
            }
        } while (inc(pwd, length));
    }
    printf("nothing found...\n");
}
