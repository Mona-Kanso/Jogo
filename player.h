#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "joystick.h"
#include "util.h"

#define MAX_X SCREEN_W
#define MAX_Y SCREEN_H

typedef struct {
    int x, y, existe;
} Bloco;

typedef struct {
    float x, y;
    float vel_y;
    int   esta_no_chao;
    int frame;
    int contador;
    int vidas;
    int agachado;
    int virado_esquerda;
    joystick       *control;
    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *sprite_correndo_direita[6];
    ALLEGRO_BITMAP *sprite_esquerda;
    ALLEGRO_BITMAP *sprite_agachado;
    ALLEGRO_BITMAP *sprite_pulando_direita[3];
    ALLEGRO_BITMAP *sprite_parado_direita;
} player;

player *player_create(float x, float y);
void    player_move(player *p, Bloco *blocos, int n_blocos);
void    player_destroy(player *p);

#endif