CC = gcc            #the compiler
CFLAGS = -g -Wall   #compiler flags

all: level_0 level_1

level_1: level_1.c
	gcc level_1.c -o level_1 -lpthread -lm  

level_0: level_0.c 
	gcc level_0.c -o level_0	

clean: 	
	rm level_0 level_1
