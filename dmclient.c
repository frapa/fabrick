#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dumbo/sock_client.h"
#include "protocol/protocol.h"

int main() {
  int err;

  int fd = connect_to_named_socket("test");
  if (fd < 0) {
    return 1;
  }

  struct req_register reg = { .client_name = "dmclient" };
  struct response res;
  err = request(fd, REQ_REGISTER, &reg, sizeof(reg), &res);
  if (err) {
    return 2;
  }
  
  printf("%d %ld %s\n", res.header.status, res.header.size, (char*)res.body);

  err = close_response(&res);
  err = close(fd);

  return 0;
}