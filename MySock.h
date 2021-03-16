void *my_socket (int type);
int my_bind (void *socket, const char *endpoint);
int my_recv (void *socket, void *msg, int flags);
int my_send (void *socket, void *msg, int flags);
int my_connect (void *socket, const char *endpoint);
int my_close (void *socket);
int aceitar(void *socket);

#define REQ 0
#define REP 1
#define PUB 2
#define MAXPENDING 1
#define BUFSIZE 32