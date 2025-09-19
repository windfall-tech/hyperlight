#include <stdio.h>

#define ASSERT(expected, actual) \
    if ((actual) != (expected)) return __LINE__;

int main(void) {
  unsigned int128 a = (unsigned int128)1 << 120;
  unsigned int128 b = (unsigned int128)0xffffffffffffffffULL;

  // multiply near-large values
  unsigned int128 prod = a * b;

  // debug: print hi/lo parts of prod, a, b
  unsigned long long prod_lo = (unsigned long long)prod;
  unsigned long long prod_hi = (unsigned long long)(prod >> 64);
  unsigned long long a_lo = (unsigned long long)a;
  unsigned long long a_hi = (unsigned long long)(a >> 64);
  unsigned long long b_lo = (unsigned long long)b;
  unsigned long long b_hi = (unsigned long long)(b >> 64);
  printf("prod = 0x%016llx%016llx\n", prod_hi, prod_lo);
  printf("a    = 0x%016llx%016llx\n", a_hi, a_lo);
  printf("b    = 0x%016llx%016llx\n", b_hi, b_lo);

  // divide and modulo basic checks
  ASSERT(b, prod / a);
  ASSERT(0, prod % a);

  // bitwise ops
  ASSERT(b, (a | b) & b);

  printf("int128 unsigned ops ok\n");
  return 0;
}
