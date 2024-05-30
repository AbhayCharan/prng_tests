#include <testu01/unif01.h>
#include <testu01/bbattery.h>
#include <stdio.h>

uint32_t xchacha_random (void);

int main (void)
{
   unif01_Gen *gen;

   gen = unif01_CreateExternGenBits ("AKalashnikova XChaCha20", xchacha_random);

   bbattery_%%BATT_NAME%% (gen);

   unif01_DeleteExternGenBits (gen);

   return 0;
}
