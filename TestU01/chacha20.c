#include "chacha20.h"
#include <stdio.h>


static uint32_t rotl32(uint32_t x, int n) 
{
	return (x << n) | (x >> (32 - n));
}

static uint32_t pack4(const uint8_t *a)
{
	uint32_t res = 0;
	res |= (uint32_t)a[0] << 0 * 8;
	res |= (uint32_t)a[1] << 1 * 8;
	res |= (uint32_t)a[2] << 2 * 8;
	res |= (uint32_t)a[3] << 3 * 8;
	return res;
}

static void unpack4(uint32_t src, uint8_t *dst) {
	dst[0] = (src >> 0 * 8) & 0xff;
	dst[1] = (src >> 1 * 8) & 0xff;
	dst[2] = (src >> 2 * 8) & 0xff;
	dst[3] = (src >> 3 * 8) & 0xff;
}


static void chacha20_init_block(struct chacha20_context *ctx, uint8_t key[], uint8_t nonce[])
{
	memcpy(ctx->key, key, sizeof(ctx->key));
	memcpy(ctx->nonce, nonce, sizeof(ctx->nonce));

	const uint8_t *magic_constant = (uint8_t*)"expand 32-byte k";
	ctx->state[0] = pack4(magic_constant + 0 * 4);
	ctx->state[1] = pack4(magic_constant + 1 * 4);
	ctx->state[2] = pack4(magic_constant + 2 * 4);
	ctx->state[3] = pack4(magic_constant + 3 * 4);
	ctx->state[4] = pack4(key + 0 * 4);
	ctx->state[5] = pack4(key + 1 * 4);
	ctx->state[6] = pack4(key + 2 * 4);
	ctx->state[7] = pack4(key + 3 * 4);
	ctx->state[8] = pack4(key + 4 * 4);
	ctx->state[9] = pack4(key + 5 * 4);
   ctx->state[10] = pack4(key + 6 * 4);
   ctx->state[11] = pack4(key + 7 * 4);
	// 64 bit counter initialized to zero by default.
	ctx->state[12] = 0;
	ctx->state[13] = pack4(nonce + 0 * 4);
	ctx->state[14] = pack4(nonce + 1 * 4);
	ctx->state[15] = pack4(nonce + 2 * 4);

	memcpy(ctx->nonce, nonce, sizeof(ctx->nonce));
}

static void chacha20_block_set_counter(struct chacha20_context *ctx, uint64_t counter)
{
	ctx->state[12] = (uint32_t)counter;
	ctx->state[13] = pack4(ctx->nonce + 0 * 4) + (uint32_t)(counter >> 32);
}

static void chacha20_block_next(struct chacha20_context *ctx) {
	// This is where the crazy voodoo magic happens.
	// Mix the bytes a lot and hope that nobody finds out how to undo it.
	for (int i = 0; i < 16; i++) ctx->keystream32[i] = ctx->state[i];

#define CHACHA20_QUARTERROUND(x, a, b, c, d) \
    x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a], 16); \
    x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c], 12); \
    x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a], 8); \
    x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c], 7);

	for (int i = 0; i < 10; i++) 
	{
		CHACHA20_QUARTERROUND(ctx->keystream32, 0, 4, 8, 12)
		CHACHA20_QUARTERROUND(ctx->keystream32, 1, 5, 9, 13)
		CHACHA20_QUARTERROUND(ctx->keystream32, 2, 6, 10, 14)
		CHACHA20_QUARTERROUND(ctx->keystream32, 3, 7, 11, 15)
		CHACHA20_QUARTERROUND(ctx->keystream32, 0, 5, 10, 15)
		CHACHA20_QUARTERROUND(ctx->keystream32, 1, 6, 11, 12)
		CHACHA20_QUARTERROUND(ctx->keystream32, 2, 7, 8, 13)
		CHACHA20_QUARTERROUND(ctx->keystream32, 3, 4, 9, 14)
	}

	for (int i = 0; i < 16; i++) ctx->keystream32[i] += ctx->state[i];

	uint32_t *counter = ctx->state + 12;
	// increment counter
	counter[0]++;
	if (0 == counter[0]) 
	{
		// wrap around occured, increment higher 32 bits of counter
		counter[1]++;
		// Limited to 2^64 blocks of 64 bytes each.
		// If you want to process more than 1180591620717411303424 bytes
		// you have other problems.
		// We could keep counting with counter[2] and counter[3] (nonce),
		// but then we risk reusing the nonce which is very bad.
		assert(0 != counter[1]);
	}
}

void chacha20_init_context(struct chacha20_context *ctx, uint8_t key[], uint8_t nonce[], uint64_t counter)
{
	memset(ctx, 0, sizeof(struct chacha20_context));

	chacha20_init_block(ctx, key, nonce);
	chacha20_block_set_counter(ctx, counter);

	ctx->counter = counter;
	ctx->position = 64;
}

void chacha20_xor(struct chacha20_context *ctx, uint8_t *bytes, size_t n_bytes)
{
	uint8_t *keystream8 = (uint8_t*)ctx->keystream32;
	for (size_t i = 0; i < n_bytes; i++) 
	{
		if (ctx->position >= 64) 
		{
			chacha20_block_next(ctx);
			ctx->position = 0;
		}
		bytes[i] ^= keystream8[ctx->position];
		ctx->position++;
	}
}

// Unique primes numbers:
/*  0x0f6b75ab2bc471c7
    0x0c9dbd5d80e68ba3
    0x017fffffffffffff
    0x1fffffffffffffff	
*/
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
uint8_t counter[8] = {0x1};

struct chacha20_context myctx = { .position = -1 };

uint32_t chacha20_32_random (struct chacha20_context *ctx)
{
    uint32_t *res;
	uint8_t *keystream8 = (uint8_t*)ctx->keystream32;

    if (ctx->position == -1)
    {
        chacha20_init_context(ctx, key, iv, 1);
        printf("\nInitialization OK\n");
        /* printf("key = 0x%lx\n", (uint64_t*) key);
        printf("key = 0x%lx\n", (uint64_t*) (key+8));
        printf("key = 0x%lx\n", (uint64_t*) (key+16));
        printf("key = 0x%lx\n", (uint64_t*) (key+24)); */
        chacha20_block_next(ctx);
	    ctx->position = 64;
    }

	if (ctx->position >= 64) 
	{
		chacha20_block_next(ctx);
		ctx->position = 0;
	}
	res = (uint32_t *) &keystream8[ctx->position]; // or ctx->keystream32[ ctx->position/4 ]
	ctx->position += 4;

    return *res;
}

uint32_t chacha32_random (void)
{
    return chacha20_32_random (&myctx);
}



