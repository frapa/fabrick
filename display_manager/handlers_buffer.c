#include "handlers.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/random.h>
#include <fcntl.h>

static uint16_t buf_count = 0;

static void generate_new_buf_name(char* buf_name) {
    buf_name[0] = '/';
    getrandom(&buf_name + 1, 16, 0);
    buf_name[17] = 0;
}

int handler_create_buffer(struct client* client, struct req_create_buffer* req) {
    // TODO: check request parameters (size) against malicius values
    // (dimesions too large or bigger than screen)
    // For now we just randomly check something
    if (req->width > 3840) return 1;
    if (req->height > 2160) return 1;
    
    // Generate next_buf_name
    char buf_name[18];
    generate_new_buf_name(buf_name);

    // Create virtual file
    int fd = shm_open(
        buf_name,
        O_RDWR | O_CREAT, // create if it does not exists
        S_IRWXU | S_IROTH | S_IWOTH // RW permission to clients
    );
    if (fd < 0) return fd;

    // Grow file to the required buffer size
    uint32_t size = req->width * req->height * (req->alpha ? 4 : 3);
    int err = ftruncate(fd, size);
    if (err) {
        shm_unlink(buf_name);
        return err;
    }

    // Map file so that we have a pointer to the buffer
    uint8_t* buf = mmap(
        NULL, size, 
        PROT_READ, // The server only needs to read the buffer
        MAP_SHARED,
        fd, 0
    );
    if (*buf < 0) return *buf;

    
}