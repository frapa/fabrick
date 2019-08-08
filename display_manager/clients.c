#include "clients.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

struct client* clients = NULL;

static void add_client_to_hash(struct client* client) {
	HASH_ADD_INT(clients, fd, client);
}

int add_client(int socket) {
    int client_fd = accept(socket, NULL, NULL);
    if (client_fd < 0) return client_fd;

    if (client_fd >= 0) {
        struct client* client = malloc(sizeof(struct client));
        client->fd = client_fd;

        add_client_to_hash(client);

        return 0;
    }
}

int remove_client(struct client* client) {
    HASH_DEL(clients, client);
}