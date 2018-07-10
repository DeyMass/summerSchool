
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BLOCK_WIDTH  15
#define BLOCK_HEIGHT 6
#define START_DEMAND 10000
#define LOAD_DELAY   2
#define BUY_RESTRICT 150
#define START_AMOUNT 1000
#define PROV_AMOUNT  500
#define PROV_SLEEP   1
#define BUYER_SLEEP  2
#define N_STORAGES   5
#define N_BUYERS     3
#define N_PROVIDERS  2

WINDOW* buyer_w[N_BUYERS];
WINDOW* prov_w[N_PROVIDERS];

//Поток поставщика
pthread_t provider[N_PROVIDERS];

//Потоки покупателей
int demands[N_BUYERS];
pthread_t buyers[N_BUYERS];

//Остатки на складах
int remains[N_STORAGES];
pthread_mutex_t storage_mut[N_STORAGES];

//TODO: графическое отображение
void graphical_update(WINDOW* win)
{
    for(int i = 0; i < N_BUYERS; i++){
        buyer_w[i] = derwin(win, BLOCK_HEIGHT, BLOCK_WIDTH, 0, i*BLOCK_WIDTH);
        box(buyer_w[i], 0, 0);
        wmove(buyer_w[i], 1, 1);
        wprintw(buyer_w[i], "Buyer#%i",i+1);
        wmove(buyer_w[i], 3, 1);
        wprintw(buyer_w[i], "demands:");
        wrefresh(buyer_w[i]);
    }
    for(int i = 0; i < N_PROVIDERS; i++){
        prov_w[i] = derwin(win, BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT + 1, i*BLOCK_WIDTH);
        box(prov_w[i], 0, 0);
        wmove(prov_w[i], 1, 1);
        wprintw(prov_w[i], "Provider #%i",i+1);
        wrefresh(prov_w[i]);
    }
    refresh();
}

void provide(int storage_index)
{
    remains[storage_index] += PROV_AMOUNT;
}

//provider WHO delivered goods to storage WHERE
void print_provider_status(int who, int delivered, int where)
{
    if (delivered){
        wmove(prov_w[who], 2, 1);
        wprintw(prov_w[who], "Sleeps");
        wmove(prov_w[who], 3, 1);
        wprintw(prov_w[who], "             ");
    }else {
        wmove(prov_w[who], 2, 1);
        wprintw(prov_w[who], "gained %i", PROV_AMOUNT);
        wmove(prov_w[who], 3, 1);
        wprintw(prov_w[who], "to st #%i", where);
    }
    refresh();
    wrefresh(prov_w[who]);
}

void provider_logic(int *myIndex)
{
    int isProvided = 0;
    while(1){
        isProvided = 0;
        for(int storage_index = 0; storage_index < N_STORAGES; storage_index++){
            if (pthread_mutex_trylock(&storage_mut[storage_index]) == 0) {
                print_provider_status(*myIndex, 0, storage_index);
                provide(storage_index);
                //printf("provider %i delivered some goods to %i storage (now it has %i)\n", *myIndex, storage_index, remains[storage_index]);
                isProvided = 1;
                sleep(LOAD_DELAY);
                print_provider_status(*myIndex, 1, storage_index);
                pthread_mutex_unlock(&storage_mut[storage_index]);
                //printf("provider falled asleep for %i secs\n", PROV_SLEEP);
                break;
            }
        }
        if (isProvided) sleep(BUYER_SLEEP);
        else continue;

    }
}

void buy(int storage_index, int *buyer)
{
    if (remains[storage_index] > 0) {
        if (remains[storage_index] <= BUY_RESTRICT && demands[*buyer] >= remains[storage_index]){
            demands[*buyer] -= remains[storage_index];
            remains[storage_index] = 0;
            return;
        }else
        if (remains[storage_index] <= BUY_RESTRICT && demands[*buyer] < remains[storage_index]){
            remains[storage_index] -= demands[*buyer];
            demands[*buyer] = 0;
            return;
        }else
        if (remains[storage_index] > BUY_RESTRICT && demands[*buyer] >= BUY_RESTRICT) {
            remains[storage_index] -= BUY_RESTRICT;
            demands[*buyer] -= BUY_RESTRICT;
            return;
        }else
        if (remains[storage_index] > BUY_RESTRICT && demands[*buyer] < BUY_RESTRICT) {
            remains[storage_index] -= demands[*buyer];
            demands[*buyer] = 0;
            return;
        }
    }
}

//expects WHO ISENTERED on WHICH storage
void print_buyer_status(int who, int isEnter, int which)
{
    if(isEnter){
        wmove(buyer_w[who], 2, 1);
        wprintw(buyer_w[who], "on st #%i", which);
    }else{
        wmove(buyer_w[who], 2, 1);
        wprintw(buyer_w[who], "Sleeps       ");
        wrefresh(buyer_w[who]);
        return;
    }
    wmove(buyer_w[who], 4, 1);
    wprintw(buyer_w[who], "        ");
    wmove(buyer_w[who], 4, 1);
    wprintw(buyer_w[who], "%i", demands[who]);
    refresh();
    wrefresh(buyer_w[who]);
}

void buyer_logic(int *myIndex)
{
    while(demands[*myIndex] > 0) {
        for (int storage_index = 0; storage_index < N_STORAGES; storage_index++) {
            if (remains[storage_index] > 0 && pthread_mutex_trylock(&storage_mut[storage_index]) == 0) {
                print_buyer_status(*myIndex, 1, storage_index);
                //printf("Buyer %i (want %i) entered storage %i (storage has %i)\n", *myIndex, demands[*myIndex], storage_index, remains[storage_index]);
                buy(storage_index, myIndex);
                sleep(LOAD_DELAY);
                pthread_mutex_unlock(&storage_mut[storage_index]);
                print_buyer_status(*myIndex, 0, storage_index);
                //printf("Buyer %i (want %i) exited storage %i (storage has %i)\n", *myIndex, demands[*myIndex], storage_index, remains[storage_index]);
            }
        }
        sleep(BUYER_SLEEP);
    }
}

int main()
{
    initscr();
    graphical_update(stdscr);
    for (int i = 0; i < N_STORAGES; i++){
        remains[i] = START_AMOUNT;
        pthread_mutex_init(&storage_mut[i], PTHREAD_MUTEX_NORMAL);
    }
    int provIndex[N_STORAGES];
    for(int i = 0; i < N_PROVIDERS; i++){
        provider[i] = START_AMOUNT;
        provIndex[i] = i;
        pthread_create(&provider[i], NULL, (void*)provider_logic, &provIndex[i]);
    }

    int buyIndex[N_BUYERS];
    for (int i = 0; i < N_BUYERS; i++){
        demands[i] = START_DEMAND;
        buyIndex[i] = i;
        pthread_create(&buyers[i], NULL, (void*)buyer_logic, &buyIndex[i]);
    }
    for(int i = 0; i < N_BUYERS; i++) {
        pthread_join(buyers[i], NULL);
    }

}