#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>		
#include <allegro5/allegro_ttf.h>
#include "util.h"
#include "background.h"
#include "player.h"

int main() {

    srand(time(NULL));

    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_TIMER      *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY    *disp  = al_create_display(TELA_LARGURA, TELA_ALTURA);
    ALLEGRO_FONT *fonte_titulo = al_load_ttf_font("fonte.ttf", 72, 0);
    ALLEGRO_BITMAP *sprite_armadilha = al_load_bitmap("sprites/armadilha.png");
    ALLEGRO_BITMAP *sprite_cogumelo = al_load_bitmap("sprites/cogumelo.png");
    ALLEGRO_BITMAP *sprite_background = al_load_bitmap("background.jpg");
    ALLEGRO_TRANSFORM   camera;

    if (!disp) return -1;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_start_timer(timer);

    ALLEGRO_COLOR cor_ceu  = al_map_rgb(100, 149, 237);
    ALLEGRO_COLOR cor_chao = al_map_rgb(139,  69,  19);
    ALLEGRO_COLOR cor_chao_desmorona = al_map_rgb(166, 124, 66);

    Bloco blocos[NUM_BLOCOS];
    Armadilhas armadilhas[NUM_BLOCOS];
    Arvores arvores[NUM_BLOCOS];
    Passaros passaros[NUM_BLOCOS];
    Cogumelos cogumelos[NUM_BLOCOS];
    
    gerar_elementos(blocos, armadilhas, arvores, passaros, cogumelos);

    player *p = player_create(COORDENADA_INICIAL_X, COORDENADA_INICIAL_Y);

    for(int i = 0; i < NUM_CORACOES; i++){
        p->coracoes[i].sprite = p->sprite_coracao_cheio;
        p->coracoes[i].x = 1000 + 80*i;
        p->coracoes[i].y = 20;
    }


    ALLEGRO_EVENT ev;
    int running = 1;
    int main_menu = 1;
    int clicou;
    int game_over = 0;
    int limite_imagem = 0;
    float background_w = al_get_bitmap_width(sprite_background);
    float background_h = al_get_bitmap_height(sprite_background);

    while (running) {
        al_wait_for_event(queue, &ev);

        switch (ev.type) {

        case ALLEGRO_EVENT_TIMER:

            al_clear_to_color(cor_ceu);
            if(game_over){

                display_game_over(fonte_titulo, p->x >= GAME_OVER);
            }
            else if(main_menu){

                ALLEGRO_MOUSE_STATE estado_mouse;
                al_get_mouse_state(&estado_mouse);

                clicou = (estado_mouse.buttons & 1);

                display_menu_inicial(cor_chao, &main_menu, fonte_titulo, &running, estado_mouse.x, estado_mouse.y, clicou);

            } else {
                player_move(p, blocos, NUM_BLOCOS);
                
                atualizar_e_colidir_arvores(arvores, p);
                colisao_armadilha(armadilhas, p);
                colisao_passaro(passaros, p);
                
                
                limite_imagem = ((int)(p->x * 0.5f)) % (int)background_w;

                al_draw_scaled_bitmap(sprite_background, 0, 0, background_w, background_h, 
                                      -limite_imagem, 0, background_w, TELA_ALTURA, 0);
                                      
                al_draw_scaled_bitmap(sprite_background, 0, 0, background_w, background_h, 
                                      -limite_imagem + background_w, 0, background_w, TELA_ALTURA, 0);

  

                atualizar_camera(p, &camera);
                
                for (int i = 0; i < NUM_BLOCOS; i++) {

                    if (blocos[i].existe) {
                        if(blocos[i].desmorona){
                            al_draw_filled_rectangle(
                            blocos[i].x, blocos[i].y,
                            blocos[i].x + BLOCO_LARGURA, blocos[i].y + BLOCO_ALTURA,
                            cor_chao_desmorona);

                        }else {
                            al_draw_filled_rectangle(
                            blocos[i].x, blocos[i].y,
                            blocos[i].x + BLOCO_LARGURA, blocos[i].y + BLOCO_ALTURA,
                            cor_chao);

                        }
                        
                        if (armadilhas[i].existe) {
                            al_draw_scaled_bitmap(sprite_armadilha, 
                                0, 0, 
                                al_get_bitmap_width(sprite_armadilha), 
                                al_get_bitmap_height(sprite_armadilha), 
                                armadilhas[i].x, armadilhas[i].y, ARMADILHA_LARGURA, ARMADILHA_ALTURA, 0);
                        }
                        
                        if (arvores[i].existe && arvores[i].sprite != NULL) {
                            float alt_orig_em_pe = al_get_bitmap_height(arvores[i].sprite_01);
                            float fator_escala = (PLAYER_ALTURA * 1.8f) / alt_orig_em_pe;

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
                        
                        if(cogumelos[i].existe){
                            al_draw_scaled_bitmap(sprite_cogumelo, 
                                0, 0, 
                                al_get_bitmap_width(sprite_cogumelo), 
                                al_get_bitmap_height(sprite_cogumelo), 
                                cogumelos[i].x, cogumelos[i].y, COGUMELO_LARGURA, COGUMELO_ALTURA, 0);
                        }
                    }

                    if(passaros[i].existe){
                        al_draw_scaled_bitmap(passaros[i].sprite, 
                                0, 0, 
                                al_get_bitmap_width(passaros[i].sprite), 
                                al_get_bitmap_height(passaros[i].sprite), 
                                passaros[i].x, passaros[i].y, PASSARO_LARGURA, PASSARO_ALTURA, 0);
                    }
                }


                atualizar_voo_passaros(passaros);
                int flag_espelhamento = p->virado_esquerda ? ALLEGRO_FLIP_HORIZONTAL : 0;

                if(p->agachado){
                    al_draw_scaled_bitmap(p->sprite,
                    0, 0,
                    al_get_bitmap_width(p->sprite),
                    al_get_bitmap_height(p->sprite),
                    p->x, p->y + PLAYER_ALTURA - PLAYER_ALTURA_AGACHADO,
                    PLAYER_LARGURA , PLAYER_ALTURA_AGACHADO, 
                    flag_espelhamento);
                } else{
                    al_draw_scaled_bitmap(p->sprite,
                    0, 0,
                    al_get_bitmap_width(p->sprite),
                    al_get_bitmap_height(p->sprite),
                    p->x, p->y, PLAYER_LARGURA , PLAYER_ALTURA, 
                    flag_espelhamento);
                }
                ALLEGRO_TRANSFORM hud;
                al_identity_transform(&hud);
                al_use_transform(&hud);

                for(int i = 0; i < NUM_CORACOES; i++){

                   al_draw_scaled_bitmap(p->coracoes[i].sprite, 
                                0, 0, 
                                al_get_bitmap_width(p->coracoes[i].sprite), 
                                al_get_bitmap_height(p->coracoes[i].sprite), 
                                p->coracoes[i].x, p->coracoes[i].y, 78, 67, 0);
                }

            }

            atualizar_blocos(blocos);
            colisao_cogumelo(cogumelos, p);
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

        if(p->vidas == 0 || p->x >= GAME_OVER){
            game_over = 1;
        }

    }

    player_destroy(p);
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_font(fonte_titulo);
    al_destroy_bitmap(sprite_background);
    destroi_obstaculos(arvores, passaros, sprite_cogumelo, sprite_armadilha);
}