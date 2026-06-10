#include <stdlib.h>
#include <stdio.h>
#include "player.h"

player *player_create(float x, float y) {
    player *p = malloc(sizeof(player));
    p->x              = x;
    p->y              = y;
    p->vel_y          = 0.0f;
    p->esta_no_chao   = 1;
    p->control        = joystick_create();
    p->sprite_esquerda = al_load_bitmap("raposaParadaEsquerda.png");
    p->sprite_agachado = al_load_bitmap("raposaAgachada.png");
    p->agachado = 0;

    p->frame = 0;
    p->sprite_parado_direita = al_load_bitmap("raposaParadaDireita.png");
    p->sprite_correndo_direita[0] = al_load_bitmap("raposaCorrendo1.png");
    p->sprite_correndo_direita[1] = al_load_bitmap("raposaCorrendo2.png");
    p->sprite_correndo_direita[2] = al_load_bitmap("raposaCorrendo3.png");
    p->sprite_correndo_direita[3] = al_load_bitmap("raposaCorrendo4.png");
    p->sprite_correndo_direita[4] = al_load_bitmap("raposaCorrendo5.png");
    p->sprite_correndo_direita[5] = al_load_bitmap("raposaCorrendo6.png");
    p->sprite_pulando_direita[0] = al_load_bitmap("raposaPulandoDireita1.png");
    p->sprite_pulando_direita[1] = al_load_bitmap("raposaPulandoDireita2.png");
    p->sprite_pulando_direita[2] = al_load_bitmap("raposaPulandoDireita3.png");
    p->sprite_coracao_cheio = al_load_bitmap("coracaoCheio.png");
    p->sprite_coracao_vazio = al_load_bitmap("coracaoVazio.png");
    p->sprite = p->sprite_parado_direita;
    p->contador = 0;
    p->vidas = 3;
    p->virado_esquerda = 0;
    return p;
}


int checar_colisao_chao(player *p, Bloco *blocos, int n) {
    float pe_x1 = p->x;
    float pe_x2 = p->x + PLAYER_W;
    float pe_y  = p->y + PLAYER_HEIGHT;
    int bx1;
    int bx2;
    int by;
    int sobreposicao_x;
    int caindo_sobre;

    for (int i = 0; i < n; i++) {
        if (!blocos[i].existe) continue;

        bx1 = blocos[i].x;
        bx2 = blocos[i].x + BLOCK_W;
        by  = blocos[i].y;

        sobreposicao_x = pe_x2 > bx1 && pe_x1 < bx2;
        caindo_sobre   = pe_y >= by && pe_y <= by + p->vel_y + 4;

        if (sobreposicao_x && caindo_sobre) {
            p->y = by - PLAYER_HEIGHT;
            return 1;
        }
    }
    return 0;
}

void checar_queda(player *p){

    if(p->y > ALTURA_CHAO + 400){
        p->x = COORDENADA_INICIAL_X;
        p->y = COORDENADA_INICIAL_Y;
        p->esta_no_chao = 1;
        p->coracoes[p->vidas - 1].sprite = p->sprite_coracao_vazio;
        p->vidas--;
        
    }
}

void player_move(player *p, Bloco *blocos, int n_blocos) {

    if (p->control->left)
        p->x -= PLAYER_SPEED;
    if (p->control->right)
        p->x += PLAYER_SPEED;

    if (p->control->up && p->esta_no_chao) {
        p->vel_y        = JUMP_FORCE;
        p->esta_no_chao = 0;
        p->frame        = 0;
        p->contador     = 0;
    }

    p->vel_y += GRAVITY;
    p->y     += p->vel_y;

    if (p->vel_y >= 0 && checar_colisao_chao(p, blocos, n_blocos)) {
        p->vel_y = 0.0f;
        if (!p->esta_no_chao) {
            p->frame    = 0;
            p->contador = 0;
        }
        p->esta_no_chao = 1;
    } else {
        p->esta_no_chao = 0;
    }

if (!p->esta_no_chao) {
        p->contador++;
        if (p->contador >= 8) {
            p->contador = 0;
            p->frame++;
            if (p->frame >= 3){
                p->frame = 2;
            }
        }
        
        if (p->control->left) p->virado_esquerda = 1;
        if (p->control->right) p->virado_esquerda = 0;
        
        p->sprite = p->sprite_pulando_direita[p->frame];

    } else {
        if (p->control->down) {
            p->sprite = p->sprite_agachado;
            p->agachado = 1;

        } else if (p->control->left || p->control->right) {
            p->agachado = 0;
            
            p->virado_esquerda = p->control->left ? 1 : 0; 

            p->contador++;
            if (p->contador >= 6) {
                p->contador = 0;
                p->frame++;
                if (p->frame >= 6){
                    p->frame = 0;
                }
            }
            p->sprite = p->sprite_correndo_direita[p->frame];

        } else {
            p->frame    = 0;
            p->contador = 0;
            p->agachado = 0;
            p->sprite   = p->sprite_parado_direita;
        }
    }
    checar_queda(p);
}

void player_destroy(player *p) {
    joystick_destroy(p->control);
    al_destroy_bitmap(p->sprite_esquerda);
    al_destroy_bitmap(p->sprite_agachado);

    for(int i = 0; i < 5; i++){
        al_destroy_bitmap(p->sprite_correndo_direita[i]);
    }
    free(p);
}