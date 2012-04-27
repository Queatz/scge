import traceback
import scge

from ast import literal_eval as leval

class Client:
	"A client that runs in the game and connects to a server."
	def __init__(self, address:str, port:int):
		self.host = scge.host()
		self.server = self.host.connect(address, port)
		self.connected = False
		self.queue = []
		self.init()
	
	def init(self):
		"Called when the client is created."
		pass
	
	def disconnect(self):
		"Disconnect from the server."
		self.host.disconnect(self.server)
		self.host.commune()
	
	def message(self, data:object):
		"Called when some string is recieved from the server."
		pass
	
	def send(self, data:object):
		"Send some data to the server."
		if not self.connected:
			self.queue.append(data)
		else:
			self.host.send(self.server, repr(data))
	
	def update(self):
		"Call this regularly."
		while True:
			e = self.host.service()
			
			if not e.who():
				break
			
			if e.type() == 'receive':
				self.message(leval(e.data()))
			elif e.type() == 'connect':
				self.connected = True
				while self.queue:
					self.send(self.queue.pop(0))
			elif e.type() == 'disconnect':
				self.connected = False
			else:
				break

class Peer:
	"A peer on the server of a connected client."
	def __init__(self, server, peer):
		self.server = server
		self.peer = peer
		self.init()
	
	def init(self):
		"Called when the peer is created."
		pass
	
	def message(self, data):
		"Called when a message was recieved from the remote client."
		pass
	
	def send(self, data):
		"Send some data to the remote client."
		self.server.host.send(self.peer, repr(data))

class Server:
	"A server."
	def __init__(self, peerFactory, port):
		self.peerFactory = peerFactory
		self.host = scge.host(port)
		self.clients = {}
		self.init()
	
	def init(self):
		"Called when the server is created."
		pass
	
	def connected(self, client):
		"Called when a client connects."
		pass
	
	def disconnected(self, client):
		"Called when a client disconnects."
		pass
	
	def send(self, peer, data):
		"Send some string to a client."
		self.host.send(peer.peer, repr(data))
	
	def update(self):
		"Call this regularly."
		while True:
			e = self.host.service()
			
			if not e.who():
				break
			
			if e.type() == 'receive':
				try:
					print(e.data())
					self.clients[e.who().id].message(leval(e.data()))
				except:
					traceback.print_exc()
			elif e.type() == 'connect':
				c = self.peerFactory(self, e.who())
				self.clients[e.who().id] = c
				self.connected(c)
			elif e.type() == 'disconnect':
				if e.who().id in self.clients:
					self.disconnected(self.clients[e.who().id])
					del self.clients[e.who().id]
			else:
				break
