import threading
from uuid import getnode as get_mac

class GameServer():
    def __init__(self):
        self.mac = get_mac()  #   get mac address as 48 bit int
        self.active_clients_mac = []
        self.send_buf = []
        self.read_buf = []

    def run(self):
        print ('Server started. Waiting for connections...');
        

if __name__ == "__main__":
    g = GameServer()
    g.run()
