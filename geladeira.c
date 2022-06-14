#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define SIZE 10

int geladeira; //Inteiro que representa quantos litros de leite tem
sem_t semPai, semMae, semTio; //Semaforos para cada pessoa que monitorara a geladeira

// Retira um litro de leite da geladeira por um determinado tempo
void * BebeLeite (void *arg) {
	for(;;){
		sleep(3);
		geladeira--;
		printf("Bebeu Leite\n");	
	}
}

// As seguintes funçoes sao de cada entidade que analisa a geladeira
void * Pai_Monitorando (void *arg) {
	for(;;){
		sem_wait(&semPai);		//Espera o Semaforo semPai ficar verde
		if (geladeira < 10){
			geladeira++;
			printf("Comprou Leite\n");
		}
		sem_post(&semMae);		//Liberando o Semaforo semMae
	}
}

void * Mae_Monitorando (void *arg) {
	for(;;){
		sem_wait(&semMae);		//Espera o Semaforo semMae ficar verde
		if (geladeira < 10){
			geladeira++;
			printf("Comprou Leite\n");
		}
		sem_post(&semTio)		//Liberando o Semaforo semTio
	}
}

void * Tio_Monitorando (void *arg) {
	for(;;){
		sem_wait(&semTio);		//Espera o Semaforo semTio ficar verde
		if (geladeira < 10){
			geladeira++;
			printf("Comprou Leite\n");
		}
		sem_post(&semPai);		//Liberando o Semaforo semPai
	}
}

int main () {
	geladeira = 2;	
	pthread_t BL, Pai, Mae, Tio;
	
	//Iniciando semaforos, com o semaforo do Pai iniciando verde
	sem_init(&semPai, 0, 1);
	sem_init(&semMae, 0, 0);
	sem_init(&semTio, 0, 0);
	
	pthread_create(&Pai, NULL, Pai_Monitorando, NULL);
	pthread_create(&Mae, NULL, Mae_Monitorando, NULL);
	pthread_create(&Tio, NULL, Tio_Monitorando, NULL);
	pthread_create(&BL, NULL, BebeLeite, NULL);
	
	for (;;) {
		sleep(1);
		if (geladeira <= 10) printf ("Geladeira: %dL\n", geladeira);
		else printf("Geladeira Cheia: %dL\n", geladeira);
	}
	printf ("\n");
	exit(0);	
}
