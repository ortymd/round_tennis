#!/usr/bin/env python

import pygame
from pygame.locals import *
from sys import exit

background_image = 'terain7.png'
boat = 'boat.png'

pygame.init()
SCREEN_SIZE = (500, 500)
screen = pygame.display.set_mode(SCREEN_SIZE, 0, 32)
pygame.display.set_caption("Pygame Demo")
background = pygame.image.load(background_image).convert()
player = pygame.image.load(boat).convert_alpha()

x, y = 150, 353
MOVE_RIGHT = 1
MOVE_LEFT = 2
direction = 0

while True:

    for event in pygame.event.get():

        if event.type == QUIT:
            exit()

        if event.type == KEYDOWN:
            if event.key == K_LEFT:
                direction = MOVE_LEFT
            elif event.key == K_RIGHT:
                direction = MOVE_RIGHT

        elif event.type == KEYUP:
            if event.key == K_LEFT:
                direction = 0
            elif event.key == K_RIGHT:
                direction = 0


    if(direction == MOVE_LEFT):
        x-=0.05
    elif(direction == MOVE_RIGHT):
        x+=0.05

    screen.blit(background, (0, 0))
    screen.blit(player, (x, y))
    pygame.display.update()

