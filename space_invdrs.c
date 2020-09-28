#include <ncurses.h>
#include <stdlib.h>
#include <stddef.h>
#include "space_library.h"
#include <unistd.h>

void inicializa_jogo (t_lista *l, t_lista *t, t_lista *b, t_lista *s, t_lista *n, int nlin, int *mv, int *score){

    /* config ncurses -----------------------------------------------------------------------*/
    initscr();              /* inicia a tela */
    cbreak();               /* desabilita o buffer de entrada */
    noecho();               /* não mostra os caracteres digitados */
    nodelay(stdscr, TRUE);  /* faz com que getch não aguarde a digitação */
    keypad(stdscr, TRUE);   /* permite a leitura das setas */
    curs_set(FALSE);        /* não mostra o cursor na tela */
    clear();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_GREEN);

    /* inicializa listas ---------------------------------------------------------------------*/
    inicializa_lista(l);
    inicializa_lista(t);
    inicializa_lista(b);
    inicializa_lista(s);
    inicializa_lista(n);

    /* inicializa elementos do jogo -----------------------------------------------------------*/
    inicializa_barreira (b, nlin);
    inicializa_aliens(l, nlin);
    inicializa_nave_tiros(nlin, t);

    *mv=0;
    *score=0;
}

void reset_game(t_lista *l, t_lista *t, t_lista *b, t_lista *s, t_lista *n, int nlin){

    destroi_lista(l);
    destroi_lista(t);
    destroi_lista(b);
    destroi_lista(s);
    destroi_lista(n);
    inicializa_lista(l);
    inicializa_lista(t);
    inicializa_lista(b);
    inicializa_lista(s);
    inicializa_lista(n);
    inicializa_barreira (b, nlin);
    inicializa_aliens(l, nlin);
    inicializa_nave_tiros(nlin, t);
}

int menu_inicio (){
     
    int k;

    while (1){
        printa_borda(100, 38);
        wattron(stdscr, COLOR_PAIR(2));
        mvprintw(5,28," _   _   _   _  _              _  __   _  _   _");
		mvprintw(6,28,"|_  |_| |_| |  |_   | |\\| \\ / |_| | ] |_ |_| |_");
		mvprintw(7,28," _| |   | | |_ |_   | | |  v  | | |_] |_ |\\   _|");
        mvprintw(10,20,"      _                      _______                      _");
        mvprintw(11,20,"   _dMMMb._              .adOOOOOOOOOba.              _,dMMMb_");
        mvprintw(12,20,"  dP'  ~YMMb            dOOOOOOOOOOOOOOOb            aMMP~  `Yb");
        mvprintw(13,20,"V      ~\"Mb          dOOOOOOOOOOOOOOOOOb          dM\"~      V");
        mvprintw(14,20,"           `Mb.       dOOOOOOOOOOOOOOOOOOOb       ,dM'");
        mvprintw(15,20,"            `YMb._   |OOOOOOOOOOOOOOOOOOOOO|   _,dMP'");
        mvprintw(16,20,"       __     `YMMM| OP'~\"YOOOOOOOOOOOP\"~`YO |MMMP'     __");
        mvprintw(17,20,"     ,dMMMb.     ~~' OO     `YOOOOOP'     OO `~~     ,dMMMb.");
        mvprintw(18,20,"  _,dP~  `YMba_      OOb      `OOO'      dOO      _aMMP'  ~Yb._");
        mvprintw(19,20,"<MMP'     `~YMMa_   YOOo      OOO      oOOP   _adMP~'      `YMM>");
        mvprintw(20,20,"     ,aa.     `~YMMb `OOOb._,dOOOb._,dOOO'dMMP~'       ,aa.");
        mvprintw(21,20,"    ,dMYYMba._         `OOOOOOOOOOOOOOOOO'          _,adMYYMb.");
        mvprintw(22,20,"   ,MP'   `YMMba._      OOOOOOOOOOOOOOOOO       _,adMMP'   `YM.");
        mvprintw(23,20,"   MP'        ~YMMMba._ YOOOOPVVVVVYOOOOP  _,adMMMMP~       `YM");
        mvprintw(24,20,"   YMb           ~YMMMM\\`OOOOI`````IOOOOO'/MMMMP~           dMP");
        mvprintw(25,20,"    `Mb.           `YMMMb`OOOI,,,,,IOOOO'dMMMP'           ,dM'");
        mvprintw(26,20,"      `'                  `OObNNNNNdOO'                   `'");
        mvprintw(27,20,"                            `~OOOOO~' ");
        
        mvprintw(33, 38, "PRESSIONE 'S' PARA jogar");
        k = getch();

        if(k == 's') {
            return 1;
        }    
    }
}

void menu_bwn(){
    int k;

    while (1){
        erase();
        printa_borda(100, 38);
        wattron(stdscr, COLOR_PAIR(2));
        move(13,38);
        printw("A PROXIMA WAVE SE APROXIMA");
        move(22,40);
        printw("APERTE S PARA INICIAR");
        k = getch();

        if(k == 's')
            return;
    }
}

void menu_fim (int *score){
     
    int k;

    while (1){
        erase();
        printa_borda(100, 38);
        wattron(stdscr, COLOR_PAIR(2));
        move(20,38);
        printw("PRESSIONE 'Q' PARA SAIR");
        move(18,45);
        printw("Score: %i", *score);
        k = getch();

        if(k == 'q') {
            endwin();
            exit(0);
        }    
    }
}

void game (t_lista *l, t_lista *t, t_lista *b, t_lista *s, t_lista *n, int *score, int nlin, int ncol, int *alien_spd){

    int key, tmp=0, mv=0;

    while (1){

        erase();

        /* ------------------------------------------------------------------------------ */
        /* colisao aliens com tiros */
        colisao_tiro_nave (l, t, score);
        /* colisao barreira com bombas */
        colisao_tiro_nave (b, s, score);
        /* colisao barreira com tiro */
        colisao_tiro_nave (b, t, score);
        /* se colisao mae nave com tiro, remove mae nave */
        if (colisao_mae_tiro (t, n, score))
            limpa_lista (n);
        /* colisao barreira com nave */
        colisao_barreira_nave (l, b, score);
        
        if (colisao_nave_alien(l, t, nlin))
            menu_fim(score);

        /* printa ----------------------------------------------------------------------- */
        printa_borda(100, nlin);
        printa_lista (l);
        printa_lista(b);
        printa_lista(n);
        printa_canhao_tiros (t);
        printa_canhao_tiros (s);
        printa_score(score);

        tmp++; /* clock */

        key = getch(); /* le teclado ----------------------------------------------------- */
       
        if(key == 'a') {
            if(t->fim->prev->posx > 0)
                move_nave(t, -1);
        }
        else if (key == 'd') {
            if (t->fim->prev->posx < (100-5))
                move_nave(t, +1);
        }
        else if (key == 'q') {
            menu_fim(score);
        }
        if (t->tamanho < 4){
            if(key == ' '){
                insere_inicio_lista(5, t);
                t->ini->prox->posy = nlin - 2;
                t->ini->prox->posx = t->fim->prev->posx + 2;
            }
        }

        /* controle --------------------------------------------------------------------- */

        if(tmp %10 == 0) /* framerate */
            refresh();

        if (tmp % 75 == 0) /* velocidade bombas */
            move_tiros(s, nlin);

        if (tmp % (*alien_spd) == 0){  /* controle velocidade aliens */
            if (ctrl_bordas_naves (nlin, ncol, &mv, l))
                *alien_spd = *alien_spd - 5;
            move_aliens(mv,l);
        }
        
        if (tmp % 75 == 0){
            move_mae(n);
        }

        if (tmp % 25 == 0) /* velocidade tiros */
            move_tiros(t, nlin);

        if (tmp % 1500 == 0){ /* controle bombas */
            splash(l, s);
        }

        if (tmp == 15000){
            inicializa_mae(n);
            tmp = 0;
        }

        if (colisao_bomba_nave (t, s)){
            menu_fim(score);
        }

        if (lista_vazia(l)) {
            reset_game(l, t, b, s, n, nlin);
            menu_bwn();
            game (l, t, b, s, n, score, nlin, ncol, alien_spd);
        }
        usleep(1000);
    }
}

int main() {
    
    t_lista l, t, b, s, n;
    int nlin, ncol, mv, score, alien_spd;

    nlin = 38;
    ncol = 100;
    alien_spd = 200;

    inicializa_jogo (&l, &t, &b, &s, &n, nlin, &mv, &score);
    menu_inicio ();
    game (&l, &t, &b, &s, &n, &score, nlin, ncol, &alien_spd);
    return 0;
}