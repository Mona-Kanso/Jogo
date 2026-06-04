#include "background.h"

void destroi_bloco_de_opcao(bloco_de_opcao *bloco){
    free(bloco);
}

void display_menu_inicial(ALLEGRO_COLOR cor_bloco_opcao, int *main_menu, ALLEGRO_FONT *fonte, int *running, float mouse_x, float mouse_y, int mouse_button){

    bloco_de_opcao *iniciar = inicializa_bloco_de_opcao(cor_bloco_opcao, 350);
    bloco_de_opcao *sair = inicializa_bloco_de_opcao(cor_bloco_opcao, 600);

    al_draw_filled_rectangle(iniciar->x-iniciar->largura/2, iniciar->y-iniciar->altura/2, iniciar->x+iniciar->largura/2, iniciar->y+iniciar->altura/2, cor_bloco_opcao);
    al_draw_filled_rectangle(sair->x-sair->largura/2, sair->y-sair->altura/2, sair->x+sair->largura/2, sair->y+sair->altura/2, cor_bloco_opcao);

    al_draw_text(fonte, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "PASSEANDO POR CURITIBA");

    al_draw_text(fonte, al_map_rgb(255, 255, 255), iniciar->x, iniciar->y - 20, ALLEGRO_ALIGN_CENTER, "Jogar");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), sair->x, sair->y - 20, ALLEGRO_ALIGN_CENTER, "Sair");

    if (mouse_button == 1){

        if(mouse_x >= iniciar->x-iniciar->largura/2 && mouse_x <= iniciar->x+iniciar->largura/2 
        && mouse_y >= iniciar->y-iniciar->altura/2 && mouse_y <= iniciar->y+iniciar->altura/2){
            *main_menu = 0;
        }

        else if(mouse_x >= sair->x-sair->largura/2 && mouse_x <= sair->x+sair->largura/2 
        && mouse_y >= sair->y-sair->altura/2 && mouse_y <= sair->y+sair->altura/2){
            *running = 0;
        }

        destroi_bloco_de_opcao(iniciar);
        destroi_bloco_de_opcao(sair);
    }
}

bloco_de_opcao *inicializa_bloco_de_opcao(ALLEGRO_COLOR cor_bloco_opcao, int coordenada_y){
    bloco_de_opcao *bloco;
    bloco = malloc(sizeof(bloco_de_opcao));
    bloco->x = SCREEN_W/2;
    bloco->y = coordenada_y;
    bloco->largura = 800;
    bloco->altura = 180;

    return bloco;
}


