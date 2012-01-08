"Draw basic shapes.  Performance is not a factor here."

import scge
import glm
import struct
import numbers

_initstate = 0

def _setup():
	global _program, _font_program, _vao, _font_vao, _vbo, _initstate, _matrix, _white, _img, _color
	
	_font_vshader = scge.shader('vertex', '''#version 330 core
	in vec2 coords;
	in vec2 texcoords;

	uniform mat4 matrix;
	
	out vec2 texcoord;

	void main() {
		texcoord = texcoords;
	
		gl_Position = matrix * vec4(coords, 0.0, 1.0);
	}
	''')
	
	_font_fshader = scge.shader('fragment', '''#version 330 core
	in vec2 texcoord;
	
	uniform sampler2D tex;
	uniform vec4 color;

	out vec4 frag;

	void main() {
		frag = color;
		frag.a *= texture2D(tex, texcoord).r;
	}
	''')

	_font_program = scge.program()
	_font_program.attach(_font_vshader)
	_font_program.attach(_font_fshader)
	_font_program.attribute(0, 'coords')
	_font_program.attribute(1, 'texcoords')
	_font_program.attribute(2, 'ink')
	_font_program.link()
	
	_vshader = scge.shader('vertex', '''#version 330 core
	in vec2 coords;
	in vec4 colors;
	in vec2 texcoords;

	uniform mat4 matrix;

	out vec4 color;
	out vec2 texcoord;

	void main() {
		color = colors;
		texcoord = texcoords;
	
		gl_Position = matrix * vec4(coords, 0.0, 1.0);
	}
	''')

	_fshader = scge.shader('fragment', '''#version 330 core
	in vec4 color;
	in vec2 texcoord;
	
	uniform sampler2D tex;

	out vec4 frag;

	void main() {
		frag = texture2D(tex, texcoord) * color;
	}
	''')
	
	_program = scge.program()
	_program.attach(_vshader)
	_program.attach(_fshader)
	_program.attribute(0, 'coords')
	_program.attribute(1, 'colors')
	_program.attribute(2, 'texcoords')
	_program.link()

	_vbo = scge.vbo((4 * 2 + 4 * 4 + 4 * 2) * 4, 'stream draw')

	_vao = scge.vao()
	scge.use_vao(_vao)
	_vao.enable(0)
	_vao.enable(1)
	_vao.enable(2)
	_vao.attribute(0, _vbo, 'float', 2, 0)
	_vao.attribute(1, _vbo, 'float', 4, (4 * 2) * 4)
	_vao.attribute(2, _vbo, 'float', 2, (4 * 2 + 4 * 4) * 4)
	
	_font_vao = scge.vao()
	scge.use_vao(_font_vao)
	_font_vao.enable(0)
	_font_vao.enable(1)

	scge.use_vao()
	
	p = scge.pixelcache(1, 1)
	p.set_pixel(0, 0, glm.vec4(1))
	_white = scge.image(p)
	
	_program.uniform('tex', _white)
	_img = _white
	
	_matrix = glm.mat4()
	
	_initstate = 1
	
	_usingDefaultProgram = True
	
	color(1)

def begin(defaultProgram = True):
	global _usingDefaultProgram
	if not _initstate:
		_setup()
	
	scge.use_vao(_vao)
	_usingDefaultProgram = defaultProgram
	
	if defaultProgram:
		scge.use_program(_program)
		_program.uniform('matrix', _matrix)
		_program.uniform('tex', _img)

def end():
	if _usingDefaultProgram:
		scge.use_program()
	scge.use_vao()

def matrix(m):
	global _matrix
	_matrix = m
	_program.uniform('matrix', m)

def _expand_color(c):
	if len(c) == 1:
		return (c[0], c[0], c[0], 1)
	elif len(c) == 2:
		return (c[0], c[0], c[0], c[1])
	elif len(c) == 3:
		return (c[0], c[1], c[2], 1)
	elif len(c) == 4:
		return c
	return (1, 1, 1, 1)

def color(p1 = None, p2 = None, p3 = None, p4 = None):
	global _color
	if isinstance(p1, glm.Vector):
		c = _expand_color(p1)
		_color = glm.vec4(*c)
		_vbo.data(struct.pack('ffff' * 4, *(c * 4)), (4 * 2) * 4)
	elif isinstance(p1, numbers.Number):
		c = _expand_color(((p1,) + ((p2,) + ((p3,) + ((p4,) if p4 is not None else ()) if p3 is not None else ()) if p2 is not None else ()) if p1 is not None else ()))
		_color = glm.vec4(*c)
		_vbo.data(struct.pack('ffff' * 4, *(c * 4)), (4 * 2) * 4)
	else:
		if p1 is None:
			c = (1,) * 4
			_color = glm.vec4(*c)
			_vbo.data(struct.pack('ffff' * 4, c), (4 * 2) * 4)
		elif p2 is None:
			c = _expand_color(p1)
			_color = glm.vec4(*c)
			_vbo.data(struct.pack('ffff' * 4, *(c * 4)), (4 * 2) * 4)
		elif p3 is None or p4 is None:
			print('Color must specify 1 point or all four.')
		else:
			_vbo.data(struct.pack('ffff' * 4, *(_expand_color(p1) + _expand_color(p2) + _expand_color(p3) + _expand_color(p4))), (4 * 2) * 4)

def texcoord(p1 = (0, 0), p2 = (0, 0), p3 = (0, 0), p4 = (0, 0)):
	_vbo.data(struct.pack('ff' * 4, *(p1 + p2 + p3 + p4)), (4 * 2 + 4 * 4) * 4)

def quad(x, y, x2, y2):
	_vbo.data(struct.pack('ff' * 4, x, y, x, y2, x2, y2, x2, y), 0)
	scge.draw('triangle fan', 4)

def triangle(x, y, x2, y2, x3, y3):
	_vbo.data(struct.pack('ff' * 3, x, y, x2, y2, x3, y3), 0)
	scge.draw('triangle', 3)

def line(x, y, x2, y2):
	_vbo.data(struct.pack('ff' * 2, x, y, x2, y2), 0)
	scge.draw('line', 2)

def point(x, y):
	_vbo.data(struct.pack('ff' * 1, x, y), 0)
	scge.draw('point', 1)

def image(img = None):
	global _img
	_img = (img if img else _white)
	_program.uniform('tex', _img)

def draw(x = 0, y = 0, s = 1):
	x2 = x + _img.width * s
	y2 = y + _img.height * s
	_vbo.data(struct.pack('ff' * 4, 0, 0, 0, 1, 1, 1, 1, 0), (4 * 2 + 4 * 4) * 4)
	_vbo.data(struct.pack('ff' * 4, x, y, x, y2, x2, y2, x2, y), 0)
	scge.draw('triangle fan', 4)

_saved_font_strings = {}

def write(fnt, sttr, x = 0, y = 0):
	T = (id(fnt), sttr)
	if T not in _saved_font_strings:
		_saved_font_strings[T] = scge.text(fnt, sttr)
	
	scge.use_program(_font_program)
	_font_program.uniform('matrix', _matrix.translate(glm.vec3(x, y, 0)))
	_font_program.uniform('color', _color)
	scge.use_vao(_font_vao)
	_saved_font_strings[T].draw(0, 1)
	scge.use_vao(_vao)
	scge.use_program(_program)
