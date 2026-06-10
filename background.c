#include "background.h"

void destroi_bloco_de_opcao(bloco_de_opcao *bloco){
    free(bloco);
}

void atualizar_camera(player *p, ALLEGRO_TRANSFORM *cam) {
    al_identity_transform(cam);
    al_translate_transform(cam,
        -(p->x - SCREEN_W / 2.0f),
        -(p->y - SCREEN_H / 2.0f));
    al_use_transform(cam);
}

void display_menu_inicial(ALLEGRO_COLOR cor_bloco_opcao, int *main_menu, ALLEGRO_FONT *fonte, int *running, float mouse_x, float mouse_y, int mouse_button){

    bloco_de_opcao *iniciar = inicializa_bloco_de_opcao(cor_bloco_opcao, 350);
    bloco_de_opcao *sair = inicializa_bloco_de_opcao(cor_bloco_opcao, 600);

    al_draw_filled_rectangle(iniciar->x-iniciar->largura/2, iniciar->y-iniciar->altura/2, iniciar->x+iniciar->largura/2, iniciar->y+iniciar->altura/2, cor_bloco_opcao);
    al_draw_filled_rectangle(sair->x-sair->largura/2, sair->y-sair->altura/2, sair->x+sair->largura/2, sair->y+sair->altura/2, cor_bloco_opcao);

    al_draw_text(fonte, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "ROLEZANDO COM JUNIMO");

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

    }
    destroi_bloco_de_opcao(iniciar);
    destroi_bloco_de_opcao(sair);
}

void display_game_over(ALLEGRO_FONT *fonte, int ganhou){

    if(ganhou){
        al_draw_text(fonte, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "VOCÊ GANHOU");
    } else{
        al_draw_text(fonte, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "VOCÊ PERDEU");
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

void gerar_elementos(Bloco *b, Armadilhas *c, Arvores *a, Passaros *p) {

    ALLEGRO_BITMAP *spriteArvore_01 = al_load_bitmap("arvore1.png");
    ALLEGRO_BITMAP *spriteArvore_02 = al_load_bitmap("arvore2.png");
    ALLEGRO_BITMAP *spriteArvore_03 = al_load_bitmap("arvore3.png");
    ALLEGRO_BITMAP *spriteArvore_04 = al_load_bitmap("arvore4.png");

    ALLEGRO_BITMAP *spritePassaro_01 = al_load_bitmap("passarinho1.png");
    ALLEGRO_BITMAP *spritePassaro_02 = al_load_bitmap("passarinho2.png");
    ALLEGRO_BITMAP *spritePassaro_03 = al_load_bitmap("passarinho3.png");

    for (int i = 0; i < NUM_BLOCOS; i++) {
        b[i].x = i * BLOCK_W;
        b[i].y = ALTURA_CHAO;
        c[i].x = i * BLOCK_W;
        c[i].y = ALTURA_CHAO - ARMADILHA_HEIGHT;
        a[i].x = i * BLOCK_W;
        a[i].y = ALTURA_CHAO - 10;
        c[i].ja_machucou = 0;

        if(i <=1 || !b[i-1].existe){
            b[i].existe = 1;
            c[i].existe = 0;
            a[i].existe = 0;
            p[i].existe = 0;
        } else {
            b[i].existe = (rand() % 100 >= 15);

            if(!b[i].existe || !b[i-1].existe || c[i-1].existe){
                c[i].existe = 0;
            } else {
                c[i].existe = (rand() % 100 >= 80);
            }

            if (b[i-1].existe && b[i].existe && !c[i-1].existe && !c[i].existe) {
                a[i].existe = (rand() % 100 >= 90);
                a[i].sprite_01 = spriteArvore_01;
                a[i].sprite_02 = spriteArvore_02;
                a[i].sprite_03 = spriteArvore_03;
                a[i].sprite_04 = spriteArvore_04;
                a[i].sprite = a[i].sprite_01;
                
                a[i].estado_queda = 0;
                a[i].contador_tempo = 0;
                a[i].ja_bateu = 0;
            } else {
                a[i].existe = 0;
                a[i].estado_queda = 0;
                a[i].contador_tempo = 0;
                a[i].ja_bateu = 0;
            }

            p[i].existe = (rand() % 100 >= 82);
            p[i].sprite_01 = spritePassaro_01;
            p[i].sprite_02 = spritePassaro_02;
            p[i].sprite_03 = spritePassaro_03;
            p[i].sprite_04 = spritePassaro_02;
            p[i].sprite = p[i].sprite_01;
            p[i].x = i*BLOCK_W;
            p[i].y = ALTURA_CHAO - 160; // Lembre-se do ajuste de altura que conversamos!
            p[i].ja_bateu = 0;
            p[i].contador_tempo = 0;
            
            p[i].estado_voo = 1;
        }

    }
}

void atualizar_e_colidir_arvores(Arvores *a, player *p) {

    float alt_orig_em_pe;
    float fator_escala;
    float p_left;
    float p_right;
    float p_top;
    float p_bottom;
    float larg_arvore_atual;
    float alt_arvore_atual;
    float distancia_x;
    float a_left;
    float a_right;
    float a_top;
    float a_bottom;

    for (int i = 0; i < NUM_BLOCOS; i++) {
        if (a[i].existe){

            alt_orig_em_pe = al_get_bitmap_height(a[i].sprite_01);
            fator_escala = (PLAYER_HEIGHT * 2.0f) / alt_orig_em_pe;

            distancia_x = p->x - a[i].x;
            if (distancia_x < 0){
                distancia_x = -distancia_x;
            }

            if (a[i].estado_queda == 0 && distancia_x < 180.0f) {
                a[i].estado_queda = 1;
            }

            if (a[i].estado_queda > 0 && a[i].estado_queda < 4) {
                a[i].contador_tempo++;
                
                if (a[i].contador_tempo >= 15) {
                    a[i].estado_queda++;
                    a[i].contador_tempo = 0;

                    if (a[i].estado_queda == 1){
                        a[i].sprite = a[i].sprite_01;
                    }

                    else if (a[i].estado_queda == 2){
                        a[i].sprite = a[i].sprite_02;
                    }
                    else if (a[i].estado_queda == 3){
                        a[i].sprite = a[i].sprite_03;
                    }
                    else{
                        a[i].sprite = a[i].sprite_04;
                    }
                }
            }

            p_left   = p->x;
            p_right  = p->x + PLAYER_W;
            p_bottom = p->y + PLAYER_HEIGHT;
            p_top = p->y;

            if(p->agachado){
                p_top = p_top + PLAYER_HEIGHT - PLAYER_HEIGHT_AGACHADO;
            }

            larg_arvore_atual = al_get_bitmap_width(a[i].sprite) * fator_escala;
            alt_arvore_atual  = al_get_bitmap_height(a[i].sprite) * fator_escala;

            a_left   = a[i].x;
            a_right  = a[i].x + larg_arvore_atual;
            a_top    = ALTURA_CHAO - alt_arvore_atual;
            a_bottom = ALTURA_CHAO;

            if (p_right > a_left && p_left < a_right && p_bottom > a_top && p_top < a_bottom) {
                if (a[i].estado_queda > 2 && a[i].estado_queda < 4 && !a[i].ja_bateu) {
                    p->coracoes[p->vidas - 1].sprite = p->sprite_coracao_vazio;
                    p->vidas--;
                    a[i].ja_bateu = 1;
                }
            }
        }
    }
}

void colisao_armadilha(Armadilhas *c, player *p){
    int p_left = p->x + 25;
    int p_right = p->x + PLAYER_W - 25;
    int p_bottom = p->y + PLAYER_HEIGHT + 10;
    int p_top = p->y -10;

    if(p->agachado){
        p_top = p_top + PLAYER_HEIGHT - PLAYER_HEIGHT_AGACHADO;
    }

    int c_left;
    int c_right;
    int c_top;
    int c_bottom;

    for(int i = 0; i < NUM_BLOCOS; i++){
        if(c[i].existe){
            c_left = c[i].x;
            c_right = c[i].x + ARMADILHA_W;
            c_top = ALTURA_CHAO - ARMADILHA_HEIGHT; 
            c_bottom = ALTURA_CHAO;

            if (p_right > c_left && p_left < c_right && p_bottom > c_top && p_top < c_bottom && !c[i].ja_machucou) {
                p->coracoes[p->vidas - 1].sprite = p->sprite_coracao_vazio;
                p->vidas--;
                c[i].ja_machucou = 1;
            }
        }
    }
}

void atualizar_voo_passaros(Passaros *passaros) {
    for (int i = 0; i < NUM_BLOCOS; i++) {
        if (!passaros[i].existe) continue;

        passaros[i].x -= 5.0f; // Ajuste este valor para deixar o pássaro mais rápido ou devagar

        passaros[i].contador_tempo++;
        if (passaros[i].contador_tempo >= 8) { // A cada 8 frames, muda a asa
            passaros[i].contador_tempo = 0;
            passaros[i].estado_voo++;
            
            // Loop da animação (1 -> 2 -> 3 -> 4 -> 1)
            if (passaros[i].estado_voo > 4) {
                passaros[i].estado_voo = 1;
            }

            // Atualiza o sprite atual
            if (passaros[i].estado_voo == 1) passaros[i].sprite = passaros[i].sprite_01;
            else if (passaros[i].estado_voo == 2) passaros[i].sprite = passaros[i].sprite_02;
            else if (passaros[i].estado_voo == 3) passaros[i].sprite = passaros[i].sprite_03;
            else passaros[i].sprite = passaros[i].sprite_04;
        }
    }
}

void colisao_passaro(Passaros *passaros, player *p) {
    // 1. FOLGA DO JOGADOR
    int margem_x_player = 25; 
    int margem_y_player = 10; 
    
    int p_left = p->x + margem_x_player;
    int p_right = p->x + PLAYER_W - margem_x_player;
    int p_bottom = p->y + PLAYER_HEIGHT - margem_y_player;
    int p_top = p->y + margem_y_player;

    // Se estiver agachado, o topo desce
    if (p->agachado) {
        p_top = (p->y + PLAYER_HEIGHT - PLAYER_HEIGHT_AGACHADO) + margem_y_player;
    }

    int passaro_left;
    int passaro_right;
    int passaro_top;
    int passaro_bottom;

    // 2. FOLGA DO PÁSSARO
    int margem_x_passaro = 15;
    int margem_y_passaro = 10;

    for (int i = 0; i < NUM_BLOCOS; i++) {
        if (!passaros[i].existe) continue;

        // Assumindo que você definiu PASSARO_W e PASSARO_HEIGHT no seu .h
        passaro_left = passaros[i].x + margem_x_passaro;
        passaro_right = passaros[i].x + PASSARO_W - margem_x_passaro;
        passaro_top = passaros[i].y + margem_y_passaro;
        passaro_bottom = passaros[i].y + PASSARO_HEIGHT - margem_y_passaro;

        // Checa se as caixas se sobrepõem
        if (p_right > passaro_left && p_left < passaro_right && p_bottom > passaro_top && p_top < passaro_bottom) {
            
            // Trava para não tomar dano contínuo (Hit Kill)
            if (!passaros[i].ja_bateu && p->vidas > 0) {
                p->coracoes[p->vidas - 1].sprite = p->sprite_coracao_vazio;
                p->vidas--;
                passaros[i].ja_bateu = 1;
            }
        }
    }
}