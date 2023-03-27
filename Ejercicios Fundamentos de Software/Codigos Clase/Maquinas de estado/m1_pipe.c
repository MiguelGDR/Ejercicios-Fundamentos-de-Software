#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define ESTADO_1 0
#define ESTADO_2 1

#define EVENTO_1 1
#define EVENTO_2 2
#define EVENTO_SALIR 3

/**
 * Funciones auxiliares
 */
char * strEstado(int estado);
char * strEvento(int e);

// Pipe
int p[2];

void m1(int signo) 
{
	int evento = EVENTO_1;
	if(write(p[1], &evento, sizeof(int)) != sizeof(int))
	{
		perror("write ev1");
		exit(1);
	}
	signal(SIGUSR1,m1);
}

void m2(int signo) 
{
	int evento = EVENTO_2;
	if(write(p[1], &evento, sizeof(int)) != sizeof(int))
	{
		perror("write ev2");
		exit(1);
	}
	signal(SIGUSR2,m2);
}

void m3(int signo) 
{
	int evento = EVENTO_SALIR;
	if(write(p[1], &evento, sizeof(int)) != sizeof(int))
	{
		perror("write ev1");
		exit(1);
	}
}

int espera_evento() 
{
	int evento, leidos;
	// Bloqueo el proceso a la espera de una señal

	do
	{
		leidos = read(p[0], &evento, sizeof(int));

		if(leidos < 0)
		{
			if(errno != EINTR)
			{
				perror("read");
				exit(1);
			}
		}
	}
	while(leidos != sizeof(int));
	
	return evento;
}

int main(int argc, char * argv[]) 
{
	//1.- registro de señales
	if (signal(SIGUSR1,m1) == SIG_ERR) 
	{
		perror("signal");
		exit(-1);
	}

	if (signal(SIGUSR2,m2) == SIG_ERR) 
	{
		perror("signal");
		exit(-1);
	}
	
	if (signal(SIGINT,m3) == SIG_ERR) 
	{
		perror("signal");
		exit(-1);
	}

	if(pipe(p) < 0)
	{
		perror("pipe");
		exit(1);
	}

	int estado = ESTADO_1;
	int evento;
	int fin = 0;

	printf("pid = %d\n", getpid());

	while (fin == 0) 
	{
		printf("Estado: %s. Esperando evento...\n",strEstado(estado));
		evento = espera_evento();
		printf("\t-> evento recibido: %s\n",strEvento(evento));
		switch (estado) 
		{
			case ESTADO_1:
				switch (evento) 
				{
					case EVENTO_1: 
						break;
					case EVENTO_2:
						estado = ESTADO_2;
						printf("\t-> tránsito a %s\n", strEstado(estado));
						break;
					case EVENTO_SALIR: 
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
					case EVENTO_1:
						estado = ESTADO_1;
						printf("\t-> tránsito a %s\n", strEstado(estado));
						break;
					case EVENTO_2:
						break;
					case EVENTO_SALIR: 
						printf("Saliendo \n");
						fin = 1;
						break;
					default: 
						printf("Evento no esperado: %d\n", evento);
						exit(-1);
				}
 				break;
			
			default:
				printf("Estado no esperado: %d\n", estado);
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
		
		default:
			return "Estado no válido";
	}
}

char * strEvento(int e) {
	switch(e) {
		case EVENTO_1: 
			return "SIGUSR1";

		case EVENTO_2: 
			return "SIGUSR2";
		
		case EVENTO_SALIR: 
			return "SIGINT";
		
		default:
			return "Evento no válido";
	}
}