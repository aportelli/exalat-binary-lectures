#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

const unsigned char code [] =
  "\xeb\x1b\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x5e\xba\x0c"
  "\x00\x00\x00\x0f\x05\xb8\x3c\x00\x00\x00\x31\xff\x0f\x05\xe8"
  "\xe0\xff\xff\xff\x48\x65\x6c\x6c\x6f\x20\x77\x6f\x72\x6c\x64\x0a";

const unsigned char str [] = "Whisky";

int main(int argc, char **argv)
{
  // It used to be easier to break computers...
  // the commented code will likely end up in SIGSEGV on modern OSs
  // for security data is tagged non-executable
  /////////////////////////////////////////////////////////////////
  // void (*hello)(void);
  //
  // hello = (void(*)(void))code;
  // hello();
  /////////////////////////////////////////////////////////////////

  char *buf;
  int  prot  = PROT_READ | PROT_WRITE | PROT_EXEC;
  int  flags = MAP_PRIVATE | MAP_ANONYMOUS;

  buf = mmap(0, sizeof(code), prot, flags, -1, 0);
  memcpy(buf, code, sizeof(code));

  ((void (*)(void))buf)();

  return EXIT_SUCCESS;
}