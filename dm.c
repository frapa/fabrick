#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "outconf.h"
#include "dumbo.h"
#include "sock_server.h"
#include "protocol.h"

int main() {
	int err;

	// int fd = open("/dev/dri/card0", O_RDWR);

	// err = drmSetMaster(fd);
	// if (err) {
	// 	printf("Cannot read /dev/dri/card0 (%d): %m\n", errno);
	// 	return 1;
	// }

	// struct display_conf conf = find_basic_conf(fd);

	// struct dumb_buf buf1 = {
	// 	.width = conf.mode.hdisplay,
	// 	.height = conf.mode.vdisplay,
	// };
	// struct dumb_buf buf2 = {
	// 	.width = conf.mode.hdisplay,
	// 	.height = conf.mode.vdisplay,
	// };
	// err = create_buffer(fd, &buf1);
	// if (err) {
	// 	printf("Impossible to create buffer (%d): %m\n", errno);
	// 	return 1;
	// }
	// err = create_buffer(fd, &buf2);
	// if (err) {
	// 	printf("Impossible to create buffer (%d): %m\n", errno);
	// 	return 1;
	// }

	int socket = listen_to_named_socket("test");

	while (1) {
		struct request req;
		err = next_request(socket, &req);
		if (err < 0) {
			fprintf(stderr, "Error while reading request (%d): %m\n", errno);
			err = send_response(&req, RES_SERVER_ERROR, NULL, 0);
			if (err < 0)
				fprintf(stderr, "Error sending response to client (%d): %m\n", errno);
			continue;
		}
		if (err > 0) {
			err = send_response(&req, RES_BAD_REQUEST, NULL, 0);
			if (err < 0)
				fprintf(stderr, "Error sending response to client (%d): %m\n", errno);
			continue;
		}

		printf("%d %ld %s\n", req.header.type, req.header.size, (char*)req.body);

		switch (req.header.type) {
			case REQ_REGISTER:
				{
					struct req_register* reg = req.body;
					printf("%s\n", reg->client_name);
				}
				break;
		}

		err = send_response(&req, req.header.type, "OK", 3);
		if (err) {
			fprintf(stderr, "Error sending response to client (%d): %m\n", errno);
			continue;
		}
		
		close_request(&req);
	}

	close(socket);

	// usleep(10 * 1000);

	// memset((uint8_t*)buf1.fb, 50, buf1.size);
	// set_buf(fd, conf, buf1);
	// usleep(500 * 1000);

	// memset((uint8_t*)buf2.fb, 200, buf2.size);
	// set_buf(fd, conf, buf2);
	// usleep(500 * 1000);
}