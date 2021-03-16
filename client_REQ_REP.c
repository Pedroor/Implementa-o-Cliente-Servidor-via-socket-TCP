#include "MySock.c"

int main(){
    printf("Conectando ao servidor ...\n");

    void *requester = my_socket(REQ);
    printf("SOCKET: %d\n", *(int*)(requester));
    int conexao = my_connect(requester, "tcp://localhost:33434");

    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        my_send(requester, "Hello", 0);
        char buffer[32];
        my_recv(requester, buffer, 0);
        printf("BUFFER: %s\n", buffer);
        memset(buffer, 0,32);
    }  
    my_close(requester);
}