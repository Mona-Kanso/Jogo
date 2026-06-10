#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>	
#include "util.h"
#include "player.h"

typedef struct{
    int x;
    int y;
    int largura;
    int altura;
} bloco_de_opcao;

typedef struct{
    int x;
    int y;
    int existe;
    int ja_machucou;
} Armadilhas;

typedef struct {
    float x, y;
    int existe;
    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *sprite_01;
    ALLEGRO_BITMAP *sprite_02;
    ALLEGRO_BITMAP *sprite_03;
    ALLEGRO_BITMAP *sprite_04;
    
    int estado_queda; 
    int contador_tempo;
    int ja_bateu;
} Arvores;

typedef struct {
    float x, y;
    int existe;
    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *sprite_01;
    ALLEGRO_BITMAP *sprite_02;
    ALLEGRO_BITMAP *sprite_03;
    ALLEGRO_BITMAP *sprite_04;

    int ja_bateu;
    int contador_tempo;
    int estado_voo;
} Passaros;

bloco_de_opcao *inicializa_bloco_de_opcao();

void display_menu_inicial(ALLEGRO_COLOR cor_bloco_opcao, int *main_menu, ALLEGRO_FONT *fonte, int *running, float mouse_x, float mouse_y, int mouse_button);

void gerar_elementos(Bloco *b, Armadilhas *c, Arvores *a, Passaros *p);

void atualizar_e_colidir_arvores(Arvores *a, player *p);

void atualizar_camera(player *p, ALLEGRO_TRANSFORM *cam);

void display_game_over(ALLEGRO_FONT *fonte, int ganhou);

void colisao_armadilha(Armadilhas *c, player *p);

void atualizar_voo_passaros(Passaros *passaros);

void colisao_passaro(Passaros *passaros, player *p);

#endif