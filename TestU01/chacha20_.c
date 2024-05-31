#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "chacha20.h"

static inline void u32t8le(uint32_t v, uint8_t p[4]) {
    p[0] = v & 0xff;
    p[1] = (v >> 8) & 0xff;
    p[2] = (v >> 16) & 0xff;
    p[3] = (v >> 24) & 0xff;
}

static inline uint32_t u8t32le(uint8_t p[4]) {
    uint32_t value = p[3];

    value = (value << 8) | p[2];
    value = (value << 8) | p[1];
    value = (value << 8) | p[0];

    return value;
}

static inline uint32_t rotl32(uint32_t x, int n) {
    // http://blog.regehr.org/archives/1063
    return x << n | (x >> (-n & 31));
}

// https://tools.ietf.org/html/rfc7539#section-2.1
static void chacha20_quarterround(uint32_t *x, int a, int b, int c, int d) {
    x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a], 16);
    x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c], 12);
    x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a],  8);
    x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c],  7);
}

static void chacha20_serialize(uint32_t in[16], uint8_t output[64]) {
    int i;
    for (i = 0; i < 16; i++) {
        u32t8le(in[i], output + (i << 2));
    }
}

static void chacha20_block(uint32_t in[16], uint8_t out[64], int num_rounds) {
    int i;
    uint32_t x[16];

    memcpy(x, in, sizeof(uint32_t) * 16);

    for (i = num_rounds; i > 0; i -= 2) {
        chacha20_quarterround(x, 0, 4,  8, 12);
        chacha20_quarterround(x, 1, 5,  9, 13);
        chacha20_quarterround(x, 2, 6, 10, 14);
        chacha20_quarterround(x, 3, 7, 11, 15);
        chacha20_quarterround(x, 0, 5, 10, 15);
        chacha20_quarterround(x, 1, 6, 11, 12);
        chacha20_quarterround(x, 2, 7,  8, 13);
        chacha20_quarterround(x, 3, 4,  9, 14);
    }

    for (i = 0; i < 16; i++) {
        x[i] += in[i];
    }

    chacha20_serialize(x, out);
}

// https://tools.ietf.org/html/rfc7539#section-2.3
static void chacha20_init_state(uint32_t s[16], uint8_t key[32], uint32_t counter, uint8_t nonce[12]) {
    int i;

    // refer: https://dxr.mozilla.org/mozilla-beta/source/security/nss/lib/freebl/chacha20.c
    // convert magic number to string: "expand 32-byte k"
    s[0] = 0x61707865;
    s[1] = 0x3320646e;
    s[2] = 0x79622d32;
    s[3] = 0x6b206574;

    for (i = 0; i < 8; i++) {
        s[4 + i] = u8t32le(key + i * 4);
    }

    s[12] = counter;

    for (i = 0; i < 3; i++) {
        s[13 + i] = u8t32le(nonce + i * 4);
    }
}

void ChaCha20XOR(uint8_t key[32], uint32_t counter, uint8_t nonce[12], uint8_t *in, uint8_t *out, int inlen) {
    int i, j;

    uint32_t s[16];
    uint8_t block[64];

    chacha20_init_state(s, key, counter, nonce);

    for (i = 0; i < inlen; i += 64) {
        chacha20_block(s, block, 20);
        s[12]++;

        for (j = i; j < i + 64; j++) {
            if (j >= inlen) {
                break;
            }
            out[j] = in[j] ^ block[j - i];
        }
    }
}

// Unique primes numbers:
/*  0x0f6b75ab2bc471c7
    0x0c9dbd5d80e68ba3
    0x017fffffffffffff
    0x1fffffffffffffff	
*/
/* Test values from Crypto++ documentation */
uint8_t key[] = {
        0x0f, 0x6b, 0x75, 0xab, 0x2b, 0xc4, 0x71, 0xc7,
        0x0c, 0x9d, 0xbd, 0x5d, 0x80, 0xe6, 0x8b, 0xa3,
        0x10, 0xF5, 0xB6, 0x18, 0xBD, 0xB6, 0xF2, 0x26,
        0x2F, 0xCC, 0x59, 0x7B, 0xB2, 0x30, 0xB3, 0xEF
    };
uint8_t iv[] = {
        0xA3, 0x45, 0xF5, 0xCF, 0x80, 0x23, 0x51, 0x7C,
        0xC0, 0xFC, 0xF0, 0x75, 0x74, 0x8C, 0x86, 0x5F,
        0x7D, 0xE8, 0xCA, 0x0C, 0x72, 0x36, 0xAB, 0xDA
    };

typedef struct
{
  uint32_t state[16];

  size_t position;
  uint8_t keystream32[64];
} ChaCha_ctx;

uint32_t chacha20_32_random (ChaCha_ctx *ctx)
{
    uint32_t *res;

    if (ctx->position == -1)
    {
        printf("\nInitialization OK\n");
        chacha20_init_state(ctx->state, key, 1, iv);
        /* Crypto++ initializes their counter to 1 instead of 0 */
	    ctx->position = 64;
    }

	if (ctx->position >= 64) 
	{
        chacha20_block(ctx->state, ctx->keystream32, 20);
        ctx->state[12]++;
		ctx->position = 0;
	}

	res = (uint32_t*) &ctx->keystream32[ctx->position];
	ctx->position += 4;

    return *res;
}

ChaCha_ctx myctx = { .position = -1 };

uint32_t chacha20_random (void)
{
    return chacha20_32_random(&myctx);
}




