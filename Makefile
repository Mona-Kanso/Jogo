CC = gcc
CFLAGS = -Wall -g

ALLEGROFLAGS = -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

all: game

game: main.c player.c joystick.c background.c
	$(CC) $(CFLAGS) main.c player.c joystick.c background.c -o game $(ALLEGROFLAGS)

clean:
	rm -f game *.o