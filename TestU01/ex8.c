#include <testu01/unif01.h>
#include <testu01/ulec.h>
#include <testu01/bbattery.h>
#include <stdio.h>

int main (void)
{
    unif01_Gen *gen;
    /*
    gen = unif01_CreateExternGenBits ("AKalashnikovaGaloisLeftLFSR", AKalashnikovaGaloisLeftLFSR);
    bbattery_SmallCrush (gen);
    bbattery_Crush (gen);
    unif01_DeleteExternGenBits (gen);
    */

    gen = ulec_Createlfsr113 (12345, 12345, 12345, 12345);
    //gen = ulec_Createlfsr88 (12345, 12345, 12345);
    bbattery_SmallCrush (gen);
    /*
    unif01_TimerSumGenWr (gen, 10000000, TRUE);
    for (i = 0; i < 100; i++)
       x += unif01_StripD (gen, 0);
    printf ("Sum = %14.10f\n", x);
    */
    ulec_DeleteGen (gen);


   return 0;
}
