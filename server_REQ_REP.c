#include "MySock.c"
int main(){   
    void *requester = my_socket(REP);
    printf("SOCKET: %d\n", *(int*)(requester));
    int rc = my_bind(requester, "tcp://localhost:33434");

char buffer [32]; 
   while (my_recv(&rc, &buffer, 0) != 0) {
    printf("BUFFER: %s\n", buffer);
    memset(buffer,0,32);  

    sleep(1);
    my_send(&rc, "World", 0);
   }
   my_close(requester);
}