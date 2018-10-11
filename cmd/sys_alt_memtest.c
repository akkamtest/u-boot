/*Uncomment to see trace*/
/*#define DEBUG*/
/*Uncomment to generate errors*/
/*#define DEBUG_ADD_ERR*/
#include <common.h>
#include <console.h>
#include "sys_alt_memtest.h"

static unsigned long long int SEED_A;
static unsigned long long int SEED_B;
static unsigned long long int SEED_C;
static unsigned long long int SEED_D;

unsigned char verify_length(vu_long start, vu_long end, uint size_of_pt) {
	vu_long length;
	length = end - start;
	if (length % size_of_pt) {
		printf ("Tested range is not a multiple of %d Byte(s) - Aborted\n", size_of_pt);
		return(1);
	}
	return(0);
}

unsigned long long int rand1 (ulong salt) {
	static unsigned int a = 18000, b = 30903, c = 15333, d = 21041;

	SEED_A  = salt*a*(SEED_A &65535) + (SEED_A >>16);
	SEED_B  = salt*b*(SEED_B &65535) + (SEED_B >>16);
	SEED_C  = salt*c*(SEED_C &65535) + (SEED_C >>16);
	SEED_D  = salt*d*(SEED_D &65535) + (SEED_D >>16);

	return ((SEED_A <<48) + ((SEED_B &65535)<<32) + (SEED_C <<16) + (SEED_D &65535));
}

void reset_seed(void) {
	unsigned long long int seed1 = 521288629;
	unsigned long long int seed2 = 362436069;
	unsigned long long int seed3 = 123456789;
	unsigned long long int seed4 = 917456120;
	
	SEED_A  = seed1;   
	SEED_B  = seed2;
	SEED_C  = seed3;   
	SEED_D  = seed4;
}

/*
Test 1 [Address test, 8bits walking ones]
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running */
ulong __attribute__((optimize("O0")))addr_tst1(vu_long start, vu_long end) {
	unsigned char i, mask, *p, *pe;
	ulong errs = 0;
	int test_num = 1;
	/* Function should be used with multiple of 8bits length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
#ifdef DEBUG_ADD_ERR
		uint err_position;
		err_position = (end - start)/2; /* error position is half way of the tested segment */
#endif
	/* Initialise tested memory range */
	p = (unsigned char *)start;
	pe = (unsigned char *)end;
	/* test each bit for all address */
	for (; p < pe; p++) {
		debug( "TEST number: %d, balise: %d, Address: %08lx, Value: %08x\n", test_num, 0, (vu_long)p, *p);
		for (i = 0; i<8; i++) {
			mask = 1<<i;
			*p &= mask;
			*p |= mask;
			if (ctrlc())
				return -1;
#ifdef DEBUG_ADD_ERR
			if (p == (unsigned char *)(start + err_position) && (i==0)) {
				*p = ~*p;
			}
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08x\n", test_num, 0, (vu_long)p, *p);
#endif
			if(*p != mask) {
				printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08x, Obtained value: %08lx\n", test_num, (vu_long)p, mask, (vu_long)*p);
				errs ++;
			}
		}
	}
	return(errs);
}

/* 
Test 2 [Address test, own address pattern]
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running */
ulong __attribute__((optimize("O0")))addr_tst2(vu_long start, vu_long end)
{
	vu_long *p, *pe;
	ulong errs = 0;
	int test_num = 2;
	/* Function should be used with multiple of 8 bytes length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
#ifdef DEBUG_ADD_ERR
			ulong err_position;
			err_position = (end - start)/2; /* error position is half way of the tested segment */
			while (err_position%8){
				err_position--;
			}
#endif
	/* Initialise tested memory range */
	p = (vu_long*)start;
	pe = (vu_long*)end;
	
	/* Write each address with it's own address */	
	for (; p < pe; p++) {
		*p = (vu_long)p;
		if (ctrlc())
			return -1;
	}
#ifdef DEBUG_ADD_ERR
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			if (ctrlc())
				return -1;
			if (p == (vu_long *)(start + err_position)) {
				*p = ~*p;
			}
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, 0, (vu_long)p, *p);
		}
#endif
	/* Each address should have its own address */
	p = (vu_long *)start;
	pe = (vu_long *)end;
	for (; p < pe; p++) 
	{
		if (ctrlc())
			return -1;
		if(*p != (vu_long)p) {
			printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, (vu_long)p, *p);
			errs ++;
		}
	}
	return(errs);
}

/*
Test 3 [Moving inversion, pattern of all Ones & Zeros]
	iter: number of verification loop
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))movinv(int iter, vu_long pattern, vu_long start, vu_long end)
{
	int i, test_num = 3;
	ulong errs = 0;
	vu_long *p, *pe;
	/* Function should be used with multiple of 8 bytes length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
#ifdef DEBUG_ADD_ERR
		ulong err_position;
		err_position = (end - start)/2; /* error position is half way of the tested segment */
		while (err_position%8){
			err_position--;
		}
#endif
	p = (vu_long*)start;
	pe = (vu_long*)end;
	/* Initialize memory with the initial pattern.  */
	for (; p < pe; p++) {
		*p = pattern;
		if (ctrlc())
			return -1;
	}
#ifdef DEBUG_ADD_ERR
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			if (p == (vu_long *)(start + err_position))	{
				*p = ~*p;
				if (ctrlc())
					return -1;
			}
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, 0, (vu_long)p, *p);
		}
#endif
	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. Test from bottom
	 * up and then from the top down.  */
	for (i=0; i<iter; i++) 
	{
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			if (*p != pattern) {
				printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, pattern, *p);
				errs ++;
			}
			*p = ~pattern;
			if (ctrlc())
				return -1;
		debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, i+1, (vu_long)p, *p);
		}
		p = (vu_long*)(end-sizeof(vu_long));
		pe = (vu_long*)start;

		while (p >= pe) {
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, iter+1, (vu_long)p, *p);
			if (*p != ~pattern) {
				printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, ~pattern, *p);
				errs ++;
			}
			*p = pattern;
			p--;
			if (ctrlc())
				return -1;
		}
	}
	return(errs);
}

/*
Test 4 [Moving inversion, 8bits wide pattern of all Ones & Zeros]
	iter: number of verification loop
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))movinv_8bit(int iter, unsigned char pattern, vu_long start, vu_long end)
{
	int test_num = 4;
	unsigned char *p, *pe, i;
	unsigned char p2 = ~pattern;
	ulong errs = 0;

	/* Function should be used with multiple of 8 bits length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}

#ifdef DEBUG_ADD_ERR
			ulong err_position;
			err_position = (end - start)/2; /* error position is half way of the tested segment */
#endif
	p = (unsigned char*)start;
	pe = (unsigned char*)end;
	/* Initialize memory with the initial pattern.  */
	for (; p < pe; p++) {
		*p = pattern;
		if (ctrlc())
			return -1;
	}
#ifdef DEBUG_ADD_ERR
		p = (unsigned char*)start;
		pe = (unsigned char*)end;
		for (; p < pe; p++) {
			if (p == (unsigned char *)(start + err_position)) {
				*p = ~*p;
			}
			if (ctrlc())
				return -1;
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08x\n", test_num, 0, (vu_long)p, *p);
		}
#endif
	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. Test from bottom
	 * up and then from the top down.  */
	for (i=0; i<iter; i++) {
		p = (unsigned char*)start;
		pe = (unsigned char*)end;
		for (; p < pe; p++) {
			if (*p != pattern) {
				printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08x, Obtained value: %08x\n", test_num, (vu_long)p, pattern, (unsigned char)*p);
				errs ++;
			}
			*p = p2;
			if (ctrlc())
				return -1;
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08x\n", test_num, i+1, (vu_long)p, *p);
		}
		p = (unsigned char*)(end-sizeof(unsigned char));
		pe = (unsigned char*)start;
		while (p >= pe) {
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08x\n", test_num, iter+1, (vu_long)p, *p);
			if (*p != p2) {
				printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08x, Obtained value: %08x\n", test_num, (vu_long)p, p2, (unsigned char)*p);
				errs ++;
			}
			*p = pattern;
			p--;
			if (ctrlc())
				return -1;
		}
	}
	return(errs);
}

/*
Test 5 [Moving inversion, random pattern]
	iter: number of verification loop
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))movinvr(int iter, vu_long start, vu_long end)
{
	int i;
	int test_num = 5;
	vu_long *p, *pe, p1;
	ulong errs = 0;
	/* Function should be used with multiple of 8 bytes length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
#ifdef DEBUG_ADD_ERR
		ulong err_position;
		err_position = (end - start)/2; /* error position is half way of the tested segment */
		while (err_position%8){
			err_position--;
		}
#endif
	/* Initialise random pattern */
	reset_seed();
	p1 = rand1(iter);
	/* Initialise tested memory range */
	p = (vu_long*)start;
	pe = (vu_long*)end;
	/* Initialize memory with the initial pattern */
	for (; p < pe; p++) {
		*p = p1;
		if (ctrlc())
			return -1;
	}
#ifdef DEBUG_ADD_ERR
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			if (p == (vu_long *)(start + err_position)) {
				*p = ~*p;
			}
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, 0, (vu_long)p, *p);
		}
#endif

	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. Test from bottom
	 * up and then from the top down.  */
	for (i=0; i<iter; i++) {
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			if (*p != p1) {
				printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, p1, *p);
				errs ++;
			}
			*p = ~p1;
			if (ctrlc())
			return -1;
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, i+1,(vu_long)p, *p);
		}
		pe = (vu_long*)start;
		p = (vu_long*)(end-sizeof(vu_long));
		while (p >= pe) {
			if (*p != ~p1) {
				printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, ~p1, *p);
				errs ++;
			}
			*p = p1;
			p--;
			if (ctrlc())
				return -1;
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, iter+i+1,(vu_long)p, *p);
		}
	}
	return(errs);
}

/*
Test 6 [block_move, random pattern]
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))move_block(vu_long start, vu_long end)
{
	vu_long length, mask = 0x80000000, i;
	ulong errs = 0;
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
	/* length should be a multiple of 2 blocks of 64 Bytes */
	if (verify_length(start, end, 2*sizeof(tab))) {
		return(errs);
	}
#ifdef DEBUG_ADD_ERR
		ulong err_position;
		err_position = (end - start)/4; /* error position is quarter-way of the segment*/
		while (err_position%8){
			err_position--;
		}
#endif
	/* length adjustement */
	length = end - start;/* length should be at least 2 blocks of 64 Bytes */
	if (length < 128) {
		printf ("Tested range is too small - it should be 128 Bytes at least\n");
		return(errs);
	}
	/* Write each address with it's own address */
	for (i=start; i < end; i+=64) {
		memcpy((void*) i, (void*)tab, 64);
		for (j=0; j<16; j++) {
			/* rotate left with carry */
			if ((tab[j] & mask) == mask) {
				tab[j]=(tab[j]<<1) + 1;
			}
			else {
				tab[j] = tab[j]<<1;
			}
		}
		if (ctrlc())
			return -1;
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
#ifdef DEBUG_ADD_ERR
	for (i=start; i < end; i+=4) {
		p1 =(uint*)i;
		if ((p1 == (uint*)(start + err_position))) {
			*p1 = 0;
		}
		debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08x\n", test_num, 0, (vu_long)p1, *p1);
	}
#endif
	for (i=start; i < end; i+=8) {
		p1 =(uint*)i;
		p2 = (uint*)(i+4);
		debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08x\n", test_num, 1, (vu_long)p1, *p1);
		debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08x\n", test_num, 2, (vu_long)p2, *p2);
		if (*p1!=*p2) {
			printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08x, Obtained value: %08x\n", test_num, (vu_long)p1, *p1, *p2);
			errs ++;
		}
		if (ctrlc())
			return -1;
	}
	return(errs);
}

/*
Test 7 [Moving inversion, 64 bits shifting pattern]
	iter: number of verification loop
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))movinv64(vu_long pattern, vu_long start, vu_long end)
{
	int k=0;
	vu_long *p, *pe, pat, comp_pat;
	int test_num = 7;
	vu_long tab[64];
	unsigned char tab_compl = 0;
	ulong errs = 0;
	/* Function should be used with multiple of 8 bytes length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
	
#ifdef DEBUG_ADD_ERR
		ulong err_position;
		err_position = (end - start)/2; /* error position is half way of the tested segment */
		while (err_position%8){
			err_position--;
		}
#endif
	/* Initialise tested memory range */
	p = (vu_long*)start;
	pe = (vu_long*)end;
	/* Initialize memory with the initial pattern.  */
	k = 0;
	pat = pattern;
	while (p < pe)
	{
		*p = pat;
		if (tab_compl == 0) {
			
			tab[k] = pat;
		}
		
		if (++k >= 64) {
			pat = pattern;
			k = 0;
			tab_compl = 1;
		}
		else {
			pat = pat << 1;
		}
		p++;
		if (ctrlc())
			return -1;
	}
#ifdef DEBUG_ADD_ERR
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			if (p == (vu_long *)(start + err_position)) {
				*p = ~*p;
			}
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, 0, (vu_long)p, *p);
		}
#endif

	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. Test from bottom
	 * up and then from the top down.  */
	p = (vu_long*)start;
	pe = (vu_long*)end;
	k = 0;
	while (1) {
		pat = tab[k];
		if (*p != pat) {
			printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, pat, *p);
			errs ++;
		}
		comp_pat = ~pat;
		*p = comp_pat;
		debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, 1,(vu_long)p, *p);
		p++;
		if (ctrlc())
			return -1;
		if (p >= pe) {
			break;
		}
		if (++k >= 64) {
			k = 0;
		}
	}
	pe = (vu_long*)start;
	p = (vu_long*)(end-sizeof(vu_long));
	while (1) {
		pat = tab[k];
		comp_pat = ~pat;
		if (*p != comp_pat) {
			printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num +1, (vu_long)p, comp_pat, *p);
			errs ++;
		}
		*p = pat;
		debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, 2,(vu_long)p, *p);
		if (p <= pe) {
			break;
		}
		p--; 
		if (--k < 0) {
			k = 63;
		}
	}
	return(errs);
}

/*
Test 8 [Half moving inversion, random sequence pattern]
	iter: random sequence modifier, use mtest itération parameter
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))rand_seq(unsigned char iter_rand, vu_long start, vu_long end)
{
	int i;
	vu_long *p, *pe, num;
	int test_num = 8;
	ulong errs = 0;
	/* Function should be used with multiple of 8 bytes length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
#ifdef DEBUG_ADD_ERR
		ulong err_position;
		err_position = (end - start)/2; /* error position is half way of the tested segment */
		while (err_position%8){
			err_position--;
		}
#endif
	reset_seed();
	/* Initialise tested memory range */
	p = (vu_long*)start;
	pe = (vu_long*)end;
	
	for (; p < pe; p++) {
		*p = rand1(iter_rand);
		if (ctrlc())
			return -1;
	}
#ifdef DEBUG_ADD_ERR
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			if (p == (vu_long *)(start + err_position)) {
				*p = ~*p;
			}
			if (ctrlc())
			return -1;
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, 0, (vu_long)p, *p);
		}
#endif
	/* Do moving inversions test. Check for initial pattern and then
	 * write the complement for each memory location. */
	for (i=0; i<2; i++) {
		reset_seed();
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			num = rand1(iter_rand);
			if (i) {
				num = ~num;
			}
			if (*p != num) {
				printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, num, *p);
				errs ++;
			}
			*p = ~num;
			if (ctrlc())
				return -1;
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, i+1, (vu_long)p, *p);
		}
	}
	return(errs);
}

/*
Test 9 [modulo 20, random pattern]
	iter: Number of writing iteration between 2 modulo 20 patterns
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))modtst(int offset, int iter, vu_long p1, vu_long p2, vu_long start, vu_long end)
{
	int k, i;
	int test_num = 9; 
	vu_long *p;
	vu_long *pe;
	ulong errs = 0;
	/* Function should be used with multiple of 8 bytes length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
#ifdef DEBUG_ADD_ERR
		ulong err_position;
		err_position = start + offset*8 + MOD_SZ*8  ; /* error position is at the second modulo */
#endif
	/* Initialise tested memory range */
	p = (vu_long*) start + offset;
	pe = (vu_long*)(end-MOD_SZ);/* adjust the ending address */
	for (; p < pe; p += MOD_SZ) {
		*p = p1;
		if (ctrlc())
			return -1;
#ifdef DEBUG_ADD_ERR
		if (p == (vu_long*)err_position) {
			*p = ~p1;
		}
#endif
	}
	/* Write the rest of memory "iter" times with the pattern complement */
	for (i=0; i<iter; i++) 
	{
		p = (vu_long*)start;
		pe = (vu_long*)end;
		k = 0;
		for (; p < pe; p++) {
			if (k != offset) {
				*p = p2;
			}
			if (++k > MOD_SZ-1) {
				k = 0;
			}
			if (ctrlc())
			return -1;
		}
#ifdef DEBUG_ADD_ERR
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) {
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, i, (vu_long)p, *p);
		}
#endif
	}
	p = (vu_long*)start + offset;
	pe = (vu_long*)(end - MOD_SZ);
	for (; p < pe; p += MOD_SZ) {
		debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", test_num, iter, (vu_long)p, *p);
		if (*p != p1) {
			printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, p1, *p);
			errs ++;
		}
		if (ctrlc())
			return -1;
	}
	return(errs);
}

/*
Test 10(0xA) part fill - [bit fade, 64bits pattern]
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))bit_fade_fill(vu_long pattern, vu_long start, vu_long end)
{
	vu_long *p, *pe;
	ulong errs = 0;
	/* Function should be used with multiple of 8 bytes length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
#ifdef DEBUG_ADD_ERR
		ulong err_position;
		err_position = (end - start)/2; /* error position is half way of the tested segment */
		while (err_position%8){
			err_position--;
		}
#endif
	/* Initialise tested memory range */
	p = (vu_long*)start;
	pe = (vu_long*)end;
	
	/* Initialize memory with the initial pattern. */
	for (;p < pe ;p++) {
		*p = pattern;
		if (ctrlc())
			return -1;
	}
#ifdef DEBUG_ADD_ERR
		p = (vu_long*)start;
		pe = (vu_long*)end;
		for (; p < pe; p++) 
		{		
			if (p == (vu_long *)(start + err_position)) {
				*p = ~*p;
			}
			if (ctrlc())
				return -1;
			debug("TEST number: %d, balise: %d, Address: %08lx, Value: %08lx\n", 10, 0, (vu_long)p, *p);
		}
#endif
	return(errs);
}

/*
Test 10(0xA) fade part - [bit fade, 64bits pattern]
	start: Starting address of the test
	end  : Ending address of the test
	stop_after_err: "1" stop the test after an error / "0" let the test running*/
ulong __attribute__((optimize("O0")))bit_fade_chk(vu_long pattern, vu_long start, vu_long end)
{
	vu_long *p, *pe;
	ulong errs = 0;
	int test_num = 10;
	/* Function should be used with multiple of 8 bytes length*/
	if (verify_length(start, end, sizeof(*p))) {
		return(errs);
	}
	/* Initialise tested memory range */
	p = (vu_long *)start;
	pe = (vu_long *)end;
	/* Make sure that nothing changed while sleeping */
	for (;p < pe ;p++) {
		if (*p != pattern) {
			printf ("ERROR TEST number: %d, Faulty address: %08lx, Expected result: %08lx, Obtained value: %08lx\n", test_num, (vu_long)p, pattern, *p);
			errs ++;
		}
		if (ctrlc())
			return -1;
	}
	return(errs);
}

ulong wait (unsigned int sec)
{
	vu_long t, wait;
	wait = 50000 * sec;
	for ( t = 1; t <= wait; t++) {
		asm("nop");
		if (ctrlc())
			return -1;
	}
	return 0;
}
