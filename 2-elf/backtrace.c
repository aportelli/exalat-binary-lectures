#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void backtrace(void)
{
  uint64_t     **rbp;
  unsigned int level = 0;

  __asm__ volatile
  (
    "movq %%rbp, %0;" 
    : "=r"(rbp)
  );

  printf("==== BACKTRACE ====\n");
  // keep walking until rbp is NULL
  // no guarantee this is a safe way to do thing
  while (rbp != NULL)
  {
    // get the return address 8B after base pointer
    printf("frame %2d: %p\n", level, *(rbp + 1));
    // set rbp to be next frame pointer
    rbp = (uint64_t **)(*rbp);
    level++;
  }
  printf("===================\n");
}

int function1(int a, int b)
{
  int c = a + b;

  backtrace();

  return c;
}

double function2(double a)
{
  double b = 3.14*function1(a, a);

  return b;
}

int main(void)
{
  printf("%f\n", function2(2.));

  return EXIT_SUCCESS;
}