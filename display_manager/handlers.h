// Header containing the definition of all available handlers.
// The implementation is split topic-wise in the handlers_*.c files
//
// A handler function if a function that takes a request as input,
// takes the necessary actions and answers returns a response to be sent.

#ifndef DM_HANDLERS
#define DM_HANDLERS

#include "clients.h"
#include "protocol/protocol.h"

int handler_create_buffer(struct client* client, struct req_create_buffer* req);

#endif