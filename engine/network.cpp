bool connected = false;
/* * Network
connection()
Establish a connection.  Returns true if it succeeded in doing so.

C++
connection();

Python
connection()
* */
bool connection() {
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
event
A network event.
	channel int
	channel()
		get the channel
	type()
		get the type of the event
	data()
		get the data, if any
	who()
		who made this event as a peer

C++
event a;

Python
a = event()
* */
int peercount = 0;

int event::channel() {
	return (int)evt.channelID;
}

const char* event::type() {
	if(evt.type == ENET_EVENT_TYPE_RECEIVE)
		return "receive";
	else if(evt.type == ENET_EVENT_TYPE_CONNECT)
		return "connect";
	else if(evt.type == ENET_EVENT_TYPE_DISCONNECT)
		return "disconnect";
	else if(evt.type == ENET_EVENT_TYPE_NONE)
		return "none";
	
	return "error";
}

const char* event::data() {
	if(evt.type == ENET_EVENT_TYPE_RECEIVE)
		return (const char*)evt.packet->data;
	return NULL;
}


peer* event::who() {
	if(evt.peer)
		return (peer*)evt.peer->data;
}

event::~event() {
	switch(evt.type){
	case ENET_EVENT_TYPE_RECEIVE:
		enet_packet_destroy(evt.packet);
		break;
	case ENET_EVENT_TYPE_DISCONNECT:
		delete (peer*)evt.peer->data;
		evt.peer->data = NULL;
		break;
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
	
	if(!host)
		err("server", "could not create");
}

server::~server() {
	enet_host_destroy(host);
}

event server::service(int a) {
	event e;
	int h;
	peer* p;
	
	h = enet_host_service(host, &e.evt, a);
	
	if(h > 0) {
		if(e.evt.peer) {
			switch(e.evt.type){
			case ENET_EVENT_TYPE_CONNECT:
					peercount++;
					p = new peer;
					p->id = peercount;
					p->who = e.evt.peer;
					e.evt.peer->data = p;
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				delete (peer*)e.evt.peer->data;
				e.evt.peer->data = NULL;
				break;
			}
		}
	}
	
	if(h < 0) {
		e.type = ENET_EVENT_TYPE_NONE;
		if(e.evt.peer)
			e.evt.peer->data = NULL;
		err("server", "service", "could not");
	}
	
	return e;
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
	
	if(!host)
		err("client", "could not create");
}

client::~client() {
	enet_host_destroy(host);
}

event client::service(int a) {
	event e;
	int h;
	h = enet_host_service(host, &e.evt, a);
	
	if(h < 0)
		err("client", "service", "could not");
	
	return e;
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
