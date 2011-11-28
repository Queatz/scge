from scge.shared.ui import Element
from scge.shared.ui.styleTypes import Margin, Size
from scge.shared.ui.common.intrinsics import Layout

import scge

class Wallpaper(Layout):
	"Good for a body."
	soliddraw = True
	def init(self):
		Layout.init(self)
		self.style.backgroundColor = scge.rgba(1) # scge.rgba
		self.style.backgroundImage = None # scge.image | None
		self.style.backgroundOffset = Size() # number | tuple
		self.style.backgroundPinning = Size() # number | tuple
		self.style.backgroundMapping = 'fixed' # fixed | scale | stretch
		self.style.backgroundScale = Size(1) # number | tuple
		self.style.backgroundAxis = 'y' # 'x' | 'y' | 'min' | 'max'
	
	def draw(self):
		scge.color(self.style.backgroundColor)
		scge.quad(0, 0, *self.style.size)
		if not self.style.backgroundImage:
			return
		
		scge.enable('texture')
		scge.use_image(self.style.backgroundImage)
		
		sx, sy = self.style.backgroundScale
		px, py = self.style.backgroundPinning
		ox, oy = self.style.backgroundOffset
		
		scge.begin('quad')
		if self.style.backgroundMapping == 'fixed':
			iw, ih = self.style.size.x / self.style.backgroundImage.width / sx, self.style.size.y / self.style.backgroundImage.height / sy
			px, py, iw, ih = -iw*px - ox, -ih*py - oy, iw * (1 - px) - ox, ih * (1 - py) - oy
			scge.texture_coordinates(px, py); scge.ipoint(0, 0)
			scge.texture_coordinates(px, ih); scge.ipoint(0, self.style.size.y)
			scge.texture_coordinates(iw, ih); scge.ipoint(self.style.size.x, self.style.size.y)
			scge.texture_coordinates(iw, py); scge.ipoint(self.style.size.x, 0)
		elif self.style.backgroundMapping == 'scale':
			sx = 1 / sx
			sy = 1 / sy
			if self.style.backgroundAxis == 'y' or self.style.backgroundAxis == 'min' and self.style.size.y < self.style.size.x or self.style.backgroundAxis == 'max' and self.style.size.y > self.style.size.x:
				sx *= self.style.size.x / self.style.size.y
			elif self.style.backgroundAxis == 'x' or self.style.backgroundAxis == 'min' and self.style.size.x < self.style.size.y or self.style.backgroundAxis == 'max' and self.style.size.x > self.style.size.y:
				sy *= self.style.size.y / self.style.size.x
			else:
				print('Wallpaper: invalid backgroundAxis:', self.style.backgroundAxis)
				ax, ay = 1, self.style.size.y / self.style.size.x
			px, py, iw, ih = -sx*px - ox, -sy*py - oy, sx * (1 - px) - ox, sy * (1 - py) - oy
			scge.texture_coordinates(px, py); scge.ipoint(0, 0)
			scge.texture_coordinates(px, ih); scge.ipoint(0, self.style.size.y)
			scge.texture_coordinates(iw, ih); scge.ipoint(self.style.size.x, self.style.size.y)
			scge.texture_coordinates(iw, py); scge.ipoint(self.style.size.x, 0)
		elif self.style.backgroundMapping == 'stretch':
			sx = 1 / sx
			sy = 1 / sy
			scge.texture_coordinates(-ox, -oy); scge.ipoint(0, 0)
			scge.texture_coordinates(-ox, sy - oy); scge.ipoint(0, self.style.size.y)
			scge.texture_coordinates(sx - ox, sy - oy); scge.ipoint(self.style.size.x, self.style.size.y)
			scge.texture_coordinates(sx - ox, -oy); scge.ipoint(self.style.size.x, 0)
		else:
			print('Wallpaper: invalid backgroundMapping:', self.style.backgroundMapping)
		scge.end()
		scge.enable('texture', False)
