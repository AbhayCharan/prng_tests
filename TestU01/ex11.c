#include <testu01/unif01.h>
#include <testu01/bbattery.h>
#include <stdio.h>

uint32_t chacha32_random (void);

int main (void)
{
   printf("sizeof(unsigned int) = %ld\n", sizeof(unsigned int));
   printf("sizeof(uint32_t) = %ld\n", sizeof(uint32_t));
   //printf("\n====================================================== ??????????????????????? OK !!!!!!\n");

   unif01_Gen *gen;

   gen = unif01_CreateExternGenBits ("AKalashnikova chacha32_random", chacha32_random);
   bbattery_SmallCrush (gen);
   //bbattery_Crush (gen);

   unif01_DeleteExternGenBits (gen);

   return 0;
}
