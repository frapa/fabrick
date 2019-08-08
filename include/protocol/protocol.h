#ifndef PROTOCOL
#define PROTOCOL

#include <stdint.h>

#define BUF_NAME_LENGTH 16

#define REQ_CREATE_BUFFER 1
#define REQ_DESTROY_BUFFER 2
#define REQ_UPDATE_BUFFER 3
#define REQ_COMMIT_BUFFER 4

#define RES_BAD_REQUEST  1
#define RES_SERVER_ERROR 2
#define RES_CREATE_BUFFER 3
#define RES_DESTRY_BUFFER 4
#define RES_UPDATE_BUFFER 5
#define RES_COMMIT_BUFFER 6


// --------

struct req_create_buffer {
    uint16_t width;
    uint16_t height;
};

struct req_destroy_buffer {
    char buffer_name[BUF_NAME_LENGTH + 2];
};

struct req_update_buffer {
    char buffer_name[BUF_NAME_LENGTH + 2];
    uint16_t width;
    uint16_t height;
};

struct req_commit_buffer {
    char buffer_name[BUF_NAME_LENGTH + 2];
};

// --------

struct res_create_buffer {
    char buffer_name[BUF_NAME_LENGTH + 2];
    uint32_t size;
};

struct res_update_buffer {
    uint32_t size;
};

#endif