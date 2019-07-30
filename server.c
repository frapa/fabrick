#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "dumbo/sock_server.h"

int main() {
  int err;

  int fd = listen_to_named_socket("test");

  while (1) {
    struct request req;
    err = next_request(fd, &req);
    if (err) {
      return err;
    }

    printf("%d %ld %s\n", req.header.type, req.header.size, (char*)req.body);

    err = send_response(&req, req.header.type, "OK", 3);
    if (err) {
      return err;
    }
    
    close_request(&req);
  }

  close(fd);

  return 0;
}