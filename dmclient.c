#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sock/client.h"
#include "protocol/protocol.h"

int main() {
    int err;

    int fd = connect_to_named_socket("test");
    if (fd < 0) {
        printf("Error: failed to connect\n");
    }

    struct response res;

    struct req_create_buffer req_create_buffer = {
    .width = 640,
    .height = 480,
    .alpha = 0,
    };
    err = request(fd, REQ_CREATE_BUFFER, &req_create_buffer, sizeof(req_create_buffer), &res);
    if (err) {
        printf("Error: request failed\n");
    }

    printf("%d\n", res.header.type);

    struct res_create_buffer* res_create_buffer = res.body;

    printf("%s %d\n", res_create_buffer->buffer_name, res_create_buffer->size);

    err = close_response(&res);
    if (err) {
        printf("Error: closing response failed\n");
    }

    err = close(fd);
    if (err) {
        printf("Error: closing socket failed\n");
    }

    return 0;
}