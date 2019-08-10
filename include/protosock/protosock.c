#include "protosock.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sock/client.h"

int create_buffer(
    int fd,
    uint16_t width, uint16_t height,
    uint16_t x, uint16_t y,
    uint32_t** buf_p, char buffer_name[BUF_NAME_LENGTH]
) {
    int err;
    struct response res;

    // Buffer create request
    struct req_create_buffer req_create_buffer = {
        .type = BUFFER_NORMAL,
        .width = width,
        .height = height,
        .x = x,
        .y = y,
    };
    err = request(fd, REQ_CREATE_BUFFER, &req_create_buffer, sizeof(req_create_buffer), &res);
    if (err) {
        return err;
    }

    struct res_create_buffer* res_create_buffer = res.body;

    // Copy stuff
    uint32_t size = res_create_buffer->size;
    memcpy(buffer_name, res_create_buffer->buffer_name, BUF_NAME_LENGTH);

    // Mmapping
    int buf_fd = shm_open(
        buffer_name,
        O_RDWR,
        S_IRWXU | S_IROTH | S_IWOTH
    );
    if (buf_fd < 0) {
        return buf_fd;
    }
    *buf_p = mmap(
        NULL, size, 
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        buf_fd, 0
    );
    if ((void*)*buf_p == MAP_FAILED) {
        return **buf_p;
    }

    // Cleanup
    err = close_response(&res);
    if (err) {
        return err;
    }

    return size;
}

int commit_buffer(int fd, char buffer_name[BUF_NAME_LENGTH]) {
    int err;
    struct response res;

    // Buffer commit request
    struct req_commit_buffer req_commit_buffer;
    memcpy(req_commit_buffer.buffer_name, buffer_name, BUF_NAME_LENGTH);
    
    err = request(fd, REQ_COMMIT_BUFFER, &req_commit_buffer, sizeof(req_commit_buffer), &res);
    if (err) {
        return err;
    }

    // Cleanup
    err = close_response(&res);
    if (err) {
        return err;
    }

    return 0;
}