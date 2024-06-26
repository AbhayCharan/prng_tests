xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
                 Starting SmallCrush
                 Version: TestU01 1.2.3
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


smarsa_BirthdaySpacings test:
-----------------------------------------------
   N =  1,  n = 5000000,  r =  0,    d = 1073741824,    t = 2,    p = 1


      Number of cells = d^t = 1152921504606846976
      Lambda = Poisson mean =      27.1051

.............................................................................................................................................................................................................................................

9 tests, 237 assertions, 0 failures

Finished in 0.00001318 seconds (real) 0.00001258 seconds (proc)

Selftest with RFC 7539 test vectors: PASSED.
Status initialization: OK.

----------------------------------------------------
Total expected number = N*Lambda      :      27.11
Total observed number                 :      23
p-value of test                       :    0.75


-----------------------------------------------
CPU time used                    :  00:00:00.89

Generator state:




***********************************************************
Test sknuth_Collision calling smultin_Multinomial

***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


smultin_Multinomial test:
-----------------------------------------------
   N =  1,  n = 5000000,  r =  0,   d = 65536,   t =  2,
       Sparse =   TRUE

       GenerCell = smultin_GenerCellSerial
       Number of cells = d^t =         4294967296
       Expected number per cell =  1 /  858.99346
       EColl = n^2 / (2k) =  2910.383046
       Hashing =   TRUE

       Collision test,    Mu =      2909.2534,    Sigma =    53.8957

-----------------------------------------------
Test Results for Collisions

Expected number of collisions = Mu    :     2909.25
Observed number of collisions         :     2875
p-value of test                       :    0.73

-----------------------------
Total number of cells containing j balls

  j =  0                              :       4289970171
  j =  1                              :          4994252
  j =  2                              :             2871
  j =  3                              :                2
  j =  4                              :                0
  j =  5                              :                0

-----------------------------------------------
CPU time used                    :  00:00:00.90

Generator state:




***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


sknuth_Gap test:
-----------------------------------------------
   N =  1,  n = 200000,  r = 22,   Alpha =        0,   Beta  = 0.00390625


-----------------------------------------------
Number of degrees of freedom          : 1114
Chi-square statistic                  : 1126.30
p-value of test                       :    0.39

-----------------------------------------------
CPU time used                    :  00:00:00.64

Generator state:




***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


sknuth_SimpPoker test:
-----------------------------------------------
   N =  1,  n = 400000,  r = 24,   d =   64,   k =   64


-----------------------------------------------
Number of degrees of freedom          :   19
Chi-square statistic                  :   19.96
p-value of test                       :    0.40

-----------------------------------------------
CPU time used                    :  00:00:00.46

Generator state:




***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


sknuth_CouponCollector test:
-----------------------------------------------
   N =  1,  n = 500000,  r = 26,   d =   16


-----------------------------------------------
Number of degrees of freedom          :   44
Chi-square statistic                  :   52.67
p-value of test                       :    0.17

-----------------------------------------------
CPU time used                    :  00:00:00.41

Generator state:




***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


sknuth_MaxOft test:
-----------------------------------------------
   N =  1,  n = 2000000,  r =  0,   d = 100000,   t =  6

      Number of categories = 100000
      Expected number per category  = 20.00


-----------------------------------------------
Number of degrees of freedom          : 99999
Chi-square statistic                  : 1.01e+5
p-value of test                       :    0.11


-----------------------------------------------
Anderson-Darling statistic            :    0.99
p-value of test                       :    0.01


-----------------------------------------------
CPU time used                    :  00:00:00.41

Generator state:




***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


svaria_WeightDistrib test:
-----------------------------------------------
   N =  1,  n = 200000,  r = 27,  k = 256,  Alpha =      0,  Beta =  0.125


-----------------------------------------------
Number of degrees of freedom          :   41
Chi-square statistic                  :   53.60
p-value of test                       :    0.09

-----------------------------------------------
CPU time used                    :  00:00:00.64

Generator state:




***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


smarsa_MatrixRank test:
-----------------------------------------------
   N =  1,  n = 20000,  r = 20,    s = 10,    L = 60,    k = 60


-----------------------------------------------
Number of degrees of freedom          :    3
Chi-square statistic                  :    3.86
p-value of test                       :    0.28

-----------------------------------------------
CPU time used                    :  00:00:00.45

Generator state:




***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


sstring_HammingIndep test:
-----------------------------------------------
   N =  1,  n = 500000,  r = 20,   s = 10,   L = 300,   d = 0



Counters with expected numbers >= 10
-----------------------------------------------
Number of degrees of freedom          : 2209
Chi-square statistic                  : 2156.74
p-value of test                       :    0.78

-----------------------------------------------
CPU time used                    :  00:00:00.58

Generator state:




***********************************************************
HOST = acd, Linux

AKalashnikova ChaCha20


swalk_RandomWalk1 test:
-----------------------------------------------
   N =  1,  n = 1000000,  r =  0,   s = 30,   L0 =  150,   L1 =  150



-----------------------------------------------
Test on the values of the Statistic H

Number of degrees of freedom          :   52
ChiSquare statistic                   :   45.06
p-value of test                       :    0.74


-----------------------------------------------
Test on the values of the Statistic M

Number of degrees of freedom          :   52
ChiSquare statistic                   :   69.88
p-value of test                       :    0.05


-----------------------------------------------
Test on the values of the Statistic J

Number of degrees of freedom          :   75
ChiSquare statistic                   :   57.22
p-value of test                       :    0.94


-----------------------------------------------
Test on the values of the Statistic R

Number of degrees of freedom          :   44
ChiSquare statistic                   :   40.67
p-value of test                       :    0.62


-----------------------------------------------
Test on the values of the Statistic C

Number of degrees of freedom          :   26
ChiSquare statistic                   :   21.42
p-value of test                       :    0.72


-----------------------------------------------
CPU time used                    :  00:00:00.58

Generator state:





========= Summary results of SmallCrush =========

 Version:          TestU01 1.2.3
 Generator:        AKalashnikova ChaCha20
 Number of statistics:  15
 Total CPU time:   00:00:06.01

 All tests were passed



