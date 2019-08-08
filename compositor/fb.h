#ifndef CMP_INIT_FB
#define CMP_INIT_FB

extern struct dumb_buf buf0;
extern struct dumb_buf buf1;
extern struct dumb_buf* cbuf;
extern struct dumb_buf* dbuf;

int init_fb();

int flip();

#endif