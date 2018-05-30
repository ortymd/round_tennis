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
from ctypes import c_char_p
from ctypes import c_uint
import threading
lib_handle = ctypes.cdll.LoadLibrary("../../lib_c/lib_send_receive.so")

data = b'this is a test message'        # build bytes object here

def send_data_test():
    data_sz=3*len(data)                     # we need to multiply in order to provide the least
                                            # payload size and hence frame size.
                                            # if least frame size is not ensured,
                                            # frame can be ignored by network equipment.
                                            # However we have not verified this yet.
    TEST = 0
    if TEST == 0:
        send_data = lib_handle.send_data                        # just for convenience
        send_data.argtypes = [c_char_p, c_uint, c_char_p]       # types of args that C function requires

        mac_dest = b'\xff\xff\xff\xff\xff\xff'                  # build bytes object here
        send_data_thread = threading.Thread (target = lib_handle.send_data,
                                             args = (data, data_sz, mac_dest))
    elif TEST == 1:
        send_data_thread = threading.Thread (target = lib_handle.send_data,
                                             args = (data, data_sz, ctypes.c_char_p(mac_dest)))
    elif TEST == 2:
        mac_dest=""
        send_data_thread = threading.Thread (target = lib_handle.send_data,
                                             args = (data, data_sz, mac_dest))

    print ('send_data thread started.')
    send_data_thread.start() 
    print ('send_data thread finished.')

def receive_test():
    receive = lib_handle.receive
    receive.argtypes = [c_char_p]
    buf_sz = 1<<6
    buf = ctypes.create_string_buffer (buf_sz)      # here we allocate memory which is used to store payload
                                                    # note, that eth headers are written to buffer as well
    receive_thread = threading.Thread (target = lib_handle.receive,
                                       args = (buf,))
    time.sleep (1)                              # give time to send thread to send data
    print ('receive thread started.')
    receive_thread.start()
    print ('receive thread finished.')
    print ('Received message is (raw bytes):\t', repr(buf.raw));
    if repr(buf.value) != data:
        print ('TEST FAILED')
    else:
        print ('TEST SUCCESS')

lib_handle.socket_init()     # initialize global socket for communication
send_data_test()
receive_test()
