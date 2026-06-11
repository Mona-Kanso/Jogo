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
} Cogumelos;

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

//Cria a tela de menu
void display_menu_inicial(ALLEGRO_COLOR cor_bloco_opcao, int *main_menu, ALLEGRO_FONT *fonte, int *running, float mouse_x, float mouse_y, int mouse_button);

//Gera os obstáculos do jogo
void gerar_elementos(Bloco *b, Armadilhas *c, Arvores *a, Passaros *p, Cogumelos *m);

//Responsável pelo dimensionamento do sprite e o cálculo de colisão 
void atualizar_e_colidir_arvores(Arvores *a, player *p);

//Atualiza a câmera para acompanhar o personagem principal 
void atualizar_camera(player *p, ALLEGRO_TRANSFORM *cam);

//Gera a tela de game over, informando se o jogador ganhou ou perdeu o jogo
void display_game_over(ALLEGRO_FONT *fonte, int ganhou);

//Cálculo de colisão da armadilha de urso
void colisao_armadilha(Armadilhas *c, player *p);

//Atualiza a coordenada x do pássaro e atualiza o sprite
void atualizar_voo_passaros(Passaros *passaros);

//Cálculo de colisão do pássaro
void colisao_passaro(Passaros *passaros, player *p);

//Cálculo de colisão do cogumelo
void colisao_cogumelo(Cogumelos *cogumelos, player *p);

//Destrói as sprites
void destroi_obstaculos(Arvores *a, Passaros *p, ALLEGRO_BITMAP *sprite_cogumelo, ALLEGRO_BITMAP *sprite_armadilha);

#endif