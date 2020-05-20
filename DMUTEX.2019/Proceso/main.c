
/* DMUTEX (2009) Sistemas Operativos Distribuidos
 * C�digo de Apoyo
 *
 * ESTE C�DIGO DEBE COMPLETARLO EL ALUMNO:
 *    - Para desarrollar las funciones de mensajes, reloj y
 *      gesti�n del bucle de tareas se recomienda la implementaci�n
 *      de las mismas en diferentes ficheros.
 */

/*
ssh y16i010@triqui.fi.upm.es
scp Proceso/main.c y16i010@triqui3.fi.upm.es:DATSI/SD/DMUTEX.2019/Proceso

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>


int puerto_udp;

#define MAX_NUM_PROC 10

typedef struct proceso{
  char *nombre; 
  int puerto;
  //struct proceso *next; 
}Process;

typedef struct mensaje{
  int tipo; 
  int posEnReloj;
  int relojes[MAX_NUM_PROC];
  char secCritica[MAX_NUM_PROC];
}Mensaje;

typedef struct sc{
  char *nombre; 
  int pEsp[MAX_NUM_PROC];
  int dentro;
  int quieroEntrar;
  int numOK;
  int relojAlpedir[MAX_NUM_PROC];
  int numesp;
}SC;


int main(int argc, char* argv[])
{
  int port, s, np;
  int num=0;
  int nSecCrit=0;
  struct sockaddr_in dir;
  char line[80],proc[80];
  Process *pArray;
  SC *scArray;

  if(argc<2)
  {
    fprintf(stderr,"Uso: proceso <ID>\n");
    return 1;
  }

  /* Establece el modo buffer de entrada/salida a l�nea */
  setvbuf(stdout,(char*)malloc(sizeof(char)*80),_IOLBF,80);
  setvbuf(stdin,(char*)malloc(sizeof(char)*80),_IOLBF,80);

  //creacion del socket
  if ((s=socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("error creando socket");
		return 1;
	}
  bzero(&dir,sizeof(dir));
  dir.sin_addr.s_addr=INADDR_ANY;
	dir.sin_family=AF_INET;
  //dir.sin_port=0;
  if (bind(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error en bind");
		close(s);
		return 1;
	}
  
  socklen_t len=sizeof(dir);
  getsockname(s, (struct sockaddr *)&dir, &len);
  port=ntohs(dir.sin_port); 
  fprintf(stdout,"%s: %d\n",argv[1],port);
  
  pArray= (Process *)malloc(20*sizeof(Process));
  if(pArray==NULL) printf("error en malloc");
  scArray= (SC *)malloc(20*sizeof(SC));
  
  for(;fgets(line,80,stdin);) {
    if(!strcmp(line,"START\n"))
      break;

    sscanf(line,"%[^:]: %d",proc,&port);

    if(!strcmp(proc,argv[1])){
     np=num;
   }

    pArray[num].nombre=(char *)malloc(sizeof(pArray[num].nombre));
    strcpy(pArray[num].nombre,proc);
    pArray[num].puerto=port;
    num=num+1;
  }

  
  /* Inicializar Reloj */
  int lc [num];
  for (int i=0; i<num; i++){
    lc[i]=0;
  }
  
  /* Procesar Acciones */
  for(;fgets(line,80,stdin);) {
    char accion[10];
    char param[70];
    sscanf(line,"%s %s\n",accion,param);

    //-------FINISH-------
    if(!strcmp(line,"FINISH\n")){
      break;
    }//finsh
    
    //-------EVENT-------
    else if(!strcmp(line,"EVENT\n")){
      printf("%s: TICK\n",pArray[np].nombre);
      int n=lc[np];
      lc[np]=n+1;
    }//event

     //-------GETCLOCK-------
    else if(!strcmp(line,"GETCLOCK\n")){
      printf("%s: LC[",pArray[np].nombre);
      for(int i=0; i<num;i++){
        if (i==(num-1)){
          printf("%d",lc[num-1]);
        }
        else{
          printf("%d,",lc[i]);
        }
      }
      printf("]\n");
    }//getclock

     //-------MESSAGETO-------
    else if(!strcmp(accion,"MESSAGETO")){
      printf("%s: TICK\n",pArray[np].nombre);
      int n=lc[np];
      lc[np]=n+1;
      printf("%s: SEND(MSG,%s)\n",pArray[np].nombre,param);
      
      Mensaje *mEnv = malloc(sizeof(Mensaje));
      mEnv->tipo=0;
      mEnv->posEnReloj= np;
      memcpy(mEnv->relojes,lc,num*sizeof(int));
    
      int puert;
      for (int i=0; i<num;i++){
        if(!strcmp(param,pArray[i].nombre)){
          puert=pArray[i].puerto;
          break;
        }
      }
      int sock;
      struct sockaddr_in servadd;

      sock=socket(AF_INET, SOCK_DGRAM, 0);
      bzero(&servadd,sizeof(servadd));
      servadd.sin_family=PF_INET;
      //servadd.sin_addr.s_addr=INADDR_ANY;
      servadd.sin_addr.s_addr=inet_addr("127.1");
      servadd.sin_port=htons(puert);
      //printf("%d\n",dir.sin_port);

      sendto(sock,mEnv,sizeof(Mensaje),0,(struct sockaddr*)&servadd,sizeof(servadd));
    }//mesageto 

     //-------RECEIVE-------
    else if(!strcmp(line,"RECEIVE\n")){
      
      struct sockaddr_in dirCli;
      Mensaje *mRec;
      mRec= malloc(sizeof(Mensaje));
      socklen_t size = sizeof(struct sockaddr_in);
      
      //s es el socket del inicio 
      recvfrom(s, mRec, sizeof(Mensaje), 0, (struct sockaddr*)&dirCli,&size);
      
      if (mRec->tipo==0){
        printf("%s: RECEIVE(%s,%s)\n",pArray[np].nombre, "MSG", pArray[mRec->posEnReloj].nombre);
      }
      else if (mRec->tipo==1){
        printf("%s: RECEIVE(%s,%s)\n",pArray[np].nombre, "LOCK", pArray[mRec->posEnReloj].nombre);
      }
      else if (mRec->tipo==2){
        printf("%s: RECEIVE(%s,%s)\n",pArray[np].nombre, "OK", pArray[mRec->posEnReloj].nombre);
      }
      

      //tick con aumento
        printf("%s: TICK\n", pArray[np].nombre);
        int n=lc[np];
        lc[np]=n+1;

      //merjear los relojes
      int *relojEntrada = mRec->relojes;
      //int *relojantesMerge = lc;
      // memcpy(relojantesMerge,lc,num*sizeof(int));
      for (int i=0; i<num; i++){
        if(lc[i]<relojEntrada[i]){
           lc[i]=relojEntrada[i];
         }
      }
      
      //cosas que hay que hacer cuando recives un LOCK
      if (mRec->tipo==1){
        int libre= 0;
        
        for (int i=0; i<nSecCrit; i++){ //miramos si el proceso tiene la SC
          if(strcmp(scArray[i].nombre,mRec->secCritica)==0){
            if (scArray[i].dentro ==1){ //si esta dentro
              libre=1;
              //printf("estoy dentro\n");
                scArray[i].numesp+=1;
                scArray[i].pEsp[mRec->posEnReloj]=scArray[i].numesp;
                
            }
            else if (scArray[i].quieroEntrar==1){ //si quiere entrar
              libre=1;
              int *relojcuandopedi = scArray[i].relojAlpedir;
             // printf("quiero entrar\n");
                
              int yomayor=0;
              int otromayor=0;

              for(int k=0; k<num; k++){
                if(relojcuandopedi[k]>relojEntrada[k]){
                  otromayor+=1;
                }
                else if (relojcuandopedi[k]<relojEntrada[k]){
                  yomayor+=1;
                }
              }
              if(yomayor>0 && otromayor==0){ //si el otro proceso lo pidió antes
                //printf("yo lo pidi antes\n");
                  scArray[i].numesp+=1;
                  scArray[i].pEsp[mRec->posEnReloj]=scArray[i].numesp;
                
              }
              else if(otromayor>0 && yomayor==0){ //si yo lo pedí antes 
                //printf("otro proceso lo pidio antes\n");
                libre=0;
              }
              else{ //si no se puede establecer quien fue antes
                //printf("no se puede establecer\n");
                if(np<mRec->posEnReloj){ //si el numero de proceso es inferior 
                 // printf("yo lo pidi antes\n");
                  scArray[i].numesp+=1;
                  scArray[i].pEsp[mRec->posEnReloj]=scArray[i].numesp;
                }
                else{
                  libre=0;
                }
              }
            }
          }//if
        }//for

        if (libre==0){
          //tick con aumento
          printf("%s: TICK\n",pArray[np].nombre);
          int n=lc[np];
          lc[np]=n+1;

          //envio de mensale de OK al proceso 
          printf("%s: SEND(OK,%s)\n",pArray[np].nombre,pArray[mRec->posEnReloj].nombre);2;
          Mensaje *mEnv = malloc(sizeof(Mensaje));
          mEnv->tipo=2;
          mEnv->posEnReloj= np;
          memcpy(mEnv->relojes,lc,num*sizeof(int));
          //mEnv->secCritica=mRec->secCritica;
          memcpy(mEnv->secCritica,mRec->secCritica,strlen(mRec->secCritica)+1);
          int puert=pArray[mRec->posEnReloj].puerto;
          
          int sock;
          struct sockaddr_in servadd;

          sock=socket(AF_INET, SOCK_DGRAM, 0);
          bzero(&servadd,sizeof(servadd));
          servadd.sin_family=PF_INET;
          servadd.sin_addr.s_addr=inet_addr("127.1");
          servadd.sin_port=htons(puert);

          sendto(sock,mEnv,sizeof(Mensaje),0,(struct sockaddr*)&servadd,sizeof(servadd));
        }
      }// si es tipo lock

      //cosas que hay que hacer cuando recives un OK
      if (mRec->tipo==2){
        for (int i=0; i<nSecCrit; i++){//buscamos la SC que queremos
          if(strcmp(scArray[i].nombre,mRec->secCritica)==0){
            scArray[i].numOK+=1;
            if(scArray[i].numOK==num-1){//tengo todos los OK necesarios
              scArray[i].dentro=1;
              scArray[i].quieroEntrar=0;
              printf("%s: MUTEX(%s)\n",pArray[np].nombre,scArray[i].nombre);
            }
          }
        }
      }//si es tipo ok

    

    }//receive
  
      //-------LOCK-------
    else if(!strcmp(accion,"LOCK")){
      //evento
      printf("%s: TICK\n",pArray[np].nombre);
      int n=lc[np];
      lc[np]=n+1;
      
      //guadamos esta seccion critica si no la tiene ya
      int ta=0;
      for(int j=0;j<nSecCrit;j++){
        if(strcmp(param,scArray[j].nombre)==0){
          ta=1;
        }
      }
      if (ta==0) {
        scArray[nSecCrit].nombre=(char *)malloc(sizeof(scArray[nSecCrit].nombre));
        strcpy(scArray[nSecCrit].nombre,param);
        scArray[nSecCrit].dentro=0;
        scArray[nSecCrit].numOK=0;
        scArray[nSecCrit].numesp=0;
        scArray[nSecCrit].quieroEntrar=1;
        //scArray[nSecCrit].relojAlpedir= malloc(num*sizeof(int));
        memcpy(scArray[nSecCrit].relojAlpedir,lc,num*sizeof(int));
        for (int k=0; k<num; k++){
          scArray[nSecCrit].pEsp[k]=0;
        }
        nSecCrit+=1;
      }
     
      //recorremos todos los procesoso para enviar el mesnaje de Lock
      for(int i=0; i<num;i++){
        if(i!=np){ //a todos los que no sea yo
          printf("%s: SEND(LOCK,%s)\n",pArray[np].nombre,pArray[i].nombre);

          Mensaje *mEnv = malloc(sizeof(Mensaje));
          mEnv->tipo=1;
          mEnv->posEnReloj= np;
          memcpy(mEnv->relojes,lc,num*sizeof(int));
          memcpy(mEnv->secCritica,param,strlen(param)+1);
          //mEnv->secCritica=param;
          int puert=pArray[i].puerto;
          
          int sock;
          struct sockaddr_in servadd;

          sock=socket(AF_INET, SOCK_DGRAM, 0);
          bzero(&servadd,sizeof(servadd));
          servadd.sin_family=PF_INET;
          servadd.sin_addr.s_addr=inet_addr("127.1");
          servadd.sin_port=htons(puert);

          sendto(sock,mEnv,sizeof(Mensaje),0,(struct sockaddr*)&servadd,sizeof(servadd));
        }//if
      }//for


    }//lock

     //-------UNLCOK-------
    else if(!strcmp(accion,"UNLOCK")){
    
      for (int i=0; i<nSecCrit; i++){//buscamos el SC en el que estamos
        if(strcmp(param,scArray[i].nombre)==0){
          scArray[i].dentro=0;
          
          while(scArray[i].numesp>0){

            for (int j= 0; j<num;j++){ //mandar ok a quien estubiese pendeinte 
              if(scArray[i].pEsp[j]==1){
                  //tick con aumento
                printf("%s: TICK\n",pArray[np].nombre);
                int n=lc[np];
                lc[np]=n+1;

                //envio de mensale de OK al proceso 
                printf("%s: SEND(OK,%s)\n",pArray[np].nombre,pArray[j].nombre);

                Mensaje *mEnv = malloc(sizeof(Mensaje));
                mEnv->tipo=2;
                mEnv->posEnReloj= np;
                memcpy(mEnv->relojes,lc,num*sizeof(int));
                //mEnv->secCritica=scArray[i].nombre;
                memcpy(mEnv->secCritica,param,strlen(param)+1);
                int puert=pArray[j].puerto;
              
                int sock;
                struct sockaddr_in servadd;

                sock=socket(AF_INET, SOCK_DGRAM, 0);
                bzero(&servadd,sizeof(servadd));
                servadd.sin_family=PF_INET;
                servadd.sin_addr.s_addr=inet_addr("127.1");
                servadd.sin_port=htons(puert);

                sendto(sock,mEnv,sizeof(Mensaje),0,(struct sockaddr*)&servadd,sizeof(servadd));


                //elimino ese de mi lista de pendientes
                scArray[i].pEsp[j]=0;
                scArray[i].numesp-=1;
                for(int m=0; m<num; m++){
                  if(scArray[i].pEsp[m]>0){
                    scArray[i].pEsp[m]=scArray[i].pEsp[m]-1;
                    
                  }
                }
                break;
              }//if
          }//for
          }//while
          break;
        }
      }

    }//unlock
      
    }//for 

  free(pArray);
  return 0;
}//main
