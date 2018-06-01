from ctypes import create_string_buffer
from ctypes import c_char_p
from ctypes import c_uint
import ctypes
import pygame
from pygame.locals import *
import threading
from uuid import getnode as get_mac # to be able to get mac of this host


class GameClient():
    def __init__(self):
                         # first we start pygame initialization
        pygame.init()    #  here we start all of the pygame stuff
        width, height = 400, 500  # set screen dimensions
        self.scree = pygame.display.set_mode((width, height))

        self.mac = get_mac()  #   get mac address as 48 bit int
        self.mac_broadcast = bytes([0xff for i in range(6)]) # build bytes object here
        self.mac_server = None              # here we store server mac address
                                            # we do not know this value until server discovery

        self.lib_handle = ctypes.cdll.LoadLibrary(       # we use this handle          
                          "../lib_c/lib_send_receive.so" # to call functions from lib
                          )                              
        self.receive = self.lib_handle.receive     # get handle to lib receive() func
        self.send_data = self.lib_handle.send_data # get handle to lib send_data() func
        self.send_data.argtypes = [c_char_p, c_uint, c_char_p] # types of args that C function requires

        self.buf_sz = 1500     # ETH_DATA_LEN (here we store payload for receive)

        self.lib_handle.socket_init()

    def __del__ (self):
        self.lib_handle.socket_close()

    def discover_server (self):
        discovery_request = 'DISCOVERY'.encode('utf-8')
        data = ctypes.create_string_buffer (    # here we allocate memory which
                discovery_request, total_payload_sz)    # is used to store/modify
        send.send_data (data, self.buf_sz, self.mac) 

    def read_keyboard(self):
        running = True
        font = pygame.font.Font(None, 50)
        player_input = ''

        pygame.event.clear() # remove all of an event type from queue
        while running:
            event = pygame.event.wait()    # here we wait until user hits keyboard
            if event.type == KEYDOWN:
                print('keydown')
                if event.unicode == 'h':
                    data_str = 'hello'
                    data = data_str.encode('utf-8')
                    data_sz = len(data_str)
                    print('data:\t', data)
                    print('data_sz:\t', data_sz)
                    print('self.mac_broadcast:\t', self.mac_broadcast)

                    self.send_data (data, data_sz,
                                   self.mac_broadcast) # send message to broadcast
                    player_input = 'hello sent'
                                                               # TODO...update to server MAC
            elif event.type == QUIT:
                break

            screen.fill ((0, 0, 0))  #  this fills up the screen with black
            block = font.render(player_input, True, (255, 255, 255))
            player_input = ''       # clear player input. Create an efect of displaying 'hello sent'
                                    # each time we send a frame
            rect = block.get_rect()
            rect.center = screen.get_rect().center
            screen.blit(block, rect)
            pygame.display.flip()

    def update_screen(self, player_input):
        screen.fill ((0, 0, 0))  #  this fills up the screen with black
        pygame.display.flip()
        return

    def read_game_updates():
        print('Receiving updates from server.')

        return

client = GameClient()
read_game_updates_thread = threading.Thread(target = client.read_game_updates)
update_screen_thread = threading.Thread(target = client.update_screen)   # create thread to update/display screen
read_keyboard_thread = threading.Thread(target = client.read_keyboard) # create thread to get key input

pygame.mixer.quit()
screen = pygame.display.set_mode((480, 360))
read_game_updates_thread.start()
update_screen_thread.start()
read_keyboard_thread.start()  #  main thread does not wait for read_keyboard_thread.
                              #  it exits once read_keyboard_thread started          
read_keyboard_thread.join()   # join all threads
update_screen_thread.join()
read_keyboard_thread.join()
