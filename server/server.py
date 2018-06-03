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
from uuid import getnode as get_mac

from connection_requests import *
from ctypes_defs import *           # import all ctypes related definitions

class GameServer:
    def __init__(self):
        self.mac = hex(get_mac()).encode('utf8')  # store source mac address as 6 raw bytes
        self.mac_broadcast = bytes([0xff for i in range(6)]) # store broadcast addr as 6 raw bytes

        self.active_clients_num = 0
        self.active_clients_max = 4     # We need this to allocate enough memory in python to
                                        # store mac addresses of all clients.
                                        # Allocate memory to store mac addresses.
        self.active_clients_mac = C.create_string_buffer(
                                  self.active_clients_max)

        self.lib_handle = C.cdll.LoadLibrary(       # we use this handle
                          "../lib_c/lib_send_receive.so" # to call functions from lib
                          )                              

        self.receive = self.lib_handle.receive       # C func
        self.receive.restype = c_int
        self.receive.argtypes = [c_char_p]

        self.send_payload = self.lib_handle.send_payload   # C func
        self.send_payload.restype = c_int
        self.send_payload.argtypes = [c_char_p, c_int, c_char_p]

        self.receive_buf_sz = 1500                # same as ETH_DATA_LEN
        self.receive_buf = create_string_buffer ( # here we store all received frames
                           self.receive_buf_sz) 

        self.lib_handle.socket_init()

    def run(self):
        print ('Server started. Waiting for connections...');

        while True:# start server main loop
            print('\n**********\nReceive() start')
            self.receive (self.receive_buf)
            print('Receive() ended\n')

            self.parse_receive_buf()

    def parse_receive_buf(self):
        print('Started parse_receive_buf()...\n')
        print('Receive buf value:\t', self.receive_buf.value)
        #print('Receive buf raw:\t', self.receive_buf.raw)

        self.parse_frame_header()   # here we get source mac address
        self.parse_frame_payload()   # here we get source mac address
        return

    def parse_frame_header(self):
        # convert to byte value
        print('Receive buf value[0:6]:\t', self.receive_buf.value[0:6])
        print('Receive buf value[6:12]:\t', self.receive_buf.value[6:12])

        print('Receive buf raw[0:6]:\t', self.receive_buf.raw[0:6])
        print('Receive buf raw[6:12]:\t', self.receive_buf.raw[6:12])

        print('Receive buf payload_value[14:20]:\t', self.receive_buf.value[14:20])
        print('Receive buf payload_raw[14:23]:\t', self.receive_buf.raw[14:23])
        print('Receive buf payload_raw[14:23]:\t', self.receive_buf.raw[14:23].decode('utf8'))

        mac_src = self.receive_buf.raw[6:12]

        return

    def __del__(self):
        self.lib_handle.socket_close()

if __name__ == "__main__":
    g = GameServer()
    g.run()
