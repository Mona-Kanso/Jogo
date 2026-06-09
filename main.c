#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>		
#include <allegro5/allegro_ttf.h>
#include "util.h"
#include "player.h"
#include "background.h"

/* ------------------------------------------------------------------ */
void atualizar_camera(player *p, ALLEGRO_TRANSFORM *cam) {
    al_identity_transform(cam);
    al_translate_transform(cam,
        -(p->x - SCREEN_W / 2.0f),
        -(p->y - SCREEN_H / 2.0f));
    al_use_transform(cam);
}

void atualizar_e_colidir_arvores(Arvores *a, int n, player *p) {
    for (int i = 0; i < n; i++) {
        if (!a[i].existe) continue;

        float alt_orig_em_pe = al_get_bitmap_height(a[i].sprite_01);
        float fator_escala = (PLAYER_HEIGHT * 2.0f) / alt_orig_em_pe;

        float distancia_x = p->x - a[i].x;
        if (distancia_x < 0) distancia_x = -distancia_x;

        if (a[i].estado_queda == 0 && distancia_x < 180.0f) {
            a[i].estado_queda = 1;
        }

        if (a[i].estado_queda > 0 && a[i].estado_queda < 4) {
            a[i].contador_tempo++;
            
            if (a[i].contador_tempo >= 15) {
                a[i].estado_queda++;
                a[i].contador_tempo = 0;

                if (a[i].estado_queda == 1) a[i].sprite = a[i].sprite_01;
                if (a[i].estado_queda == 2) a[i].sprite = a[i].sprite_02;
                if (a[i].estado_queda == 3) a[i].sprite = a[i].sprite_03;
                if (a[i].estado_queda == 4) a[i].sprite = a[i].sprite_04;
            }
        }

        float p_left   = p->x;
        float p_right  = p->x + PLAYER_W;
        float p_top    = p->agachado ? (p->y + PLAYER_HEIGHT - PLAYER_HEIGHT_AGACHADO) : p->y;
        float p_bottom = p->y + PLAYER_HEIGHT;

        float larg_arvore_atual = al_get_bitmap_width(a[i].sprite) * fator_escala;
        float alt_arvore_atual  = al_get_bitmap_height(a[i].sprite) * fator_escala;

        float a_left   = a[i].x;
        float a_right  = a[i].x + larg_arvore_atual;
        float a_top    = ALTURA_CHAO - alt_arvore_atual; // Rente ao chão
        float a_bottom = ALTURA_CHAO;

        if (p_right > a_left && p_left < a_right && p_bottom > a_top && p_top < a_bottom) {
            if (a[i].estado_queda > 0 && !a[i].ja_bateu) {

                a[i].ja_bateu = 1;
            }
        }
    }
}

void gerar_elementos(Bloco *b, Cercas *c, Arvores *a, int n) {

    ALLEGRO_BITMAP *sprite_01 = al_load_bitmap("arvore1.png");
    ALLEGRO_BITMAP *sprite_02 = al_load_bitmap("arvore2.png");
    ALLEGRO_BITMAP *sprite_03 = al_load_bitmap("arvore3.png");
    ALLEGRO_BITMAP *sprite_04 = al_load_bitmap("arvore4.png");
    for (int i = 0; i < n; i++) {
        b[i].x = i * BLOCK_W;
        b[i].y = ALTURA_CHAO;
        c[i].x = i * BLOCK_W;
        c[i].y = ALTURA_CHAO - 95;
        a[i].x = i * BLOCK_W;
        a[i].y = ALTURA_CHAO - 10;

        if(i <=1 || !b[i-1].existe){
            b[i].existe = 1;
            c[i].existe = 0;
            a[i].existe = 0;
        } else{
            b[i].existe = (rand()%100 >= 15);

            if(!b[i].existe || !b[i-1].existe || c[i-1].existe){
                c[i].existe = 0;
            } else{
                c[i].existe = (rand()%100 >= 80);
            }

            if (b[i-1].existe && b[i].existe) {
                        a[i].existe = (rand()%100 >= 90);
                        a[i].sprite_01 = sprite_01;
                        a[i].sprite_02 = sprite_02;
                        a[i].sprite_03 = sprite_03;
                        a[i].sprite_04 = sprite_04;
                        a[i].sprite = a[i].sprite_01;
                        
                        // INICIALIZAÇÃO:
                        a[i].estado_queda = 0;
                        a[i].contador_tempo = 0;
                        a[i].ja_bateu = 0;
                    } else {
                        a[i].existe = 0;
                        a[i].estado_queda = 0;
                        a[i].contador_tempo = 0;
                        a[i].ja_bateu = 0;
                    }
                }
    }
}

/* ------------------------------------------------------------------ */
int main() {
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_TIMER      *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY    *disp  = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_FONT *fonte_titulo = al_load_ttf_font("fonte.ttf", 72, 0);
    ALLEGRO_BITMAP *sprite_cerca = al_load_bitmap("cerca.png");
    ALLEGRO_TRANSFORM   camera;

    if (!disp) return -1;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_start_timer(timer);

    ALLEGRO_COLOR cor_ceu  = al_map_rgb(100, 149, 237);
    ALLEGRO_COLOR cor_chao = al_map_rgb(139,  69,  19);

    Bloco blocos[NUM_BLOCOS];
    Cercas cercas[NUM_BLOCOS];
    Arvores arvores[NUM_BLOCOS];
    gerar_elementos(blocos, cercas, arvores, NUM_BLOCOS);


    player *p = player_create(COORDENADA_INICIAL_X, COORDENADA_INICIAL_Y);

    ALLEGRO_EVENT ev;
    int running = 1;
    int main_menu = 1;
    int clicou;

    while (running) {
        al_wait_for_event(queue, &ev);

        switch (ev.type) {

        case ALLEGRO_EVENT_TIMER:

            al_clear_to_color(cor_ceu);
            if(main_menu){

                ALLEGRO_MOUSE_STATE estado_mouse;
                al_get_mouse_state(&estado_mouse);

                clicou = (estado_mouse.buttons & 1);

                display_menu_inicial(cor_chao, &main_menu, fonte_titulo, &running, estado_mouse.x, estado_mouse.y, clicou);

            } else {
                // 1. Atualiza lógica de movimentação do jogador
                player_move(p, blocos, NUM_BLOCOS);
                
                // 2. ATUALIZA QUEDA E COLISÃO DAS ÁRVORES (NOVO)
                atualizar_e_colidir_arvores(arvores, NUM_BLOCOS, p);
                
                atualizar_camera(p, &camera);
                
                for (int i = 0; i < NUM_BLOCOS; i++) {
                    if (blocos[i].existe) {
                        al_draw_filled_rectangle(
                            blocos[i].x, blocos[i].y,
                            blocos[i].x + BLOCK_W, blocos[i].y + BLOCK_H,
                            cor_chao);
                        
                        if (cercas[i].existe) {
                            al_draw_scaled_bitmap(sprite_cerca, 
                                0, 0, 
                                al_get_bitmap_width(sprite_cerca), 
                                al_get_bitmap_height(sprite_cerca), 
                                cercas[i].x, cercas[i].y, 112, 95, 0);
                        }
                        
                        // DESENHO DA ÁRVORE COM ESCALA CORRIGIDA E DINÂMICA
                        if (arvores[i].existe && arvores[i].sprite != NULL) {
                            float alt_orig_em_pe = al_get_bitmap_height(arvores[i].sprite_01);
                            float fator_escala = (PLAYER_HEIGHT * 1.8f) / alt_orig_em_pe;

                            float larg_final = al_get_bitmap_width(arvores[i].sprite) * fator_escala;
                            float alt_final = al_get_bitmap_height(arvores[i].sprite) * fator_escala;
                            float y_rente_ao_chao = ALTURA_CHAO - alt_final;

                            al_draw_scaled_bitmap(
                                arvores[i].sprite, 
                                0, 0, al_get_bitmap_width(arvores[i].sprite), al_get_bitmap_height(arvores[i].sprite),
                                arvores[i].x, y_rente_ao_chao, larg_final, alt_final, 
                                0
                            );
                        }                                
                    }
                }
                        
                int flag_espelhamento = p->virado_esquerda ? ALLEGRO_FLIP_HORIZONTAL : 0;

                if(p->agachado){
                    al_draw_scaled_bitmap(p->sprite,
                    0, 0,
                    al_get_bitmap_width(p->sprite),
                    al_get_bitmap_height(p->sprite),
                    p->x, p->y + PLAYER_HEIGHT - PLAYER_HEIGHT_AGACHADO,
                    PLAYER_W , PLAYER_HEIGHT_AGACHADO, 
                    flag_espelhamento); // <--- AQUI
                } else{
                    al_draw_scaled_bitmap(p->sprite,
                    0, 0,
                    al_get_bitmap_width(p->sprite),
                    al_get_bitmap_height(p->sprite),
                    p->x, p->y, PLAYER_W , PLAYER_HEIGHT, 
                    flag_espelhamento); // <--- E AQUI
                }
                ALLEGRO_TRANSFORM hud;
                al_identity_transform(&hud);
                al_use_transform(&hud);

            }


            al_flip_display();
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:  p->control->left  = 1; break;
            case ALLEGRO_KEY_RIGHT: p->control->right = 1; break;
            case ALLEGRO_KEY_UP:    p->control->up    = 1; break;
            case ALLEGRO_KEY_DOWN:  p->control->down  = 1; break;
            }
            break;

        case ALLEGRO_EVENT_KEY_UP:
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:  p->control->left  = 0; break;
            case ALLEGRO_KEY_RIGHT: p->control->right = 0; break;
            case ALLEGRO_KEY_UP:    p->control->up    = 0; break;
            case ALLEGRO_KEY_DOWN:  p->control->down  = 0; break;
            }
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = 0;
            break;
        }
    }

    player_destroy(p);
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_font(fonte_titulo);
    return 0;
}