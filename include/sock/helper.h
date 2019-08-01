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
    struct req_header header;
    void* body;
};

struct res_header {
    uint8_t type;
    size_t size;
};

struct response {
    struct res_header header;
    void* body;
};

void set_sockaddr_name(struct sockaddr_un* addr, char* socket_name) {
    memset(addr->sun_path, 0, sizeof(addr->sun_path));
    // Write name but leave null at the beginning
    strncpy(addr->sun_path + 1, socket_name, sizeof(addr->sun_path) - 1);
}

#endif