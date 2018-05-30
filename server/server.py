import threading
import ctypes
from uuid import getnode as get_mac


class GameServer():
    def __init__(self):
        self.mac = get_mac()  #   get mac address as 48 bit int
        self.active_clients_mac = []

        self.lib_handle = ctypes.cdll.LoadLibrary("../lib_c/lib_send_receive.so")
        self.receive = self.lib_handle.receive       # C func
        self.send_data = self.lib_handle.send_data   # C func
        self.buf_sz = 1<<6
        self.receive_buf = ctypes.create_string_buffer (self.buf_sz) # byte object to write data to in C part
        self.lib_handle.socket_init()       

    def run(self):
        print ('Server started. Waiting for connections...');
        self.receive (self.receive_buf)

if __name__ == "__main__":
    g = GameServer()
    g.run()
