import os, numbers, ast

# Simple class to handle settings
class Settings:
	"""Settings manager.  It can be used like a dictionary."""
	def __init__(self, data = None):
		self.data = {}
		
		if type(data) == dict:
			self.data = data
		elif type(data) == str:
			self.load(data)
	
	def __str__(self):
		return str(self.data)
	
	def __repr__(self):
		return 'Settings(' + str(self.data) + ')'
	
	def __bool__(self):
		return bool(self.data)
	
	def __len__(self):
		return len(self.data)
	
	def __getitem__(self, k):
		if k in self.data:
			return self.data[k]
		return None
	
	def __setitem__(self, k, v):
		if not isinstance(k, str):
			raise AttributeError("Only string may be used as settings.")
		
		if not isinstance(v, (str, numbers.Number, dict, set, type(None))):
			raise ValueError("Only string numbers, dicts, and sets may be used as settings' values.")
		
		if isinstance(k, str):
			if '\n' in k:
				raise AttributeError("Settings may not contain linebreaks.")
			if '=' in k:
				raise AttributeError("Settings may not contain equals signs.")
			
		if isinstance(v, str):
			if '\n' in k:
				raise ValueError("Settings' values may not contain linebreaks.")
		
		self.data[k] = v
	
	def __delitem__(self, k):
		if k in self.data:
			del self.data[k]
	
	def save(self, filename):
		"""Save settings to file."""
		s = ''
		
		for l in self.data:
			d = self.data[l]
			if d is None:
				s += l+'\n'
			else:
				s += l+'='+repr(self.data[l])+'\n'
		
		if len(s) > 0:
			f = open(filename, 'w')
			f.write(s)
			f.close()
			return True
		return False
	
	def load(self, filename):
		"""Load settings from file."""
		if type(filename) == str and os.path.isfile(filename):
			f = open(filename, 'r')
			for l in f:
				s = l.find('=')
				
				if s > 0:
					print(l[s+1:-1])
					self.data[l[:s]] = ast.literal_eval(l[s+1:-1])
				else:
					self.data[l[:s]] = None
			f.close()
			return True
		elif type(filename) == dict:
			self.data = filename
			return True
		return False
