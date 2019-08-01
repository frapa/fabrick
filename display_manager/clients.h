#ifndef DM_CLIENTS
#define DM_CLIENTS

#include "ut/uthash.h"

struct client {
	UT_hash_handle hh;
    int fd;
	// uint64_t id;
	// char client_name[32];
	// uint64_t buffer_id[16];
};

// Client hash table
static struct client* clients = NULL;

// accept new client connections and adds them to the client hash map
int accept_clients(int socket);

#endif