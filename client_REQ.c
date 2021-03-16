#include "MySock.c"

int main(){
    printf("Conectando ao servidor ...\n");

    void *requester = my_socket(REQ);
    printf("SOCKET: %d\n", *(int*)(requester));
    int conexao = my_connect(requester, "tcp://0.0.0.0:12345");

    int request_nbr;
    for (request_nbr = 0; request_nbr != 100; request_nbr++) {
        //printf("Entrei %d vezes\n", request_nbr);
        //my_send(requester, "Hello", 0);
        char buffer[32];
        my_recv(requester, buffer, 0);
        printf("BUFFER: %s\n", buffer);

    }  
    my_close(requester);
}