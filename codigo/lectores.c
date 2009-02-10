#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
/*DELAYS*/
#define DELAY_ENTRE_ESCRITORES 10000000
#define DELAY_ENTRE_LECTORES 1000000
#define DELAY_ESCRIBIR 1
#define DELAY_LEER 2000000
#define SEM_EXCLUSION 1
#define SEM_BUFFER 2
#define SEM_CANT_LECTORES 3
const int CANT_LECTORES = 5;
const int CANT_ESCRITORES = 2;
const char ARCHIVO[] = "buffer";

void leer_buffer(void);
void escribir_buffer(void);
void delay(int valor);

void lector(void){
	/*Si ya existen los sem´aforos no actualiza sus valores*/
	int exclusion = crear_sem(SEM_EXCLUSION,1);
	int sem_buffer = crear_sem(SEM_BUFFER,1);
	int cant_lectores = crear_sem(SEM_CANT_LECTORES, 0);
	p_sem(exclusion);/* entro en region critica */
	v_sem(cant_lectores);/* incremento la cantidad de lectores */
	printf("[%i] Cantidad de lectores=%i\n",getpid(),val_sem(cant_lectores));
	fflush(0);
	/* el primer lector cierra el buffer*/
	if(val_sem(cant_lectores) == 1){
		p_sem(sem_buffer);
		printf("[%i] Primer lector cierra el buffer\n", getpid());
		fflush(0);
	}
	v_sem(exclusion);/* salgo de region critica */
	leer_buffer();
	p_sem(exclusion); /* entro a una nueva region critica */
	p_sem(cant_lectores); /*un lector menos,nunca sera negativo*/
	/* si no hay mas lectores abro el buffer */
	if(val_sem(cant_lectores) <= 0){
		printf("[%i] Se fue el ultimo lector, abre el buffer\n", getpid());
		fflush(0);
		v_sem(sem_buffer);
	}
	v_sem(exclusion);
}

void escritor(void){
	int sem_buffer = crear_sem(SEM_BUFFER,1);
	printf("[%i] Escritor intenta escribir...\n", getpid());
	fflush(0);
	p_sem(sem_buffer);
	escribir_buffer();
	v_sem(sem_buffer);
}

int crear_escritores(void){
	int i=0;
	for(i=0 ; i < CANT_ESCRITORES ; i++){
		if(fork()==0){
			escritor();
			_exit(0);
		}else{
			delay(DELAY_ENTRE_ESCRITORES);
		}
	}
	for(i=0; i< CANT_ESCRITORES; i++) wait(0);
	return(0);
}

int darTiempoParaEscribir(int nro_lector){
	int cantLectoresPorEscritor = CANT_LECTORES / CANT_ESCRITORES;
	while(nro_lector>0){
		nro_lector = nro_lector - cantLectoresPorEscritor;
	}
	return nro_lector==0;
}

int crear_lectores(void){
	int i=0;
	for(i=0; i< CANT_LECTORES; i++){
		if(fork()==0){
			lector();
			_exit(0);
		}
		if(darTiempoParaEscribir(i)){
			delay(9000000);
		}else{
			delay(DELAY_ENTRE_LECTORES);
		}
	}
	for(i=0; i< CANT_LECTORES; i++) wait(0);
	return 0;
}

int main(int argc, char* argv[]){
	int i = 0;
	open(ARCHIVO, O_CREAT);
	escribir_buffer();
	if (fork() == 0)
	{
		crear_escritores();
		_exit(0);
	}
	else
	{
		crear_lectores();
	}
	wait(0);
	/*Uso crear_sem() para no usar variables globales,*/
	/*asi los referencio por su nombre*/
	liberar_sem(crear_sem(SEM_EXCLUSION,9));
	liberar_sem(crear_sem(SEM_BUFFER,9));
	liberar_sem(crear_sem(SEM_CANT_LECTORES,9));
	return 0;
}

void escribir_buffer(void){
	int fd = open(ARCHIVO, O_WRONLY);
	int pid = getpid();
	write(fd, &pid, 3);
	close(fd);
	printf("[%i] Escribiendo dato: %i\n", pid, pid);
	fflush(0);
	delay(DELAY_ESCRIBIR);
}

void leer_buffer(void){
	int fd = open(ARCHIVO, O_RDONLY);
	int leido;
	read(fd, &leido, 3);
	close(fd);
	printf("[%i] Leyendo dato: %i\n", getpid(), leido);
	fflush(0);
	delay(DELAY_LEER);
}

void delay(int valor){
	while(valor-- > 0);
}