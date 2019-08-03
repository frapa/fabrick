// Helper functions to work with clients
// and manage client data like attributes and such.

#ifndef DM_CLIENTS
#define DM_CLIENTS

#include "shbuf.h"
#include "ut/uthash.h"

struct client {
	UT_hash_handle hh;
    int fd;
	// uint64_t id;
	// char client_name[32];
	uint8_t buf_num;
	struct shbuf buffers[16];
};

// Client hash table
static struct client* clients = NULL;

// Accepts new client connections and adds them to the client hash map
int accept_clients(int socket);

#endif