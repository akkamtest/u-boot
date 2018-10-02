#ifndef _MEMTEST_H_
#define _MEMTEST_H_

#ifndef CONFIG_SYS_INIT_SP_ADDR
#define	CONFIG_SYS_INIT_SP_ADDR	0xBEF7000000000000
#endif /* CONFIG_SYS_MEMTEST_START */

#define MOD_SZ					20
#define MEMTEST_MOD_OFFSET 		0
#define MEMTEST_ITERATION  		10
#define MEMTEST_RAND_SEED_1		5
#define MEMTEST_RAND_SEED_2		17
#define MEMTEST_PATTERN_64_A	0xB607C9D1A4E85F23
#define MEMTEST_PATTERN_64_B	0xA5A5A5A5A5A5A5A5
#define MEMTEST_PATTERN_64_C	0x5A5A5A5A5A5A5A5A
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
#define IS_MEMTEST_1		 	0x0001
#define IS_MEMTEST_2		 	0x0002
#define IS_MEMTEST_3		 	0x0004
#define IS_MEMTEST_4		 	0x0008
#define IS_MEMTEST_5		 	0x0010
#define IS_MEMTEST_6		 	0x0020
#define IS_MEMTEST_7		 	0x0040
#define IS_MEMTEST_8		 	0x0080
#define IS_MEMTEST_9		 	0x0100
#define IS_MEMTEST_10		 	0x0200

unsigned char verify_length(vu_long start, vu_long end, uint size_of_pt);
unsigned long long int rand1 (unsigned char salt);
void reset_seed(void);
void error(vu_long adr, vu_long good, vu_long bad, int test_num);
void mtest_debug(uint test_num, uint balise, vu_long adr, vu_long value);
unsigned char addr_tst1(vu_long start, vu_long end, unsigned char stop_after_err);
unsigned char addr_tst2(vu_long start, vu_long end, char stop_after_err);
unsigned char movinv (int iter, vu_long start, vu_long end, unsigned char stop_after_err);
unsigned char movinv_8bit (int iter, vu_long start, vu_long end, vu_long stop_after_err);
unsigned char movinvr (int iter, vu_long start, vu_long end, unsigned char stop_after_err);
unsigned char move_block(vu_long start, vu_long end, unsigned char stop_after_err);
unsigned char movinv64(vu_long start, vu_long end, unsigned char stop_after_err);
unsigned char rand_seq(unsigned char iter_rand, vu_long start, vu_long end, unsigned char stop_after_err);
unsigned char modtst(int offset, int iter, vu_long p1, vu_long p2, vu_long start, vu_long end, unsigned char stop_after_err);
unsigned char bit_fade_fill(vu_long p1, vu_long start, vu_long end, unsigned char stop_after_err);
unsigned char bit_fade_chk(vu_long p1, vu_long start, vu_long end, unsigned char stop_after_err);
void wait (unsigned int sec);
#endif /* _TEST_H_ */
