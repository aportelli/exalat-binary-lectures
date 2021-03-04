#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <openssl/sha.h>

void sha256_string(char outputBuffer[65], const char *string)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

static unsigned char pwd[] = 
{
  0x10, 0x77, 0x3f, 0x39, 0xc6, 0x42, 0xc6, 0xde, 
  0x9b, 0xc4, 0x6, 0xa9, 0x3a, 0x4a, 0x47, 0xc6, 
  0xc6, 0x85, 0x9b, 0xa7, 0xb3
};

int main(void)
{
  int isDebug;

  #ifdef __linux__
  isDebug = (ptrace(PTRACE_TRACEME, 0) < 0);
  #elif defined __APPLE__
  {
    int junk;
    int mib[4];
    struct kinfo_proc info;
    size_t size;

    info.kp_proc.p_flag = 0;

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
    assert(junk == 0);

    isDebug = ((info.kp_proc.p_flag & P_TRACED) != 0);
  }
  #endif
  if (isDebug)
  {
    printf("================================\n");
    printf("Nice try ;-) Bye!\n");
    printf("================================\n");
    exit(1);
  }

  char buf[1024], hash[65], pwdhash[65];

  printf("Enter password: ");
  scanf("%s", buf);
  sha256_string(hash, buf);

  for (unsigned int m = 0; m < sizeof(pwd); ++m)
  {
    unsigned char c = pwd[m];
    c = -c;
    c ^= m;
    c += m;
    c = -c;
    c -= 0xfc;
    c = (c >> 0x1) | (c << 0x7);
    c += 0xd5;
    c ^= 0xc7;
    c -= 0x4b;
    c ^= m;
    c = ~c;
    c = -c;
    c -= 0xaa;
    c = ~c;
    c -= m;
    c = -c;
    c ^= 0x8;
    c -= 0x3c;
    c = (c >> 0x1) | (c << 0x7);
    c -= 0xdf;
    c = ~c;
    c = -c;
    c ^= 0xa5;
    c += m;
    c = (c >> 0x6) | (c << 0x2);
    c = -c;
    c ^= 0x7d;
    c -= 0x2b;
    c = (c >> 0x6) | (c << 0x2);
    c += 0xfb;
    pwd[m] = c;
  }
  
  /* pwd = "L^ay2PRGnsxwn%akCV05" */
  sha256_string(pwdhash, (char *)pwd);
  if (strcmp(pwdhash, hash) == 0)
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
