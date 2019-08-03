#ifndef DM_SHBUF
#define DM_SHBUF

struct shbuf {
    int fd;
    char file_name[18];
    uint16_t width;
    uint16_t height;
    uint8_t alpha;
    uint32_t size;
    uint8_t* p;
};

#endif