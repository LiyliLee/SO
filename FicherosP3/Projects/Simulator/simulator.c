#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N_PARADAS 5 // número de paradas de la ruta
#define EN_RUTA 0 // autobús en ruta
#define EN_PARADA 1 // autobús en la parada
#define MAX_USUARIOS 40 // capacidad del autobús
#define USUARIOS 4 // numero de usuarios

// estado inicial
int estado = EN_RUTA;

int parada_actual = 0; // parada en la que se encuentra el autobus

int n_ocupantes = 0; // ocupantes que tiene el autobús

// personas que desean subir en cada parada
int esperando_parada[N_PARADAS]; //= {0,0,...0};

// personas que desean bajar en cada parada
int esperando_bajar[N_PARADAS]; //= {0,0,...0};

// Otras definiciones globales (comunicación y sincronización)
pthread_t autobus;
pthread_t usuarios[USUARIOS];

pthread_cond_t busParado;
pthread_cond_t busMoviendo;

pthread_mutex_t mBus;
pthread_mutex_t mSubir;
pthread_mutex_t mBajar;

void Autobus_En_Parada(){
	estado = EN_PARADA;
	printf("Personas que quieren subir %d \n", esperando_parada[parada_actual]);
	printf("Personas que quieren bajar %d \n", esperando_bajar[parada_actual]);

	pthread_cond_broadcast(&busParado);
	pthread_mutex_lock(&mBus);

	//Subidas y bajadas
	while(esperando_parada[parada_actual] != 0 || esperando_bajar[parada_actual] != 0){
		pthread_cond_wait(&busMoviendo, &mBus);
	}

	pthread_mutex_unlock(&mBus);
	estado = EN_RUTA;
}

void Conducir_Hasta_Siguiente_Parada(){
	printf("Autobus conduciendo hacia la siguiente parada \n");

	//Hacer un sleep para el trayecto
    	sleep(random() % 10);

	//Actualizar el numero de parada
	parada_actual = (parada_actual + 1) % N_PARADAS;
    	printf("Autobus ha llegado a la siguiente parada: %d\n", parada_actual);
}

void Subir_Autobus(int id_usuario, int origen){
	
	pthread_mutex_lock(&mSubir);

	printf("El usuario %d se quiere subir en esta parada %d \n", id_usuario, origen);


	esperando_parada[origen] = esperando_parada[origen] + 1;

	while(parada_actual != origen){
		pthread_cond_wait(&busParado, &mSubir);
	}

	esperando_parada[parada_actual] = esperando_parada[parada_actual] -1;
	if(esperando_parada[parada_actual] == 0)
		pthread_cond_broadcast(&busMoviendo);

	printf("El usuario %d ha subido al autobus\n", id_usuario);

	pthread_mutex_unlock(&mSubir); 

}
void Bajar_Autobus(int id_usuario, int destino){
	
	pthread_mutex_lock(&mBajar);

	printf("El usuario %d se quiere bajar en esta parada %d \n", id_usuario, destino);

	esperando_bajar[destino] = esperando_bajar[destino] + 1;

	while(parada_actual != destino){
		pthread_cond_wait(&busParado, &mBajar);
	}

	esperando_bajar[parada_actual] = esperando_bajar[parada_actual] -1;
	if(esperando_bajar[parada_actual] == 0)
		pthread_cond_broadcast(&busMoviendo);

	printf("El usuario %d ha bajado del autobus\n", id_usuario);

	pthread_mutex_unlock(&mBajar); 
}

void * thread_autobus(void * args) {
	while (1) {
		// esperar a que los viajeros suban y bajen
		Autobus_En_Parada();
		// conducir hasta siguiente parada
		Conducir_Hasta_Siguiente_Parada();
	}
}

void Usuario(int id_usuario, int origen, int destino) {
	// Esperar a que el autobus esté en parada origen para subir
	Subir_Autobus(id_usuario, origen);
	// Bajarme en estación destino
	Bajar_Autobus(id_usuario, destino);
}

void * thread_usuario(void * arg) {
	int id_usuario = (int)arg;
	int a, b;
	// obtener el id del usario
	while (1) {
		a=rand() % N_PARADAS;
		do{
			b=rand() % N_PARADAS;
		} while(a==b);
		Usuario(id_usuario,a,b);
	}
}



int main(int argc, char* argv[])
{
	int i;
	// Definición de variables locales a main
	// Opcional: obtener de los argumentos del programa la capacidad del
	// autobus, el numero de usuarios y el numero de paradas

	// Crear el thread Autobus
	pthread_create(&autobus, NULL, thread_autobus, NULL); //MIRAR LO DE VOID I
	// Crear thread para el usuario i
	for(i=0; i<USUARIOS; i++) 
		pthread_create(&usuarios[i], NULL, thread_usuario, (void*)i);

	// Esperar terminación de los hilos   
	for(i=0; i<USUARIOS; i++) 
		pthread_join(usuarios[i],NULL);

	pthread_join(autobus, NULL);


	return 0;
}

