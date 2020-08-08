// Sip Hash needs well defined 64-bit ints, even on 32-bit systems
#include <stdint.h>
// Sip hash has a 128-bit key which should be fairly random
// This comes from the RadioGatun[32] hash of "https://maradns.samiam.org"
uint64_t sipKey1 = 0xded6cbc72f7eeb4fULL;
uint64_t sipKey2 = 0x81875fe84b1705d7ULL;

uint64_t SipHash(const char *str, size_t l) {
  uint64_t v0, v1, v2, v3, m;
  int shift = 0, round = 0;
  size_t offset = 0;

  // We calculate the hash via SipHash, for security reasons
  v0 = sipKey1 ^ 0x736f6d6570736575ULL;
  v1 = sipKey2 ^ 0x646f72616e646f6dULL;
  v2 = sipKey1 ^ 0x6c7967656e657261ULL;
  v3 = sipKey2 ^ 0x7465646279746573ULL;
  m = 0;
  while(offset <= l) {
    if(offset < l) {
      m |= (uint64_t)(str[offset] & 0xff) << shift;  
      shift += 8;
    }
    while(shift >= 64 || offset == l) { // "while" to avoid goto
      if(offset == l && shift != 64) {
        m |= (uint64_t)(l & 0xff) << 56;
        offset++;
      }
      shift = 0;
      v3 ^= m;
      for(round = 0; round < 2; round++) {
        v0 += v1; v2 += v3;
        v1 = (v1 << 13) | (v1 >> 51);
        v3 = (v3 << 16) | (v3 >> 48);
        v1 ^= v0; v3 ^= v2;
        v0 = (v0 << 32) | (v0 >> 32);
        v2 += v1; v0 += v3;
        v1 = (v1 << 17) | (v1 >> 47);
        v3 = (v3 << 21) | (v3 >> 43);
        v1 ^= v2; v3 ^= v0;
        v2 = (v2 << 32) | (v2 >> 32);
      }
      v0 ^= m;
      shift = 0;
      m = 0;
    }
    offset++;
  }   
  v2 ^= 255;
  for(round = 0; round < 4; round++) {
    v0 += v1; v2 += v3;
    v1 = (v1 << 13) | (v1 >> 51);
    v3 = (v3 << 16) | (v3 >> 48);
    v1 ^= v0; v3 ^= v2;
    v0 = (v0 << 32) | (v0 >> 32);
    v2 += v1; v0 += v3;
    v1 = (v1 << 17) | (v1 >> 47);
    v3 = (v3 << 21) | (v3 >> 43);
    v1 ^= v2; v3 ^= v0;
    v2 = (v2 << 32) | (v2 >> 32);
  }
  return v0 ^ v1 ^ v2 ^ v3;
} 