#include <stdio.h>

#define ASSERT(expected, actual) \
    if ((actual) != (expected)) return __LINE__;

int main(void) {
  int128 a = (int128)1 << 127; // most negative if interpreted signed
  int128 b = -1;
  int128 sum = a + b;

  // Check that adding -1 doesn't crash and gives expected sign bit
  ASSERT(1, (sum >> 127)); // sign bit should remain 1

  // subtracting a from itself => 0
  ASSERT(0, a - a);

  printf("int128 signed edges ok\n");
  return 0;
}
