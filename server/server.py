import time
import ctypes
import player
import field
import pygame
from pygame.locals import *
import threading


def read_keyboard():
	pygame.event.clear()
	while True:
		print("start")
		event = pygame.event.wait()			# here we wait until user hits keyboard
		print("end")
		time.sleep(2)
		player_input = ''
		font = pygame.font.Font(None, 50)
		if event.type == KEYDOWN:
			if event.unicode == 'h':
				player_input = 'hello'
		elif event.type == QUIT:
			return

		#pygame.display.flip()

def count_threads():
	while True:
		time.sleep(1)
		print('Number of threads alive:', threading.active_count())


read_keyboard_thread = threading.Thread(target = read_keyboard)			# create thread to get key input

count_threads_thread = threading.Thread(target = count_threads)			# this is used to count alive threads

#create thread to display field
#..TODO..

pygame.init()
screen = pygame.display.set_mode((480, 360))
#read_keyboard()
#count_threads_thread.start()
read_keyboard_thread.start()
