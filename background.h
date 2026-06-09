#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>	
#include "util.h"

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
} Cercas;

typedef struct {
    float x, y;
    int existe;
    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *sprite_01;
    ALLEGRO_BITMAP *sprite_02;
    ALLEGRO_BITMAP *sprite_03;
    ALLEGRO_BITMAP *sprite_04;
    
    // NOVAS VARIÁVEIS DE CONTROLE:
    int estado_queda;    // 0 = Em pé, 1 = Começou a cair, 2 = No meio, 3 = Quase no chão, 4 = Totalmente caída
    int contador_tempo;  // Controla a velocidade da animação (quantos frames do timer dura cada sprite)
    int ja_bateu;        // Garante que o print "caiu" aconteça apenas UMA vez por queda
} Arvores;

bloco_de_opcao *inicializa_bloco_de_opcao();

void display_menu_inicial(ALLEGRO_COLOR cor_bloco_opcao, int *main_menu, ALLEGRO_FONT *fonte, int *running, float mouse_x, float mouse_y, int mouse_button);