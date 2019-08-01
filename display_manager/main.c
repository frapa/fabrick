#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include "dumbo/outconf.h"
#include "dumbo/dumbo.h"
#include "sock/server.h"
#include "protocol/protocol.h"

#include "./clients.h"

// int random = -2;
// uint64_t uuid() {
// 	if (random < 0) {
// 		random = open("/dev/urandom", O_RDONLY);
// 		if (random < 0) {
// 			fprintf(stderr, "Impossible to read /dev/urandom\n");
// 			exit(1);
// 		}
// 	}

// 	uint64_t id = 0;
// 	read(random, &id, 8);

// 	return id;
// }

// void add_client(struct client* client) {
// 	printf("%ld\n", client->id);
// 	HASH_ADD(hh, clients, id, sizeof(uint64_t), client);
// } 

// struct client* find_client(uint64_t client_id) {
// 	struct client* client = NULL;
// 	printf("%ld %p %p %p\n", client_id, &client_id, clients, client);
// 	HASH_FIND(hh, clients, &client_id, sizeof(uint64_t), client);
// 	return client;
// } 

void send_response_handle_error(int client_fd, uint8_t status, void* body, size_t body_size) {
	int err = send_response(client_fd, status, body, body_size);
	if (err < 0) fprintf(stderr, "Error sending response to client (%d): %m\n", errno);
}

void bad_request(int client_fd) {
	send_response_handle_error(client_fd, RES_BAD_REQUEST, NULL, 0);
}

void server_error(int client_fd) {
	send_response_handle_error(client_fd, RES_SERVER_ERROR, NULL, 0);
}

int main() {
	int err;

	// int fd = open("/dev/dri/card0", O_RDWR);

	// err = drmSetMaster(fd);
	// if (err) {
	// 	printf("Cannot read /dev/dri/card0 (%d): %m\n", errno);
	// 	return 1;
	// }

	// struct display_conf conf = find_basic_conf(fd);

	// struct dumb_buf buf1 = {
	// 	.width = conf.mode.hdisplay,
	// 	.height = conf.mode.vdisplay,
	// };
	// struct dumb_buf buf2 = {
	// 	.width = conf.mode.hdisplay,
	// 	.height = conf.mode.vdisplay,
	// };
	// err = create_buffer(fd, &buf1);
	// if (err) {
	// 	printf("Impossible to create buffer (%d): %m\n", errno);
	// 	return 1;
	// }
	// err = create_buffer(fd, &buf2);
	// if (err) {
	// 	printf("Impossible to create buffer (%d): %m\n", errno);
	// 	return 1;
	// }

	int socket = listen_to_named_socket("test");
	if (socket < 0) {
		fprintf(stderr, "Error: Socket already in use.\n");
		exit(1);
	}

	while (1) {
		err = accept_clients(socket);
		if (err) {
			fprintf(stderr, "Error: Cannot accept client connection, %s.\n",
				strerror(errno));
		}
	// 	struct request req;
	// 	err = next_request(socket, &req);
	// 	if (err < 0) {
	// 		fprintf(stderr, "Error while reading request (%d): %m\n", errno);
	// 		server_error(&req);
	// 		continue;
	// 	}
	// 	if (err > 0) {
	// 		bad_request(&req);
	// 		continue;
	// 	}

	// 	printf("%d %ld %p\n", req.header.type, req.header.size, &req.body);

	// 	switch (req.header.type) {
	// 		// case REQ_REGISTER:
	// 		// 	{
	// 		// 		struct req_register* req_body = req.body;

	// 		// 		struct client* client = malloc(sizeof(client));
	// 		// 		while (1) {
	// 		// 			client->id = uuid();
	// 		// 			if (clients == NULL) break;

	// 		// 			struct client* c = find_client(client->id);
	// 		// 			if (c == NULL) break;
	// 		// 		}

	// 		// 		memcpy(client->client_name, req_body->client_name, 32);
	// 		// 		add_client(client);

	// 		// 		struct res_register res_body = { .client_id = client->id };
	// 		// 		send_response_handle_error(&req, RES_REGISTER, &res_body, sizeof(res_body));
	// 		// 	}
	// 		// 	break;
	// 		// case REQ_CREATE_BUFFER:
	// 		// 	{
	// 		// 		printf("asdas\n");
	// 		// 		struct req_create_buffer* req_body = req.body;

	// 		// 		struct client* client = find_client(client->id);
	// 		// 		if (client == NULL) {
	// 		// 			bad_request(&req);
	// 		// 			break;
	// 		// 		}

	// 		// 		// uuid_generate_random(client.id);

	// 		// 		// struct res_create_buffer res_body;
	// 		// 		// memcpy(res_body.buffer_id, client.id, 16);
	// 		// 		// send_response_handle_error(&req, RES_REGISTER, &res_body, sizeof(res_body));
	// 		// 	}
	// 		// 	break;
	// 		default:
	// 			bad_request(&req);
	// 	}
		
	// 	close_request(&req);
	}

	close(socket);

	// usleep(10 * 1000);

	// memset((uint8_t*)buf1.fb, 50, buf1.size);
	// set_buf(fd, conf, buf1);
	// usleep(500 * 1000);

	// memset((uint8_t*)buf2.fb, 200, buf2.size);
	// set_buf(fd, conf, buf2);
	// usleep(500 * 1000);
}