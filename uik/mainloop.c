#include "mainloop.h"
#include "sock/client.h"

void mainloop() {
    int fd = connect_to_named_socket("fabrick");
    if (fd < 0) {
        return printf("Error: failed to connect\n");
    }
}