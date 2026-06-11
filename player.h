#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "joystick.h"
#include "util.h"

typedef struct {
    int x, y, existe;
    
    int desmorona;
    int pisado;
    int timer_queda;
} Bloco;

typedef struct{
    int x;
    int y;
    int vida;
    ALLEGRO_BITMAP *sprite;
} Coracao;

void atualizar_blocos(Bloco *blocos);
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
    ALLEGRO_BITMAP *sprite_correndo[6];
    ALLEGRO_BITMAP *sprite_agachado;
    ALLEGRO_BITMAP *sprite_pulando[3];
    ALLEGRO_BITMAP *sprite_parado_direita;
    ALLEGRO_BITMAP *sprite_coracao_cheio;
    ALLEGRO_BITMAP *sprite_coracao_vazio;
    Coracao coracoes[NUM_CORACOES];
} player;

player *player_create(float x, float y);
void player_move(player *p, Bloco *blocos, int n_blocos);
void player_destroy(player *p);

#endif