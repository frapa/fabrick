#ifndef SOCK_HELPER
#define SOCK_HELPER

#include <stdint.h>
#include <string.h>
#include <sys/un.h>

struct req_header {
    uint8_t type;
    size_t size;
};

struct request {
    // public
    struct req_header header;
    void* body;
    // private
    int client_fd;
};

struct res_header {
    uint8_t status;
    size_t size;
};

struct response {
    // public
    struct res_header header;
    void* body;
    // private
};

void set_sockaddr_name(struct sockaddr_un* addr, char* socket_name) {
    memset(addr->sun_path, 0, sizeof(addr->sun_path));
    // Write name but leave null at the beginning
    strncpy(addr->sun_path + 1, socket_name, sizeof(addr->sun_path) - 1);
}

#endif