// Helper functions to work with clients
// and manage client data like attributes and such.

#ifndef DM_CLIENTS
#define DM_CLIENTS

#include "shbuf.h"
#include "ut/uthash.h"

struct client {
	UT_hash_handle hh;
    int fd;
	struct shbuf* buffers;
};

// Client hash table
extern struct client* clients;

// Accepts new client connections and adds them to the client hash map
int add_client(int socket);

int remove_client(struct client* client);

#endif