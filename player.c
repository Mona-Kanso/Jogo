#include <stdlib.h>
#include <stdio.h>
#include "player.h"

void atualizar_blocos(Bloco *blocos) {
    for (int i = 0; i < NUM_BLOCOS; i++) {

        if (blocos[i].existe && blocos[i].desmorona && blocos[i].pisado) {
            
            blocos[i].timer_queda++;

            if (blocos[i].timer_queda > 30) {
                blocos[i].y += 5.0f; 
                if (blocos[i].y > ALTURA_CHAO + 400) {
                    blocos[i].existe = 0;
                }
            }
        }
    }
}

player *player_create(float x, float y) {
    player *p = malloc(sizeof(player));
    p->x              = x;
    p->y              = y;
    p->vel_y          = 0.0f;
    p->esta_no_chao   = 1;
    p->control        = joystick_create();
    p->sprite_agachado = al_load_bitmap("sprites/raposaAgachada.png");
    p->agachado = 0;

    p->frame = 0;
    p->sprite_parado_direita = al_load_bitmap("sprites/raposaParadaDireita.png");
    p->sprite_correndo[0] = al_load_bitmap("sprites/raposaCorrendo1.png");
    p->sprite_correndo[1] = al_load_bitmap("sprites/raposaCorrendo2.png");
    p->sprite_correndo[2] = al_load_bitmap("sprites/raposaCorrendo3.png");
    p->sprite_correndo[3] = al_load_bitmap("sprites/raposaCorrendo4.png");
    p->sprite_correndo[4] = al_load_bitmap("sprites/raposaCorrendo5.png");
    p->sprite_correndo[5] = al_load_bitmap("sprites/raposaCorrendo6.png");
    p->sprite_pulando[0] = al_load_bitmap("sprites/raposaPulandoDireita1.png");
    p->sprite_pulando[1] = al_load_bitmap("sprites/raposaPulandoDireita2.png");
    p->sprite_pulando[2] = al_load_bitmap("sprites/raposaPulandoDireita3.png");
    p->sprite_coracao_cheio = al_load_bitmap("sprites/coracaoCheio.png");
    p->sprite_coracao_vazio = al_load_bitmap("sprites/coracaoVazio.png");
    p->sprite = p->sprite_parado_direita;
    p->contador = 0;
    p->vidas = 3;
    p->virado_esquerda = 0;
    return p;
}


int checar_colisao_chao(player *p, Bloco *blocos, int n) {
    float px1 = p->x;
    float px2 = p->x + PLAYER_LARGURA;
    float py  = p->y + PLAYER_ALTURA;
    int bx1;
    int bx2;
    int by;
    int sobreposicao_x;
    int caindo_sobre;

    for (int i = 0; i < n; i++) {
        if(blocos[i].existe){

            bx1 = blocos[i].x;
            bx2 = blocos[i].x + BLOCO_LARGURA;
            by  = blocos[i].y;

            sobreposicao_x = px2 > bx1 && px1 < bx2;
            caindo_sobre   = py >= by && py <= by + p->vel_y + 4;

            if (sobreposicao_x && caindo_sobre) {
                p->y = by - PLAYER_ALTURA;
                if (blocos[i].desmorona && !blocos[i].pisado) {
                    blocos[i].pisado = 1;
                }
                return 1;
            }
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
        p->x -= VELOCIDADE_JOGADOR;
    if (p->control->right)
        p->x += VELOCIDADE_JOGADOR;

    if (p->control->up && p->esta_no_chao) {
        p->vel_y  = FORCA_PULO;
        p->esta_no_chao = 0;
        p->frame = 0;
        p->contador = 0;
    }

    p->vel_y += GRAVIDADE;
    p->y     += p->vel_y;

    if (p->vel_y >= 0 && checar_colisao_chao(p, blocos, n_blocos)) {
        p->vel_y = 0.0f;
        if (!p->esta_no_chao) {
            p->frame = 0;
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
        
        p->sprite = p->sprite_pulando[p->frame];

    } else {
        if (p->control->down) {
            p->sprite = p->sprite_agachado;
            p->agachado = 1;

        } else if (p->control->left || p->control->right) {
            p->agachado = 0;
            
            if(p->control->left){
                p->virado_esquerda = 1;
            } else{
                p->virado_esquerda = 0;
            }

            p->contador++;
            if (p->contador >= 6) {
                p->contador = 0;
                p->frame++;
                if (p->frame >= 6){
                    p->frame = 0;
                }
            }
            p->sprite = p->sprite_correndo[p->frame];

        } else {
            p->frame = 0;
            p->contador = 0;
            p->agachado = 0;
            p->sprite = p->sprite_parado_direita;
        }
    }
    checar_queda(p);
}

void player_destroy(player *p) {
    joystick_destroy(p->control);
    al_destroy_bitmap(p->sprite_agachado);
    al_destroy_bitmap(p->sprite_coracao_cheio);
    al_destroy_bitmap(p->sprite_coracao_vazio);

    for(int i = 0; i < 6; i++){
        al_destroy_bitmap(p->sprite_correndo[i]);
    }

    for(int i = 0; i < 3; i++){
        al_destroy_bitmap(p->sprite_pulando[i]);
    }
    free(p);
}