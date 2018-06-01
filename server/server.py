# ctypes string_buffers are heavliy used in both game server and clients.
# These entities can be perceived as memory locations allocated by python
# and cleaned by python. 
# Note that string buffer can be accessed in 'value' and in 'raw' form.
# Here is an example from source:
#   self.receive_buf.value
#   self.receive_buf.raw
# 
# Raw representation is known as 'bytes object' in python. Note that
# raw representation of buffer in passed to all API functions of
# lib_send_receive

import threading
import ctypes as C
from ctypes import *
from uuid import getnode as get_mac

discovery_req = 'DISCOVERY'.encode('utf-8') # payload for discovery req

class GameServer:
    def __init__(self):
        self.mac = get_mac()  #   get mac address as 48 bit int
        self.active_clients_mac = None  # we do not know any clients until 
                                        # server discovery

        self.lib_handle = cdll.LoadLibrary("../lib_c/lib_send_receive.so")

        self.receive = self.lib_handle.receive       # C func
        self.receive.restype = c_int
        self.receive.argtypes = [c_char_p]

        self.send_data = self.lib_handle.send_data   # C func
        self.send_data.restype = c_int
        self.send_data.argtypes = [c_char_p, c_int, c_char_p]

        self.receive_buf_sz = 1500                # same as ETH_DATA_LEN
        self.receive_buf = create_string_buffer ( # here we store all received frames
                           self.receive_buf_sz) 

        self.lib_handle.socket_init()

    def run(self):
        print ('Server started. Waiting for connections...');

        print('Receive buf value:\t', self.receive_buf.value)
        #print('Receive buf raw:\t', self.receive_buf.raw)
        while True:# start server main loop
            print('\nReceive() start')
            self.receive (self.receive_buf)
            print('Receive() ended\n')

            self.parse_receive_buf()


    def parse_receive_buf(self):
        print('Started parse...\n')
        print('Receive buf value:\t', self.receive_buf.value)
        #print('Receive buf raw:\t', self.receive_buf.raw)

        self.parse_mac_dest()   # here we get source mac address
        return

    def parse_mac_dest(self):
        # convert to byte value
        print('Receive buf value[0:6]:\t', self.receive_buf.value[0:6])
        print('Receive buf raw[0:6]:\t', self.receive_buf.raw[0:6])
        print('Receive buf value[6:12]:\t', self.receive_buf.value[6:12])
        print('Receive buf raw[6:12]:\t', self.receive_buf.raw[6:12])
        print('Receive buf payload_raw[14:]:\t', self.receive_buf.raw[14:])
        print('Receive buf payload_value[14:]:\t', self.receive_buf.value[14:])
        print('Receive buf payload_value[14:]:\t', self.receive_buf.value)
        print('Receive buf payload_value[14:]:\t', type(self.receive_buf.raw))
        return

    def __del__(self):
        self.lib_handle.socket_close()

if __name__ == "__main__":
    g = GameServer()
    g.run()
