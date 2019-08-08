#ifndef SOCK_SERVER
#define SOCK_SERVER

#include "helper.h"

// Limited by /proc/sys/net/core/somaxconn
#define MAX_CONNECTIONS 128

int listen_to_named_socket(char *socket_name);

int listen_to_file_socket(char *socket_path);

int next_request(int client_fd, struct request *request);

int send_response(int client_fd, uint8_t type, void *body, size_t body_size);

int close_request(struct request *request);

#endif