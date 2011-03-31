bool connected = false;
/* * Network
connection()
Establish a connection.

C++
connection();

Python
connection()
* */
void connection() {
	if(enet_initialize() >= 0) {
		atexit(connection_off);
		connected = true;
		return true;
	}
	return false;
}

/* *
connection_off()
Disable networking.

C++
connection_off();

Python
connection_off()
* */
void connection_off() {
	if(connected) {
		enet_deinitialize();
		connected = false;
	}
}

/* * Network Types
server
A server.
	service(int)
		give service to the clients, optionally waiting some milliseconds

C++
server a();
a.service();

server b(2000, 32, 1, 128, 256); //server on port 2000, max 32 connected clients, 1 channel, 128 downstream limit, and 256 upstream limit

Python
a = server()
a.service()

b = server(2000, 32, 1, 128, 256) #server on port 2000, max 32 connected clients, 1 channel, 128 downstream limit, and 256 upstream limit
* */
server::server(int a, int b, int c, int d, int e) {
	if(!connected)
		connection();
	
	address.host = ENET_HOST_ANY;
	address.port = a;
	
	host = enet_host_create(&address, b, c, d, e);
}

server::~server() {
	enet_host_destroy(host);
}

ENetEvent server::service(int a) {
	ENetEvent b;
	
	if(enet_host_service(host, &b, a) > 0)
		return b;
}

/* * Network Types
client
A client.
	connect(string = "localhost", int = 2000, int = 1)
		connect to a server, on a port, with max channels of
	disconnect()
		disconnect from the server
	send(string = "", int = 0)
		queue a message on a channel
	commune()
		send all the queued messages
	service(int = 2000)
		listen for commands from the server, within a timeout

C++
client a();

client b(1, 1, 128, 256); //client with max 1 connections, 1 channel, 128 downstream limit, and 256 upstream limit

Python
a = client();

b = client(1, 1, 128, 256); #client with max 1 connections, 1 channel, 128 downstream limit, and 256 upstream limit
* */
client::client(int a, int b, int c, int d) {
	if(!connected)
		connection();
	
	host = enet_host_create(NULL, a, b, c, d);
}

client::~client() {
	enet_host_destroy(host);
}

ENetEvent client::service(int a) {
	ENetEvent b;
	
	if(enet_host_service(host, &b, a) > 0)
		return b;
}

void client::send(const char* b, int c) {
	ENetPacket* d = enet_packet_create(b, strlen(b) + 1, ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(peer, c, d);
}

void client::commune() {
	enet_host_flush(host);
}

void client::connect(const char* a, int b, int c) {
	ENetAddress d;

	enet_address_set_host(&d, a);
	d.port = b;

	peer = enet_host_connect(host, &d, c, 0);    
}

void client::disconnect() {
	enet_peer_disconnect(peer, 0);
}
