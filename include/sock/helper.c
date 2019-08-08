#include "helper.h"

#include <stdlib.h>

void set_sockaddr_name(struct sockaddr_un* addr, char* socket_name) {
    memset(addr->sun_path, 0, sizeof(addr->sun_path));
    // Write name but leave null at the beginning
    strncpy(addr->sun_path + 1, socket_name, sizeof(addr->sun_path) - 1);
}