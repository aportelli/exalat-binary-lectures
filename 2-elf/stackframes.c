#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void stackFrameInfo(void)
{
  uint64_t **rbp; // it does not have to be a double pointer
                  // it is just convenient for what follows

  __asm__ volatile
  (
    "movq %%rbp, %0;" 
    : "=r"(rbp)
  );

  // here rbp is the base pointer of stackFrameInfo
  printf("==== STACK FRAME INFO ====\n");
  // 8B after is the return address of stackFrameInfo
  // i.e. the caller of stackFrameInfo
  printf("Called from %p\n", *(rbp + 1));
  // we actually want the base pointer of the caller...
  // remember, this is the first stack element (at the bottom)
  rbp = (uint64_t **)(*rbp);
  // again, return address is 8B after
  printf("Stack base %p -- return address %p\n", rbp, *(rbp + 1));
  printf("==========================\n");
}

int function1(int a, int b)
{
  int c = a + b;

  stackFrameInfo();

  return c;
}

double function2(double a)
{
  double b = 3.14*function1(a, a);

  stackFrameInfo();

  return b;
}

int main(void)
{
  stackFrameInfo();

  printf("%f\n", function2(2.));

  return EXIT_SUCCESS;
}