#include <testu01/unif01.h>
#include <testu01/bbattery.h>
#include <stdio.h>

unsigned int AKalashnikovaGaloisRightLFSR (void);

int main (void)
{
   //printf("sizeof(unsigned int) = %ld\n", sizeof(unsigned int));

   unif01_Gen *gen;

   gen = unif01_CreateExternGenBits ("AKalashnikovaGaloisRightFSR", AKalashnikovaGaloisRightLFSR);
   bbattery_SmallCrush (gen);
   //bbattery_Crush (gen);

   unif01_DeleteExternGenBits (gen);

   return 0;
}
