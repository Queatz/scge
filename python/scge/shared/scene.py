class Switcher:
	def __init__(self):
		self.current = None
	
	def update(self):
		"Update the current scene."
		if self.current:
			self.current.update()
			return True
		return False

	def draw(self):
		"Draw the current scene."
		if self.current:
			self.current.draw()
		
class Scene:
	def __init__(self, s):
		self.switcher = s
		self.init()
	
	def init(self):
		"Called on construction of the scene."
		pass
	
	def goto(self):
		"Switch to this scene.  Override this to make a transition and not blur the last scene immediately."
		if self.switcher.current:
			self.switcher.current.blur()
		
		self.switcher.current = self
		self.focus()
	
	def focus(self):
		"Called when scene gains view."
		pass
	
	def fade(self, value):
		"For use if the scene supports transitions."
		pass
	
	def blur(self):
		"Called when scene loses view."
		pass
	
	def update(self):
		"Called to update scene."
		pass
	
	def draw(self):
		"Called to draw the scene."
		pass
