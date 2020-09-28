#include <ncurses.h>
#include <stdlib.h>
#include <stddef.h>
#include "space_library.h"
#include <unistd.h>
/*
  Insere o elemento item no início da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/

void move_mae (t_lista *n){
    n->atual = n->ini->prox;
    while (n->atual->prox){
        n->atual->posx++;
        n->atual = n->atual->prox;
    }
}

int colisao_mae_tiro (t_lista *t, t_lista *n, int *score){

    int item;

    t->atual = t->ini->prox;
    while (t->atual->prox){
        n->atual = n->ini->prox;
        while (n->atual->prox){
            if ((t->atual->posx == n->atual->posx) && (t->atual->posy == n->atual->posy)){
                remove_item_atual (&item, t);
                *score = *score + 500;
                return 1;
            }
            if(n->atual->posx == 108)
                return 1;
            n->atual = n->atual->prox;
        }
        t->atual = t->atual->prox;
    }
    return 0;
}

void inicializa_mae (t_lista *n){

    int j, k, pos_x, pos_y;

    /* inicializa nave mae */

    pos_x = -8; pos_y = 2;

    for (j=1; j<=3; j++){
        if (j==1){
            for (k=1; k<=7; k++){
                insere_inicio_lista(8, n);
                n->ini->prox->posx = pos_x + k;
                n->ini->prox->posy = pos_y;
            }
        }
        if (j==2){
            for (k=0; k<=8; k++){
                insere_inicio_lista(8, n);
                n->ini->prox->posx = pos_x + k;
                n->ini->prox->posy = pos_y+1;
            }
        }
        if (j==3){
            for (k=1; k<=7; k++){
                insere_inicio_lista(8, n);
                n->ini->prox->posx = pos_x + k;
                n->ini->prox->posy = pos_y+2;
            }
        }
    }
}

int colisao_nave_alien(t_lista *l, t_lista *t, int nlin){
    
    l->atual = l->ini->prox;
    while (l->atual->prox){
        if (l->atual->posy == nlin - 3)
            return 1;
        if  (l->atual->posy+2 == nlin-4 && 
            ((l->atual->posx == t->fim->prev->posx) ||
            (l->atual->posx+1 == t->fim->prev->posx)||
            (l->atual->posx+2 == t->fim->prev->posx)||
            (l->atual->posx+3 == t->fim->prev->posx)||
            (l->atual->posx+4 == t->fim->prev->posx)))
            return 1;
        l->atual = l->atual->prox;
    }
    return 0;
}

int insere_inicio_lista(int item, t_lista *l){
    t_nodo *new;

    new = malloc(sizeof(t_nodo));
    if(!new) return 0;
    new->prox = l->ini;
    new->prev = NULL;

    l->ini->chave = item;
    l->ini->prev = new;
    l->ini = new;
    
    l->tamanho++;
    return 1;
}

void splash(t_lista *l, t_lista *s){
    int x, i;

    i=0;
    x = rand() % (l->tamanho);
    l->atual = l->ini->prox;
    while ((i <= x) && (l->atual != l->fim->prev)){
        l->atual = l->atual->prox;
        i++;
    }
    if (l->atual->posy != 0){
        insere_inicio_lista(7, s);
        s->ini->posx = l->atual->posx+1;
        s->ini->posy = l->atual->posy+2;
    }
}

void printa_borda (int x, int y){
    int i;

    wattron(stdscr, COLOR_PAIR(1));
    move(0,0);
    printw(" ");
    move(0,100);
    printw(" ");

    for (i=1; i < y; i++){
        move(i,0);
        printw(" ");
        move(i,x);
        printw(" ");
    }

    for (i=1; i < 100; i++){
        move(0,i);
        printw(" ");
        move(y-1,i);
        printw(" ");
    }

}

void move_aliens (int mv, t_lista *l){
    
    l->atual = l->ini->prox;

    while (l->atual->prox){
        if ((l->atual->chave == 1) || (l->atual->chave == 2) || (l->atual->chave == 3)){
            if (mv == 1)
                l->atual->posx++;
            if (mv == 0)
                l->atual->posx--;
        }
        l->atual = l->atual->prox;
    }
}

/* 1 = nave_tipo1 | 2 = nave_tipo2 | 3 = nave_tipo3 | 4 = canhao | 5 = tiros | 6 = barreira | 7 = bomba */

void abaixa_alien (t_lista *l){

    l->atual = l->ini->prox;
    while (l->atual->prox){
        if ((l->atual->chave == 1) || (l->atual->chave == 2) || (l->atual->chave == 3)){
            l->atual->posy++;
        }
        l->atual = l->atual->prox;
    }
}

int ctrl_bordas_naves (int nlin, int ncol, int *mv, t_lista *l){
    
    l->atual = l->ini->prox;
    
    while (l->atual->prox){
        if ((l->atual->chave == 1) || (l->atual->chave == 2) || (l->atual->chave == 3)){
            if (l->atual->posx == 100-5){
                (*mv) = 0;
                abaixa_alien(l);
                return 1;
            }
            if(l->atual->posx == 1){
                (*mv) = 1;
                abaixa_alien(l);
                return 1;
            }
        }
        l->atual = l->atual->prox;
    }
    return 0;
}

void move_nave(t_lista *t, int x){
    t->fim->prev->posx = t->fim->prev->posx + x;
}

int colisao_bomba_nave (t_lista *t, t_lista *s){
    
    s->atual = s->ini->prox;
    while (s->atual->prox){
        if(s->atual->posy == t->fim->prev->posy){
            if  ((s->atual->posx == t->fim->prev->posx) ||
                (s->atual->posx == t->fim->prev->posx+1) ||
                (s->atual->posx == t->fim->prev->posx+2) ||
                (s->atual->posx == t->fim->prev->posx+3) ||
                (s->atual->posx == t->fim->prev->posx+4)){
                    return 1;
            }
        }   
        s->atual = s->atual->prox;
    }
    return 0;
}

void inicializa_nave_tiros(int nlin, t_lista *t){

    insere_inicio_lista(4, t);
    t->ini->prox->posy = nlin-2;
    t->ini->prox->posx = 20;
}

void inicializa_aliens (t_lista *l, int nlin){
    int i, pos_x=7, pos_y=4;
    

    /* inicializa os 55 aliens em seus respectivas posições */
    for (i=1; i<=55; i++) {
        
        if (i<12){
            insere_inicio_lista(1, l); /*1 = alien_1*/
            l->ini->prox->posx = pos_x+1;
            l->ini->prox->posy = pos_y;
            l->ini->prox->state = 1; /* 1 = vivo*/
        }        

        if ((11<i) && (i<34)){
            insere_inicio_lista(2, l); /*1 = alien_1*/
            l->ini->prox->posx = pos_x;
            l->ini->prox->posy = pos_y;
            l->ini->prox->state = 1; /* 1 = vivo*/    
        }   

        if (i>33){
            insere_inicio_lista(3, l); /*1 = alien_1*/
            l->ini->prox->posx = pos_x;
            l->ini->prox->posy = pos_y;
            l->ini->prox->state = 1; /* 1 = vivo*/   
        }
        
        pos_x = pos_x+7;

        if (i%11 == 0){
            pos_x = 7;
            pos_y = pos_y + 4;
        }
    }
}

void inicializa_barreira (t_lista *l, int nlin){
    int i, j, k, pos_x, pos_y;

    /* inicializa as 4 barreiras */

    pos_x = 11; pos_y = nlin - 7;

    for (i=1; i<=5; i++){
            for (j=1; j<=3; j++){
                if (j==1){
                    for (k=1; k<=5; k++){
                        insere_inicio_lista(6, l);
                        l->ini->prox->posx = pos_x + k;
                        l->ini->prox->posy = pos_y;
                    }
                }
                if (j==2){
                    for (k=0; k<=6; k++){
                        insere_inicio_lista(6, l);
                        l->ini->prox->posx = pos_x + k;
                        l->ini->prox->posy = pos_y+1;
                    }
                }
                if (j==3){
                    for (k=0; k<=1; k++){
                        insere_inicio_lista(6, l);
                        l->ini->prox->posx = pos_x + k;
                        l->ini->prox->posy = pos_y+2;
                        insere_inicio_lista(6, l);
                        l->ini->prox->posx = pos_x + k + 5;
                        l->ini->prox->posy = pos_y+2;
                    }
                }
            }
        pos_x = pos_x + 18;
    }

}

void printa_score (int *score){
    move(0, 43);
    wattron(stdscr, COLOR_PAIR(1));
    printw("||Score: %i||", *score);
}

void colisao_tiro_nave (t_lista *l, t_lista *t, int *score){
    int x, y, x_t, y_t, item;

    t->atual = t->ini->prox;
    while (t->atual->prox){
        l->atual = l->ini->prox;
        while (l->atual->prox){
            if (((l->atual->chave == 2) || (l->atual->chave == 3)) && (t->atual->chave == 5)){
                x = l->atual->posx;
                y = l->atual->posy;
                x_t = t->atual->posx;
                y_t = t->atual->posy; 
                if  (((x == x_t) && (y == y_t)) ||
                    ((x == x_t) && (y+1 == y_t)) ||
                    ((x == x_t) && (y+2 == y_t)) ||
                    ((x+4 == x_t) && (y == y_t)) ||
                    ((x+4 == x_t) && (y+1 == y_t)) ||
                    ((x+4 == x_t) && (y+2 == y_t)) ||
                    ((x+1 == x_t) && (y+2 == y_t)) ||
                    ((x+2 == x_t) && (y+2 == y_t)) ||
                    ((x+3 == x_t) && (y+2 == y_t))){
                        /*move(l->atual->posy,l->atual->posx);
                        printw(" \'/ ");
                        move(l->atual->posy+1,l->atual->posx);
                        printw("-   -");
                        move(l->atual->posy+2,l->atual->posx);
                        printw(" /,\\ ");*/
                        remove_item_atual(&item, t);
                        remove_item_atual(&item, l);
                        *score = *score +50;
                    }
            }
            if ((l->atual->chave == 1) && (t->atual->chave == 5)){
                x = l->atual->posx;
                y = l->atual->posy;
                x_t = t->atual->posx;
                y_t = t->atual->posy;
                if  (((x == x_t) && (y == y_t)) ||
                    ((x == x_t) && (y+1 == y_t)) ||
                    ((x == x_t) && (y+2 == y_t)) ||
                    ((x+2 == x_t) && (y == y_t)) ||
                    ((x+2 == x_t) && (y+1 == y_t)) ||
                    ((x+2 == x_t) && (y+2 == y_t)) ||
                    ((x+1 == x_t) && (y+2 == y_t))){
                        remove_item_atual(&item, t);
                        remove_item_atual(&item, l);
                        *score = *score +60;
                    }
            }

            if ((l->atual->chave == 6) && ((t->atual->chave == 5) || t->atual->chave == 7)){
                x = l->atual->posx;
                y = l->atual->posy;
                x_t = t->atual->posx;
                y_t = t->atual->posy;
                if  ((x == x_t) && (y == y_t)){
                        remove_item_atual(&item, t);
                        remove_item_atual(&item, l);
                        *score = *score -20;
                    }
            }
            
            l->atual = l->atual->prox;
        }
        t->atual = t->atual->prox;
    }
}

void colisao_barreira_nave (t_lista *l, t_lista *t, int *score){
    int x, y, x_t, y_t, item;

    t->atual = t->ini->prox;
    while (t->atual->prox){
        l->atual = l->ini->prox;
        while (l->atual->prox){
            if (((l->atual->chave == 2) || (l->atual->chave == 3)) && (t->atual->chave == 6)){
                x = l->atual->posx;
                y = l->atual->posy;
                x_t = t->atual->posx;
                y_t = t->atual->posy; 
                if  (((x == x_t) && (y == y_t)) ||
                    ((x == x_t) && (y+1 == y_t)) ||
                    ((x == x_t) && (y+2 == y_t)) ||
                    ((x+4 == x_t) && (y == y_t)) ||
                    ((x+4 == x_t) && (y+1 == y_t)) ||
                    ((x+4 == x_t) && (y+2 == y_t)) ||
                    ((x+1 == x_t) && (y+2 == y_t)) ||
                    ((x+2 == x_t) && (y+2 == y_t)) ||
                    ((x+3 == x_t) && (y+2 == y_t))){
                        remove_item_atual(&item, t);
                        *score = *score -50;
                    }
            }
            if ((l->atual->chave == 1) && (t->atual->chave == 6)){
                x = l->atual->posx;
                y = l->atual->posy;
                x_t = t->atual->posx;
                y_t = t->atual->posy;
                if  (((x == x_t) && (y == y_t)) ||
                    ((x == x_t) && (y+1 == y_t)) ||
                    ((x == x_t) && (y+2 == y_t)) ||
                    ((x+2 == x_t) && (y == y_t)) ||
                    ((x+2 == x_t) && (y+1 == y_t)) ||
                    ((x+2 == x_t) && (y+2 == y_t)) ||
                    ((x+1 == x_t) && (y+2 == y_t))){
                        remove_item_atual(&item, t);
                        *score = *score -50;
                    }
            }          
            l->atual = l->atual->prox;
        }
        t->atual = t->atual->prox;
    }

}

void move_tiros (t_lista *t, int nlin){
    int item;

    t->atual = t->ini->prox;

    while (t->atual->prox){
        if ((t->atual->chave) == 5){
            if (t->atual->posy == 1)
                remove_item_atual(&item, t);
            else
                t->atual->posy = t->atual->posy - 1;
        }
        if ((t->atual->chave) == 7){
            if (t->atual->posy == nlin - 2)
                remove_item_atual(&item, t);
            else
                t->atual->posy = t->atual->posy + 1;
        }
        t->atual = t->atual->prox;
    }
}

void printa_canhao_tiros (t_lista *t){

    t->atual = t->ini->prox;

    while (t->atual->prox){
        move(t->atual->posy, t->atual->posx);

        if ((t->atual->chave) == 4){
            wattron(stdscr, COLOR_PAIR(2));
            printw("[XXX]");
            move (t->atual->posy-1, t->atual->posx);
            printw(" /^\\ ");
        }   

        if ((t->atual->chave) == 5){
            wattron(stdscr, COLOR_PAIR(2));
            move (t->atual->posy, t->atual->posx);
            printw("|");
        }

        if ((t->atual->chave) == 7){
            if (t->atual->posx != 0){
                wattron(stdscr, COLOR_PAIR(2));
                move (t->atual->posy, t->atual->posx);
                printw("$");
            }
        }
        t->atual = t->atual->prox;
    }
}

void printa_lista (t_lista *l){
    
    l->atual = l->ini->prox;
    while (l->atual->prox){
        move(l->atual->posy, l->atual->posx);

        if ((l->atual->chave) == 1){
            wattron(stdscr, COLOR_PAIR(2));
            printw(" A ");
            move(l->atual->posy+1, l->atual->posx);
            printw("AMA");
            move(l->atual->posy+2, l->atual->posx);
            if (l->atual->posx % 2 == 0)
                printw("/X\\");
            else
                printw("|X|");
            
        }
        if ((l->atual->chave) == 2){
            wattron(stdscr, COLOR_PAIR(2));
            printw(".v_v.");
            move(l->atual->posy+1, l->atual->posx);
            printw("}WMW{");
            move(l->atual->posy+2, l->atual->posx);
            if (l->atual->posx % 2 == 0)
                printw(" / \\ ");
            else
                printw(" | | ");
            
        }

        if ((l->atual->chave) == 3){
            wattron(stdscr, COLOR_PAIR(2));
            printw(" bYd");
            move(l->atual->posy+1, l->atual->posx);
            printw("d-M-b");
            move(l->atual->posy+2, l->atual->posx);
            if (l->atual->posx % 2 == 0)
                printw("_/-\\_");
            else
                printw("\\/-\\/");
        }
        
        if ((l->atual->chave) == 6){
            wattron(stdscr, COLOR_PAIR(3));
            printw(" ");
        }
        
         if ((l->atual->chave) == 8){
            wattron(stdscr, COLOR_PAIR(3));
            printw(" ");
        }

        l->atual = l->atual->prox;    
    }

}

/*
  Cria uma lista vazia. Ela eh duplamente encadeada e tem sentinelas no
  inicio e no final. Tambem tem um apontador para um elemento qualquer.
*/
int inicializa_lista(t_lista *l){
    t_nodo *s1, *s2;
    s1 = malloc(sizeof(t_nodo));
    s2 = malloc (sizeof(t_nodo));
    if (!s1 || !s2) return 0;
    
    s1->prev = NULL;
    s2->prox = NULL;
    s1->prox = s2;
    s2->prev = s1;

    l->ini = s1;
    l->atual = NULL;
    l->fim = s2;
    
    l->tamanho = 0;
    return 1;
}

/*
  Retorna 1 se a lista está vazia e zero caso contrário.
*/
int lista_vazia(t_lista *l){

    if (l->ini->prox == l->fim)
        return 1;
    else
        return 0;
}
/*
  Remove todos os elementos da lista e faz com que ela aponte para NULL.
*/
void destroi_lista(t_lista *l){
    t_nodo *aux;

    while (l->ini){
        aux = l->ini;
        l->ini = l->ini->prox;
        free(aux);
    }
    l->ini = NULL;
    l->fim = NULL;
    l->atual = NULL;
    l->tamanho = 0;

}

/*
  Retorna o tamanho da lista em *tam.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int tamanho_lista(int *tam, t_lista *l){
    *tam = l->tamanho;
    if(!tam)
        return 0;
    else
        return 1;
}
/*
  Insere o elemento item no final da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int insere_fim_lista(int item, t_lista *l){
    t_nodo *aux;
    
    aux = malloc(sizeof(t_nodo));
    aux->prox = NULL;
    aux->chave = 0;
    aux->prev = l->fim;
    l->fim->chave = item;
    l->fim->prox = aux;
    l->fim = aux;
    l->tamanho++;

    if (!aux)
        return 0;
    else
        return 1;    
}
/*
  Insere o elemento item na lista de maneira que ela fique em ordem
  crescente, do início para o final dela.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int insere_ordenado_lista(int item, t_lista *l){
    t_nodo *aux, *new;    

    if (!lista_vazia(l)){
        if (item <= (l->ini->prox->chave)){
            if (insere_inicio_lista(item, l))
                return 1;
            else
                return 0;
        }
        if ((l->fim->prev->chave) <= item){
            if (insere_fim_lista(item, l))
                return 1;
            else
                return 0;
        }

        aux = l->ini->prox->prox;
        while (item < (aux->chave))
            aux = aux->prox;

        new = malloc(sizeof(t_nodo));
        new->chave = item;
        new->prev = aux;
        new->prox = aux->prox;
        aux->prox->prev = new;        
        aux->prox = new;
        l->tamanho++;

        if (!new)
            return 0;
        else
            return 1;
    }
    else
        if (insere_inicio_lista(item, l))
            return 1;
        else
            return 0;
}
/*
  Remove o primeiro elemento da lista e o retorna em *item.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int remove_inicio_lista(int *item, t_lista *l){
    t_nodo *aux;
    
    if (lista_vazia(l))
        return 0;
    
    aux = l->ini->prox;
    aux->prox->prev = l->ini;
    l->ini->prox = aux->prox;
    free(aux);  

    if(!aux)
        return 1;
    else
        return 0;
}
/*
  Remove o último elemento da lista e o retorna em *item.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int remove_fim_lista(int *item, t_lista *l){
    t_nodo *aux;

    if (lista_vazia(l))
        return 0;

    aux = l->fim;
    aux->prev->prox = NULL;
    aux->prev->chave = 0;
    free(aux);

    if(!aux)
        return 1;
    else
        return 0;
}

/*
  Se o elemento chave existir na lista, o retorna em *item.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário
  (elemento não encontrado também retorna zero).
*/
int remove_item_lista(int chave, int *item, t_lista *l){
    int i;
       
    if (lista_vazia(l))
        return 0;      

    l->fim->chave = chave;
    l->atual = l->ini->prox;

    for (i=1 ; l->atual->chave != chave ; i++)
        l->atual = l->atual->prox;

    if (i > (l->tamanho)){
        l->atual->prox->prev = l->atual->prev;
        l->atual->prev->prox = l->atual->prox;
        free(l->atual);
        l->tamanho--;
    }
    return 1;
}
/*
  Retorna 1 se o elemento contendo a chave chave existe na lista,
  caso contrário retorna zero.
*/
int pertence_lista(int chave, t_lista *l){
    int i, tam;    
    t_nodo *aux;
    
    if (lista_vazia(l))
        return 0;

    l->fim->chave = chave;
    i = 0;
    aux = l->ini->prox;
    while ((aux->chave) != chave){
        aux = aux->prox;
        i++;
    }

    tamanho_lista(&tam, l);
    l->fim->chave = 0;
    if (i < tam)
        return 1;
    else
        return 0;
}

/* 
  Inicializa o ponteiro atual para o primeiro elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int inicializa_atual_inicio(t_lista *l){

    if (lista_vazia(l))
        return 0;    

    l->atual = l->ini->prox;
    if (l->atual == l->ini->prox)
        return 1;
    else
        return 0;
}

/* 
  Inicializa o ponteiro atual para o ultimo elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int inicializa_atual_fim(t_lista *l){

    if (lista_vazia(l))
        return 0;

    l->atual = l->fim->prev;
    if(l->atual)
        return 1;
    else
        return 0;
}

/*
  Faz o ponteiro atual apontar para o próximo nodo da lista l e retorna 
  este ponteiro. Se atual estiver apontando para o último, isto é, não 
  tem próximo, retorna NULL.
*/
void incrementa_atual(t_lista *l){

    l->atual = l->atual->prox;
}

/*
  Faz o ponteiro atual apontar para o nodo anterior da lista l e retorna 
  este ponteiro. Se atual estiver apontando para o primeiro, isto é, não 
  tem anterior, retorna NULL.
*/
void decrementa_atual(t_lista *l){

    l->atual = l->atual->prev;
}

/*
  Retorna em *item o valor contido na chave apontada pelo ponteiro atual. 
  Se atual não for válido a função retorna zero senão retorna 1.
*/
int consulta_item_atual(int *item, t_lista *l){

    if (l->atual){
        *item = l->atual->chave;
        return 1;
    }
    else
        return 0;
}

/*
   Remove o elemento apontado por atual da lista l e o retorna em *item.
  Faz o atual apontar para o sucessor do nodo removido.
  Retorna 1 se houve sucesso e zero caso contrário.
*/
int remove_item_atual(int *item, t_lista *l){
    t_nodo *aux;

    if (l->atual){
        if ((l->atual == l->ini) || (l->atual == l->fim)){
            return 0;
        }

        aux = l->atual;
        *item = l->atual->chave;
        aux->prev->prox = aux->prox;
        aux->prox->prev = aux->prev;
        free(aux);
        l->tamanho--;
        return 1;
        }
    else
        return 0;
}

void limpa_lista (t_lista *n){

    int item;

    n->atual = n->ini->prox;
    while(n->atual->prox){
        remove_item_atual(&item, n);
        n->atual = n->atual->prox;
    }
}