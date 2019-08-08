#include "handlers.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/random.h>
#include <fcntl.h>
#include "ut/uthash.h"

#include "shbuf.h"
#include "sock/server.h"
#include "protocol/protocol.h"

#include "state.h"

#define VALID_CHAR_NUM 65
char char_table[VALID_CHAR_NUM] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";

static void generate_new_buf_name(char* buf_name) {
    // Get some random noise
    uint8_t noise[BUF_NAME_LENGTH];
    getrandom(noise, BUF_NAME_LENGTH, 0);

    // Fill buffer indexing with the noise the char_table
    buf_name[0] = '/';
    for (int i = 0; i < BUF_NAME_LENGTH; ++i) {
        buf_name[i+1] = char_table[noise[i] % VALID_CHAR_NUM];
    }
    buf_name[BUF_NAME_LENGTH + 1] = 0;
}

int handler_create_buffer(struct client* client, struct req_create_buffer* req) {
    // TODO: check request parameters (size) against malicius values
    // (dimesions too large or bigger than screen)
    // For now we just randomly check something
    if (req->width > 3840) return 1;
    if (req->height > 2160) return 1;
    
    // Generate buf_name randomly, due to security
    // reasons. In this way it's not possible to guess
    // the names of the buffers of other clients.
    char buf_name[BUF_NAME_LENGTH + 2];
    generate_new_buf_name(buf_name);

    // Create virtual file
    int fd = shm_open(
        buf_name,
        O_RDWR | O_CREAT, // create if it does not exists
        S_IRWXU | S_IROTH | S_IWOTH // RW permission to clients
    );
    if (fd < 0) return fd;

    // Grow file to the required buffer size
    uint32_t size = req->width * req->height * 4;
    int err = ftruncate(fd, size);
    if (err) {
        shm_unlink(buf_name);
        return err;
    }

    // Map file so that we have a pointer to the buffer
    uint32_t* buf_p = mmap(
        NULL, size, 
        PROT_READ, // The server only needs to read the buffer
        MAP_SHARED,
        fd, 0
    );
    if (*buf_p < 0) return *buf_p;

    struct shbuf* buf = malloc(sizeof(struct shbuf));
    buf->fd = fd;
    buf->width = req->width;
    buf->height = req->height;
    buf->size = size;
    buf->p = buf_p;
    buf->p_committed = malloc(size);
    memcpy(buf->file_name, buf_name, BUF_NAME_LENGTH + 2);
    
    // Update client with the new buffer information
    HASH_ADD_STR(client->buffers, file_name, buf);

    // Prepare and send response
    struct res_create_buffer res = {
        .size = size,
    };
    memcpy(res.buffer_name, buf_name, BUF_NAME_LENGTH + 2);
    send_response(client->fd, RES_CREATE_BUFFER,
                  &res, sizeof(res));

    state.dirty = 1;
}

int handler_destroy_buffer(struct client* client, struct req_destroy_buffer* req) {
    // TODO: automatically remove on disconnection
    int err;
    struct shbuf* buf;
    
    HASH_FIND_STR(client->buffers, req->buffer_name, buf);
    HASH_DEL(client->buffers, buf);

    err = munmap(buf->p, buf->size);
    if (err) return err;

    err = close(buf->fd);
    if (err) return err;

    free(buf);

    state.dirty = 1;
}

int handler_update_buffer(struct client* client, struct req_update_buffer* req) {
    // TODO: unsupported for now
}

int handler_commit_buffer(struct client* client, struct req_commit_buffer* req) {
    // TODO: proper locking with mutexes
    int err;
    struct shbuf* buf;
    
    HASH_FIND_STR(client->buffers, req->buffer_name, buf);
    
    memcpy(buf->p_committed, buf->p, buf->size);

    state.dirty = 1;
}