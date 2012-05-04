bool connection() {
	if(enet_initialize() >= 0) {
		atexit(connection_off);
		connected = true;
		return true;
	}
	return false;
}

void connection_off() {
	if(connected) {
		enet_deinitialize();
		connected = false;
	}
}

const char* peer::state() {
	if(!who)
		return "disconnected";
	
	switch(who->state) {
	case ENET_PEER_STATE_CONNECTION_SUCCEEDED:
	case ENET_PEER_STATE_CONNECTED:
		return "connected";
	case ENET_PEER_STATE_CONNECTING:
	case ENET_PEER_STATE_ACKNOWLEDGING_CONNECT:
	case ENET_PEER_STATE_CONNECTION_PENDING:
		return "connecting";
	case ENET_PEER_STATE_DISCONNECT_LATER:
	case ENET_PEER_STATE_DISCONNECTING:
	case ENET_PEER_STATE_ACKNOWLEDGING_DISCONNECT:
		return "disconnecting";
	case ENET_PEER_STATE_DISCONNECTED:
	case ENET_PEER_STATE_ZOMBIE:
	default:
		return "disconnected";
	}
}

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

#ifdef WITH_PYTHON
PyObject* event::data() {
	if(!evt.packet || evt.type != ENET_EVENT_TYPE_RECEIVE)
		Py_RETURN_NONE;
	
	PyObject* result = PyBytes_FromStringAndSize((const char*)evt.packet->data, (Py_ssize_t)evt.packet->dataLength);
	if(result)
		return result;
	
	Py_RETURN_NONE;
}
#else
const void* event::data() {
	if(evt.packet && evt.type == ENET_EVENT_TYPE_RECEIVE)
		return (const void*)evt.packet->data;
	return NULL;
}
#endif

unsigned int event::length() {
	if(evt.packet && evt.type == ENET_EVENT_TYPE_RECEIVE)
		return (unsigned int)evt.packet->dataLength;
	return 0;
}


peer* event::who() {
	if(evt.peer)
		return (peer*)evt.peer->data;
	return NULL;
}

event::event() {
}

event::~event() {
	switch(evt.type){
	case ENET_EVENT_TYPE_RECEIVE:
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

host::host(int a, int b, int c, int d, int e) {
	if(!connected)
		connection();
	
	if(a) {
		address.host = ENET_HOST_ANY;
		address.port = a;
		
		me = enet_host_create(&address, b, c, d, e);
	}
	else
		me = enet_host_create(0, b, c, d, e);
	
	if(!me)
		err("host", "could not initiate");
}

host::~host() {
	enet_host_destroy(me);
}

event* host::service(float a) {
	ENetEvent t;
	int h;
	
	h = enet_host_service(me, &t, (int)(a * 1000.0));
	
	if(h > 0) {
		peer* p;
		event* e = new event;
		memcpy(&e->evt, &t, sizeof(ENetEvent));
		if(e->evt.peer->data == NULL) {
			peercount++;
			p = new peer;
			p->id = peercount;
			p->who = e->evt.peer;
			e->evt.peer->data = p;
		}
		return e;
	}
	return NULL;
}

#ifdef WITH_PYTHON
void host::send(peer* p, PyObject* b, int c, bool r, bool s) {
	if(!PyBytes_Check(b)) {
		PyErr_SetString(PyExc_TypeError, "Must be a bytes object.");
		return;
	}
	
	ENetPacket* d = enet_packet_create((const void*)PyBytes_AsString(b), PyBytes_Size(b), (r ? ENET_PACKET_FLAG_RELIABLE : 0) | (s ? ENET_PACKET_FLAG_UNSEQUENCED : 0));

	enet_peer_send(p->who, c, d);
}

void host::broadcast(PyObject* b, int c, bool r, bool s) {
	if(!PyBytes_Check(b)) {
		PyErr_SetString(PyExc_TypeError, "Must be a bytes object.");
		return;
	}
	
	ENetPacket* d = enet_packet_create((const void*)PyBytes_AsString(b), PyBytes_Size(b), (r ? ENET_PACKET_FLAG_RELIABLE : 0) | (s ? ENET_PACKET_FLAG_UNSEQUENCED : 0));

	enet_host_broadcast(me, c, d);
}

#else

void host::send(peer* p, const void* b, unsigned int l, int c, bool r, bool s) {
	ENetPacket* d = enet_packet_create(b, l, (r ? ENET_PACKET_FLAG_RELIABLE : 0) | (s ? ENET_PACKET_FLAG_UNSEQUENCED : 0));

	enet_peer_send(p->who, c, d);
}

void host::broadcast(const void* b, unsigned int l, int c, bool r, bool s) {
	ENetPacket* d = enet_packet_create(b, l, (r ? ENET_PACKET_FLAG_RELIABLE : 0) | (s ? ENET_PACKET_FLAG_UNSEQUENCED : 0));
	
	enet_host_broadcast(me, c, d);
}
#endif

void host::commune() {
	enet_host_flush(me);
}

peer* host::connect(const char* a, int b, int c, unsigned int z) {
	ENetAddress d;
	ENetPeer* p;
	peer* n;
	
	enet_address_set_host(&d, a);
	d.port = b;

	p = enet_host_connect(me, &d, c, z);
	
	if(!p) {
		err("host", "connect", "could not");
		return NULL;
	}
	
	peercount++;
	n = new peer;
	n->who = p;
	n->id = peercount;
	p->data = (void*) n;
	
	return n;
}

void host::disconnect(peer* p, unsigned int d) {
	enet_peer_disconnect(p->who, d);
}
