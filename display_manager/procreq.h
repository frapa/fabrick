// Helper logic to process the requests of a client
// and redirect them to the proper handler.
//
// Also does some convenience things like casting message bodies
// to the right type and emitting error responses, so that
// the handlers are as simple as possible.

#ifndef DM_PROCREQ
#define DM_PROCREQ

#include "clients.h"

// Reads all requests from a client connection and processes them
void process_client_requests(struct client* client);

#endif