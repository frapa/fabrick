#include "fb.h"

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "dumbo/dumbo.h"

#include "state.h"

struct dumb_buf buf0;
struct dumb_buf buf1;
struct dumb_buf* cbuf;
struct dumb_buf* dbuf;

int init_fb() {
    int err;

    int fd = open("/dev/dri/card0", O_RDWR);

	err = drmSetMaster(fd);
	if (err) {
		printf("Cannot read /dev/dri/card0 (%d): %m\n", errno);
		return 1;
	}

	state.display_conf = find_basic_conf(fd);

	buf0.width = state.display_conf.mode.hdisplay;
	buf0.height = state.display_conf.mode.vdisplay;
	err = create_buffer(fd, &buf0);
	if (err) {
		printf("Impossible to create buffer (%d): %m\n", errno);
		return 1;
	}

	buf1.width = state.display_conf.mode.hdisplay;
	buf1.height = state.display_conf.mode.vdisplay;
	err = create_buffer(fd, &buf1);
	if (err) {
		printf("Impossible to create buffer (%d): %m\n", errno);
		return 1;
	}

    state.fb_fd = fd;

    cbuf = &buf0;
    dbuf = &buf1;

	return 0;
}

int flip() {
	set_buf(state.fb_fd, state.display_conf, *dbuf);

	struct dumb_buf* new_dbuf = cbuf;
	cbuf = dbuf;
	dbuf = new_dbuf;
}