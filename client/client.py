import ctypes
import pygame
import select  #  this we use to block in wait for user input
from pygame.locals import *
import threading

# we use this handle to call functions from lib_test.so
lib_handle=ctypes.cdll.LoadLibrary("./lib_c/lib_send_receive.so")

class GameClient():
    def __init__(self):
    self.mac = get_mac()  #   get mac address as 48 bit int

    self.lib_handle = ctypes.cdll.LoadLibrary("../lib_c/lib_send_receive.so")
    self.receive = self.lib_handle.receive       # C func
    self.send_data = self.lib_handle.send_data   # C func

    self.buf_sz = 1<<6
    self.send_data_buf = ctypes.create_string_buffer (self.buf_sz) # byte object data from which is wriiten/send to in C part
    self.receive_buf = ctypes.create_string_buffer (self.buf_sz) # byte object to write data to in C part

    self.lib_handle.socket_init()

    def discover_server(self):
                    self.send_data (self.send_data_buf, self.buf_sz, self.mac)  # send message to broadcast

    def read_keyboard(self):
        running = True
        font = pygame.font.Font(None, 50)
        mac_broadcast = b'\xff\xff\xff\xff\xff\xff'                  # build bytes object here

        pygame.event.clear()
        while running:
            event = pygame.event.wait()    # here we wait until user hits keyboard
            if event.type == KEYDOWN:
                print('keydown')
                if event.unicode == 'h':
                    self.send_data_buf = b'hello'
                    self.send_data (self.send_data_buf, self.buf_sz, mac_broadcast)  # send message to broadcast
                                                                                # TODO...update to server MAC
            elif event.type == QUIT:
                break

            screen.fill ((0, 0, 0))  #  this fills up the screen with black
            block = font.render(player_input, True, (255, 255, 255))
            rect = block.get_rect()
            rect.center = screen.get_rect().center
            screen.blit(block, rect)
            pygame.display.flip()

def update_screen():
    return

def read_game_updates():
    return

# create
read_game_updates_thread = threading.Thread(target = read_game_updates)

# create thread to update/display screen
update_screen_thread = threading.Thread(target = update_screen)

# create thread to get key input
read_keyboard_thread = threading.Thread(target = read_keyboard)

pygame.init()    #  here we start all of the pygame stuff
pygame.mixer.quit()
screen = pygame.display.set_mode((480, 360))
read_game_updates_thread.start()
update_screen_thread.start()
read_keyboard_thread.start()  #  main thread does not wait for read_keyboard_thread.
                              #  it exits once read_keyboard_thread started          
