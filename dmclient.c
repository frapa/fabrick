#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sock/client.h"
#include "protocol/protocol.h"

int main() {
  int err;

  printf("%d\n", getpid());

  int fd = connect_to_named_socket("test");
  if (fd < 0) {
    return 1;
  }

  struct req_register req_register = { .client_name = "dmclient" };
  struct response res;
  err = request(fd, REQ_REGISTER, &req_register, sizeof(req_register), &res);
  if (err) {
    return 2;
  }
  
  uint64_t client_id = ((struct res_register*)res.body)->client_id;

  close(fd);
  fd = connect_to_named_socket("test");

  struct req_create_buffer req_create_buffer = {
    .client_id = client_id,
    .width = 640,
    .height = 480,
    .alpha = 0,
  };
  printf("a\n");
  err = request(fd, REQ_CREATE_BUFFER, &req_create_buffer, sizeof(req_create_buffer), &res);
  if (err) {
    return 3;
  }
  printf("b\n");

  err = close_response(&res);
  if (err) {
    return 4;
  }

  err = close(fd);
  if (err) {
    return 5;
  }

  return 0;
}