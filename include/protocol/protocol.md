# Fabrick protocol

The fabrick protocol spcifies how the communication between a fabrick server and client
should take place. The protocol is kept as simple as possible, while trying to support
common features.

## Calls

The client can perform the following calls:

`register`

This call notifies the server that there is a new client. The server returns
a client id that can be used in all further requests to identify the client.

`create_buffer`