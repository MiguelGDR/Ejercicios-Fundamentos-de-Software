#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#define ESTADO_1 1
#define ESTADO_2 2
#define ESTADO_3 3

#define E_SIGUSR1 1
#define E_SIGUSR2 2
#define E_SIGINT  3
#define TIMEOUT 4


/**
 * Funciones auxiliares
 */
char * strEstado(int estado);
char * strEvento(int e);

/**
 * Variable global para notificar al programa principal
 * la llegada del evento
 */
int evento_recibido;

void m1(int signo) {
	evento_recibido = E_SIGUSR1;
	signal(SIGUSR1,m1);
}

void m2(int signo) {
	evento_recibido = E_SIGUSR2;
	signal(SIGUSR2,m2);
}

void m3(int signo) {
	evento_recibido = E_SIGINT;
}

void m4(int signo) {
	evento_recibido = TIMEOUT;
	signal(SIGALRM, m4);
}

int espera_evento() {
	pause();
	return evento_recibido;
}

int main(int argc, char * argv[]) {
	

	//1.- registro de señales
	if (signal(SIGUSR1,m1) == SIG_ERR) {
		perror("signal");
		exit(-1);
	}

	if (signal(SIGUSR2,m2) == SIG_ERR) {
		perror("signal");
		exit(-1);
	}
	
	if (signal(SIGINT,m3) == SIG_ERR) {
		perror("signal");
		exit(-1);
	}

	if (signal(SIGALRM,m4) == SIG_ERR) {
		perror("signal");
		exit(-1);
	}

	int estado = ESTADO_1;
	int evento;
	int fin = 0;

	printf("pid = %d\n", getpid());

	// estructura de datos para setitimer
	
	struct timeval tv_e2, tv_e3, tv_zero;
	tv_e2.tv_sec = 2;
	tv_e2.tv_usec = 500000;
	tv_e3.tv_sec = 3;
	tv_e3.tv_usec = 750000;
	tv_zero.tv_sec = 0;
	tv_zero.tv_usec = 0;
	struct itimerval it_e2, it_e3, it_zero;
	it_e2.it_value = tv_e2;
	it_e2.it_interval = tv_zero;
	it_e3.it_value = tv_e3;
	it_e3.it_interval = tv_zero;
	it_zero.it_value = tv_zero;
	it_zero.it_interval = tv_zero;



	while (fin == 0) {
		printf("Estado: %s. Esperando evento...\n",strEstado(estado));
		evento = espera_evento();
		printf("\t-> evento recibido: %s\n",strEvento(evento));
		switch (estado) {
			case ESTADO_1:
				switch (evento) {
					case E_SIGUSR1: 
						break;
					case E_SIGUSR2:
						estado = ESTADO_2;
						printf("\t-> tránsito a %s\n", strEstado(estado));
						//activamos el temporizador
						if (setitimer(ITIMER_REAL, &it_e2, NULL) < 0) {
							perror("setitimer");
							exit(-1);
						}
						break;
					case E_SIGINT: 
						printf("Saliendo \n");
						fin = 1;
						break;
					default: 
						printf("Evento no esperado: %d\n", evento);
						exit(-1);
				}
 				break;
			
			case ESTADO_2: 
				switch (evento) {
					case E_SIGUSR1:
						estado = ESTADO_1;
						printf("\t-> tránsito a %s\n", strEstado(estado));
						//cancelamos el temporizador
						if (setitimer(ITIMER_REAL, &it_zero, NULL) < 0) {
							perror("setitimer");
							exit(-1);
						}
						break;
					case E_SIGUSR2:
						//reseteamos el temporizador
						if (setitimer(ITIMER_REAL, &it_e2, NULL) < 0) {
							perror("setitimer");
							exit(-1);
						}
						break;
					case TIMEOUT: 
						estado = ESTADO_3;
						printf("\t-> tránsito a %s\n", strEstado(estado));
						//activamos el temporizador para este estado
						if (setitimer(ITIMER_REAL, &it_e3, NULL) < 0) {
							perror("setitimer");
							exit(-1);
						}
						break;
					case E_SIGINT: 
						printf("Saliendo \n");
						fin = 1;
						break;
					default: 
						printf("Evento no esperado: %d\n", evento);
						exit(-1);
				}
 				break;

			case ESTADO_3: 
				switch (evento) {
					case E_SIGUSR1:
						//reseteamos el tiempo
						if (setitimer(ITIMER_REAL, &it_e3, NULL) < 0) {
							perror("setitimer");
							exit(-1);
						}
						break;
					case E_SIGUSR2:
						//transitamos de estado
						estado = ESTADO_1;
						printf("\t-> tránsito a %s\n", strEstado(estado));
						//cancelamos el temporizador
						if (setitimer(ITIMER_REAL, &it_zero, NULL) < 0) {
							perror("setitimer");
							exit(-1);
						}
						break;
					case TIMEOUT:
						//transitamos de estado
						estado = ESTADO_1;
						printf("\t-> tránsito a %s\n", strEstado(estado));
						break;
					case E_SIGINT: 
						printf("Saliendo \n");
						fin = 1;
						break;
					default: 
						printf("Evento no esperado: %d\n", evento);
						exit(-1);
				}
 				break;
			
			default:
				printf("Estado no espera: %d\n", estado);
				exit(-1);
		}

	}

	//Se liberan recursos
	printf("Fin de la máquina\n");
	
	return 0;
}

char * strEstado(int estado) {
	switch(estado) {
		case ESTADO_1: 
			return "ESTADO_1";

		case ESTADO_2: 
			return "ESTADO_2";

		case ESTADO_3: 
			return "ESTADO_3";
		
		default:
			return "Estado no válido";
	}
}

char * strEvento(int e) {
	switch(e) {
		case E_SIGUSR1: 
			return "SIGUSR1";

		case E_SIGUSR2: 
			return "SIGUSR2";
		
		case E_SIGINT: 
			return "SIGINT";

		case TIMEOUT: 
			return "TIMEOUT";
		
		default:
			return "Estado no válido";
	}
}