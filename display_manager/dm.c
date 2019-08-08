#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>

#include "sock/server.h"
#include "ut/uthash.h"

#include "./clients.h"
#include "./procreq.h"

void* run_dm() {
	int err;

	int socket = listen_to_named_socket("fabrick");
	if (socket < 0) {
		fprintf(stderr, "Error: Socket already in use.\n");
		exit(1);
	}

	while (1) {
		// We poll all sockets and listen for events, then take the
		// appropriate action according to which socket got which event.
		nfds_t nfds = 1 + HASH_COUNT(clients);
		struct pollfd *fds = malloc(sizeof(struct pollfd) * nfds);

		// Server socket
		fds[0].fd = socket;
		fds[0].events = POLLIN;

		// Client sockets
		struct client* client;
		int i = 1;
		for (client = clients; client != NULL; client = client->hh.next) {
			fds[i].fd = client->fd;
			fds[i].events = POLLIN;
			++i;
		}

		int ret = poll(fds, nfds, -1);
		if (ret < 0) {
			fprintf(stderr, "Error: Cannot poll socket, %s.\n",
				strerror(errno));
		}

		if (ret > 0) {
			// TODO: guard agains malicious clients that keep sending connections
			if (fds[0].revents & POLLIN) {
				err = add_client(socket);
				if (err) {
					fprintf(stderr, "Error: Cannot accept client connection, %s.\n",
						strerror(errno));
				}
			}

    		// TODO: guard agains malicious clients that keep sending messages
			for (int j = 1; j < nfds; ++j) {
				int client_fd = fds[j].fd;
				HASH_FIND_INT(clients, &client_fd, client);

				// client closed connection
				if (fds[j].revents & POLLHUP) {
					remove_client(client);
					continue;
				}

				if (fds[j].revents & POLLIN) {
					process_client_requests(client);
				}
			}
		}
		
		free(fds);
	}

	close(socket);
}