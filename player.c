#include <stdlib.h>
#include "player.h"

player *player_create(float x, float y) {
    player *p = malloc(sizeof(player));
    p->x              = x;
    p->y              = y;
    p->vel_y          = 0.0f;
    p->esta_no_chao   = 0;
    p->control        = joystick_create();
    p->sprite_esquerda = al_load_bitmap("raposaParadaEsquerda.png");
    p->sprite_agachado = al_load_bitmap("raposaAgachada.png");

    /* Vale a pena fazer uma lista circular aqui*/
    p->frame = 0;
    p->sprite_parado_direita = al_load_bitmap("raposaParadaDireita.png");
    p->sprite_correndo_direita[0] = al_load_bitmap("raposaCorrendo1.png");
    p->sprite_correndo_direita[1] = al_load_bitmap("raposaCorrendo2.png");
    p->sprite_correndo_direita[2] = al_load_bitmap("raposaCorrendo3.png");
    p->sprite_correndo_direita[3] = al_load_bitmap("raposaCorrendo4.png");
    p->sprite_correndo_direita[4] = al_load_bitmap("raposaCorrendo5.png");
    p->sprite = p->sprite_parado_direita;
    return p;
}


int checar_colisao_chao(player *p, Bloco *blocos, int n) {
    float pe_x1 = p->x;
    float pe_x2 = p->x + PLAYER_W;
    float pe_y  = p->y + PLAYER_HEIGHT;

    for (int i = 0; i < n; i++) {
        if (!blocos[i].existe) continue;

        int bx1 = blocos[i].x;
        int bx2 = blocos[i].x + BLOCK_W;
        int by  = blocos[i].y;

        int sobreposicao_x = pe_x2 > bx1 && pe_x1 < bx2;
        int caindo_sobre   = pe_y >= by && pe_y <= by + p->vel_y + 4;

        if (sobreposicao_x && caindo_sobre) {
            p->y = by - PLAYER_HEIGHT;
            return 1;
        }
    }
    return 0;
}

void player_move(player *p, Bloco *blocos, int n_blocos) {
    /* Movimento horizontal */
    if (p->control->left) {
        p->x -= PLAYER_SPEED;
        p->sprite = p->sprite_esquerda;
    } else if (p->control->right) {
        p->x += PLAYER_SPEED;
        p->sprite = p->sprite_correndo_direita[p->frame];
        p->frame++;
        if(p->frame >= 5){
            p->frame = 0;
        }
    } else if (p->control->down) {
        p->sprite = p->sprite_agachado;
    }

    /* Pulo */
    else if (p->control->up && p->esta_no_chao) {
        p->vel_y        = JUMP_FORCE;
        p->esta_no_chao = 0;
    }

    /* Gravidade */
    p->vel_y += GRAVITY;
    p->y     += p->vel_y;

    /* Colisão com blocos */
    if (p->vel_y >= 0 && checar_colisao_chao(p, blocos, n_blocos)) {
        p->vel_y        = 0.0f;
        p->esta_no_chao = 1;
    } else {
        p->esta_no_chao = 0;
    }
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