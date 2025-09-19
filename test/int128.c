#include <stdio.h>
#include <stdint.h>

#define ASSERT(expected, actual) \
    if ((actual) != (expected)) return __LINE__;

int main(void) {
    int128 a = 5;
    int128 b = 3;
    ASSERT(8, a + b);
    ASSERT(2, a - b);
    ASSERT(15, a * b);
    ASSERT(1, a / b);
    ASSERT(2, a % b);
    ASSERT(1, a & b);
    ASSERT(7, a | b);
    ASSERT(6, a ^ b);
    ASSERT(20, a << 2);
    ASSERT(2, a >> 1);
    ASSERT(1, a > b);
    ASSERT(1, b < a);
    ASSERT(16, sizeof(int128)); // 16 bytes = 128 bits
    printf("int128 full ops ok\n");
    return 0;
}
