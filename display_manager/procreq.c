#include "procreq.h"

#include <stdio.h>
#include <errno.h>
#include "handlers.h"

static void send_response_handle_error(int fd, uint8_t status, void* body, size_t body_size) {
	int err = send_response(fd, status, body, body_size);
	if (err < 0) 
        fprintf(stderr, "Error: cannot send response, %s\n",
            strerror(errno));
}

static void bad_request(int client_fd) {
	send_response_handle_error(client_fd, RES_BAD_REQUEST, NULL, 0);
}

static void server_error(int client_fd) {
	send_response_handle_error(client_fd, RES_SERVER_ERROR, NULL, 0);
}

// This function is there only to make it easier to read
static void proxy_to_handler(struct client* client, struct request* req) {
    int err = 0;

    switch (req->header.type) {
        case REQ_CREATE_BUFFER:
            err = handler_create_buffer(client,
                (struct req_create_buffer*)req->body);
            break;

        default:
            // The client sent an unsupported operation
            bad_request(client->fd);
    }

    if (err < 0) server_error(client->fd);
    if (err > 0) bad_request(client->fd);
}

void process_client_requests(struct client* client) {
    int err;

    // TODO: guard agains malicious clients that keep sending messages
    while (1) {
        struct request req;
        err = next_request(client->fd, &req);
        // TODO: what happens if a client closes the connection (e.g. is closed)?
		if (err < 0) {
            // This error code means that we got some system error
            // such as invalid socket.
			fprintf(stderr, "Error: cannot read request, %s\n",
                strerror(errno));
			server_error(client->fd);
			continue;
		}
		if (err > 0) {
            // This error code means that there was a problem
            // reading the request, for instance bad formatting,
            // wrong length.
			bad_request(client->fd);
			continue;
		}

        // At this point we are sure the request is valid and was
        // correctly read. We can now pass the request to the correct
        // handler function.
        proxy_to_handler(client, &req);

        close_request(&req);
    }
}