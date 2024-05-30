#include <testu01/unif01.h>
#include <testu01/bbattery.h>
#include <stdio.h>

uint32_t chacha32_random (void);

int main (void)
{
   unif01_Gen *gen = unif01_CreateExternGenBits ("AKalashnikova ChaCha20", chacha32_random);

   bbattery_SmallCrush (gen);

   unif01_DeleteExternGenBits (gen);

   return 0;
}
