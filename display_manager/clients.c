#include "clients.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

static void add_client(struct client* client) {
	HASH_ADD(hh, clients, fd, sizeof(int), client);
}

int accept_clients(int socket) {
    while (1) {
        int client_fd = accept(socket, NULL, NULL);
		if (client_fd == EAGAIN && client_fd == EWOULDBLOCK) {
            // No more connections
            return 0;
        } else if (client_fd >= 0) {
            struct client* client = malloc(sizeof(client));
            client->fd = client_fd;

            add_client(client);

            continue;
		}

        return client_fd;
    }
}