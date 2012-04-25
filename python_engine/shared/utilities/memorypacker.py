class Mlob:
	def __repr__(self):
		return 'Mlob(' + str(self.offset) + ', ' + str(self.size) + ')'
	def __init__(self, o, s):
		self.offset = o
		self.size = s
	def __iter__(self):
		yield self.offset
		yield self.size

class MemoryPacker:
	"Simple memory allocator.  You must free exact blocks that you allocated, no partials."
	def __init__(self, size):
		# Max size
		self.size = size
		
		# Starting of spaces that are free
		self.freeStarts = {}
		
		# Ending of spaces that are free
		self.freeEnds = {}
		
		# Initialize
		if self.size > 0:
			self._newfree(0, self.size)
	
	def _newfree(self, o, s):
		if o in self.freeStarts:
			# Pre-expand already free space
			e = self.freeStarts[o]
			e.offset -= s
			e.size += s
			del self.freeStarts[o]
			self.freeStarts[e.offset] = e
		elif o in self.freeEnds:
			# Expand already free space
			e = self.freeEnds[o]
			e.size += s
			del self.freeEnds[o]
			self.freeEnds[e.offset + e.size] = e
		else:
			# New free space
			m = Mlob(o, s)
			self.freeStarts[o] = m
			self.freeEnds[o + s] = m
	
	def expand(self, amount):
		if amount < 0:
			raise ValueError('Cannot shrink a memory glob.')
		
		self._newfree(self.size, amount)
		self.size += amount
	
	def new(self, size):
		for m in sorted(self.freeStarts.values(), key = lambda a: a.size):
			if m.size >= size:
				o = m.offset
				if m.size == size:
					# Nothing free left
					e = m.offset + m.size
					del self.freeEnds[e]
					del self.freeStarts[m.offset]
				else:
					del self.freeStarts[o]
					m.offset += size
					self.freeStarts[m.offset] = m
					m.size -= size
					# Still some space is free
				return Mlob(o, size) 
		return None
	
	def free(self, g):
		self._newfree(g.offset, g.size)
