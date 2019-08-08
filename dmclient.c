#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sock/client.h"
#include "protocol/protocol.h"

int main() {
    int err;

    int fd = connect_to_named_socket("fabrick");
    if (fd < 0) {
        return printf("Error: failed to connect\n");
    }

    struct response res;

    struct req_create_buffer req_create_buffer = {
        .width = 480,
        .height = 360,
    };
    err = request(fd, REQ_CREATE_BUFFER, &req_create_buffer, sizeof(req_create_buffer), &res);
    if (err) {
        return printf("Error: request failed\n");
    }

    struct res_create_buffer* res_create_buffer = res.body;

    int buf_fd = shm_open(
        res_create_buffer->buffer_name,
        O_RDWR,
        S_IRWXU | S_IROTH | S_IWOTH
    );
    if (buf_fd < 0) {
        perror("Err:");
        return printf("Error: cannot access shared mem\n");
    }
    uint32_t* buf_p = mmap(
        NULL, res_create_buffer->size, 
        PROT_READ | PROT_WRITE, // The server only needs to read the buffer
        MAP_SHARED,
        buf_fd, 0
    );
    if ((void*)buf_p == MAP_FAILED) {
        perror("Err:");
        return printf("Error: cannot mmap buffer\n");
    }
    
    memset(buf_p, 255, res_create_buffer->size);

    struct req_commit_buffer req_commit_buffer;
    memcpy(req_commit_buffer.buffer_name, res_create_buffer->buffer_name, 18);
    
    err = request(fd, REQ_COMMIT_BUFFER, &req_commit_buffer, sizeof(req_commit_buffer), &res);
    if (err) {
        return printf("Error: commit request failed\n");
    }

    sleep(3);

    memset(buf_p, 120, res_create_buffer->size);

    err = request(fd, REQ_COMMIT_BUFFER, &req_commit_buffer, sizeof(req_commit_buffer), &res);
    if (err) {
        return printf("Error: commit request failed\n");
    }

    sleep(10);

    err = close_response(&res);
    if (err) {
        return printf("Error: closing response failed\n");
    }

    err = close(fd);
    if (err) {
        return printf("Error: closing socket failed\n");
    }

    return 0;
}