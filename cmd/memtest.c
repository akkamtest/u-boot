//#include <stdio.h>
//#include "stdint.h"

#include <common.h>
#include "memtest.h"

// Uncomment for debug
#define DEBUG_MEMTEST 1

static unsigned long long int SEED_A;
static unsigned long long int SEED_B;
static unsigned long long int SEED_C;
static unsigned long long int SEED_D;

unsigned char verify_start_param(ulong start)
{
	if ((start < MEMTEST_LOWEST_ADDR) || (start > MEMTEST_HIGHEST_ADDR))
	{
		printf ("Error with Start address\n");
		return(1);
	}
	return(0);
}

unsigned char verify_end_param(ulong end)
{
	if ((end < MEMTEST_LOWEST_ADDR) || ((end+8) > MEMTEST_HIGHEST_ADDR))
	{
		printf ("Error with End address\n");
		return(1);
	}
	return(0);
}

unsigned char verify_stop_param(ulong stop)
{
	if (stop != 0)
	{
		if (stop != 1)
		{
			printf ("Error with Stop bit\n");
			return(1);
		}
	}
	return(0);
}

unsigned char verify_length(ulong start, ulong end, uint size_of_pt)
{
	ulong length;
	length = end - start;
	if (length % size_of_pt)
	{
		printf ("Tested range is not a multiple of %d Bytes\n", size_of_pt);
		return(1);
	}
	return(0);
}

unsigned long long int rand1 (unsigned char salt)
{
   static unsigned int a = 18000, b = 30903, c = 15333, d = 21041;

   SEED_A  = salt*a*(SEED_A &65535) + (SEED_A >>16);
   SEED_B  = salt*b*(SEED_B &65535) + (SEED_B >>16);
   SEED_C  = salt*c*(SEED_C &65535) + (SEED_C >>16);
   SEED_D  = salt*d*(SEED_D &65535) + (SEED_D >>16);

   return ((SEED_A <<48) + ((SEED_B &65535)<<32) + (SEED_C <<16) + (SEED_D &65535));
}

void reset_seed(void)
{
	unsigned long long int seed1 = 521288629;
	unsigned long long int seed2 = 362436069;
	unsigned long long int seed3 = 123456789;
	unsigned long long int seed4 = 917456120;
	
   SEED_A  = seed1;   
   SEED_B  = seed2;
   SEED_C  = seed3;   
   SEED_D  = seed4;
}

/*  Allows to visualize which test return the error, the faulty address and the difference between the expected value and the one read */
void error(ulong adr, ulong good, ulong bad, int test_num)
{
	printf ("\nERROR\nTEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, adr, good, bad);
	/*printf ("	Faulty address: %08lx\n", adr);
	printf ("		>Expected result: %08lx\n", good);
	printf ("		>Obtained value : %08lx\n", bad);	*/
}

#ifdef DEBUG_MEMTEST
	/*  Allows to visualize which test return the error, the faulty address and the difference between the expected value and the one read */
	void mtest_debug(uint test_num, uint balise, ulong adr, ulong value)
{
	printf ("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, balise, adr, value);
	/*printf ("	> Address: %08lx\n", adr);
	printf ("	> Value  : %08lx\n", value);	*/
}
#endif

/* Walking ones */
/* Test 0 [Address test, 8bits walking ones, no cache] */
// Start: Starting address of the test
// End  : Ending address of the test
// stop_after_err: 1 stop the test after an error / 0 let the test running
unsigned char addr_tst1(ulong start, ulong end, unsigned char stop_after_err)
{
	unsigned char i, mask, *p, *pe;
	int test_num = 1;

	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err))
	{
		return(1);
	}
		
	/* Initialise tested memory range */
	p = (unsigned char *)start;
	pe = (unsigned char *)end;	
	
	/* test each bit for all address */
	for (; p < pe; p++) 
	{
		for (i = 0; i<8; i++) 
		{
			mask = 1<<i;
			*p &= mask;
			*p |= mask;
#ifdef DEBUG_MEMTEST
			mtest_debug(test_num, 0, (ulong)p, *p);
#endif
			if(*p != mask) 
			{
				error((ulong)p, mask, (ulong)*p, test_num);
				if (stop_after_err == 1)
				{
					return(1);	
				}
			}
		}
	}
	return(0);
}

unsigned char addr_tst2(ulong start, ulong end, char stop_after_err)
{
	ulong *p, *pe;
	int test_num = 2;	

	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 8))
	{
		return(1);
	}
	
	/* Initialise tested memory range */
	p = (ulong*)start;
	pe = (ulong*)end;
	
	/* Write each address with it's own address */	
	for (; p < pe; p++) 
	{		
		*p = (ulong)p;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 0, (ulong)p, *p);
#endif
	}

	/* Each address should have its own address */
	p = (ulong *)start;
	pe = (ulong *)end;
	for (; p < pe; p++) 
	{		
		if(*p != (ulong)p) 
		{
			error((ulong)p, (ulong)p, *p, test_num);
			if (stop_after_err == 1)
			{
				return(1);	
			}
		}
	}
	return(0);
}

unsigned char movinv (int iter, ulong start, ulong end, unsigned char stop_after_err)
{
	int i;
	int test_num = 3;
	ulong *p, *pe;
	ulong p1 = 0xA5A5A5A5A5A5A5A5;
	
	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 8))
	{
		return(1);
	}
		
	p = (ulong*)start;
	pe = (ulong*)end;
	
	/* Initialize memory with the initial pattern.  */
	for (; p < pe; p++) 
	{
		*p = p1;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 0, (ulong)p, *p);
#endif
	}
	
	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. Test from bottom
	 * up and then from the top down.  */
	for (i=0; i<iter; i++) 
	{
		p = (ulong*)start;
		pe = (ulong*)end;
		for (; p < pe; p++) 
		{
			if (*p != p1) 
			{
				error((unsigned long long int)p, p1, *p, test_num);
				if (stop_after_err == 1)
				{			
					return(1);	
				}
			}
			*p = ~p1;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, i+1, (ulong)p, *p);
#endif
		}
		
		p = (ulong*)(end-sizeof(ulong));
		pe = (ulong*)start;
		do 
		{			
			if (*p != ~p1) 
			{
				error((unsigned long long int)p, ~p1, *p, test_num);
				if (stop_after_err == 1)
				{
					return(1);	
				}
			}
			*p = p1;
		} while (--p >= pe);
	}
	return(0);
}


unsigned char movinv_8bit (int iter, ulong start, ulong end, ulong stop_after_err)
{
	int i;
	int test_num = 4;
	unsigned char *p, *pe;
	unsigned char p1 = 0x0F;
	unsigned char p2 = ~p1;
	
	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err))
	{
		return(1);
	}
	
	p = (unsigned char*)start;
	pe = (unsigned char*)end;
	
	/* Initialize memory with the initial pattern.  */
	for (; p < pe; p++)
	{
		*p = p1;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 0, (ulong)p, *p);
#endif
	}
	
	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. Test from bottom
	 * up and then from the top down.  */
	for (i=0; i<iter; i++) 
	{
		p = (unsigned char*)start;
		pe = (unsigned char*)end;

		for (; p < pe; p++) 
		{
			if (*p != p1) 
			{
				error((ulong)p, p1, (unsigned char)*p, test_num);
				if (stop_after_err == 1)
				{
					return(1);	
				}
			}
			*p = p2;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, i+1, (ulong)p, *p);
#endif
		}
		p = (unsigned char*)(end-sizeof(unsigned char));
		pe = (unsigned char*)start;
		do 
		{
			if (*p != p2) 
			{
				error((ulong)p, p2, (unsigned char)*p, test_num);
				if (stop_after_err == 1)
				{
					return(1);	
				}
			}
			*p = p1;
		} while (--p >= pe);
	}
	return(0);
}


unsigned char movinvr (int iter, ulong start, ulong end, unsigned char stop_after_err)
{
	int i;
	int test_num = 5;
	ulong *p, *pe;
	ulong p1;
	
	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 8))
	{
		return(1);
	}
	
	/* Initialise random pattern */
	reset_seed();
	p1 = rand1(iter);
	
	/* Initialise tested memory range */
	p = (ulong*)start;
	pe = (ulong*)end;
	
	/* Initialize memory with the initial pattern */
	for (; p < pe; p++) 
	{
		*p = p1;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 0,(ulong)p, *p);
#endif	
	}

	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. Test from bottom
	 * up and then from the top down.  */
	for (i=0; i<iter; i++) 
	{
		p = (ulong*)start;
		pe = (ulong*)end;

		for (; p < pe; p++) 
		{
			if (*p != p1) 
			{
				error((ulong)p, p1, *p, test_num);
				if (stop_after_err == 1)
				{
					return(1);	
				}
			}
			*p = ~p1;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, i+1,(ulong)p, *p);
#endif
		}
		pe = (ulong*)start;
		p = (ulong*)(end-sizeof(ulong));
		do 
		{
			if (*p != ~p1) 
			{
				error((ulong)p, ~p1, *p, test_num);
				if (stop_after_err == 1)
				{
					return(1);	
				}
			}
			*p = p1;
	#ifdef DEBUG_MEMTEST
			mtest_debug(test_num, iter+i+1,(ulong)p, *p);
	#endif
		} while (--p >= pe);
	}
	return(0);
}


unsigned char movinv64(ulong start, ulong end, unsigned char stop_after_err)
{
	int k=0;
	ulong *p, *pe, pat, comp_pat, p1 = MEMTEST_PATTERN_64_A;
	int test_num = 7;
	ulong tab[64];
	unsigned char tab_compl = 0;
	
	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 8))
	{
		return(1);
	}
	
	/* Initialise tested memory range */
	p = (ulong*)start;
	pe = (ulong*)end;

	/* Initialize memory with the initial pattern.  */
	k = 0;
	pat = p1;	
	while (p < pe)
	{
		*p = pat;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 0,(ulong)p, *p);
#endif		
		if (tab_compl == 0)
		{
			
			tab[k] = pat;
		}
		
		if (++k >= 64)
		{
			pat = p1;
			k = 0;
			tab_compl = 1;
		}
		else
		{
			pat = pat << 1;
		}
		p++;
	}


	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. Test from bottom
	 * up and then from the top down.  */
	p = (ulong*)start;
	pe = (ulong*)end;
	k = 0;
	while (1)
	{			
		pat = tab[k];
		if (*p != pat)
		{
			error((unsigned long long int)p, pat, *p, test_num);
			if (stop_after_err == 1)
			{
				return(1);	
			}
		}
		comp_pat = ~pat;
		*p = comp_pat;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 1,(ulong)p, *p);
#endif
		
		p++;
		if (p >= pe)
		{
			break;
		}
		if (++k >= 64)
		{
			k = 0;
		}

	}

	pe = (ulong*)start;
	p = (ulong*)(end-sizeof(ulong));
	while (1)
	{		
		pat = tab[k];
		comp_pat = ~pat;	
		if (*p != comp_pat)
		{
			error((unsigned long long int)p, comp_pat, *p, test_num+1);
			if (stop_after_err == 1)
			{
				return(1);	
			}
		}
		*p = pat;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 2,(ulong)p, *p);
#endif
		if (p <= pe)
		{
			break;
		}
		p--; 
		if (--k < 0)
		{
			k = 63;
		}
	}

	return(0);
}

unsigned char rand_seq(unsigned char iter_rand, ulong start, ulong end, unsigned char stop_after_err)
{
	int i;
	ulong *p, *pe, num;
	int test_num = 8;
	
	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 8))
	{
		return(1);
	}
	reset_seed();	
	/* Initialise tested memory range */
	p = (ulong*)start;
	pe = (ulong*)end;
	
	for (; p < pe; p++) 
	{
		*p = rand1(iter_rand);
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 0, (ulong)p, *p);
#endif	
	}


	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. */
	for (i=0; i<2; i++)
	{
		reset_seed();
		p = (ulong*)start;
		pe = (ulong*)end;
	
		for (; p < pe; p++)
		{			
			num = rand1(iter_rand);
			if (i)
			{
				num = ~num;
			}
			if (*p != num)
			{
				error((ulong)p, num, *p, test_num);
				if (stop_after_err == 1)
				{
					return(1);	
				}
			}
			*p = ~num;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, i+1, (ulong)p, *p);
#endif	
		}
	}
	return(0);
}


unsigned char modtst(int offset, int iter, ulong p1, ulong p2, ulong start, ulong end, unsigned char stop_after_err)
{
	int k, i;
	int test_num = 9;
	ulong *p;
	ulong *pe;
	//end -= MOD_SZ * 8;	/* adjust the ending address */
	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 8))
	{
		return(1);
	}
	
	/* Initialise tested memory range */
	p = (ulong*)start + offset;	
	pe = (ulong*)(end-MOD_SZ);/* adjust the ending address */
	
	for (; p < pe; p += MOD_SZ)
	{
		*p = p1;		
	} 

	/* Write the rest of memory "iter" times with the pattern complement */
	for (i=0; i<iter; i++) 
	{
		//calculate_chunk(&start, &end);
		p = (ulong*)start;
		pe = (ulong*)end;
		k = 0;
		for (; p < pe; p++) 
		{
			if (k != offset)
			{
				*p = p2;
			}
			if (++k > MOD_SZ-1)
			{
				k = 0;
			}
		}
	}
	
#ifdef DEBUG_MEMTEST
	p = (ulong*)start;	
	pe = (ulong*)end;
	for (; p < pe; p++) 
	{	
		mtest_debug(test_num, 0, (ulong)p, *p);
	}
#endif		
	
	p = (ulong*)start + offset;
	//end -= MOD_SZ*8;	/* adjust the ending address */
	pe = (ulong*)(end-MOD_SZ * 8);
	for (; p < pe; p += MOD_SZ) 
	{
		if (*p != p1) 
		{
			error((ulong)p, p1, *p, test_num);
		}
		if (stop_after_err == 1)
		{
			return(1);	
		}
	}
	return(0);
}

unsigned char bit_fade_fill(ulong p1, ulong start, ulong end, unsigned char stop_after_err)
{
	ulong *p, *pe;
#ifdef DEBUG_MEMTEST
	int test_num = 10;
#endif	
	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 8))
	{
		return(1);
	}
		
	/* Initialise tested memory range */
	p = (ulong*)start;
	pe = (ulong*)end;
	
	/* Initialize memory with the initial pattern. */		
	for (;p < pe ;p++)
	{
		*p = p1;
#ifdef DEBUG_MEMTEST
		mtest_debug(test_num, 0, (ulong)p, *p);
#endif	
	}
	return(0);
}
	
unsigned char bit_fade_chk(ulong p1, ulong start, ulong end, unsigned char stop_after_err)
{
	ulong *p, *pe, bad;
	int test_num = 10;
	
	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 8))
	{
		return(1);
	}
	
	/* Initialise tested memory range */
	p = (ulong *)start;
	pe = (ulong *)end;

	/* Make sure that nothing changed while sleeping */
	for (;p < pe ;p++)
	{
		if ((bad=*p) != p1)
		{
			error((ulong)p, p1, bad, test_num);
			if (stop_after_err == 1)
			{
				return(1);	
			}
		}
	}
	return(0);
}

void wait (unsigned int sec)
{
	ulong t, wait;
	wait = 50000 * sec;
	for ( t = 1; t <= wait; t++)
	{
		printf("\rusec: %lx", t*10);
	}
	printf("\n");
	return;
}


unsigned char move_block(ulong start, ulong end, unsigned char stop_after_err)
{
	ulong length, mask = 0x80000000, i;
	uint tab[16] =  {MEMTEST_PATTERN_MB_0,
					MEMTEST_PATTERN_MB_1,
					MEMTEST_PATTERN_MB_2,
					MEMTEST_PATTERN_MB_3,
					MEMTEST_PATTERN_MB_4,
					MEMTEST_PATTERN_MB_5,
					MEMTEST_PATTERN_MB_6,
					MEMTEST_PATTERN_MB_7,
					MEMTEST_PATTERN_MB_8,
					MEMTEST_PATTERN_MB_9,
					MEMTEST_PATTERN_MB_10,
					MEMTEST_PATTERN_MB_11,
					MEMTEST_PATTERN_MB_12,
					MEMTEST_PATTERN_MB_13,
					MEMTEST_PATTERN_MB_14,
					MEMTEST_PATTERN_MB_15};
	uint j,test_num = 6;	
	uint *p1, *p2;

	/* Test parameters */
	if (verify_start_param(start)||verify_end_param(end)||verify_stop_param(stop_after_err)||verify_length(start, end, 128)) /* length should be a multiple of 2 blocks of 64 Bytes */
	{
		return(1);
	}
	/* length adjustement */
	length = end - start;
	if (length < 128)	/* length should be at least 2 blocks of 64 Bytes */
	{
		printf ("Tested range is too small - it should be 128 Bytes at least\n");
		return(0);
	}
	/*
	if (length % 128)	
	{
		printf ("Tested range is not a multiple of 128 Bytes\n");
		return(0);
	}
	*/
	/* Write each address with it's own address */	
	for (i=start; i < end; i+=64)
	{	
		memcpy((void*) i, (void*)tab, 64);	
		for (j=0; j<16; j++)
		{
			/* rotate left with carry */
			if ((tab[j] & mask) == mask)
			{
				tab[j]=(tab[j]<<1) + 1;
			}
			else
			{
				tab[j] = tab[j]<<1;
			}
		}
	}
	/*
		At the end of all this 
		- the second half equals the inital value of the first half
		- the first half is right shifted 32-bytes (with wrapping)
	*/

	/* Move first half to second half*/
	memcpy((void*) (start + length/2), (void*) start, length/2);
	/* Move the second half, less the last 32-bytes. To the first half, offset plus 32-bytes*/
	memcpy((void*) (start + 32), (void*) (start + length/2), length/2 - 32);
	/* Move last 8 DWORDS (32-bytes) of the second half to the start of the first half */
	memcpy((void*) (start), (void*) (end - 32), 32);
#ifdef DEBUG_MEMTEST
	for (i=start; i < end; i+=4)
	{	
		p1 =(uint*)i;
		mtest_debug(test_num, 0, (ulong)p1, *p1);
		//printf ("addr p1: %08lx, value p1: %08x\n",i, *p1); 
	}
#endif	

	for (i=start; i < end; i+=8)
	{	
		p1 =(uint*)i;
		p2 = (uint*)(i+4);
		
#ifdef DEBUG_MEMTEST
		//printf ("addr p1: %08lx, value p1: %08x, addr p2: %08lx, value p2: %08x\n",i, *p1, i+4, *p2); 
		mtest_debug(test_num, 1, (ulong)p1, *p1);
		mtest_debug(test_num, 2, (ulong)p2, *p2);
#endif		
		if (*p1!=*p2)
		{
			error((ulong)p1, *p1, *p2, test_num);
		}
		if (stop_after_err == 1)
		{
			return(1);	
		}
	}

	return(0);
}
