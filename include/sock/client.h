#ifndef SOCK_CLIENT
#define SOCK_CLIENT

#include "helper.h"

int connect_to_named_socket(char *socket_name);

int connect_to_file_socket(char *socket_path);

// int _send_request(int fd, uint8_t type, void *body, size_t body_size);
// int _get_response(int fd, struct response *response);

int request(int fd, uint8_t type, void *body, size_t body_size, struct response *response);

int close_response(struct response *response);

#endif