#!/usr/bin/env python2

import pytest
import os.path
import u_boot_utils as util

End   = 0x101000
#@pytest.mark.buildconfigspec('cmd_mem')
def test_mtest_addr_tst1(u_boot_console):

	""" test of algo addr_tst1
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 1
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'
	
	""" test of algo addr_tst1
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 1 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	
	""" test of algo addr_tst1
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 1 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	
	""" test of algo addr_tst1
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1


def test_mtest_addr_tst2(u_boot_console):

	""" test of algo addr_tst2
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 2
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'

	""" test of algo addr_tst2
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {}'.format(hex(memtest_num), hex(Stop), '', ''))
	assert 'TEST number: 2 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo addr_tst2
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 2 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo addr_tst2
	Stop  = 0
	Start = 0x100001  > 
	End   = 0x101000  > Length is not a multiple of 8 bits
	iteration = 0x1
	"""
	Start = 0x100001
	# memory length : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested range is not a multiple of 8 Bytes' in response,'Length Error undetected'
	Start = 0x100000
		
	
	""" test of algo addr_tst2
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1
	
	
def test_mtest_movinv(u_boot_console):

	""" test of algo movinv
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 3
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'

	""" test of algo movinv
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {}'.format(hex(memtest_num), hex(Stop), '', ''))
	assert 'TEST number: 3 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo movinv
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 3 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo movinv
	Stop  = 0
	Start = 0x100001  > 
	End   = 0x101000  > Length is not a multiple of 8 bits
	iteration = 0x1
	"""
	Start = 0x100001
	# memory length : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested range is not a multiple of 8 Bytes' in response,'Length Error undetected'
	Start = 0x100000
		
	
	""" test of algo movinv
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1

def test_mtest_movinv_8bit(u_boot_console):

	""" test of algo movinv_8bit
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 4
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'
	
	""" test of algo movinv_8bit
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 4 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo movinv_8bit
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 4 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo movinv_8bit
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1


def test_mtest_movinvr(u_boot_console):

	""" test of algo movinvr
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 5
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'

	""" test of algo movinvr
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 5 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo movinvr
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 5 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo movinvr
	Stop  = 0
	Start = 0x100001  > 
	End   = 0x101000  > Length is not a multiple of 8 bits
	iteration = 0x1
	"""
	Start = 0x100001
	# memory length : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested range is not a multiple of 8 Bytes' in response,'Length Error undetected'
	Start = 0x100000
		
	
	""" test of algo movinvr
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1

def test_mtest_move_block(u_boot_console):

	""" test of algo move_block
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 6
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'

	""" test of algo move_block
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 6 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo move_block
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 6 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo move_block
	Stop  = 0
	Start = 0x100001  > 
	End   = 0x101000  > Length is not a multiple of 8 bits
	iteration = 0x1
	"""
	Start = 0x100001
	# memory length : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested range is not a multiple of 128 Bytes' in response,'Length Error undetected'
	Start = 0x100000
		
	
	""" test of algo move_block
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1
	
	
def test_mtest_movinv64(u_boot_console):

	""" test of algo movinv64
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 7
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'

	""" test of algo movinv64
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 7 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo movinv64
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 7 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo movinv64
	Stop  = 0
	Start = 0x100001  > 
	End   = 0x101000  > Length is not a multiple of 8 bits
	iteration = 0x1
	"""
	Start = 0x100001
	# memory length : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested range is not a multiple of 8 Bytes' in response,'Length Error undetected'
	Start = 0x100000
		
	
	""" test of algo movinv64
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1

def test_mtest_rand_seq(u_boot_console):

	""" test of algo rand_seq
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 8
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'

	""" test of algo rand_seq
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 8 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo rand_seq
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 8 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo rand_seq
	Stop  = 0
	Start = 0x100001  > 
	End   = 0x101000  > Length is not a multiple of 8 bits
	iteration = 0x1
	"""
	Start = 0x100001
	# memory length : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested range is not a multiple of 8 Bytes' in response,'Length Error undetected'
	Start = 0x100000
		
	
	""" test of algo rand_seq
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1



def test_mtest_modtst(u_boot_console):

	""" test of algo modtst
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 9
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '1 errors found' in response, 'Memory error undetected'

	""" test of algo modtst
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 9 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo modtst
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 9 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo modtst
	Stop  = 0
	Start = 0x100001  > 
	End   = 0x101000  > Length is not a multiple of 8 bits
	iteration = 0x1
	"""
	Start = 0x100001
	# memory length : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested range is not a multiple of 8 Bytes' in response,'Length Error undetected'
	Start = 0x100000
		
	
	""" test of algo modtst
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 1 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1


def test_mtest_bit_fade(u_boot_console):

	""" test of algo bit_fade
	Stop = 0 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	memtest_num = 10
	Start = 0x100000
	iteration = 1
	Stop = 0
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert '2 errors found' in response, 'Memory error undetected'

	""" test of algo bit_fade
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	Stop = 1
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 10 interrupted after first error' in response, 'Error undetected or stop err bit not functionnal'
	
	""" test of algo bit_fade
	Stop = 1 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x3 <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	iteration = 0x3
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'TEST number: 10 interrupted after first error' in response, 'Memory error undetected or stop err bit not functionnal'
	iteration = 0x1
	
	""" test of algo bit_fade
	Stop  = 0
	Start = 0x100001  > 
	End   = 0x101000  > Length is not a multiple of 8 bits
	iteration = 0x1
	"""
	Start = 0x100001
	# memory length : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested range is not a multiple of 8 Bytes' in response,'Length Error undetected'
	Start = 0x100000
		
	
	""" test of algo bit_fade
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0xff <
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	
	Stop = 0
	iteration = 0xff
	# memory stat : ko   utest positif if error detected
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num), hex(Stop), hex(Start), hex(End), hex(iteration)))
	assert 'Tested 255 iteration(s) with 2 errors.' in response, 'Memory error undetected or iteration parameter not functionnal'
	iteration = 0x1




def test_signature_mtest(u_boot_console):
	
	""" test of mtest command inputs
	Test number : 15 <
	Stop = 0
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1 
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	# Id test error
	memtest_num_test = 15
	Stop_test = 0
	Start_test =0x00100000
	End_test = 0x00101000
	iteration_test = 0x1
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num_test), hex(Stop_test), hex(Start_test), hex(End_test), hex(iteration_test)))
	assert 'Invalid test id' in response, 'Invalid test id :  Failed Detection'
	
	""" test of mtest command inputs
	Test number : 1
	Stop = 3 <
	Start = 0x100000
	End   = 0x101000
	iteration = 0x1 
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	# Stop after error bit
	memtest_num_test = 1
	Stop_test = 3 
	Start_test =0x00100000
	End_test = 0x00101000
	iteration_test = 0x1
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num_test), hex(Stop_test), hex(Start_test), hex(End_test), hex(iteration_test)))
	assert 'Invalid [stop] argument, should be 0 or 1' in response, 'Invalid [stop] argument, should be 0 or 1 :  Failed Detection'
	
	""" test of mtest command inputs
	Test number : 1
	Stop = 0
	Start = 0x101000 <
	End   = 0x100000 <
	iteration = 0x1 
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	# Test of memory area
	memtest_num_test = 1
	Stop_test = 0
	Start_test = 0x00101000
	End_test = 0x00100000
	iteration_test = 0x1
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num_test), hex(Stop_test), hex(Start_test), hex(End_test), hex(iteration_test)))
	assert 'Inavlid memory area (null or less than 0' in response, 'Inavlid memory area (null or less than 0 : Failed Detection'

	""" test of mtest command inputs
	Test number : 1
	Stop = 0
	Start = 0x000000 <
	End   = 0x101000
	iteration = 0x1 
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	#Start or End address Error
	memtest_num_test = 1
	Stop_test = 0
	Start_test =0x0
	End_test = 0x101000
	iteration_test = 0x1
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num_test), hex(Stop_test), hex(Start_test), hex(End_test), hex(iteration_test)))
	assert ('Error : start address min =' in response and 'end address max ='in response), 'Start address Error : Failed Detection'

	""" test of mtest command inputs
	Test number : 1
	Stop = 0
	Start = 0x00100000 
	End   = 0x00101008 <
	iteration = 0x1 
	An error is injected when DEBUG_MEMTEST is active (cf. memtest.c)
	"""
	#Start or End address Error
	memtest_num_test = 1
	Stop_test = 0
	Start_test = 0x00100000
	End_test = 0x00101008
	iteration_test = 0x1
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num_test), hex(Stop_test), hex(Start_test), hex(End_test), hex(iteration_test)))
	assert ('Error : start address min =' in response and 'end address max ='in response), 'Start address Error : Failed Detection'

	""" test of mtest command inputs
	Test number : 0 < Verify if all tests can be launched
	Stop = 0 
	Start = 0x00100000 
	End   = 0x00101000
	iteration = 0x1 
	"""
	# Launch all test, do not stop at first memory error encountered
	memtest_num_test = 0
	Stop_test = 0
	Start_test =0x00100000
	End_test = 0x00101000
	iteration_test = 0x1
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num_test), hex(Stop_test), hex(Start_test), hex(End_test), hex(iteration_test)))
	assert ('TEST number: 1' in response 
		and 'TEST number: 2'in response
		and 'TEST number: 3'in response
		and 'TEST number: 4'in response
		and 'TEST number: 5'in response
		and 'TEST number: 6'in response
		and 'TEST number: 7'in response
		and 'TEST number: 8'in response
		and 'TEST number: 9'in response
		and 'TEST number: 1'in response
		and 'TEST number: 8'in response
		and 'TEST number: 9'in response
		and 'TEST number: 10'in response), 'Launch all tests : Failed Detection to detect memory error'

	""" test of mtest command inputs
	Test number : 0 < Verify if all tests can be launched
	Stop = 1 < but stop after first error 
	Start = 0x00100000 
	End   = 0x00101000
	iteration = 0x1 
	"""
	# Launch all test, stop at first memory error encountered
	memtest_num_test = 0
	Stop_test = 1
	Start_test =0x00100000
	End_test = 0x00101000
	iteration_test = 0x1
	response = u_boot_console.run_command('mtest {} {} {} {} {}'.format(hex(memtest_num_test), hex(Stop_test), hex(Start_test), hex(End_test), hex(iteration_test)))
	assert 'TEST number: 1 interrupted after first error' in response, 'Failed to stop at first error'
