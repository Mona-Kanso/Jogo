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

bloco_de_opcao *inicializa_bloco_de_opcao();

void display_menu_inicial(ALLEGRO_COLOR cor_bloco_opcao, int *main_menu, ALLEGRO_FONT *fonte, int *running, float mouse_x, float mouse_y, int mouse_button);