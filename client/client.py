import sys
sys.path.append('../server/')

import pygame
from pygame.locals import *

import threading

from uuid import getnode as get_mac # to be able to get mac of this host

from connection_requests import *
from functions import *
from ctypes_defs import *           # import all ctypes related definitions
import pdb

class GameClient():
    def __init__(self):
                         # first we start pygame initialization
        #pygame.init()    #  here we start all of the pygame stuff

        self.mac_broadcast = bytes([0xff for i in range(6)]) # store broadcast addr as 6 raw bytes
        self.mac = bytes([0x44,0x8a,0x5b,0x90,0xa4,0xe0])
        self.mac_server = None              # here we store server mac address
                                            # we do not know this value until server discovery

        self.lib_handle = C.cdll.LoadLibrary(       # we use this handle
                          "../lib_c/lib_send_receive.so" # to call functions from lib
                          )                              
                                                     
        self.receive = self.lib_handle.receive       # set up receive() handle
        self.receive.restype = c_int
        self.receive.argtypes = [c_char_p]
                                                     
        self.send_payload = self.lib_handle.send_payload   # set up send() handle
        self.send_payload.restype = c_int
        self.send_payload.argtypes = [c_char_p, c_int, c_char_p]

        self.receive_buf_sz = 1500     # ETH_DATA_LEN 
        self.receive_buf = create_string_buffer ( # here we store all received frames
                           self.receive_buf_sz) 

        self.payload_sz = 1500     # ETH_DATA_LEN 
        self.payload = create_string_buffer ( # here we write payload to be sent 
                           self.payload_sz)
                      
        self.lib_handle.socket_init() # initialize global socket for both receive/send

        width, height = 400, 500  # set screen dimensions
        self.screene = pygame.display.set_mode((width, height))

    def __del__ (self):
        self.lib_handle.socket_close()

    def send_discover_server (self):
        num_of_requests = 3

        for i in range(num_of_requests):      # send num_of_requests discovery requests
            C.memmove (self.payload, discover_server, len(discover_server))
            print ('Client sending discovery:\nmac_dest:\t', self.mac)
            print ('Payload is :\t', self.payload)
            self.send_payload (self.payload, self.payload_sz, C.cast(self.mac, c_char_p))

    def read_keyboard(self):
        running = True
        font = pygame.font.Font(None, 50)
        player_input = ''

        pygame.event.clear() # remove all of an event type from queue
        while running:
            event = pygame.event.wait()    # here we wait until user hits keyboard
            if event.type == KEYDOWN:
                print('keydown')
                if event.key == K_h: # if user presses 'h'
                    payload_str = 'hello'
                    payload = payload_str.encode('utf8')
                    payload_sz = len(payload_str)
                    print('payload:\t', payload)
                    print('payload_sz:\t', payload_sz)
                    print('self.mac_broadcast:\t', self.mac_broadcast)

                    self.send_payload (payload, payload_sz,
                                   self.mac_broadcast) # send message to broadcast
                    player_input = 'hello sent'
                elif event.key == K_ESCAPE:
                    break
                                                               
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

client.send_discover_server()

#read_game_updates_thread.start()
#update_screen_thread.start()
#read_keyboard_thread.start()

                              #  it exits once read_keyboard_thread started          
#   read_keyboard_thread.join()   # join all threads
#   update_screen_thread.join()
#   read_keyboard_thread.join()
