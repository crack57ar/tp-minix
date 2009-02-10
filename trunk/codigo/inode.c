/* inode.c ---- Un programa que muestra por pantalla la informacion
* almacenada en un inodo, ya sea que este apunta a un archivo,
* a un dispositivo de caracteres, de bloques, directorio u otro.
* GRUPO 2 SO - UBA - FCEN - 1er Cuatrimestre de 2005
* Alumnos: Chiocchio - Honore - Tursi
*/
#include <sys/types.h>
#include <sys/stat.h>/* strut stat */
#include <stdio.h>/* printf() */
#include <time.h>/* ctime()*/
#include <stdlib.h>/* exit() y system()*/
/* Mascaras para filtrar permisos de USER */
#define S_ISRWXU(m) (((m) & S_IRWXU) == S_IRWXU)
#define S_ISRUSR(m) (((m) & S_IRUSR) == S_IRUSR)
#define S_ISWUSR(m) (((m) & S_IWUSR) == S_IWUSR)
#define S_ISXUSR(m) (((m) & S_IXUSR) == S_IXUSR)
/* Mascaras para filtrar permisos de GROUP */
#define S_ISRWXG(m) (((m) & S_IRWXG) == S_IRWXG)
#define S_ISRGRP(m) (((m) & S_IRGRP) == S_IRGRP)
#define S_ISWGRP(m) (((m) & S_IWGRP) == S_IWGRP)
#define S_ISXGRP(m) (((m) & S_IXGRP) == S_IXGRP)
/* Mascaras para filtrar permisos de OTHER */
#define S_ISRWXO(m) (((m) & S_IRWXO) == S_IRWXO)
#define S_ISROTH(m) (((m) & S_IROTH) == S_IROTH)
#define S_ISWOTH(m) (((m) & S_IWOTH) == S_IWOTH)
#define S_ISXOTH(m) (((m) & S_IXOTH) == S_IXOTH)
/* Declaracion de funciones */
void showDevice(dev_t data);
void showINodeNumber(ino_t data);
void showModeWord(mode_t data, dev_t deviceType);
void showHardLinks(nlink_t data);
void showUid(uid_t data);
void showGid(gid_t data);
void showSize(off_t data);
void showLastAccess(time_t data);
void showLastDataModification(time_t data);
void showLastInodeModification(time_t data);
void showOwnerPermissionsWith(mode_t data);
void showGroupPermissionsWith(mode_t data);
void showOtherPermissionsWith(mode_t data);
void showInodeTypeWith(mode_t data, dev_t deviceType);
void showInode(struct stat *buffer);

/* Entry point del programa */
int main(int argc, char *argv[])
{
	struct stat buffer;
	if (stat(argv[1],&buffer) == -1)
	{
		perror(argv[0]);
		exit(-1);
	}
	else
	{
		system("clr");
		printf("Sumario del archivo: %s\n", argv[1]);
		printf("------------------------------------------------\n");
		showInode(&buffer);
	}
	return 0;
}

/* Muestra la estructura apuntada en ’buffer’ */
void showInode(struct stat *buffer)
{
	showModeWord(buffer->st_mode, buffer->st_rdev);
	showDevice(buffer->st_dev);
	showINodeNumber(buffer->st_ino);
	showHardLinks(buffer->st_nlink);
	showUid(buffer->st_uid);
	showGid(buffer->st_gid);
	showSize(buffer->st_size);
	showLastAccess(buffer->st_atime);
	showLastDataModification(buffer->st_mtime);
	showLastInodeModification(buffer->st_ctime);
}

/* Muestra todos los datos relacionados con el modo del inodo */
void showModeWord(mode_t mode, dev_t deviceType)
{
	showInodeTypeWith(mode, deviceType);
	printf("* Permisos:\n");
	showOwnerPermissionsWith(mode);
	showGroupPermissionsWith(mode);
	showOtherPermissionsWith(mode);
	printf("* La palabra de modo es: %u\n", mode);
}

/* Muestra de que tipo es el inodo */
void showInodeTypeWith(mode_t mode, dev_t deviceType )
{
	if(S_ISDIR(mode))
		printf("* El inodo apunta a un directorio\n");
	else if(S_ISCHR(mode))
		printf("* El inodo apunta al dispositivo de caracteres %u\n",deviceType);
	else if(S_ISBLK(mode))
		printf("* El inodo apunta al dispositivo de bloques %u\n",deviceType);
	else if(S_ISFIFO(mode))
		printf("* El inodo apunta a un pipe\n");
}

/* Muestra los permisos para el owner del archivo */
void showOwnerPermissionsWith(mode_t mode)
{
	printf("OWNER: ");
	if(S_ISRWXU(mode)) printf("rwx\n");
	else
	{
		if(S_ISRUSR(mode)) printf("r"); else printf("-");
		if(S_ISWUSR(mode)) printf("w"); else printf("-");
		if(S_ISXUSR(mode)) printf("x\n"); else printf("-\n");
	}
}

/* Muestra los permisos para el grupo del archivo */
void showGroupPermissionsWith(mode_t mode)
{
	printf("GROUP: ");
	if(S_ISRWXG(mode)) printf("rwx\n");
	else
	{
		if(S_ISRGRP(mode)) printf("r"); else printf("-");
		if(S_ISWGRP(mode)) printf("w"); else printf("-");
		if(S_ISXGRP(mode)) printf("x\n"); else printf("-\n");
	}
}

/* Muestra los permisos para aquellos usuarios que
no son ni owner ni del grupo del owner */
void showOtherPermissionsWith(mode_t mode)
{
	printf("OTHER: ");
	if(S_ISRWXO(mode)) printf("rwx\n");
	else
	{
		if(S_ISROTH(mode)) printf("r"); else printf("-");
		if(S_ISWOTH(mode)) printf("w"); else printf("-");
		if(S_ISXOTH(mode)) printf("x\n"); else printf("-\n");
	}
}

/* Muestra el ID del dispositivo donde reside el inodo
(ej= diskette, cd-rom, rigido, etc...)*/
void showDevice(dev_t residentDevice)
{
	printf("* El inodo reside fisicamente en el dispositivo: %i\n",residentDevice);
}

/* Muestra el numero del inodo */
void showINodeNumber(ino_t inodeNumber)
{
	printf("* El numero del inodo es: %u\n", inodeNumber);
}

/*Muestra la cantidad de hard links asociados al inodo */
void showHardLinks(nlink_t number)
{
	if(number == 1)
		printf("* El archivo posee 1 hard link\n");
	else
		printf("* El archivo posee %u hard links\n", number);
}

/* Muestra el UserId del inodo */
void showUid(uid_t uid)
{
	printf("* El userId del archivo es: %u\n", uid);
}
/* Muestra el GroupId del inodo */
void showGid(gid_t gid)
{
	printf("* El groupId del archivo es: %u\n", gid);
}

/* Muestra el tamanio en bytes del archivo */
void showSize(off_t size)
{
	printf("* El tamano del archivo es de %u bytes\n", size);
}

/* Muestra la hora de ultimo acceso al archivo */
void showLastAccess(time_t time)
{
	printf("* La hora de ultimo acceso es: %s", ctime(&time));
}

/* Muestra la hora de ultima modificacion de datos */
void showLastDataModification(time_t time)
{
	printf("* La hora de la ultima modificacion de datos es: %s",ctime(&time));
}

/* Muestra la hora de ultima modificacion del inodo */
void showLastInodeModification(time_t time)
{
	printf("* La hora de la ultima modificacion del inodo es: %s",ctime(&time));
}
