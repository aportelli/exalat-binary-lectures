#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int function1(int a, int b)
{
  int c = a + b;

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
