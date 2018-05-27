import ctypes
import pygame
import select  #  this we use to block in wait for user input
from pygame.locals import *
import threading
import player

# we use this handle to call functions from lib_test.so
lib_handle=ctypes.cdll.LoadLibrary(
        "/home/dim/Documents/projects/plvision/round_tennis/lib_c/tests/lib_test.so")
def read_keyboard():
    running = True
    player_input = ''
    font = pygame.font.Font(None, 50)

    pygame.event.clear()
    while running:
        event = pygame.event.wait()    # here we wait until user hits keyboard
        if event.type == KEYDOWN:
            print('keydown')
            if event.unicode == 'h':
                player_input = 'hello'
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
