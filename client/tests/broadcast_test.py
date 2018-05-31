# !!! run with sudo !!!
# this is a simple test of communication between two threads 
# on localhost interface
# when passing 'char*' arguments from Python to C we used Python bytes objects
# in such case we do not need any additional conversion and the additional
# benefit is that MAC address (as required by C) is stored in 6 bytes of memory 

# i believe we'll have to use binascii module to work with mac addresses passed from 
# C to Python

import ctypes
import time
import time
from ctypes import c_char_p
from ctypes import c_uint
import threading

# Here we load lib in main thread. It appears that
# each thread created in program should load the lib as well.
# We did not find out why ctypes is set up to work in such a way.
# Hopefully we do everything right here and let the source be with us.
lib_handle = ctypes.cdll.LoadLibrary("../../lib_c/lib_send_receive.so")   

test_data = b'this is a test message'        # build bytes object here
total_payload_sz = 1<<6

def send_data_test():
    lib_handle = ctypes.cdll.LoadLibrary("../../lib_c/lib_send_receive.so")
    data = ctypes.create_string_buffer (test_data, total_payload_sz)  # here we allocate memory which is used to store/modify,
                                                                      # in case length is less than min allowed, payload
    data_sz = len (test_data)                      # note that len (data) return size of whole payload, that is 
                                                   # total_payload_sz. This is the reason we call len (test_data) here

    TEST = 0
    if TEST == 0:
        send_data = lib_handle.send_data                        # just for convenience
        send_data.argtypes = [c_char_p, c_uint, c_char_p]       # types of args that C function requires
        mac_dest = b'\xff\xff\xff\xff\xff\xff'                  # build bytes object here
        for i in range (2):
            print ('Sending\t', i)
            send_data (data, data_sz, mac_dest)
    elif TEST == 1:
        send_data_thread = threading.Thread (target = lib_handle.send_data,
                                             args = (data, data_sz, ctypes.c_char_p(mac_dest)))
    elif TEST == 2:
        mac_dest=""
        send_data_thread = threading.Thread (target = lib_handle.send_data,
                                             args = (data, data_sz, mac_dest))

def receive_test():
    lib_handle = ctypes.cdll.LoadLibrary("../../lib_c/lib_send_receive.so")
    receive = lib_handle.receive
    receive.argtypes = [c_char_p]
    buf = ctypes.create_string_buffer (total_payload_sz)      # here we allocate memory which is used to store payload

    print ('Receive func started.')
    time.sleep (1)
    receive (buf) 
    print ('\nReceived message is (raw bytes):\n', repr(buf.raw));
    # import rpdb                     # start debugging here
    # rpdb.set_trace()
    if repr(buf.value) != test_data:
        print ('TEST FAILED')
    else:
        print ('TEST SUCCESS')

lib_handle.socket_init()     # initialize global socket for communication

receive_test_thread = threading.Thread (target = receive_test)
send_data_test_thread = threading.Thread (target = send_data_test)

receive_test_thread.start()    # start thread that receives test packets
send_data_test_thread.start()   # start thread that sends test packets

receive_test_thread.join()
send_data_test_thread.join()

lib_handle.close_socket()
