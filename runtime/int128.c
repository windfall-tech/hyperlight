// Minimal runtime helpers for 128-bit integer arithmetic used by codegen.
#include <stdint.h>
#include <stdio.h>

// All helpers use pointers to 16-byte little-endian values for operands
// and store the result to the first pointer. This avoids complex ABI.

static __int128 load_le128(const unsigned char *p) {
  __int128 v = 0;
  for (int i = 15; i >= 0; i--) {
    v <<= 8;
    v |= (__int128)(unsigned char)p[i];
  }
  return v;
}

static unsigned __int128 load_u_le128(const unsigned char *p) {
  unsigned __int128 v = 0;
  for (int i = 15; i >= 0; i--) {
    v <<= 8;
    v |= (unsigned __int128)(unsigned char)p[i];
  }
  return v;
}

static void store_le128(unsigned char *p, __int128 v) {
  for (int i = 0; i < 16; i++) {
    p[i] = (unsigned char)(v & 0xff);
    v >>= 8;
  }
}

static void store_u_le128(unsigned char *p, unsigned __int128 v) {
  for (int i = 0; i < 16; i++) {
    p[i] = (unsigned char)(v & 0xff);
    v >>= 8;
  }
}

void __hl_i128_add(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  store_le128(res, va + vb);
}

void __hl_i128_sub(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  store_le128(res, va - vb);
}

void __hl_i128_mul(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  unsigned long long va_lo = (unsigned long long)va;
  unsigned long long va_hi = (unsigned long long)(va >> 64);
  unsigned long long vb_lo = (unsigned long long)vb;
  unsigned long long vb_hi = (unsigned long long)(vb >> 64);
  fprintf(stderr, "__hl_i128_mul: a=0x%016llx%016llx b=0x%016llx%016llx\n",
          va_hi, va_lo, vb_hi, vb_lo);
  store_le128(res, va * vb);
}

void __hl_i128_div(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  store_le128(res, va / vb);
}

void __hl_i128_mod(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  store_le128(res, va % vb);
}

void __hl_i128_and(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  store_le128(res, va & vb);
}

void __hl_i128_or(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  store_le128(res, va | vb);
}

void __hl_i128_xor(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  store_le128(res, va ^ vb);
}

void __hl_i128_shl(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  uint64_t sh = 0;
  for (int i = 7; i >= 0; i--) sh = (sh << 8) | b[i];
  unsigned long long va_lo = (unsigned long long)va;
  unsigned long long va_hi = (unsigned long long)(va >> 64);
  fprintf(stderr, "__hl_i128_shl: a=0x%016llx%016llx sh=%llu\n",
          va_hi, va_lo, (unsigned long long)sh);
  store_le128(res, va << (sh & 127));
}

void __hl_i128_shr(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  uint64_t sh = 0;
  for (int i = 7; i >= 0; i--) sh = (sh << 8) | b[i];
  store_le128(res, va >> (sh & 127));
}

void __hl_i128_cmp(unsigned char *res, unsigned char *a, unsigned char *b) {
  __int128 va = load_le128(a);
  __int128 vb = load_le128(b);
  long long out = 0;
  if (va < vb) out = -1;
  else if (va > vb) out = 1;
  for (int i = 0; i < 8; i++) { res[i] = (unsigned char)(out & 0xff); out >>= 8; }
}

/* Unsigned variants */
void __hl_u128_div(unsigned char *res, unsigned char *a, unsigned char *b) {
  unsigned __int128 va = load_u_le128(a);
  unsigned __int128 vb = load_u_le128(b);
  // Diagnostic: detect divide-by-zero and print operands for debugging.
  unsigned long long va_lo = (unsigned long long)va;
  unsigned long long va_hi = (unsigned long long)(va >> 64);
  unsigned long long vb_lo = (unsigned long long)vb;
  unsigned long long vb_hi = (unsigned long long)(vb >> 64);
  if (vb == 0) {
    // Print to stderr to avoid mixing with test stdout expectations.
    fprintf(stderr, "__hl_u128_div: divide by zero! a=0x%016llx%016llx b=0x%016llx%016llx\n",
            va_hi, va_lo, vb_hi, vb_lo);
  } else {
    fprintf(stderr, "__hl_u128_div: a=0x%016llx%016llx b=0x%016llx%016llx\n",
            va_hi, va_lo, vb_hi, vb_lo);
  }
  store_u_le128(res, va / vb);
}

void __hl_u128_mod(unsigned char *res, unsigned char *a, unsigned char *b) {
  unsigned __int128 va = load_u_le128(a);
  unsigned __int128 vb = load_u_le128(b);
  unsigned long long va_lo = (unsigned long long)va;
  unsigned long long va_hi = (unsigned long long)(va >> 64);
  unsigned long long vb_lo = (unsigned long long)vb;
  unsigned long long vb_hi = (unsigned long long)(vb >> 64);
  if (vb == 0) {
    fprintf(stderr, "__hl_u128_mod: modulo by zero! a=0x%016llx%016llx b=0x%016llx%016llx\n",
            va_hi, va_lo, vb_hi, vb_lo);
  } else {
    fprintf(stderr, "__hl_u128_mod: a=0x%016llx%016llx b=0x%016llx%016llx\n",
            va_hi, va_lo, vb_hi, vb_lo);
  }
  store_u_le128(res, va % vb);
}

void __hl_u128_shr(unsigned char *res, unsigned char *a, unsigned char *b) {
  unsigned __int128 va = load_u_le128(a);
  uint64_t sh = 0;
  for (int i = 7; i >= 0; i--) sh = (sh << 8) | b[i];
  store_u_le128(res, va >> (sh & 127));
}

void __hl_u128_cmp(unsigned char *res, unsigned char *a, unsigned char *b) {
  unsigned __int128 va = load_u_le128(a);
  unsigned __int128 vb = load_u_le128(b);
  long long out = 0;
  if (va < vb) out = -1;
  else if (va > vb) out = 1;
  for (int i = 0; i < 8; i++) { res[i] = (unsigned char)(out & 0xff); out >>= 8; }
}
