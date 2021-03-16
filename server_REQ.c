#include "MySock.c"
#include <pthread.h>
#include <setjmp.h>
#include <time.h>

#define TRY do{jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){ 
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)
void *ThreadMain(void *arg);
void *atualizaAleatorio(void *arg);

struct ThreadArgs {
  int clntSock;
};
int numeroAleatorio;
char letraAleatoria[32];

void *myThreadFun(void *vargp) 
{ 
    while(1){
        sleep(5);
        memset(letraAleatoria,' ', 32*sizeof(letraAleatoria[0]));
        printf("Letra aleatória trocada.\n"); 
        srand((unsigned) time(NULL));
            numeroAleatorio = rand() % 100000;
            sprintf(letraAleatoria, "%d", numeroAleatorio);
    }
    
}
   
int main(){   
    void *publisher = my_socket(PUB);
    int rc = my_bind(publisher, "tcp://0.0.0.0:12345");
     pthread_t thread_id; 
    pthread_create(&thread_id, NULL, myThreadFun, NULL); 
    //pthread_join(thread_id, NULL); 
   
    while(1){
    int clntSock = aceitar(publisher);

    struct ThreadArgs *threadArgs = (struct ThreadArgs *) malloc(
        sizeof(struct ThreadArgs));
    if (threadArgs == NULL)
      puts("malloc() failed");
    threadArgs->clntSock = clntSock;

    pthread_t threadID;

    int returnValue = pthread_create(&thread_id, NULL, ThreadMain, threadArgs);
    if (returnValue != 0)
      puts("pthread_create() failed");
    printf("Thread criada, ID: %ld\n", (long int) thread_id);
  }
   
}


void *ThreadMain(void *threadArgs) {
  pthread_detach(pthread_self());

  int clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
  free(threadArgs);
  
  while(1){
      sleep(2);

      // SEM O TRY CATCH, EM UM DETERMINADO MOMENTO O SEND VAI TENTAR ENVIAR UMA MENSAGEM PRA UM SOCKET FECHADO E VAI 
      // CRASHAR O SERVIDOR. 
      
      //TRY{     
         ssize_t numBytesSent = send(clntSock, letraAleatoria, 32, 0);
          if (numBytesSent < 0){}
            //puts("send() falhou");
          else if (numBytesSent != 32)
            puts("send() enviou número inesperado de bytes");  
            //THROW;
      //}CATCH{
       // close(clntSock);

     //}ETRY;
    }
  return (NULL);
}
