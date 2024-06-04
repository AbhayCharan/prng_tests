#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "AKalashnikovaChaCha20Core.c"

int32_t main (void)
{
    const size_t blksize = sizeof(cc20ctx.keystream32);
    const void *blkptr = (void*) &cc20ctx.keystream32[0];

    // At first run selftest with RFC 7539 test vectors
    if (chacha20_run_selftests(&cc20ctx) > 0)
    {
        printf("Selftest status: FAIL. Exit.\n");
        exit(EXIT_FAILURE);
    }

    chacha20_init_context(&cc20ctx, key, nonce, 1);
    freopen(NULL, "wb", stdout);  // Only necessary on Windows, but harmless.

    while (1) {
        chacha20_block_next (&cc20ctx);
        fwrite(blkptr, blksize, 1, stdout);
    }

    return 0;
}
