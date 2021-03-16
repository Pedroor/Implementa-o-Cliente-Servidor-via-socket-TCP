#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "MySock.h"

int sock_AUX;
int tipo;
void *my_socket (int type){
    sock_AUX = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    tipo = type;
    if (sock_AUX < 0)
        puts("socket() falhou");
    return &sock_AUX;
}

int my_connect (void *socket, const char *endpoint){
    // LEMBRAR DE TRATAR CASO FOR LOCALHOST

    int len = strlen(endpoint); 
    int i;
    int pontos = 0;
    int barra = 0;
    char endereco[32];
    int endereco_indice = 0;
    char porta[32];
    int porta_indice = 0;
    for(i = 0; i < len; i++){
        if (endpoint[i] == ':')
            pontos++;
        if (endpoint[i] == '/')
            barra++;

        if (pontos == 1 && barra == 2){
            barra = 3;
        }else if (pontos == 1 && barra == 3){
            endereco[endereco_indice] = endpoint[i];
            endereco_indice++;
        }
        if (pontos == 2){
            endereco[endereco_indice] = '\0';
            pontos = 3;
        }else if(pontos == 3){
            porta[porta_indice] = endpoint[i];
            porta_indice++;
        }
        if (strcmp(endereco, "localhost") == 0){
            endereco[0] = '1';
            endereco[1] = '2';
            endereco[2] = '7';
            endereco[3] = '.';
            endereco[4] = '0';
            endereco[5] = '.';
            endereco[6] = '0';
            endereco[7] = '.';
            endereco[8] = '1';
        }
        if (i == len - 1 ){
            porta[porta_indice] = '\0';
        }
    }
    printf("ENDEREÇO: %s, PORTA: %s\n", endereco, porta);

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    int rtnVal = inet_pton(AF_INET, endereco, &servAddr.sin_addr.s_addr);
    if (rtnVal == 0)
        puts("string de endereço inválida");
    else if (rtnVal < 0)
        puts("inet_pton() falhou");
    servAddr.sin_port = htons(atoi(porta));

  int conexao = connect(*(int*)(socket), (struct sockaddr *) &servAddr, sizeof(servAddr));
  if (conexao < 0)
    puts("connect() falhou");
return conexao;
}

int clntSock;
int my_bind (void *socket, const char *endpoint){
 // LEMBRAR DE TRATAR CASO FOR LOCALHOST

    int len = strlen(endpoint); 
    int i;
    int pontos = 0;
    int barra = 0;
    char endereco[32];
    int endereco_indice = 0;
    char porta[32];
    int porta_indice = 0;
    for(i = 0; i < len; i++){

        if (endpoint[i] == ':')
            pontos++;
        if (endpoint[i] == '/')
            barra++;

        if (pontos == 1 && barra == 2){
            barra = 3;
        }else if (pontos == 1 && barra == 3){
            endereco[endereco_indice] = endpoint[i];
            endereco_indice++;
        }
        if (pontos == 2){
            endereco[endereco_indice] = '\0';
            pontos = 3;
        }else if(pontos == 3){
            porta[porta_indice] = endpoint[i];
            porta_indice++;
        }
        if (strcmp(endereco, "localhost") == 0){
            endereco[0] = '1';
            endereco[1] = '2';
            endereco[2] = '7';
            endereco[3] = '.';
            endereco[4] = '0';
            endereco[5] = '.';
            endereco[6] = '0';
            endereco[7] = '.';
            endereco[8] = '1';
        }

        if (i == len - 1 ){
            porta[porta_indice] = '\0';
        }
    }
    printf("ENDEREÇO: %s, PORTA: %s\n", endereco, porta);

    int sock = *(int*)(socket);
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(porta));

  if (bind(sock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    puts("bind() falhou");
  if (listen(sock, MAXPENDING) < 0)
    puts("listen() falhou");

    if (tipo == 2){
        return 0;
    }
    return aceitar(socket);
}

int aceitar(void *socket){
    int sock = *(int*)(socket);
    struct sockaddr_in clntAddr; 
    socklen_t clntAddrLen = sizeof(clntAddr);

    clntSock = accept(sock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0)
        puts("accept() falhou");

    char clntName[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL)
        printf("Manipulando cliente %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    else
        puts("Incapaz de obter endereço do cliente");
    return clntSock;
}

int my_send (void *socket, void *msg, int flags){
    int sock = *(int*)(socket);
    size_t echoStringLen = strlen(msg);
    ssize_t numBytes = send(sock, msg, echoStringLen, 0);
    if (numBytes < 0)
        puts("send() falhou");
    else if (numBytes != echoStringLen)
        puts("send() enviou um número inesperado de bytes");
}

int my_recv (void *socket, void *msg, int flags){
    int sock = *(int*)(socket);
    ssize_t numBytesRcvd = recv(sock, msg, 32, 0);
    if (numBytesRcvd < 0)
        puts("recv() falhou");

    return numBytesRcvd;
}
int my_close (void *socket){
    close(*(int*)socket);
    printf("Fechou conexão\n");
}