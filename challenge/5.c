#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// password: RpaxY5VCmvFnM^$TN9o5
static const char pwdhash[] = 
    "8c9705667fa4ad7d24fb582ac835894a6be1624ed789f7d4bfc0192c8688d4e1";

int check_pwd(const char *password)
{
    int  pwd_good = 0;
    char buf[24];
    char hash[65];
        
    strcpy(buf, password);
    sha256_string(hash, buf);
    if (strcmp(hash, pwdhash) == 0)
    {
        pwd_good = 1;
    }

    return pwd_good;
}

int main(void)
{
  char buf[1024];

  printf("Enter password: ");
  scanf("%s", buf);
  if (check_pwd(buf))
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
