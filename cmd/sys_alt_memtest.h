#ifndef _MEMTEST_H_
#define _MEMTEST_H_

#define MOD_SZ					20
#define MEMTEST_MOD_OFFSET		0
#define MEMTEST_ITERATION		10
#define MEMTEST_RAND_SEED_1		5
#define MEMTEST_RAND_SEED_2		17
#define MEMTEST_PATTERN_MB_0	0x00000001
#define MEMTEST_PATTERN_MB_1	0x00000001
#define MEMTEST_PATTERN_MB_2	0x00000001
#define MEMTEST_PATTERN_MB_3	0x00000001
#define MEMTEST_PATTERN_MB_4	0xfffffffe
#define MEMTEST_PATTERN_MB_5	0xfffffffe
#define MEMTEST_PATTERN_MB_6	0x00000001
#define MEMTEST_PATTERN_MB_7	0x00000001
#define MEMTEST_PATTERN_MB_8	0x00000001
#define MEMTEST_PATTERN_MB_9	0x00000001
#define MEMTEST_PATTERN_MB_10	0xfffffffe
#define MEMTEST_PATTERN_MB_11	0xfffffffe
#define MEMTEST_PATTERN_MB_12	0x00000001
#define MEMTEST_PATTERN_MB_13	0x00000001
#define MEMTEST_PATTERN_MB_14	0xfffffffe
#define MEMTEST_PATTERN_MB_15	0xfffffffe

unsigned char verify_length(vu_long start, vu_long end, uint size_of_pt);
unsigned long long int rand1(ulong salt);
void reset_seed(void);
ulong addr_tst1(vu_long start, vu_long end);
ulong addr_tst2(vu_long start, vu_long end);
ulong movinv(int iter, vu_long pattern, vu_long start, vu_long end);
ulong movinv_8bit(int iter, unsigned char pattern, vu_long start, vu_long end);
ulong movinvr(int iter, vu_long start, vu_long end);
ulong move_block(vu_long start, vu_long end);
ulong movinv64(vu_long pattern, vu_long start, vu_long end);
ulong rand_seq(unsigned char iter_rand, vu_long start, vu_long end);
ulong modtst(int offset, int iter, vu_long p1, vu_long p2, vu_long start
			, vu_long end);
ulong bit_fade_fill(vu_long pattern, vu_long start, vu_long end);
ulong bit_fade_chk(vu_long pattern, vu_long start, vu_long end);
ulong wait(unsigned int sec);
#endif /* _TEST_H_ */
