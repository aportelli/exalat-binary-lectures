#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    const char   msg[] = "Hello world\n";
    const size_t size  = sizeof(msg);
    ssize_t      ret;

    __asm__ volatile
    (
        "movq $1, %%rax;"
        "movq $1, %%rdi;"
        "movq %1, %%rsi;"
        "movq %2, %%rdx;"
        "syscall"
        : "=a"(ret)
        : "g"(msg), "g"(size)
        : "%rdi", "%rsi", "%rdx", "%rcx", "%r11"
    );

    return EXIT_SUCCESS;
}
