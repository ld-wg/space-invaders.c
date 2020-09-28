struct t_nodo {
    int chave;
    int posx;
    int posy;
    int state;
    struct t_nodo *prox;
    struct t_nodo *prev;
};
typedef struct t_nodo t_nodo;

struct t_lista {
    t_nodo *ini;
    t_nodo *atual;
    t_nodo *fim;
    int tamanho;
};

typedef struct t_lista t_lista;

int colisao_mae_tiro (t_lista *t, t_lista *n, int *score);

void move_mae (t_lista *n);

void inicializa_mae(t_lista *n);

int colisao_nave_alien(t_lista *l, t_lista *t, int nlin);

int colisao_bomba_nave (t_lista *t, t_lista *s);

void splash(t_lista *l, t_lista *t);

void colisao_barreira_nave (t_lista *l, t_lista *t, int *score);

void inicializa_barreira (t_lista *l, int nlin);

void abaixa_alien (t_lista *l);

void printa_score (int *score);

void printa_borda (int x, int y);

void move_tiros (t_lista *t, int nlin);

void colisao_tiro_nave (t_lista *l, t_lista *t, int *score);

void inicializa_nave_tiros(int nlin, t_lista *t);

void printa_canhao_tiros (t_lista *t);

void move_nave(t_lista *t, int x);

int ctrl_bordas_naves (int nlin, int ncol, int *mv, t_lista *l);

void move_aliens (int mv, t_lista *l);

void inicializa_aliens (t_lista *l, int nlin);

void limpa_lista (t_lista *n);

/*
  Insere o elemento item no início da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int insere_inicio_lista(int item, t_lista *l);

void printa_lista (t_lista *l);

/*
  Cria uma lista vazia. Ela eh duplamente encadeada e tem sentinelas no
  inicio e no final. Tambem tem um apontador para um elemento qualquer.
*/
int inicializa_lista(t_lista *l);

/*
  Retorna 1 se a lista está vazia e zero caso contrário.
*/
int lista_vazia(t_lista *l);
/*
  Remove todos os elementos da lista e faz com que ela aponte para NULL.
*/
void destroi_lista(t_lista *l);

/*
  Retorna o tamanho da lista em *tam.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int tamanho_lista(int *tam, t_lista *l);
/*
  Insere o elemento item no final da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int insere_fim_lista(int item, t_lista *l);
/*
  Insere o elemento item na lista de maneira que ela fique em ordem
  crescente, do início para o final dela.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int insere_ordenado_lista(int item, t_lista *l);
/*
  Remove o primeiro elemento da lista e o retorna em *item.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int remove_inicio_lista(int *item, t_lista *l);
/*
  Remove o último elemento da lista e o retorna em *item.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int remove_fim_lista(int *item, t_lista *l);

/*
  Se o elemento chave existir na lista, o retorna em *item.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário
  (elemento não encontrado também retorna zero).
*/
int remove_item_lista(int chave, int *item, t_lista *l);
/*
  Retorna 1 se o elemento contendo a chave chave existe na lista,
  caso contrário retorna zero.
*/
int pertence_lista(int chave, t_lista *l);

/* 
  Inicializa o ponteiro atual para o primeiro elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int inicializa_atual_inicio(t_lista *l);

/* 
  Inicializa o ponteiro atual para o ultimo elemento da lista.
  Retorna 1 se a operação foi bem sucedida e zero caso contrário.
*/
int inicializa_atual_fim(t_lista *l);

/*
  Faz o ponteiro atual apontar para o próximo nodo da lista l e retorna 
  este ponteiro. Se atual estiver apontando para o último, isto é, não 
  tem próximo, retorna NULL.
*/
void incrementa_atual(t_lista *l);

/*
  Faz o ponteiro atual apontar para o nodo anterior da lista l e retorna 
  este ponteiro. Se atual estiver apontando para o primeiro, isto é, não 
  tem anterior, retorna NULL.
*/
void decrementa_atual(t_lista *l);

/*
  Retorna em *item o valor contido na chave apontada pelo ponteiro atual. 
  Se atual não for válido a função retorna zero senão retorna 1.
*/
int consulta_item_atual(int *item, t_lista *l);

/*
  Remove o elemento apontado por atual da lista l e o retorna em *item.
  Faz o atual apontar para o sucessor do nodo removido.
  Retorna 1 se houve sucesso e zero caso contrário.
*/
int remove_item_atual(int *item, t_lista *l);

