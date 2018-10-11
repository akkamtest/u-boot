#!/usr/bin/env python2

import pytest
import os.path
import u_boot_utils as util


#@pytest.mark.buildconfigspec('cmd_mem')
@pytest.mark.timeout(240)
def test_signature_mtest(u_boot_console):

    # Test of memory area: end < start
    start_test = '00101000'
    end_test = '00101000'
    pattern_test = 'A5A5A5A5A5A5A5A5'
    iteration_test = '1'
    response = u_boot_console.run_command('mtest {} {} {} {}'.format(start_test, end_test, pattern_test, iteration_test))
    assert 'Refusing to do empty test' in response, 'Failed to detect negative memory size'

    # Test of memory area: end = start
    start_test = '00100000'
    end_test = '00100000'
    pattern_test = 'A5A5A5A5A5A5A5A5'
    iteration_test = '1'
    response = u_boot_console.run_command('mtest {} {} {} {}'.format(start_test, end_test, pattern_test, iteration_test))
    assert 'Refusing to do empty test' in response, 'Failed to detect null memory size'

    # Test of memory area: memory length should be a multiple of 128 bytes in order to launch every tests with success
    start_test = '00000000'
    end_test = '00000010'
    pattern_test = 'A5A5A5A5A5A5A5A5'
    iteration_test = '1'
    response = u_boot_console.run_command('mtest {} {} {} {}'.format(start_test, end_test, pattern_test, iteration_test))
    assert 'Tested range is not a multiple of 128 Byte' in response, 'Failed to detect non 128 bytes multiple memory size'
    
    # Test of memory area: memory length should be a multiple of 8 bytes in order to launch most tests with success
    start_test = '00000000'
    end_test = '00000011'
    pattern_test = 'A5A5A5A5A5A5A5A5'
    iteration_test = '1'
    response = u_boot_console.run_command('mtest {} {} {} {}'.format(start_test, end_test, pattern_test, iteration_test))
    assert 'Tested range is not a multiple of 8 Byte' in response, 'Failed to detect non 8 bytes multiple memory size'
    
    # Test of pattern: maximum pattern size and value
    start_test = '00000000'
    end_test = '00001000'
    pattern_test = 'FFFFFFFFFFFFFFFF'
    iteration_test = '1'
    response = u_boot_console.run_command('mtest {} {} {} {}'.format(start_test, end_test, pattern_test, iteration_test))
    assert '0 errors found' in response, 'Failed to use maximum value pattern'

    # Test of pattern: maximum pattern value
    start_test = '00000000'
    end_test = '00001000'
    pattern_test = '0'
    iteration_test = '1'
    response = u_boot_console.run_command('mtest {} {} {} {}'.format(start_test, end_test, pattern_test, iteration_test))
    assert '0 errors found' in response, 'Failed to use minimum value pattern'
    
    # Test of pattern: over maximum pattern value
    start_test = '00000000'
    end_test = '00001000'
    pattern_test = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF'
    iteration_test = '1'
    response = u_boot_console.run_command('mtest {} {} {} {}'.format(start_test, end_test, pattern_test, iteration_test))
    assert '0 errors found' in response, 'Failed to truncate pattern'
    
#@pytest.mark.buildconfigspec('cmd_mem') 
def test_signature_mtest_2(u_boot_console):
    # Test of iteration: Try 255 iterations
    start_test = '00000000'
    end_test = '00001000'
    pattern_test = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF'
    iteration_test = 'ff'
    response = u_boot_console.run_command('mtest {} {} {} {}'.format(start_test, end_test, pattern_test, iteration_test))
    assert 'Tested 255 iteration' in response, 'Failed launch 255 iterations'
