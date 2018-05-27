import ctypes
import player
import field
import pygame
from pygame.locals import *
import threading

# we use this handle to call functions from lib_test.so
lib_handle=ctypes.cdll.LoadLibrary(
        "/home/dim/Documents/projects/plvision/round_tennis/lib_c/tests/lib_test.so")

def read_keyboard():
    pygame.event.clear()
        while True:
            print("start")
            event = pygame.event.wait()    # here we wait until user hits keyboard
            print("end")
            player_input = ''
            font = pygame.font.Font(None, 50)
            if event.type == KEYDOWN:
                if event.unicode == 'h':
                    player_input = 'hello'
                elif event.type == QUIT:
                    return

            screen.fill ((0, 0, 0))
            block = font.render(player_input, True, (255, 255, 255))
            rect = block.get_rect()
            rect.center = screen.get_rect().center
            screen.blit(block, rect)
            pygame.display.flip()

#create thread to get key input
read_keyboard_thread = threading.Thread(target = read_keyboard)

#create thread to display field
#..TODO..

pygame.init()
screen = pygame.display.set_mode((480, 360))
read_keyboard_thread.start()
