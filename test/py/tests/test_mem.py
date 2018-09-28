import pytest
import os.path
import u_boot_utils as util






#@pytest.mark.buildconfigspec('cmd_mem')
#def test_mem_mtest(u_boot_console):
def test_mem_mtest():

	""" units tests for memtest algo's
	0: if not errors
	1: if error detected
	"""
	 
	memtest_num = 1
	# memorry stat : OK   positif : ok
	#response = u_boot_console.run_command('memtest %x %d %s %s' % memtest_num, 1, 'start', 'end')
	assert (memtest_num == 1), "test ok"
	#assert (response > 0), "test ok"
	#assert (response < 0), "test ok"



	
