#ifndef PROTOCOL
#define PROTOCOL

#include <stdint.h>

#define BUF_NAME_LENGTH 18

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

#define BUFFER_NORMAL 0
#define BUFFER_FULLSCREEN 1
#define BUFFER_DOCK 2
#define BUFFER_BACKGROUND 3

#define BUFFER_DOCK_TOP 0
#define BUFFER_DOCK_BOTTOM 2
#define BUFFER_DOCK_LEFT 3
#define BUFFER_DOCK_RIGHT 4

// --------

struct req_create_buffer {
    uint8_t type;
    uint16_t width;
    uint16_t height;
    // Buffer normal
    uint16_t x;
    uint16_t y;
    // Dock
    uint8_t side;
};

struct req_destroy_buffer {
    char buffer_name[BUF_NAME_LENGTH];

};

struct req_update_buffer {
    char buffer_name[BUF_NAME_LENGTH];
    uint16_t width;
    uint16_t height;
    // Buffer normal
    uint16_t x;
    uint16_t y;
};

struct req_commit_buffer {
    char buffer_name[BUF_NAME_LENGTH];
};

// --------

struct res_create_buffer {
    char buffer_name[BUF_NAME_LENGTH];
    uint32_t size;
};

struct res_update_buffer {
    uint32_t size;
};

#endif