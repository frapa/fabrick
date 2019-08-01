#ifndef SOCK_SERVER
#define SOCK_SERVER

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "sock/helper.h"

// Limited by /proc/sys/net/core/somaxconn
#define MAX_CONNECTIONS 128

int listen_to_named_socket(char* socket_name) {
    int err;

    // STEP 1: Create socket descriptor
    int fd = socket(
        AF_LOCAL,      // local socket (fastest variant)
        SOCK_STREAM | SOCK_NONBLOCK,  // reliable, bidirectional
        0             // system picks protocol
    );

    if (fd < 0) return fd;

    // STEP 2: The socket "address" is its name (this only works on linux)
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    set_sockaddr_name(&addr, socket_name);

    // STEP 3: Bind socket so that clients can connect
    err = bind(fd, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) return err;

    // STEP 4: Start listening to the incoming messages
    err = listen(fd, MAX_CONNECTIONS);
    if (err < 0) return err;

    return fd;
}

int listen_to_file_socket(char* socket_path) {
    int err;

    // STEP 1: Create socket descriptor
    int fd = socket(
        AF_LOCAL,      // local socket (fastest variant)
        SOCK_STREAM | SOCK_NONBLOCK,  // reliable, bidirectional
        0             // system picks protocol
    );

    if (fd < 0) return fd;

    // STEP 2: The socket "address" is its fil path (this is a unix standard)
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path));

    // STEP 3: Bind socket so that clients can connect
    err = bind(fd, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) return err;

    // STEP 4: Start listening to the incoming messages
    err = listen(fd, MAX_CONNECTIONS);
    if (err < 0) return err;

    return fd;
}

int next_request(int client_fd, struct request* request) {
    ssize_t count;

    // First read header which contains the request size
    // and is always in a fixed format
    count = read(client_fd, &request->header, sizeof(request->header));
    if (count < 0) return count;

    if (count == sizeof(request->header)) {
        // Allocate necessary memory and read body
        request->body = malloc(request->header.size);
        count = read(client_fd, request->body, request->header.size);

        if (count < 0) {
            // Clean-up allocation
            free(request->body);
            return 1;
        }

        if (count != request->header.size) {
            // The request sent by the client is malformed
            // Clean-up allocation
            free(request->body);

            return 1;
        }
    } else {
        // The request sent by the client is malformed,
        return 1;
    }

    return 0;
}

int send_response(int client_fd, uint8_t status, void* body, size_t body_size) {
    int err;

    struct res_header header;
    header.type = status;
    header.size = body_size;
    
    err = write(client_fd, &header, sizeof(header));
    if (err < 0) {
        return err;
    }

    if (body) {
        err = write(client_fd, body, body_size);
        if (err < 0) {
            return err;
        }
    }

    return 0;
}

int close_request(struct request* request) {
    // Clean-up allocation
    free(request->body);
}

#endif