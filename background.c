#include "background.h"

void destroi_bloco_de_opcao(bloco_de_opcao *bloco){
    free(bloco);
}

void atualizar_camera(player *p, ALLEGRO_TRANSFORM *cam) {
    al_identity_transform(cam);
    al_translate_transform(cam,
        -(p->x - TELA_LARGURA / 2.0f),
        -(p->y - TELA_ALTURA / 2.0f));
    al_use_transform(cam);
}

void display_menu_inicial(ALLEGRO_COLOR cor_bloco_opcao, int *main_menu, ALLEGRO_FONT *fonte, int *running, float mouse_x, float mouse_y, int mouse_button){

    bloco_de_opcao *iniciar = inicializa_bloco_de_opcao(cor_bloco_opcao, 350);
    bloco_de_opcao *sair = inicializa_bloco_de_opcao(cor_bloco_opcao, 600);

    al_draw_filled_rectangle(iniciar->x-iniciar->largura/2, iniciar->y-iniciar->altura/2, iniciar->x+iniciar->largura/2, iniciar->y+iniciar->altura/2, cor_bloco_opcao);
    al_draw_filled_rectangle(sair->x-sair->largura/2, sair->y-sair->altura/2, sair->x+sair->largura/2, sair->y+sair->altura/2, cor_bloco_opcao);

    al_draw_text(fonte, al_map_rgb(255, 255, 255), TELA_LARGURA / 2, 100, ALLEGRO_ALIGN_CENTER, "ROLEZANDO COM JUNIMO");

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
        al_draw_text(fonte, al_map_rgb(255, 255, 255), TELA_LARGURA / 2, 100, ALLEGRO_ALIGN_CENTER, "VOCÊ GANHOU");
    } else{
        al_draw_text(fonte, al_map_rgb(255, 255, 255), TELA_LARGURA / 2, 100, ALLEGRO_ALIGN_CENTER, "VOCÊ PERDEU");
    }
}

bloco_de_opcao *inicializa_bloco_de_opcao(ALLEGRO_COLOR cor_bloco_opcao, int coordenada_y){
    bloco_de_opcao *bloco;
    bloco = malloc(sizeof(bloco_de_opcao));
    bloco->x = TELA_LARGURA/2;
    bloco->y = coordenada_y;
    bloco->largura = 800;
    bloco->altura = 180;

    return bloco;
}

void gerar_elementos(Bloco *b, Armadilhas *c, Arvores *a, Passaros *p, Cogumelos *m) {

    ALLEGRO_BITMAP *spriteArvore_01 = al_load_bitmap("sprites/arvore1.png");
    ALLEGRO_BITMAP *spriteArvore_02 = al_load_bitmap("sprites/arvore2.png");
    ALLEGRO_BITMAP *spriteArvore_03 = al_load_bitmap("sprites/arvore3.png");
    ALLEGRO_BITMAP *spriteArvore_04 = al_load_bitmap("sprites/arvore4.png");

    ALLEGRO_BITMAP *spritePassaro_01 = al_load_bitmap("sprites/passarinho1.png");
    ALLEGRO_BITMAP *spritePassaro_02 = al_load_bitmap("sprites/passarinho2.png");
    ALLEGRO_BITMAP *spritePassaro_03 = al_load_bitmap("sprites/passarinho3.png");

    for (int i = 0; i < NUM_BLOCOS; i++) {
        b[i].x = i * BLOCO_LARGURA;
        b[i].y = ALTURA_CHAO;
        c[i].x = i * BLOCO_LARGURA;
        c[i].y = ALTURA_CHAO - ARMADILHA_ALTURA;
        a[i].x = i * BLOCO_LARGURA;
        a[i].y = ALTURA_CHAO - 10;
        c[i].ja_machucou = 0;
        m[i].x = i * BLOCO_ALTURA;
        m[i].y = ALTURA_CHAO - COGUMELO_ALTURA;

        b[i].desmorona = 0;
        b[i].pisado = 0;
        b[i].timer_queda = 0;

        if(i <=1 || !b[i-1].existe || b[i-1].desmorona){
            b[i].existe = 1;
            c[i].existe = 0;
            a[i].existe = 0;
            p[i].existe = 0;
            m[i].existe = 0;
        } else {

            b[i].existe = (rand() % 100 >= 15);
            if(!b[i].existe || !b[i-1].existe){
                b[i].desmorona = (rand() % 100 >= 15);
                b[i].existe = b[i].desmorona;
                p[i].existe = 0;
                c[i].existe = 0;
                m[i].existe = 0;
                a[i].existe = 0;
                a[i].estado_queda = 0;
                a[i].contador_tempo = 0;
                a[i].ja_bateu = 0;
            }

            if(!c[i-1].existe){
                c[i].existe = (rand() % 100 >= DIFICULDADE);
                
                if(!c[i].existe && !a[i-1].existe){
                    a[i].existe = (rand() % 100 >= DIFICULDADE);

                    if(a[i].existe){
                        a[i].sprite_01 = spriteArvore_01;
                        a[i].sprite_02 = spriteArvore_02;
                        a[i].sprite_03 = spriteArvore_03;
                        a[i].sprite_04 = spriteArvore_04;
                        a[i].sprite = a[i].sprite_01;
                        
                        a[i].estado_queda = 0;
                        a[i].contador_tempo = 0;
                        a[i].ja_bateu = 0;

                    }

                    else{
                        m[i].existe = (rand() % 100 >= DIFICULDADE);
                    }
                }
            
            }

            if(p[i-1].existe){
                p[i].existe = 0;
            } else{
                p[i].existe = (rand() % 100 >= DIFICULDADE);
                if(p[i].existe){

                    p[i].sprite_01 = spritePassaro_01;
                    p[i].sprite_02 = spritePassaro_02;
                    p[i].sprite_03 = spritePassaro_03;
                    p[i].sprite_04 = spritePassaro_02;
                    p[i].sprite = p[i].sprite_01;
                    p[i].x = i*BLOCO_LARGURA;
                    p[i].y = ALTURA_CHAO - 160;
                    p[i].ja_bateu = 0;
                    p[i].contador_tempo = 0;
                    
                    p[i].estado_voo = 1;
                }
            }
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
            fator_escala = (PLAYER_ALTURA * 2.0f) / alt_orig_em_pe;

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
            p_right  = p->x + PLAYER_LARGURA;
            p_bottom = p->y + PLAYER_ALTURA;
            p_top = p->y;

            if(p->agachado){
                p_top = p_top + PLAYER_ALTURA - PLAYER_ALTURA_AGACHADO;
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
    int p_right = p->x + PLAYER_LARGURA - 25;
    int p_bottom = p->y + PLAYER_ALTURA + 10;
    int p_top = p->y -10;

    if(p->agachado){
        p_top = p_top + PLAYER_ALTURA - PLAYER_ALTURA_AGACHADO;
    }

    int c_left;
    int c_right;
    int c_top;
    int c_bottom;

    for(int i = 0; i < NUM_BLOCOS; i++){
        if(c[i].existe){
            c_left = c[i].x;
            c_right = c[i].x + ARMADILHA_LARGURA;
            c_top = ALTURA_CHAO - ARMADILHA_ALTURA; 
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

        if(passaros[i].existe){

            passaros[i].x -= 5.0f;
            passaros[i].contador_tempo++;

            if (passaros[i].contador_tempo >= 8) {
                passaros[i].contador_tempo = 0;
                passaros[i].estado_voo++;
                
                if (passaros[i].estado_voo > 4) {
                    passaros[i].estado_voo = 1;
                }

                if (passaros[i].estado_voo == 1){
                    passaros[i].sprite = passaros[i].sprite_01;
                }
                else if (passaros[i].estado_voo == 2){
                    passaros[i].sprite = passaros[i].sprite_02;
                }
                else if (passaros[i].estado_voo == 3){
                    passaros[i].sprite = passaros[i].sprite_03;
                }
                else{ 
                    passaros[i].sprite = passaros[i].sprite_04;
                }
            }
        }
    }
}

void colisao_passaro(Passaros *passaros, player *p) {
    int margem_x_player = 25; 
    int margem_y_player = 10; 
    
    int p_left = p->x + margem_x_player;
    int p_right = p->x + PLAYER_LARGURA - margem_x_player;
    int p_bottom = p->y + PLAYER_ALTURA - margem_y_player;
    int p_top = p->y + margem_y_player;

    if (p->agachado) {
        p_top = (p->y + PLAYER_ALTURA - PLAYER_ALTURA_AGACHADO) + margem_y_player;
    }

    int passaro_left;
    int passaro_right;
    int passaro_top;
    int passaro_bottom;

    int margem_x_passaro = 15;
    int margem_y_passaro = 10;

    for (int i = 0; i < NUM_BLOCOS; i++) {
        if (!passaros[i].existe) continue;

        passaro_left = passaros[i].x + margem_x_passaro;
        passaro_right = passaros[i].x + PASSARO_LARGURA - margem_x_passaro;
        passaro_top = passaros[i].y + margem_y_passaro;
        passaro_bottom = passaros[i].y + PASSARO_ALTURA - margem_y_passaro;

        if (p_right > passaro_left && p_left < passaro_right && p_bottom > passaro_top && p_top < passaro_bottom) {
            
            if (!passaros[i].ja_bateu && p->vidas > 0) {
                p->coracoes[p->vidas - 1].sprite = p->sprite_coracao_vazio;
                p->vidas--;
                passaros[i].ja_bateu = 1;
            }
        }
    }
}

void colisao_cogumelo(Cogumelos *cogumelos, player *p) {
    int p_left = p->x + 25;
    int p_right = p->x + PLAYER_LARGURA - 25;
    int p_bottom = p->y + PLAYER_ALTURA;
    int p_top = p->y;

    if (p->agachado) {
        p_top = p_top + PLAYER_ALTURA - PLAYER_ALTURA_AGACHADO;
    }

    int c_left, c_right, c_top, c_bottom;

    for (int i = 0; i < NUM_BLOCOS; i++) {
        if (cogumelos[i].existe) {
            c_left = cogumelos[i].x;
            c_right = cogumelos[i].x + COGUMELO_LARGURA;
            c_top = ALTURA_CHAO - COGUMELO_ALTURA; 
            c_bottom = ALTURA_CHAO;

            if (p_right > c_left && p_left < c_right && p_bottom > c_top && p_top < c_bottom) {
                
                p->vel_y = -30.0f;
                p->esta_no_chao = 0;
                p->y = c_top - PLAYER_ALTURA;

                p->frame = 0;
                p->contador = 0;
            }
        }
    }
}

void destroi_obstaculos(Arvores *a, Passaros *p, ALLEGRO_BITMAP *sprite_cogumelo, ALLEGRO_BITMAP *sprite_armadilha) {

    for(int i = 0; i < NUM_BLOCOS; i++){
        if(a[i].existe){
            al_destroy_bitmap(a[i].sprite_01);
            al_destroy_bitmap(a[i].sprite_02);
            al_destroy_bitmap(a[i].sprite_03);
            al_destroy_bitmap(a[i].sprite_04);
            break;
        }
    }

    for(int i = 0; i < NUM_BLOCOS; i++){
        if(p[i].existe){
            al_destroy_bitmap(p[i].sprite_01);
            al_destroy_bitmap(p[i].sprite_02);
            al_destroy_bitmap(p[i].sprite_03);
            break;
        }
    }

    al_destroy_bitmap(sprite_cogumelo);
    al_destroy_bitmap(sprite_armadilha);
}
