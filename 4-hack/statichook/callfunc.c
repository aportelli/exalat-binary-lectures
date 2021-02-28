#include <stdlib.h>
#include <stdio.h>

void func(const int id)
{
    printf("calling original with id %d\n", id);
}

int main(void)
{
    func(15431);

    return EXIT_SUCCESS;
}
