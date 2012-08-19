import scge
import scge.shared.basic as basic
import glm

class TileChunk(list):
	def __init__(self, s):
		for x in range(s):
			self.append([0] * s)
		
		self.worth = 0

class Tilemap:
	def __init__(self, s, i, c = 16):
		self.chunks = {}
		self.size = s
		self.image = i
		self.chunkSize = c
	
	def _ttype_to_imgoffs(self, t):
		w = self.image.size / self.size
		return glm.vec2(t % w.x, t // w.x) / w
	
	def pixel(self, p):
		at = glm.ivec2(p // self.size)
		
		t = self.tile(at)
		if not t:
			return None
		t -= 1
		w = self.image.size.x // self.size

		return glm.ivec2(t % w, t // w) * self.size + glm.ivec2(p - at * self.size)
	
	def tile(self, p, v = None):
		c = p // self.chunkSize
		o = p - c * self.chunkSize
		
		if v is None:
			if c not in self.chunks:
				return 0
			return self.chunks[c][o.y][o.x]
		
		if c not in self.chunks:
			if v == 0:
				return 0
			
			self.chunks[c] = TileChunk(self.chunkSize)
		
		z = self.chunks[c][o.y][o.x]
		
		if z == v:
			return z
		
		self.chunks[c][o.y][o.x] = v
		
		if v == 0 and z != 0:
			self.chunks[c].worth -= 1
			
			if self.chunks[c].worth <= 0:
				del self.chunks[c]
		elif v != 0 and z == 0:
			self.chunks[c].worth += 1
		
		return v
	
	def tileAt(self, p, v = None):
		return self.tile(glm.ivec2(p / self.size), v)
	
	def snap(self, p):
		return p / self.size // 1 * self.size
	
	def overlapping(self, p1, p2):
		p = glm.ivec2()
		for p.x in range(int(p1.x // self.size), int(p2.x // self.size) + 1):
			for p.y in range(int(p1.y // self.size), int(p2.y // self.size) + 1):
				yield (self.tile(p), p)
		raise StopIteration
	
	def draw(self, v1, v2):
		basic.image(self.image)
		vv1 = glm.ivec2(v1 / self.size // self.chunkSize)
		vv2 = glm.ivec2((v2 + self.chunkSize / 2) / self.size // self.chunkSize) + 1
		
		w = self.image.size / self.size
		s = glm.vec2(1) / w
		
		rcs = range(self.chunkSize)
		
		lt = None
		
		for y in range(vv1.y, vv2.y):
			for x in range(vv1.x, vv2.x):
				c = glm.ivec2(x, y)
				if c in self.chunks:
					cc = self.chunks[c]
					ccc = c * self.size * self.chunkSize
					for yy in rcs:
						ccyy = cc[yy]
						for xx in rcs:
							t = ccyy[xx]
							if t:
								p1 = ccc + glm.ivec2(xx, yy) * self.size
								if(p1.x + self.size < v1.x or p1.y + self.size < v1.y or p1.x > v2.x or p1.y > v2.y):
									continue
								
								t -= 1
								
								if lt != t:
									t1 = glm.vec2(t % w.x, t // w.x) / w
									basic.texcoord(t1, t1 + s)
								
								lt = t
								
								basic.quad(p1, p1 + self.size)
