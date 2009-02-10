#define NUM_FILOSOFOS 5
#define NUM_PLATOS_DE_ARROZ_POR_FILOSOFO 50
int semaforos[NUM_FILOSOFOS]; /* Semaforos para los palitos */
void filosofoNoUltimo(int i);
void filosofoUltimo();
void piensa();
void come();

void filosofoNoUltimo(int i)
{
	/* Cantidad de veces que el filosofo va a comer */
	int contador = NUM_PLATOS_DE_ARROZ_POR_FILOSOFO;
	/* Al iniciar el proceso adquirimos el derecho a utilizar los */
	/* semaforos creados e inicializados por el proceso padre */
	crear_sem(i, 1);
	crear_sem(i + 1, 1);
	while (contador > 0)
	{
		piensa(i);
		/* Tomamos primero el palito de la izquierda y luego el */
		/* de la derecha */
		p_sem(semaforos[i]);
		p_sem(semaforos[i + 1]);
		come(i);
		/* Liberamos los palitos en el orden inverso */
		v_sem(semaforos[i + 1]);
		v_sem(semaforos[i]);
		contador--;
 	}
}

void filosofoUltimo()
{
	/* Cantidad de veces que el filosofo va a comer */
	int contador = NUM_PLATOS_DE_ARROZ_POR_FILOSOFO;
	/* Al iniciar el proceso adquirimos el derecho a utilizar los */
	/* semaforos creados e inicializados por el proceso padre */
	crear_sem(0, 1);
	crear_sem(NUM_FILOSOFOS - 1, 1);
	while (contador > 0)
	{
		piensa(NUM_FILOSOFOS - 1);
		/* Tomamos primero el palito de la derecha y luego el */
		/* de la izquierda */
		p_sem(semaforos[0]);
		p_sem(semaforos[NUM_FILOSOFOS - 1]);
		come(NUM_FILOSOFOS - 1);
		/* Liberamos los palitos en el orden inverso */
		v_sem(semaforos[NUM_FILOSOFOS - 1]);
		v_sem(semaforos[0]);
		contador--;
	}
}

void piensa(int i)
{
	printf("* %i esta pensando.\n", i);
	fflush(0);
	/* Aca iria un algoritmo de pensamiento */
}

void come(int i)
{
	/* Delay, o cantidad de arroces por plato ;) */
	int contador = 1000000;
	printf("* %i esta comiendo.\n", i);
	fflush(0);
	/* Come los arroces */
	while (contador > 0)
		contador--;
}

int main(int argc, char* argv[])
{
	int i;
	/* Creamos tantos semaforos como filosofos haya en la mesa (palitos) */
	for (i = 0; i < NUM_FILOSOFOS; i++)
	Sistemas Operativos - Trabajo Pr´actico Minix - 2da Entrega 17
	semaforos[i] = crear_sem(i, 1);
	/* Creamos los primeros N - 1 filosofos que tienen similar */
	/* comportamiento */
	for (i = 0; i < NUM_FILOSOFOS - 1; i++)
	{
		if (fork() == 0)
		{
			filosofoNoUltimo(i);
			exit(0);
		}
	}
	/* Creamos el ultimo filosofo, el que tiene comportamiento distinto */
	/* a los demas */
	if (fork() == 0)
	{
		filosofoUltimo();
		exit(0);
	}
	/* Esperamos a que todos los filosofos terminen de cenar y */
	/* liberamos los semaforos */
	for (i = 0; i < NUM_FILOSOFOS; i++)
	{
		wait(0);
		liberar_sem(semaforos[i]);
	}
	return 0;
}
