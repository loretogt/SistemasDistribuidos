#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "zerocopyMQ.h"

int main(int argc, char *argv[]) {
    int n, lncola;
    char *op, *cola;
    do {
        printf("\nIntroduzca la longitud del nombre de la cola (ctrl-D para terminar): ");
        n=scanf("%d", &lncola);
        if (n==EOF) continue;
        if (n==0) {
		scanf("%*s");
		continue;
	}
        printf("\nSeleccione nombre de cola y operación (ctrl-D para terminar)\n\tC:create|D:destroy|P:put\n\tG:get no bloqueante|B:get bloqueante\n");
        op=malloc(2);
	cola=malloc(lncola+1);
	// posible buffer overflow al leer el nombre de la cola
        n=scanf("%s%1s", cola, op);
        if (n==2) {
            switch(op[0]) {
                case 'C':
                    if (createMQ(cola)<0)
                        printf("error creando la cola %s\n", cola);
                    else
                        printf("cola %s creada correctamente\n", cola);
                    break;
                case 'D':
                    if (destroyMQ(cola)<0)
                        printf("error eliminando la cola %s\n", cola);
                    else
                        printf("cola %s eliminada correctamente\n", cola);
                    break;
                case 'G': 
                case 'B': ;
                    uint32_t tam;
                    void *mensaje;
                    if (get(cola, &mensaje, &tam, (op[0]=='B')?true:false)<0)
                        printf("error leyendo de la cola %s\n", cola);
                    else {
                        printf("lectura de la cola %s correcta\n", cola);
                        if (tam) {
                            char *fich=malloc(255+1);
                            int fd;
                            do {
                                printf("Introduzca el nombre del fichero para almacenar el mensaje: ");
                                n=scanf("%""255""s", fich);
                                fd = creat(fich, 0644);
                                free(fich);
		            } while (fd == -1);
                            write(fd, mensaje, tam);
                            close(fd);
                            free(mensaje);
                        }
                        else printf("no hay mensajes\n");
                    }
                    break;
                case 'P': ;
                    char *fich=malloc(255+1);
                    int fd;
                    struct stat st;
                    void *f;
                    do {
                        printf("Introduzca el nombre del fichero que contiene el mensaje: ");
                        n=scanf("%""255""s", fich);
                        fd = open(fich, O_RDONLY);
                        free(fich);
		    } while (fd == -1);
                    fstat(fd, &st);
                    f=mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
                    close(fd);
                    if (put(cola, f, st.st_size)<0)
                        printf("error escribiendo en la cola %s\n", cola);
                    else
                        printf("escritura en la cola %s correcta\n", cola);
                    munmap(f, st.st_size);
                    break;
                default:
                    printf("operación no válida\n");
            }
            free(cola);
            free(op);
        }
    } while (n!=EOF);
    return 0;
}

