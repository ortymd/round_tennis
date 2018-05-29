# !!! run with sudo !!!
# this is a simple test of communication between two threads 
# on localhost interface

import ctypes
import threading
lib_handle = ctypes.cdll.LoadLibrary("../../lib_c/lib_send_receive.so")

lib_handle.socket_init()     # initialize global socket for communication

def  send_data_test():
    data='this is a test message'
    data_sz=3*len(data)     # we need to multiply in order to provide the least
                            # payload size and hence frame size
                            # if least frame size is not ensured,
                            # frame can be ignored by network equipment
    mac_dest="ffffffffffff"
    send_data_thread = threading.Thread (target = lib_handle.send_data,
                                         args = (data, data_sz, ctypes.c_char_p(mac_dest)))
    send_data_thread.start() 

def receive_test():
    buf=[1]
    receive_thread = threading.Thread (target = lib_handle.receive,
                                       args = (buf))
    receive_thread.start()

send_data_test()
#receive_test()
