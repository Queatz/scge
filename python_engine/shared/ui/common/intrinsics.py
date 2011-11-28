from scge.shared.ui import Element
from scge.shared.ui.styleTypes import Margin

class Layout(Element):
	def init(self):
		self.style.margin = Margin()
		self.style.padding = Margin()
		self.style.align = 'left' # 'left' | 'center' | 'right'
		self.style.verticalAlign = 'top' # 'top' | 'middle' | 'bottom' | 'expand'
	
	def refit(self):
		pass
