class Strip(dict):
	def __init__(self, sector_size = 256, dim = 0, parent = None):
		# What dimension this is
		self.dim = dim
		
		# Parent dimension strip and position, for .discard()
		self.parent = parent
		
		# How big the top-level strips are
		self.sector_size = sector_size
		
		# What's in this strip, and not in a deeper dimension
		self.here = []
	
	def __getitem__(self, k):
		if not k in self:
			self[k] = Strip(self.sector_size, self.dim + 1, (self, k))
		return dict.__getitem__(self, k)
	
	def insert(self, x):
		if len(x) != self.dim:
			return self[x[self.dim] // self.sector_size].insert(x)
		
		if x in self.here:
			return None
		
		self.here.append(x)
		return self
	
	def remove(self, x):
		self.here.remove(x)
		
		self.discard()
	
	def find(self, low, high):
		r = range(self.dim)
		for x in self.here:
			c = True
			for d in r:
				if x[d] < low[d] or x[d] > high[d]:
					c = False
					break
			if c:
				yield x
		
		if len(low) > self.dim:
			for z in range(int(low[self.dim] // self.sector_size), int(high[self.dim] // self.sector_size) + 1):
				if z in self:
					for x in self[z].find(low, high):
						yield x
	
	def discard(self, k = None):
		if k is not None:
			del self[k]
		
		if self.parent is not None and not self.here and not self:
			self.parent[0].discard(self.parent[1])

class Scape:
	"Dimentionless and boundless optimized search map."
	
	def __init__(self):
		# First dimention
		self.dim0 = Strip()
		
		# For (re)moving
		self.where = {}
	
	def insert(self, x):
		self.where[hash(x) if x.__hash__ else id(x)] = self.dim0.insert(x)
	
	def remove(self, x):
		z = (hash(x) if x.__hash__ else id(x))
		self.where[z].remove(x)
		del self.where[z]
	
	def move(self, x):
		e = self.dim0.insert(x)
		if e is not None:
			z = (hash(x) if x.__hash__ else id(x))
			self.where[z].remove(x)
			self.where[z] = e
	
	def find(self, low, high):
		if len(low) != len(high):
			raise Exception('Low and high must be of the same length.')
		
		for x in self.dim0.find(low, high):
			yield x
