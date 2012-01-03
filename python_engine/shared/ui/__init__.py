#
# Manual
#
# There are two global types used in the
# interface and it's elements.  These are:
#
#
#  Offset - a position, a size...
#   Initiates with: (); (x, y); (Offset)
#   add, sub work with another Offset or number
#   eq (==) with another Offset
#   bool is False if x and y are 0
#   Is always integral
#
#  Bounds - an area, with offset and size
#   Initiates with: (); (x, y, w, h);
#    (Bounds); (Offset); (Offset, Offset)
#   Can be added / subtracted with Offset
#    and Bounds
#   eq (==) with other Bounds
#   .valid() returns if it's width and height
#    are > 0
#   bool is if it is valid
#   .overlap(Bounds) crops itself into the other
#   .within(Bounds) is if it is inside the other
#
#
#  Drags are drawn by a class with .bounds and .draw()
#
#
#  Drag
#   bounds = Bounds()
#   .draw() is called to draw it
#
#
#  The Interface holds everything to do with the interface.
#
#
#  Interface
#   Initiated with (bodyType) where bodyType is
#    ommitted or the type of element the body will be
#
#   .body is the root element in the interface
#   .active is the current active element
#
#   .new(Element) creates a new element for it
#   .update() updates the interface (animations and such)
#   .redraw() the interface
#   .draw() the interface, returns True if it did
#
#   These are the event functions.  States are:
#    0 released
#    1 pressed
#    2 repeated
#
#   .string(string)
#   .key(key, state)
#   .button(button, state)
#   .mousemove(x, y)
#   .scroll(amount)
#
#   Elemental functions
#
#   .focus(Element)
#   .blur() the active element
#
#   .capture(Element) captures all events for an element
#   .uncapture() releases the capture
#
#   .mouse(Element) find where the mouse is at on an Element
#   .below() is which element is under the mouse
#    regardless of capture
#
#   .drag(Drag, button, button state, callback)
#    The cancel callback is optional
#   .cancelDrag(silent) cancels the drag
#    silent won't then call the cancel callback
#
#   .timeout(in, callback, args) sets a callback in a
#    amount of seconds.  args are supplied to the callback
#
#
#   And finally, the Element itself.
#
#
#   Element
#    soliddraw - if the element is not seethrough at all
#    focusable - if the element is focusable
#    imaginary - has no bounds, is just an offset
#
#   General Functions
#
#   .init() is called right after it is created
#   .append(Element, pos) inserts an element at the given
#    position, if ommitted it goes at the eld
#   .remove(Element)
#   .order(Element, pos) moves an element in the stacking
#   .traverse(reverse) recursively iterates all child elements
#   .loopTraverse(reverse) loops through all elements,
#    starting from this one
#   .childOf(Element) in complete ancestry
#   .globalOffset()
#   .getElementAt(x, y) find which element is on top at
#    these coordinates
#   .select(pos) checks to see if this pixel in the element
#    is considered to be part of it or not
#
#   .redraw() mark it for redrawing
#   .draw()
#   .refit()
#   .childfit() after it's children are done fitting in
#
#   Button calls
#
#   .click(button)
#   .mousedown(button)
#   .mouseup(button)
#
#   Active calls
#
#   .focus()
#   .blur()
#
#   .string(str)
#   .key(key) pressed
#   .keyup(key)
#   .keydown(key)
#
#   Over calls
#
#   .scroll(amount)
#
#   .mousemove(x, y)
#   .mouseon()
#   .mouseoff()
#
#   Drag calls
#
#   .DRAGON(Drag) - not capitalized -
#   .dragoff()
#   .dragmove(Drag, Offset)
#   .drop(Drag)

import scge
import glm

import time
import numbers
import math

from scge.shared.ui.styleTypes import Offset

# # # Types # # #

class Bounds:
	def __init__(self, x = 0, y = 0, w = 0, h = 0):
		if isinstance(x, Bounds):
			self.x = x.x
			self.y = x.y
			self.w = x.w
			self.h = x.h
		elif isinstance(x, Offset):
			self.x = x.x
			self.y = x.y
			if isinstance(y, Offset):
				self.w = y.x
				self.h = y.y
		else:
			self.x = x
			self.y = y
			self.w = w
			self.h = h
	
	def __setattr__(self, a, v):
		object.__setattr__(self, a, math.floor(v))
	
	def __repr__(self):
		return 'Bounds(' + str(self.x) + ', ' + str(self.y) + ', ' + str(self.w) + ', ' + str(self.h) + ')'
	
	def __iter__(self):
		yield self.x
		yield self.y
		yield self.w
		yield self.h
	
	def __bool__(self):
		return self.valid()
	
	def __eq__(self, other):
		if not isinstance(other, Bounds):
			raise NotImplementedError
		if other.x == self.x and other.y == self.y and other.w == self.w and other.h == self.h:
			return True
		return False
	
	def __add__(self, other):
		if isinstance(other, Offset):
			return Bounds(self.x + other.x, self.y + other.y, self.w, self.h)
		elif isinstance(other, Bounds):
			return Bounds(self.x + other.x, self.y + other.y, self.w + other.w, self.h + other.h)
		raise NotImplementedError
	
	def __sub__(self, other):
		if isinstance(other, Offset):
			return Bounds(self.x - other.x, self.y - other.y, self.w, self.h)
		elif isinstance(other, Bounds):
			return Bounds(self.x - other.x, self.y - other.y, self.w - other.w, self.h - other.h)
		raise NotImplementedError
	
	def valid(self):
		"It is valid if it's width and height are both greater than 0."
		return self.w > 0 and self.h > 0
	
	def overlap(self, other):
		"Crops it to the union of it and the other."
		if not isinstance(other, Bounds):
			raise NotImplementedError
		if self.x < other.x:
			self.w -= other.x - self.x
			self.x = other.x
		if self.y < other.y:
			self.h -= other.y - self.y
			self.y = other.y
		self.w = min(self.w, other.x - self.x + other.w)
		self.h = min(self.h, other.y - self.y + other.h)
	
	def within(self, other):
		"True if it is fully contained by the other."
		if self.x < other.x: return False
		if self.y < other.y: return False
		if self.x + self.w > other.x + other.w: return False
		if self.y + self.h > other.y + other.h: return False
		return True

# It's the actual thing that is dragged
# It is to be subclassed into useful drag types
class Drag:
	bounds = Bounds()
	def draw(self):
		pass

# # # ELEMENT # # #

class Element:
	focusable = False
	imaginary = False
	soliddraw = False
	def __init__(self, i = None):
		# Mechanics
		self.interface = i
		self.parent = None
		self.children = []
		self.dirty = True
		self.messy = False
		self._changed = False
		
		# Standard
		self.style = styleTypes.StyleDef(self._boundschanged, self.redraw)
		
		self.init()
	
	# Mechanics
	
	def traverse(self, reverse = False):
		"Method to walk through all child elements."
		for e in reversed(self.children) if reverse else self.children:
			yield e
			for a in e.traverse(reverse):
				yield a
	
	def _loopTraverse(self, reverse, i):
		i = self.children.index(i)
		
		n = len(self.children)
		
		if n > 1:
			for e in range(i - 1, -1, -1) if reverse else range(i + 1, n):
				c = self.children[e]
				yield c
				for a in c.traverse(reverse):
					yield a
		
		yield self
		
		if self.parent:
			for a in self.parent._loopTraverse(reverse, self):
				yield a
		
		if n > 1:
			for e in reversed(range(i - 1, -1, -1) if not reverse else range(i + 1, n)):
				c = self.children[e]
				yield c
				for a in c.traverse(reverse):
					yield a
	
	def loopTraverse(self, reverse = False):
		"Loop through children, then siblings, drop to parent, repeat until we're back here."
		if self.children:
			for e in self.traverse(reverse):
				yield e
		
		if self.parent:
			for e in self.parent._loopTraverse(reverse, self):
				yield e
	
	def getElementAt(self, pos, reverse = True, _reserved = Offset(0, 0)):
		"Find which element is on top under this position."
		# Outside of bounds = no
		if pos.x < 0 or pos.x >= self.style.size.x or pos.y < 0 or pos.y >= self.style.size.y:
			if not self.imaginary: return None, None
		
		# Children come first
		for e in reversed(self.children) if reverse else self.children:
			a = e.getElementAt(pos - e.style.offset, reverse, _reserved + e.style.offset)
			if a[0]: return a
		
		# Elements may not be perfectly square, so check for that
		if self.select(pos):
			return self, _reserved
		
		# Finally, well we tried everything...
		return None, None
	
	def globalOffset(self):
		"Returns the actual offset of the element according to the interface."
		o = Offset(self.style.offset)
		p = self.parent
		while p:
			o += p.style.offset
			p = p.parent
		
		return o 
	
	def childOf(self, e):
		"Check if this element is at all a child of another element."
		p = self.parent
		while p:
			if p is e:
				return True
			p = p.parent
		return False
	
	# Methods
	
	def append(self, e, i = None):
		"Give this element's children another sibling."
		# Remove it from any existing parent
		if e.parent:
			e.parent.remove(e)
		
		e.parent = self
		if i is None:
			self.children.append(e)
		else:
			self.children.insert(i, e)
		
		# Refit
		self._boundschanged()
	
	def order(self, e, i = None):
		self.children.remove(e)
		if i is None:
			self.children.append(e)
		else:
			self.children.insert(i, e)
		
		# Refit
		self._boundschanged()
	
	def remove(self, e):
		"Remove a child element."
		e.parent = None
		self.children.remove(e)
		
		# Refit
		self._boundschanged()
	
	def _draw(self, _clip, _offset = Offset(), parent_draw = False):
		if parent_draw:
			self.dirty = True
		
		parent_draw = self.dirty == True
		
		if not self.imaginary or isinstance(self.dirty, tuple):
			if not self.imaginary:
				_clip.overlap(Bounds(_offset, self.style.size))
			
			if isinstance(self.dirty, tuple):
				_clip.overlap(self.dirty[1] + _offset)
			if not _clip.valid():
				self.dirty = False
				return
			
			scge.scissor(_clip.x, _clip.y, _clip.w, _clip.h)
		
		if isinstance(self.dirty, tuple):
			drawc = True
			startfrom = self.children.index(self.dirty[0])
		else:
			drawc = self.draw() is not False
			startfrom = 0
		
		self.dirty = False
		
		if drawc:
			l = Offset()
			if startfrom:
				for e in self.children[startfrom:]:
					self.interface.matrix = InterfaceMatrix(self.interface.matrix.translate(glm.vec2(e.style.offset.x - l.x, e.style.offset.y - l.y)))
					e._draw(Bounds(_clip), Offset(_offset + e.style.offset), parent_draw)
					l = e.style.offset
			else:
				for e in self.children:
					self.interface.matrix = InterfaceMatrix(self.interface.matrix.translate(glm.vec2(e.style.offset.x - l.x, e.style.offset.y - l.y)))
					e._draw(Bounds(_clip), Offset(_offset + e.style.offset), parent_draw)
					l = e.style.offset
			self.interface.matrix = InterfaceMatrix(self.interface.matrix.translate(glm.vec2(-l.x, -l.y)))
	
	def _boundschanged(self, resized = True):
		self._changed = True
		if self.messy:
			return
		
		if resized:
			self.messy = True
		
		self.interface._refit(self)
		self._redraw(None, None, True)
	
	def _refit(self, up = False):
		# Suppose there is a dialog with a long, wrapping sentance in it.
		# The dialog is horizontally resizeable.
		# The dialog automatically expands vertically to hold all the text.
		# To refit the text, and compute it's height, we need to know
		# The width of the dialog.  So first the dialog resizes, then
		# the text refits to the width of the dialog and computes a height,
		# and finally the dialog resizes vertically to accomidate the text.
		# Now everyone is happy.
		self.refit()
		if self.messy:
			for e in self.children:
				e._refit(True)
		
		self._childfit(up)
		self.messy = False
		self._changed = False
	
	def _childfit(self, up = False):
		self.childfit()
		if not up and self.parent and self._changed:
			self.parent._childfit()
	
	def _redraw(self, e = None, b = None, c = False):
		# Already fully dirty
		if self.dirty is True:
			return
		
		if not e:
			# Full redraw
			self.dirty = True
		else:
			# b may get overwritten, but we need to know if bounds were specified
			bb = bool(b)
			
			# Make bounds in self's space
			if isinstance(b, Bounds):
				b = b + e.style.offset
			else:
				if e.imaginary:
					b = None
					self.dirty = True
				else:
					b = Bounds(e.style.offset, e.style.size)
			
			# Overlap bounds
			if b:
				if isinstance(self.dirty, tuple):
					if self.dirty[0] is e and e.soliddraw:
						if not b.within(self.dirty[1]):
							self.dirty = e, Bounds(e.style.offset, e.style.size)
						# Do nothing if redrawing a second element is contained already
					else:
						self.dirty = True
				else:
					# Not yet dirty, so use this element's bounds if it is solid
					if e.soliddraw or bb:
						self.dirty = (e, b)
					else:
						self.dirty = True
		
		if self.parent:
			self.parent._redraw((None if c else self), (b if isinstance(self.dirty, tuple) else None))
	
	def redraw(self, bc = False):
		"Mark for redraw.  The parameter is to specify if the bounds have changed."
		self._redraw(None, None, bc)
	
	# General
	
	def init(self):
		"Called right after the element is created."
		pass
	
	def select(self, p):
		"Called to know if this element claims this pixel."
		return True
	
	def draw(self):
		"Called when the element needs to be drawn.  Returning False skips drawing of children."
		pass
	
	def refit(self):
		"Called when the element might have to readjust itself.  A return of False skips fitting children."
		pass
	
	def childfit(self):
		"Called when the elements children have finished readjusting."
		pass
	
	# Mouse Calls
	
	def mousemove(self, p):
		"Called when the mouse moves from one place to another over the same element."
		pass
	
	def mouseon(self):
		"Called when the mouse enters the element."
		pass
	
	def mouseoff(self):
		"Called when the mouse leaves the element."
		pass
	
	def mousedown(self, b):
		"Called when the mouse goes down on the element."
		pass
	
	def mouseup(self, b):
		"Called when the mouse is released over the element."
		pass
	
	def click(self, b):
		"Called when the mouse went down on the same element and then was released without transforming into a drag."
		pass
	
	def scroll(self, a):
		"Called when the mouse is scrolled over the element."
		pass
	
	# Drag Calls
	
	def dragon(self, d):
		"Called when a dragitem was moved on the element."
		pass
	
	def dragmove(self, d, p):
		"Called when a drag moves above the element."
		pass
	
	def dragoff(self):
		"Called when a dragitem departed the element."
		pass
	
	def drop(self, d):
		"Called when a dragitem was dropped onto the element.  Shall return True if it ate the drag."
		return False
	
	# Active Calls
	
	def focus(self):
		"Called when the element recieves focus."
		pass
	
	def blur(self):
		"Called when the element loses focus."
		pass
	
	def string(self, s):
		"Called when a string was typed on the keyboard."
		pass
	
	def keydown(self, k):
		"Called when a key went down on the keyboard."
		pass
	
	def keyup(self, k):
		"Called when a key was lifted on the keyboard."
		pass
	
	def key(self, k):
		"Called when a key was pressed on the keyboard, including repeats."
		pass

# # # INTERFACE # # #

class InterfaceMatrix(glm.mat4):
	def translate(self, vec):
		if isinstance(vec, glm.vec2):
			return glm.mat4.translate(self, glm.vec3(vec, 0))
		return glm.mat4.translate(self, vec)

class Interface:
	def __init__(self, e = None):
		# List of callbacks that will be handled
		self.timeouts = []
		
		# Current element that has focus
		self.active = None
		
		# Element the mouse is over
		self.over = None
		
		# Active drag
		self.dragging = None
		
		# Element that is capturing all events, if any
		self.captured = None
		
		# For clicked
		self.last_down_on = None
		self.last_down_on_b = -1
		
		# Mouse position caches
		self.mouse_position_in_over = None
		self.mouse_global = (0, 0)
		self._mouse_moved_this_tick = False
		
		# There is a delay when changing the element to scroll so the mouse doesn't get caught
		self.scroll_element = None
		self.scroll_time = 0
		self.scroll_timeout = 1
		
		self.matrix = None
		
		self._refitting_state = False
		self.mess = set()
		
		# Root element
		if e:
			self.body = e(self)
		else:
			# Default body shall be solid clear color
			self.body = Element(self)
			self.body.draw = lambda: scge.clear()
			self.body.soliddraw = True
		
		# Default to covering the whole window
		dd = scge.window_dimensions()
		self.body.style.size = Offset(dd.x, dd.y)
	
	# Event functions
	
	def string(self, s):
		"Call this when a string is typed."
		if self.active:
			self.active.string(s)
	
	def key(self, k, s):
		"Call this when a key is pressed. (0 = released, 1 = pressed, 2 = repeated)"
		
		if self.captured:
			if s == 0:
				self.captured.keyup(k)
			else:
				if s == 1:
					self.captured.keydown(k)
				self.captured.key(k)
			return
		
		default = True
		if self.active:
			if s == 0:
				self.active.keyup(k)
			else:
				if s == 1:
					self.active.keydown(k)
				
				if self.active.key(k) == False:
					default = False
		
		# If element func returns False then cancel default action
		if not default:
			return
		
		# Defaults
		if s > 0:
			# Tab
			if k == 'tab':
				if not self.active:
					a = self.body
				else:
					a = self.active
			
				for e in a.loopTraverse(scge.key('shift')):
					if e.focusable and (not self.active or not self.active.childOf(e)):
						a = e
						break
			
				if a != self.active:
					self.focus(a)
	
	def button(self, b, s):
		"Call this when a button on the mouse is pressed (1) or released (0)."
		
		# Precedence order = captured, drag, any
		if self.captured:
			if s == 1:
				self.last_down_on = self.captured
				self.last_down_on_b = b
				self.captured.mousedown(b)
			elif s == 0:
				if self.last_down_on is self.captured and self.last_down_on_b == b:
					self.captured.click(b)
				self.captured.mouseup(b)
		elif self.dragging:
			if self.over:
				if b == self.dragging.button and s != self.dragging.buttonstate:
					# If the same-button-the-drag-started-with's state was inverted then it was a drop
					self.cancelDrag(self.over.drop(self.dragging.drag))
				else:
					# Otherwise cancel dragging
					self.cancelDrag()
		else:
			if self.over:
				if s == 1:
					self.last_down_on = self.over
					self.last_down_on_b = b
					self.over.mousedown(b)
					if self.over is not self.active:
						self.focus(self.over if self.over.focusable else None)
				elif s == 0:
					if self.last_down_on is self.over and self.last_down_on_b == b:
						self.over.click(b)
					self.over.mouseup(b)
	
	def mousemove(self, x = None, y = None):
		"Call this when the mouse moves."
		
		# We don't need overloads of mousemoves
		# But we might want them for precision in some apps...
		if x:
			self._mouse_moved_this_tick = (x, y)
		elif not self._mouse_moved_this_tick:
			self._mouse_moved_this_tick = True
	
	def _mousemove(self, x, y):
		# Moving the mouse cancels the scroll timeout
		self.scroll_time = 0
		
		# Calling mousemove() with no position arguments assumes the mouse moved in z
		if x is not None:
			self.mouse_global = (x, y)
			if self.dragging:
				self.body.dirty = True
		else:
			if not self.mouse_global:
				return
			x, y = self.mouse_global
		
		# Captures get all mousemove events
		if self.captured:
			go = self.captured.globalOffset()
			mouse_position_in_over = (x - go.x, y - go.y)
			
			if self.mouse_position_in_over != mouse_position_in_over:
				self.mouse_position_in_over = mouse_position_in_over
				if self.over is not self.captured:
					if self.over:
						self.over.mouseoff()
					self.over = self.captured
					self.over.mouseon(Offset(*self.mouse_position_in_over))
			
				self.captured.mousemove(Offset(*self.mouse_position_in_over))
			return
		
		# Find the element the mouse is now on
		over, go = self.body.getElementAt(Offset(x - self.body.style.offset.x, y - self.body.style.offset.y))
		
		if over is None and self.over is None:
			return
		
		if go is not None:
			go += self.body.style.offset
		
		# Handle move events
		if over is self.over:
			mouse_position_in_over = (x - go.x, y - go.y)
			if mouse_position_in_over != self.mouse_position_in_over:
				self.mouse_position_in_over = mouse_position_in_over
				if self.dragging:
					self.over.dragmove(self.dragging.drag, Offset(*mouse_position_in_over))
				else:
					self.over.mousemove(Offset(*mouse_position_in_over))
		else:
			if self.over:
				if self.dragging:
					self.over.dragoff()
				else:
					self.over.mouseoff()
			self.over = over
			if self.over:
				self.mouse_position_in_over = (x - go.x, y - go.y)
				if self.dragging:
					self.over.dragon(self.dragging.drag)
					self.over.dragmove(self.dragging.drag, Offset(*self.mouse_position_in_over))
				else:
					self.over.mouseon()
					self.over.mousemove(Offset(*self.mouse_position_in_over))
	
	def scroll(self, o):
		t = time.time()
		if self.captured:
			self.captured.scroll(o)
		elif not self.dragging:
			if not self.scroll_element or t - self.scroll_time > self.scroll_timeout:
				self.scroll_element = self.over
			if self.scroll_element:
				self.scroll_element.scroll(o)
		
		self.scroll_time = t
	
	# Element Functions
	
	def focus(self, e = None):
		"Focuses an element."
		if self.active:
			self.active.blur()
		self.active = e
		if self.active:
			self.active.focus()
	
	def blur(self):
		"Blurs the active element."
		if self.active:
			self.active.blur()
		self.active = None
	
	def capture(self, e):
		"Cause an element to capture all events for itself.  It will recieve mousemove events outside of it's bounds.  Returns True on success, and False on failure."
		
		if self.captured:
			print('ui: Refusing to capture for', e, 'as is already captured by', self.captured)
			return False
		
		if self.dragging:
			print('ui: Cannot capture while dragging.')
			return
		
		if self.active is not self and self.active:
			self.active.blur()
			self.active = None
		
		self.captured = e
		
		self.mousemove()
		
		return True
	
	def uncapture(self):
		"Release captured state."
		
		if not self.captured:
			print('ui: Captured state was attempted to be released but it is not in a captured state.')
			return
		
		self.captured = None
		
		self.mousemove()
	
	def drag(self, d, b, s, c = None):
		"Initiates a drag."
		
		if self.captured:
			print('ui: Cannot begin a drag while captured.')
			return
		
		if self.dragging:
			print('ui: There is already a drag.')
			return
		
		if self.over:
			self.over.mouseoff()
			self.over = None
		
		self.dragging = _DragMechanics(d, b, s, c)
		
		self.mousemove()
		self.body.dirty = True
	
	def cancelDrag(self, silent = False):
		"Cancels the drag, optionally skipping the cancel callback."
		
		if not self.dragging:
			print('ui: No drag to cancel yet cancelDrag was called.')
			return
		
		if not silent and self.dragging.call:
			self.dragging.call()
		
		if self.over:
			self.over.dragoff()
			self.over = None
		
		self.dragging = None
		
		self.mousemove()
		self.body.dirty = True
	
	def mouse(self, e):
		"Returns the mouse position in the element."
		return Offset(*self.mouse_global) - e.globalOffset()
	
	def below(self):
		"Find the real element that is below the mouse."
		return self.body.getElementAt(Offset(*self.mouse_global) - self.body.style.offset)[0]
	
	def timeout(self, t, f, *a):
		t = _Timeout(time.time() + t, f, *a)
		self.timeouts.append(t)
		return t
	
	# Game Functions
	
	def new(self, t):
		"Returns a new element of given type in this interface."
		return t(self)
	
	def update(self):
		"Update timeouts."
		
		if self._mouse_moved_this_tick:
			if self._mouse_moved_this_tick is True:
				self._mousemove(None, None)
			else:
				self._mousemove(*self._mouse_moved_this_tick)
			self._mouse_moved_this_tick = False
		
		t = 0
		tme = time.time()
		while t < len(self.timeouts):
			if self.timeouts[t](tme):
				del self.timeouts[t]
			else:
				t += 1
	
	def redraw(self):
		"Mark the interface for a redraw."
		self.body.redraw()
	
	def _refit(self, e):
		if self._refitting_state is True:
			return False
		self.mess.add(e)
		return True
	
	def draw(self):
		"Draw the interface."
		
		# Refit
		
		if self.mess:
			self._refitting_state = True
			for e in self.mess:
				e._refit()
			self._refitting_state = False
			self.mess = set()
			self.mousemove()
		
		# Draw no matter what if the interface is seethrough
		if not self.body.soliddraw:
			self.body.dirty = True
		
		if self.body.dirty:
			self.matrix = InterfaceMatrix(glm.ortho(self.body.style.offset.x, self.body.style.offset.x + self.body.style.size.x, self.body.style.offset.y, self.body.style.offset.y + self.body.style.size.y, -1, 1))
			scge.viewport(self.body.style.offset.x, self.body.style.offset.y, self.body.style.size.x, self.body.style.size.y)
			
			scge.enable('scissor')
			
			self.matrix = InterfaceMatrix(self.matrix.translate(glm.vec2(self.body.style.offset)))
			self.body._draw(Bounds(self.body.style.offset, self.body.style.size), Offset(self.body.style.offset))
			self.matrix = InterfaceMatrix(self.matrix.translate(-glm.vec2(self.body.style.offset)))
			self.body.dirty = False
		
			scge.enable('scissor', False)
		
			if self.dragging:
				self.matrix = InterfaceMatrix(self.matrix.translate(glm.vec2(self.mouse_global)))
				self.dragging.drag.draw()
				self.matrix = InterfaceMatrix(self.matrix.translate(-glm.vec2(self.mouse_global)))
		
			return True
		return False

# # # Mechanics # # #

# The backbone of the drag
class _DragMechanics:
	def __init__(self, d, b, s, c):
		self.drag = d
		self.button = b
		self.buttonstate = s
		self.call = c

# A timeout to be called
class _Timeout:
	def __init__(self, when, func, *args):
		self.func = func
		self.when = when
		self.args = args
	
	def __bool__(self):
		return self.when >= 0
	
	def kill(self, call = False):
		if call:
			self(-1)
		self.when = -1
	
	def __call__(self, t):
		if self.when < 0:
			return True
		
		if t >= self.when or t < 0:
			self.func(*self.args)
			self.when = -1
			return True
		return False
