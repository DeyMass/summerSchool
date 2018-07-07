
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

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

//Поток поставщика
pthread_t provider[N_PROVIDERS];

//Потоки покупателей
int demands[N_BUYERS];
pthread_t buyers[N_BUYERS];

//5 складов
int remains[N_STORAGES];
pthread_mutex_t storage_mut[N_STORAGES];

//TODO: графическое отображение
void graphical_update(WINDOW* win);

void provide(int storage_index)
{
    remains[storage_index] += PROV_AMOUNT;
}

void provider_logic(int *myIndex)
{
    int isProvided = 0;
    while(1){
        isProvided = 0;
        for(int storage_index = 0; storage_index < N_STORAGES; storage_index++){
            if (pthread_mutex_trylock(&storage_mut[storage_index]) == 0) {
                provide(storage_index);
                printf("provider %i delivered some goods to %i storage (now it has %i)\n", *myIndex, storage_index, remains[storage_index]);
                isProvided = 1;
                sleep(LOAD_DELAY);
                pthread_mutex_unlock(&storage_mut[storage_index]);
                printf("provider falled asleep for %i secs\n", PROV_SLEEP);
                break;
            }
        }
        if (isProvided) sleep(BUYER_SLEEP);
        else continue;

    }
}

void buy(int storage_index, int *buyer){
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

void buyer_logic(int *myIndex)
{
    while(demands[*myIndex] > 0) {
        for (int storage_index = 0; storage_index < N_STORAGES; storage_index++) {
            if (remains[storage_index] > 0 && pthread_mutex_trylock(&storage_mut[storage_index]) == 0) {
                printf("Buyer %i (want %i) entered storage %i (storage has %i)\n", *myIndex, demands[*myIndex], storage_index, remains[storage_index]);
                buy(storage_index, myIndex);
                sleep(LOAD_DELAY);
                pthread_mutex_unlock(&storage_mut[storage_index]);
                printf("Buyer %i (want %i) exited storage %i (storage has %i)\n", *myIndex, demands[*myIndex], storage_index, remains[storage_index]);
            }
        }
        sleep(BUYER_SLEEP);
    }
}

int main()
{
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