#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "../include/externs.h"
#include "../include/utilities.h"
#include "../include/generators.h"
#include "../include/genutils.h"

double
lcg_rand(int N, double SEED, double* DUNIF, int NDIM)
{
	int    i;
	double	DZ, DOVER, DZ1, DZ2, DOVER1, DOVER2;
	double	DTWO31, DMDLS, DA1, DA2;

	DTWO31 = 2147483648.0; /* DTWO31=2**31  */
	DMDLS  = 2147483647.0; /* DMDLS=2**31-1 */
	DA1 = 41160.0;       /* DA1=950706376 MOD 2**16 */
	DA2 = 950665216.0;   /* DA2=950706376-DA1 */

	DZ = SEED;
	if ( N > NDIM )
		N = NDIM;
	for ( i=1; i<=N; i++ ) {
		DZ = floor(DZ);
		DZ1 = DZ*DA1;
		DZ2 = DZ*DA2;
		DOVER1 = floor(DZ1/DTWO31);
		DOVER2 = floor(DZ2/DTWO31);
		DZ1 = DZ1-DOVER1*DTWO31;
		DZ2 = DZ2-DOVER2*DTWO31;
		DZ = DZ1+DZ2+DOVER1+DOVER2;
		DOVER = floor(DZ/DMDLS);
		DZ = DZ-DOVER*DMDLS;
		DUNIF[i-1] = DZ/DMDLS;
		SEED = DZ;
	}

	return SEED;
}

void
lcg()
{
	double	*DUNIF, SEED;
	int		i, counter;
	unsigned bit;
	int		num_0s, num_1s, v, bitsRead;

	SEED = 23482349.0;
	if ( ((epsilon = (BitSequence *) calloc(tp.n, sizeof(BitSequence))) == NULL) ||
	     ((DUNIF = (double*)calloc(tp.n, sizeof(double))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}
	counter = 1;

	for ( v=0; v<tp.numOfBitStreams; v++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		SEED = lcg_rand(tp.n, SEED, DUNIF, tp.n);
		for ( i=0; i<tp.n; i++ ) {
			if ( DUNIF[i] < 0.5 ) {
				bit = 0;
				num_0s++;
			}
			else {
				bit = 1;
				num_1s++;
			}
			bitsRead++;
			epsilon[i] = bit;
		}
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
		}
	free(DUNIF);
	free(epsilon);
}


void
quadRes1()
{
	int		k, num_0s, num_1s, bitsRead, done;
	BYTE	p[64], g[64], x[128];
	
	if ( ((epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}
	ahtopb("987b6a6bf2c56a97291c445409920032499f9ee7ad128301b5d0254aa1a9633fdbd378d40149f1e23a13849f3d45992f5c4c6b7104099bc301f6005f9d8115e1", p, 64);
	ahtopb("3844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5", g, 64);
	num_0s = 0;
	num_1s = 0;
	done = 0;
	bitsRead = 0;
	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		done = 0;
		bitsRead = 0;
		do {
			memset(x, 0x00, 128);
			ModMult(x, g, 64, g, 64, p,64);
			memcpy(g, x+64, 64);
			done = convertToBits(g, 512, tp.n, &num_0s, &num_1s, &bitsRead);
		} while ( !done );
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}
	free(epsilon);

	return;
}

void
quadRes2()
{
	BYTE	g[64], x[129], t1[65];
	BYTE	One[1], Two, Three[1];
	int		k, num_0s, num_1s, bitsRead, done;
	
	if ( ((epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}
	One[0] = 0x01;
	Two = 0x02;
	Three[0] = 0x03;

	ahtopb("7844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5", g, 64);
	
	for( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		done = 0;
		bitsRead = 0;
		do {
			memset(t1, 0x00, 65);
			memset(x, 0x00, 129);
			smult(t1, Two, g, 64);		/* 2x */
			add(t1, 65, Three, 1);		/* 2x+3 */
			Mult(x, t1, 65, g, 64);		/* x(2x+3) */
			add(x, 129, One, 1);		/* x(2x+3)+1 */
			memcpy(g, x+65, 64);
			done = convertToBits(g, 512, tp.n, &num_0s, &num_1s, &bitsRead);
		} while ( !done) ;
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}
	free(epsilon);

	return;
}

void
cubicRes()
{
	BYTE	g[64], tmp[128], x[192];
	int		k, num_0s, num_1s, bitsRead, done;
	
	if ( ((epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}
	
	ahtopb("7844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5", g, 64);

	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		done = 0;
		do {
			memset(tmp, 0x00, 128);
			memset(x, 0x00, 192);
			Mult(tmp, g, 64, g, 64);
			Mult(x, tmp, 128, g, 64); // Don't need to mod by 2^512, just take low 64 bytes
			memcpy(g, x+128, 64);
			done = convertToBits(g, 512, tp.n, &num_0s, &num_1s, &bitsRead);
		} while ( !done );
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}
	free(epsilon);

	return;
}

void
exclusiveOR()
{
	int		i, num_0s, num_1s, bitsRead;
	BYTE	bit_sequence[127];
	
	if ( ((epsilon = (BitSequence *)calloc(tp.n,sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}

	memcpy(bit_sequence, "0001011011011001000101111001001010011011101101000100000010101111111010100100001010110110000000000100110000101110011111111100111", 127);
	num_0s = 0;
	num_1s = 0;
	bitsRead = 0;
	for (i=0; i<127; i++ ) {
		if ( bit_sequence[i]  ) {
			epsilon[bitsRead] = 1;
			num_1s++;
		}
		else {
			epsilon[bitsRead] = 0;
			num_1s++;
		}
		bitsRead++;
	}
	for ( i=127; i<tp.n*tp.numOfBitStreams; i++ ) {
		if ( bit_sequence[(i-1)%127] != bit_sequence[(i-127)%127] ) {
			bit_sequence[i%127] = 1;
			epsilon[bitsRead] = 1;
			num_1s++;
		}
		else {
			bit_sequence[i%127] = 0;
			epsilon[bitsRead] = 0;
			num_0s++;
		}
		bitsRead++;
		if ( bitsRead == tp.n ) {
			fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
			nist_test_suite();
			num_0s = 0;
			num_1s = 0;
			bitsRead = 0;
		}
	}
	free(epsilon);
		
	return;
}


void
modExp()
{
	int		k, num_0s, num_1s, bitsRead, done;
	BYTE	p[64], g[64], x[192], y[20];

	if ( (epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}
	ahtopb("7AB36982CE1ADF832019CDFEB2393CABDF0214EC", y, 20);
	ahtopb("987b6a6bf2c56a97291c445409920032499f9ee7ad128301b5d0254aa1a9633fdbd378d40149f1e23a13849f3d45992f5c4c6b7104099bc301f6005f9d8115e1", p, 64);
	ahtopb("3844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5", g, 64);

	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		done = 0;
		do {
			memset(x, 0x00, 128);
			ModExp(x, g, 64, y, 20, p, 64);	      /* NOTE:  g must be less than p */
			done = convertToBits(x, 512, tp.n, &num_0s, &num_1s, &bitsRead);
			memcpy(y, x+44, 20);
			} while ( !done );
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}
	free(epsilon);

	return;
}

void
bbs()
{
	int		i, v, bitsRead;
	BYTE	p[64], q[64], n[128], s[64], x[256];
	int		num_0s, num_1s;

	if ( (epsilon = (BitSequence*)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}
	ahtopb("E65097BAEC92E70478CAF4ED0ED94E1C94B154466BFB9EC9BE37B2B0FF8526C222B76E0E915017535AE8B9207250257D0A0C87C0DACEF78E17D1EF9DC44FD91F", p, 64);
	ahtopb("E029AEFCF8EA2C29D99CB53DD5FA9BC1D0176F5DF8D9110FD16EE21F32E37BA86FF42F00531AD5B8A43073182CC2E15F5C86E8DA059E346777C9A985F7D8A867", q, 64);
	memset(n, 0x00, 128);
	Mult(n, p, 64, q, 64);
	memset(s, 0x00, 64);
	ahtopb("10d6333cfac8e30e808d2192f7c0439480da79db9bbca1667d73be9a677ed31311f3b830937763837cb7b1b1dc75f14eea417f84d9625628750de99e7ef1e976", s, 64);
	memset(x, 0x00, 256);
	ModSqr(x, s, 64, n, 128);

	for ( v=0; v<tp.numOfBitStreams; v++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		for ( i=0; i<tp.n; i++ ) {
			ModSqr(x, x, 128, n, 128);
			memcpy(x, x+128, 128);
			if ( (x[127] & 0x01) == 0 ) {
				num_0s++;
				epsilon[i] = 0;
			}
			else {
				num_1s++;
				epsilon[i] = 1;
			}
			bitsRead++;
			if ( (i % 50000) == 0 )
				printf("\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s);
		}

		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}
	free(epsilon);
}


// The exponent, e, is set to 11
// This results in k = 837 and r = 187
void
micali_schnorr()
{
	long	i, j;
	int		k=837, num_0s, num_1s, bitsRead, done;
	BYTE	p[64], q[64], n[128], e[1], X[128], Y[384], Tail[105];

	if ( (epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}
	ahtopb("E65097BAEC92E70478CAF4ED0ED94E1C94B154466BFB9EC9BE37B2B0FF8526C222B76E0E915017535AE8B9207250257D0A0C87C0DACEF78E17D1EF9DC44FD91F", p, 64);
	ahtopb("E029AEFCF8EA2C29D99CB53DD5FA9BC1D0176F5DF8D9110FD16EE21F32E37BA86FF42F00531AD5B8A43073182CC2E15F5C86E8DA059E346777C9A985F7D8A867", q, 64);
	memset(n, 0x00, 128);
	Mult(n, p, 64, q, 64);
	e[0] = 0x0b;
	memset(X, 0x00, 128);
	ahtopb("237c5f791c2cfe47bfb16d2d54a0d60665b20904ec822a6", X+104, 24);

	for ( i=0; i<tp.numOfBitStreams; i++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		do {
			ModExp(Y, X, 128, e, 1, n, 128);
			memcpy(Tail, Y+23, 105);
			for ( j=0; j<3; j++ )
				bshl(Tail, 105);
			done = convertToBits(Tail, k, tp.n, &num_0s, &num_1s, &bitsRead);
			memset(X, 0x00, 128);
			memcpy(X+104, Y, 24);
			for ( j=0; j<5; j++ )
				bshr(X+104, 24);
		} while ( !done );

		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}
	free(epsilon);
}

//  Uses 160 bit Xkey and no XSeed (b=160)
//  This is the generic form of the generator found on the last page of the Change Notice for FIPS 186-2
void
SHA1()
{
	ULONG	A, B, C, D, E, temp, Wbuff[16];
	BYTE	Xkey[20], G[20], M[64];
	BYTE	One[1] = { 0x01 };
	int		i, num_0s, num_1s, bitsRead;
	int		done;
	ULONG	tx[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
	
	if ( ((epsilon = (BitSequence *) calloc(tp.n,sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}

	ahtopb("ec822a619d6ed5d9492218a7a4c5b15d57c61601", Xkey, 20);
//	ahtopb("E65097BAEC92E70478CAF4ED0ED94E1C94B15446", Xkey, 20);
//	ahtopb("6BFB9EC9BE37B2B0FF8526C222B76E0E91501753", Xkey, 20);
//	ahtopb("5AE8B9207250257D0A0C87C0DACEF78E17D1EF9D", Xkey, 20);
//	ahtopb("D99CB53DD5FA9BC1D0176F5DF8D9110FD16EE21F", Xkey, 20);
	
	for ( i=0; i<tp.numOfBitStreams; i++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		do {
			memcpy(M, Xkey, 20);
			memset(M+20, 0x00, 44);
			
			// Start: SHA Steps A-E
			A = tx[0];
			B = tx[1];
			C = tx[2];
			D = tx[3];
			E = tx[4];

			memcpy((BYTE *)Wbuff, M, 64);
#ifdef LITTLE_ENDIAN
			byteReverse(Wbuff, 20);
#endif
			sub1Round1( 0 );  sub1Round1( 1 );  sub1Round1( 2 );  sub1Round1( 3 );
			sub1Round1( 4 );  sub1Round1( 5 );  sub1Round1( 6 );  sub1Round1( 7 );
			sub1Round1( 8 );  sub1Round1( 9 );  sub1Round1( 10 ); sub1Round1( 11 );
			sub1Round1( 12 ); sub1Round1( 13 ); sub1Round1( 14 ); sub1Round1( 15 );
			sub2Round1( 16 ); sub2Round1( 17 ); sub2Round1( 18 ); sub2Round1( 19 );
			Round2( 20 ); Round2( 21 ); Round2( 22 ); Round2( 23 );
			Round2( 24 ); Round2( 25 ); Round2( 26 ); Round2( 27 );
			Round2( 28 ); Round2( 29 ); Round2( 30 ); Round2( 31 );
			Round2( 32 ); Round2( 33 ); Round2( 34 ); Round2( 35 );
			Round2( 36 ); Round2( 37 ); Round2( 38 ); Round2( 39 );
			Round3( 40 ); Round3( 41 ); Round3( 42 ); Round3( 43 );
			Round3( 44 ); Round3( 45 ); Round3( 46 ); Round3( 47 );
			Round3( 48 ); Round3( 49 ); Round3( 50 ); Round3( 51 );
			Round3( 52 ); Round3( 53 ); Round3( 54 ); Round3( 55 );
			Round3( 56 ); Round3( 57 ); Round3( 58 ); Round3( 59 );
			Round4( 60 ); Round4( 61 ); Round4( 62 ); Round4( 63 );
			Round4( 64 ); Round4( 65 ); Round4( 66 ); Round4( 67 );
			Round4( 68 ); Round4( 69 ); Round4( 70 ); Round4( 71 );
			Round4( 72 ); Round4( 73 ); Round4( 74 ); Round4( 75 );
			Round4( 76 ); Round4( 77 ); Round4( 78 ); Round4( 79 );
			
			A += tx[0];
			B += tx[1];
			C += tx[2];
			D += tx[3];
			E += tx[4];
			
			memcpy(G, (BYTE *)&A, 4);
			memcpy(G+4, (BYTE *)&B, 4);
			memcpy(G+8, (BYTE *)&C, 4);
			memcpy(G+12, (BYTE *)&D, 4);
			memcpy(G+16, (BYTE *)&E, 4);
#ifdef LITTLE_ENDIAN
			byteReverse((ULONG *)G, 20);
#endif
			// End: SHA Steps A-E

			done = convertToBits(G, 160, tp.n, &num_0s, &num_1s, &bitsRead);
			add(Xkey, 20, G, 20);
			add(Xkey, 20, One, 1);
		} while ( !done );
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}
	free(epsilon);
}

#include <x86gprintrin.h> // _lrotr() and _popcnt64()
#include <stdint.h>
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

// Працюємо з 64-бітовим регістром зсуву та відвідною послідовністю [59, 60, 62, 63].
//ULONG LFSR_GAMMA = 0xec822a619d6ed5d9; // initialize LFSR register
//ULONG LFSR_GAMMA = 0xaaaaaaaaaaaaaaaa; // initialize LFSR register
ULONG LFSR_GAMMA =   0x0f6b75ab2bc471c7; // initialize LFSR register by Prime numbers:
                                         // 0x0af944215c1040f7
                                         // 0x0c9dbd5d80e68ba3
                                         // 0x0aaaaaaaaaaaaaab

void
AKalashnikovaFibonacciLFSR()
{
	int j, k, num_0s, num_1s, bitsRead, done;
	BYTE *pB;

	if ( (epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}

	/*
	char buff[64] = "prettyprintBstr title > ";
   	BYTE tmp[8];
	ahtopb("416e617374617369", tmp, 8);
	prettyprintBstr(buff, tmp, 8); // print N bytes in HEX
	*/
	printf("..................................................\n");

	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		done = 0;
		do {
			//printf("LFSR_GAMMA hex = 0x%lx\n", LFSR_GAMMA);
            // Get MSB:
//			unsigned char h = (uint8_t)((LFSR_GAMMA>>(64-8))&0xFF); // LFSR_GAMMA & 0xFF00000000000000u
			//unsigned char h = (unsigned char)(((uint8_t)((LFSR_GAMMA>>(64-8))&0xFF))^1); // LFSR_GAMMA & 0xFF00000000000000u
//			done = convertToBits(&h, 8, tp.n, &num_0s, &num_1s, &bitsRead);
			// Fails: 'igamc: UNDERFLOW' + LinearComplexity, ~5m

            // Get LSB:
		   	pB = (BYTE *)&LFSR_GAMMA;  // LFSR_GAMMA & 0x00000000000000FFu
			*pB ^= 1;
			//printf("		 h hex = 0x%0x\n", *pB);
			done = convertToBits(pB, 8, tp.n, &num_0s, &num_1s, &bitsRead);
			// Fails: 'igamc: UNDERFLOW' + 3 fails, ~5m
            // 7m 48s

			for ( j=0; j<8; j++ )
			{
                // The Fibonacci polynomial (or feedback polynomial): F(x) = x^64 + x^63 + x^61 + x^60 + 1
                // Tap sequence is  [64, 4, 3, 1, 0], counterpart sequence is [64, 63, 61, 60, 0].
                //                                                    /feedback bits\
                // Long Long representation of bits here is inverted: 1 2 3 4 5 6 7 8 ............... 60 61 62 63 64
				ULONG feedback_bit = ((LFSR_GAMMA >> 0) ^ (LFSR_GAMMA >> 1) ^ (LFSR_GAMMA >> 3) ^ (LFSR_GAMMA >> 4)) & 1u;
                // or ULONG feedback_bit = _popcnt64(LFSR_GAMMA & 0x001Bu);
                LFSR_GAMMA = (LFSR_GAMMA >> 1) | (feedback_bit << 63);
                // or LFSR_GAMMA = _lrotr(((feedback_bit ^ LFSR_GAMMA) & 1u) ^ LFSR_GAMMA, 1);
			}
		} while ( !done );
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}

	free(epsilon);
}

void
AKalashnikovaGaloisRightLFSR()
{
	int j, k, num_0s, num_1s, bitsRead, done;
	BYTE *pB;

	if ( (epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}

	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		done = 0;
		do {
			//printf("LFSR_GAMMA hex = 0x%lx\n", LFSR_GAMMA);
		   	pB = (BYTE *)&LFSR_GAMMA; // LFSR_GAMMA & 0x00000000000000FFu
			*pB ^= 1;
			//printf("		 h hex = 0x%0x\n", *pB);
			done = convertToBits(pB, 8, tp.n, &num_0s, &num_1s, &bitsRead);
			// Fails: depends on Initial value - Rank, 2 x NonOverlappingTemplate, 7m48s

			for ( j=0; j<8; j++ )
			{
				ULONG feedback_bit = LFSR_GAMMA & 1u;  // Get LSB (i.e., the output bit).
				//LFSR_GAMMA >>= 1;					   // Shift register.
				//if (feedback_bit)					   // If the output bit is 1,
				//	LFSR_GAMMA ^= 0xD800000000000000u; //	 apply toggle mask.
					// 1101100000000000000000000000000000000000000000000000000000000000

                // This value is then negated (two's complement negation), which creates a value of either all 0s or all 1s, if the output bit is 0 or 1, respectively. By ANDing the result with the tap-value (e.g., 0xB400 in the second example) before applying it as the toggle mask, it acts functionally as a conditional to either apply or not apply the toggle mask based on the output bit.
                LFSR_GAMMA = (LFSR_GAMMA >> 1) ^ (-feedback_bit & 0xD800000000000000u);

				// https://en.wikipedia.org/wiki/Linear-feedback_shift_register
				// The branch if (lsb) lfsr ^= 0xB400u;can also be written as lfsr ^= (-lsb) & 0xB400u; which may produce more efficient code on some compilers. In addition, the left-shifting variant may produce even better code, as the msb is the carry from the addition of lfsr to itself.
				//  LFSR_GAMMA ^= (-feedback_bit) & 0xD800000000000000u;
			}
		} while ( !done );
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}

	free(epsilon);
}

void
AKalashnikovaGaloisLeftLFSR()
{
	int j, k, num_0s, num_1s, bitsRead, done;
	BYTE *pB;

	if ( (epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}

	printf("LFSR_GAMMA = %lu\n", LFSR_GAMMA);
	printf("LFSR_GAMMA hex = 0x%lx\n", LFSR_GAMMA);
	printf("..................................................\n");
	printf("   5 bin = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(5));			// 00000101
	printf("   5 hex = 0x%08x\n", 5);											// 0x00000005
	printf("  -5 bin = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY((int)-5));	// 11111011
	printf("  -5 hex = 0x%08x\n", (int)-5);										// 0xfffffffb
	printf("..................................................\n");

	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		done = 0;
		do {
/*			
 			printf("  LFSR_GAMMA hex = 0x%lx\n", LFSR_GAMMA);				  // 0x416e617374617369
		   	BYTE* tmp = (unsigned char *)&LFSR_GAMMA;
			prettyprintBstr('\0', tmp, 8); // print N bytes in HEX as in Memory: 69736174 73616E41
			printf("	?0 = 0x%0x\n", *(unsigned char *)&LFSR_GAMMA);
			printf("	?1 = 0x%0x\n", *((unsigned char *)(&LFSR_GAMMA) + 1));
			printf("	?2 = 0x%0x\n", *((unsigned char *)(&LFSR_GAMMA) + 2));
			printf("	?3 = 0x%0x\n", *((unsigned char *)(&LFSR_GAMMA) + 3));
			printf("	?4 = 0x%0x\n", *((unsigned char *)(&LFSR_GAMMA) + 4));
			printf("..................................................\n");
*/
		   	pB = (BYTE *)&LFSR_GAMMA; // LFSR_GAMMA & 0x00000000000000FFu
			*pB ^= 1;
			//printf("		 h hex = 0x%0x\n", *pB);
			done = convertToBits(pB, 8, tp.n, &num_0s, &num_1s, &bitsRead);
			// Fails: none, 7m50s

			for ( j=0; j<8; j++ )
			{
                // taps:  64, 4, 3, 1, 0 ; feedback polynomial: G(x) = x^64 + x^4 + x^3 + x^1 + 1
				uint64_t feedback_bit = (int64_t)LFSR_GAMMA < 0;	  // Get MSB (i.e., the output bit).
				LFSR_GAMMA <<= 1;									  // Shift register.
				if (feedback_bit)									  // If the output bit is 1,
					LFSR_GAMMA ^= 0x000000000000001Bu;				  //	 apply toggle mask.
					// 0000000000000000000000000000000000000000000000000000000000011011
			}
		} while ( !done );
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}

	free(epsilon);
}
//==========================================================================================
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct chacha20_context
{
	uint32_t keystream32[16];
	size_t position;

	uint8_t key[32];
	uint8_t nonce[12];
	uint64_t counter;

	uint32_t state[16];
};

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

void
AKalashnikovaChaCha20()
{
	int j, k, num_0s, num_1s, bitsRead, done;
	BYTE *pB;

	if ( (epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}

    chacha20_init_context(&myctx, key, iv, 1);
    printf("\nInitialization OK\n");
    chacha20_block_next(&myctx);
    
    uint64_t *p64 = (uint64_t*) &key[0];
    printf("key0 = 0x%lx\n", *p64);
    printf("key1 = 0x%lx\n", *(p64+1));
    printf("key2 = 0x%lx\n", *(p64+2));
    printf("key3 = 0x%lx\n", *(p64+3));
    
	printf("LFSR_GAMMA = %lu\n", LFSR_GAMMA);
	printf("LFSR_GAMMA hex = 0x%lx\n", LFSR_GAMMA);
	printf("..................................................\n");
	printf("   5 bin = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(5));			// 00000101
	printf("   5 hex = 0x%08x\n", 5);											// 0x00000005
	printf("  -5 bin = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY((int)-5));	// 11111011
	printf("  -5 hex = 0x%08x\n", (int)-5);										// 0xfffffffb
	printf("..................................................\n");

	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		done = 0;
		do {
/*			
 			printf("  LFSR_GAMMA hex = 0x%lx\n", LFSR_GAMMA);				  // 0x416e617374617369
		   	BYTE* tmp = (unsigned char *)&LFSR_GAMMA;
			prettyprintBstr('\0', tmp, 8); // print N bytes in HEX as in Memory: 69736174 73616E41
			printf("	?0 = 0x%0x\n", *(unsigned char *)&LFSR_GAMMA);
			printf("	?1 = 0x%0x\n", *((unsigned char *)(&LFSR_GAMMA) + 1));
			printf("	?2 = 0x%0x\n", *((unsigned char *)(&LFSR_GAMMA) + 2));
			printf("	?3 = 0x%0x\n", *((unsigned char *)(&LFSR_GAMMA) + 3));
			printf("	?4 = 0x%0x\n", *((unsigned char *)(&LFSR_GAMMA) + 4));
			printf("..................................................\n");
*/

        	pB = (BYTE *) &myctx.keystream32[0];  // 512 bits
			//*pB ^= 1;
			//printf("		 h hex = 0x%0x\n", *pB);
			done = convertToBits(pB, 512, tp.n, &num_0s, &num_1s, &bitsRead);
			// Fails: none, 7m50s

            chacha20_block_next(&myctx);

		} while ( !done );
		fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		nist_test_suite();
	}

	free(epsilon);
}
