connection::~connection() {
	off();
}

void connection::on() {
	enet_initialize();
}

void connection::off() {
	enet_deinitialize();
}

server::server(int a, int b, int c, int d, int e) {
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

client::client(int a, int b, int c, int d) {
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
