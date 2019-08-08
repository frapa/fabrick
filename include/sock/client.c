#include "client.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>

int connect_to_named_socket(char* socket_name) {
    // STEP 1: Create socket descriptor
    int fd = socket(
        AF_LOCAL,     // local socket (fastest variant)
        SOCK_STREAM,  // reliable, bidirectional
        0             // system picks protocol
    );

    if (fd < 0) return -1;

    // STEP 2: The socket "address" is its name (this only works on linux)
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    set_sockaddr_name(&addr, socket_name);

    // STEP 3: Connect to the socket created by the server
    int err = connect(fd, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) return -1;

    return fd;
}

int connect_to_file_socket(char* socket_path) {
    // STEP 1: Create socket descriptor
    int fd = socket(
        AF_LOCAL,     // local socket (fastest variant)
        SOCK_STREAM,  // reliable, bidirectional
        0             // system picks protocol
    );

    if (fd < 0) return -1;

    // STEP 2: The socket "address" is its fil path (this is a unix standard)
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path));

    // STEP 3: Connect to the socket created by the server
    int err = connect(fd, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) return err;

    return fd;
}

int _send_request(int fd, uint8_t type, void* body, size_t body_size) {
    int err;

    struct req_header header;
    header.type = type;
    header.size = body_size;
    
    err = write(fd, &header, sizeof(header));
    if (err < 0) {
        return err;
    }

    if (body) {
        err = write(fd, body, body_size);
        if (err < 0) {
            return err;
        }
    }

    return 0;
}

int _get_response(int fd, struct response* response) {
    ssize_t count;

    // First read header which contains the request size
    // and is always in a fixed format
    count = read(fd, &response->header, sizeof(response->header));
    if (count < 0) return count;

    if (count == sizeof(response->header)) {
        // Allocate necessary memory and read body
        response->body = malloc(response->header.size);
        count = read(fd, response->body, response->header.size);
        
        if (count < 0) {
            // Clean-up allocation
            free(response->body);
            return 1;
        }

        if (count != response->header.size) {
            // The response sent by the server is malformed
            // Clean-up allocation
            free(response->body);

            return 1;
        }
    } else {
        // The response sent by the server is malformed,
        return 1;
    }

    return 0;
}

int request(int fd, uint8_t type, void* body, size_t body_size, struct response* response) {
    int err;

    err = _send_request(fd, type, body, body_size);
    if (err) return err;

    err = _get_response(fd, response);
    if (err) return err;

    return 0;
}

int close_response(struct response* response) {
    // Clean-up allocation
    free(response->body);
}