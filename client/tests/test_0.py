# Here we test whether it is possible to assign raw bytes from ctypes string buffer
# into a python class and access class fields afterwards.


class EthHeader():
    def  __init__(self, receive_buf_14_bytes):
        self.mac_src = receive_buf_14_bytes[0:6]
        self.mac_dest = receive_buf_14_bytes[6:13]
        self.eth_type = receive_buf_14_bytes[13:15]

class GameServer:
    def __init__(self):
        self.mac = get_mac()  #   get mac address as 48 bit int
        self.active_clients_mac = None  # we do not know any clients until 
                                        # server discovery

        self.lib_handle = cdll.LoadLibrary("../../lib_c/lib_send_receive.so")

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
        print('\nReceive() started')
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
        print('Receive buf raw[0:6]:\t', self.receive_buf.raw[0:6])
        print('Receive buf raw[6:12]:\t', self.receive_buf.raw[6:13])
        print('Receive buf raw[13:15]:\t', self.receive_buf.raw[13:15])

        return

    def __del__(self):
        self.lib_handle.socket_close()

if __name__ == "__main__":
    g = GameServer()
    g.run()
