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
peer
A connected peer.
	id int
		an integer assigned to this peer
* */

/* * Network Types
event
A network event.
	channel()
		get the channel
	type()
		get the type of the event
	data()
		get the data, if any
	who()
		who made this event as a peer
	resolve()
		optionally call this after done inspecting the event
		it will delete the peer and clear any packet

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
	return "none";
}

const char* event::data() {
	if(evt.packet && evt.type == ENET_EVENT_TYPE_RECEIVE)
		return (const char*)evt.packet->data;
	return NULL;
}


peer* event::who() {
	if(evt.peer)
		return (peer*)evt.peer->data;
}

void event::resolve() {
	switch(evt.type){
	case ENET_EVENT_TYPE_RECEIVE:
		if(evt.packet)
			enet_packet_destroy(evt.packet);
		break;
	case ENET_EVENT_TYPE_DISCONNECT:
		if(evt.peer) {
			delete (peer*)evt.peer->data;
			evt.peer->data = NULL;
		}
		break;
	}
}

/* * Network Types
host
A networkable host.
	service(int)
		give service to connected peers, optionally waiting some milliseconds
	commune()
		send all the queued messages
		#no need to call this if you service() regularly
	connect(string = "localhost", int = 2000, int = 1)
		connect to a server, on a port, with max channels of and return a new peer
	disconnect(peer, unsigned int = 0)
		disconnect from a peer, optionally with some unusable data
	send(peer, string = "", int = 0)
		queue a message on a channel
	broadcast(string = "", int = 0)
		queue a message on a channel
	service(int = 2000)
		listen for commands from the server, within a timeout

C++
host client();
client.service();

host server(2000, 32, 1, 128, 256); //server on port 2000, max 32 connected clients, 1 channel, 128 downstream limit, and 256 upstream limit

Python
client = host()
client.connect()

server = host(2000, 32, 1, 128, 256) #server on port 2000, max 32 connected clients, 1 channel, 128 downstream limit, and 256 upstream limit
* */
host::host(int a, int b, int c, int d, int e) {
	if(!connected)
		connection();
	
	if(a) {
		address.host = ENET_HOST_ANY;
		address.port = a;
		
		me = enet_host_create(&address, b, c, d, e);
	}
	else
		me = enet_host_create(NULL, b, c, d, e);
	
	if(!me)
		err("host", "could not initiate");
}

host::~host() {
	enet_host_destroy(me);
}

event host::service(int a) {
	event e;
	int h;
	peer* p;
	
	h = enet_host_service(me, &e.evt, a);
	
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
			}
		}
	}
	
	if(h < 0) {
		e.evt.type = ENET_EVENT_TYPE_NONE;
		if(e.evt.peer)
			e.evt.peer->data = NULL;
		e.evt.peer = NULL;
	}
	
	return e;
}

void host::send(peer* p, const char* b, int c, bool r, bool s) {
	ENetPacket* d = enet_packet_create(b, strlen(b) + 1, (r ? ENET_PACKET_FLAG_RELIABLE : 0) | (s ? ENET_PACKET_FLAG_UNSEQUENCED : 0));

	enet_peer_send(p->who, c, d);
}

void host::broadcast(const char* b, int c, bool r, bool s) {
	ENetPacket* d = enet_packet_create(b, strlen(b) + 1, (r ? ENET_PACKET_FLAG_RELIABLE : 0) | (s ? ENET_PACKET_FLAG_UNSEQUENCED : 0));

	enet_host_broadcast(me, c, d);
}

void host::commune() {
	enet_host_flush(me);
}

peer* host::connect(const char* a, int b, int c) {
	ENetAddress d;
	ENetPeer* p;
	peer* n;
	
	enet_address_set_host(&d, a);
	d.port = b;

	p = enet_host_connect(me, &d, c, 0);
	
	if(!p) {
		err("host", "connect", "couldn not");
		return NULL;
	}
	
	peercount++;
	n = new peer;
	n->who = p;
	n->id = peercount;
	
	return n;
}

void host::disconnect(peer* p, unsigned int d) {
	enet_peer_disconnect(p->who, d);
}
