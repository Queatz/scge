import numbers

# # # Mechanics # # #

class Style:
	_callback_ = None
	refitting = 0
	
	def _addcallback_(self, c):
		if self._callback_ is None:
			self._callback_ = {c}
		else:
			self._callback_.add(c)
	
	def _discardcallback_(self, c):
		self._callback_.discard(c)
	
	def __setattr__(self, a, v):
		call = v != getattr(self, a, None)
		object.__setattr__(self, a, v)
		if call and self._callback_:
			for c in self._callback_:
				c()

class StyleDef:
	def __init__(self, fit_callback, draw_callback):
		
		# Standard
		o = Offset()
		o.refitting = 1
		s = Offset()
		s.refitting = 2
		self.offset = o
		self.size = s
		
		# Mechanics
		object.__setattr__(self, '_fit_callback', fit_callback)
		object.__setattr__(self, '_draw_callback', draw_callback)
	
	def __getattribute__(self, a):
		try:
			a = object.__getattribute__(self, a)
			if hasattr(a, '__get__'):
				return a.__get__(self)
			return a
		except AttributeError:
			return None
	
	def __setattr__(self, a, v):
		if getattr(self, a) is not None:
			d = object.__getattribute__(self, a)
			if d is not None and hasattr(d, '__set__'):
				d.__set__(self, v)
			else:
				object.__setattr__(self, a, v)
				
				if v != d and self._draw_callback:
					self._draw_callback()
		else:
			object.__setattr__(self, a, v)
			if self._draw_callback:
				self._draw_callback()
			if hasattr(v, '_callback_'):
				v._addcallback_(self._offset_ if v.refitting == 1 else self._sized_ if v.refitting == 2 else self._dirty_)
	
	def __delattr__(self, a):
		v = object.__getattribute__(self, a)
		object.__delattr__(self, a)
		if self._draw_callback:
			self._draw_callback()
		if hasattr(v, '_callback_'):
			v._discardcallback_(self._offset_ if v.refitting == 1 else self._sized_ if v.refitting == 2 else self._dirty_)
	
	def _dirty_(self):
		if self._draw_callback:
			self._draw_callback()
	
	def _offset_(self):
		if self._fit_callback:
			self._fit_callback(False)
	
	def _sized_(self):
		if self._fit_callback:
			self._fit_callback()



# # # COMMON # # #


class Offset(Style):
	def __init__(self, x = 0, y = 0):
		if isinstance(x, Offset):
			self.x = x.x
			self.y = x.y
		else:
			self.x = x
			self.y = y
	
	def __bool__(self):
		return bool(self.x) or bool(self.y)
	
	def __eq__(self, other):
		if isinstance(other, Offset) and other.x == self.x and other.y == self.y:
			return True
		return False
	
	def __add__(self, other):
		if isinstance(other, Offset):
			return Offset(self.x + other.x, self.y + other.y)
		elif isinstance(other, numbers.Number):
			return Offset(self.x + other, self.y + other)
		raise NotImplementedError
	
	def __sub__(self, other):
		if isinstance(other, Offset):
			return Offset(self.x - other.x, self.y - other.y)
		elif isinstance(other, numbers.Number):
			return Offset(self.x - other, self.y - other)
		raise NotImplementedError
	
	def __mul__(self, other):
		if isinstance(other, Offset):
			return Offset(self.x * other.x, self.y * other.y)
		elif isinstance(other, numbers.Number):
			return Offset(self.x * other, self.y * other)
		raise NotImplementedError
	
	def __truediv__(self, other):
		if isinstance(other, Offset):
			return Offset(self.x / other.x, self.y / other.y)
		elif isinstance(other, numbers.Number):
			return Offset(self.x / other, self.y / other)
		raise NotImplementedError
	
	def __setattr__(self, a, v):
		if a == 'x' or a == 'y':
			v = round(v)
		Style.__setattr__(self, a, v)
	
	def __repr__(self):
		return str(tuple(self))
	
	def __iter__(self):
		yield self.x
		yield self.y
	
	def __len__(self):
		return 2
	
	def __set__(self, obj, v):
		if hasattr(v, '__iter__'):
			self.fromIter(v)
		else:
			self.x = v
			self.y = v
	
	def fromIter(self, i):
		i = tuple(i)
		if len(i) == 1:
			self.x = i[0]
			self.y = i[0]
		elif len(i) == 2:
			self.x = i[0]
			self.y = i[1]
		else:
			self.x = 0
			self.y = 0


class Size(Style):
	def __init__(self, *a):
		self.fromIter(a)
	
	def __repr__(self):
		return str(tuple(self))
	
	def __iter__(self):
		yield self.x
		yield self.y
	
	def __len__(self):
		return 2
	
	def __set__(self, obj, v):
		if hasattr(v, '__iter__'):
			self.fromIter(v)
		else:
			self.x = v
			self.y = v
	
	def fromIter(self, i):
		i = tuple(i)
		if len(i) == 1:
			self.x = i[0]
			self.y = i[0]
		elif len(i) == 2:
			self.x = i[0]
			self.y = i[1]
		else:
			self.x = 0
			self.y = 0

class Margin(Style):
	refitting = True
	
	def __init__(self, *a):
		self.fromIter(a)
	
	def __repr__(self):
		return str(tuple(self))
	
	def __iter__(self):
		yield self.left
		yield self.top
		yield self.right
		yield self.bottom
	
	def __len__(self):
		return 4
	
	def __set__(self, obj, v):
		if hasattr(v, '__getattr__'):
			self.fromIter(v)
		else:
			self.left = v
			self.top = v
			self.right = v
			self.bottom = v
	
	def fromIter(self, i):
		i = tuple(i)
		if len(i) == 1:
			self.left = i[0]
			self.top = i[0]
			self.right = i[0]
			self.bottom = i[0]
		elif len(i) == 2:
			self.left = i[0]
			self.top = i[1]
			self.right = i[0]
			self.bottom = i[1]
		elif len(i) == 3:
			self.left = i[0]
			self.top = i[1]
			self.right = i[2]
			self.bottom = i[1]
		elif len(i) == 4:
			self.left = i[0]
			self.top = i[1]
			self.right = i[2]
			self.bottom = i[3]
		else:
			self.left = 0
			self.top = 0
			self.right = 0
			self.bottom = 0
