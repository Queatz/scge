"Draw basic shapes.  Performance is not a factor here."

import scge
import glm
import struct

_initstate = 0
_texcoordsdirty = True

s_f = struct.calcsize('=f')
s_f2_4 = s_f * 2 * 4
s_f6_4 = s_f * 6 * 4
s_f8_4 = s_f * 8 * 4

def _setup(wd):
	global _wd, _program, _font_program, _vao, _font_vbo, _font_vao, _vbo, _initstate, _matrix, _white, _img, _color
	
	_wd = wd
	
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
	#extension GL_ARB_texture_rectangle : require
	
	in vec2 texcoord;
	
	uniform sampler2DRect tex;
	uniform vec4 color;

	out vec4 frag;

	void main() {
		frag = color;
		frag.a *= texture2DRect(tex, texcoord).r;
		if(frag.a == 0.)
			discard;
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
		if(frag.a == 0.)
			discard;
	}
	''')
	
	_program = scge.program()
	_program.attach(_vshader)
	_program.attach(_fshader)
	_program.attribute(0, 'coords')
	_program.attribute(1, 'colors')
	_program.attribute(2, 'texcoords')
	_program.link()

	_vbo = scge.vbo(s_f8_4, 'stream draw')

	_vao = scge.vao()
	_wd.use(_vao)
	_vao.enable(0)
	_vao.enable(1)
	_vao.enable(2)
	_vao.attribute(0, _vbo, 'float', 2, 0)
	_vao.attribute(1, _vbo, 'float', 4, s_f2_4)
	_vao.attribute(2, _vbo, 'float', 2, s_f6_4)
	
	_font_vbo = scge.vbo(s_f6_4, 'stream draw')
	
	_font_vao = scge.vao()
	_wd.use(_font_vao)
	_font_vao.enable(0)
	_font_vao.enable(1)
	_font_vao.attribute(0, _font_vbo, 'float', 2, 0, s_f * 4)
	_font_vao.attribute(1, _font_vbo, 'float', 4, s_f * 2, s_f * 4)
	
	p = scge.pixelcache(glm.ivec2(1, 1))
	p.pixel(glm.ivec2(0, 0), glm.vec4(1))
	_white = scge.image(p)
	
	_program.uniform('tex', _white)
	_img = _white
	
	_matrix = glm.ortho(0, wd.size().x, 0, wd.size().y, -1, 1)
	
	_usingDefaultProgram = True
	
	color(glm.vec4(1))
	
	_initstate = 1

def begin(wd, defaultProgram = True):
	global _usingDefaultProgram
	if not _initstate:
		_setup(wd)
	
	_wd.use(_vao)
	_usingDefaultProgram = defaultProgram
	
	if defaultProgram:
		_wd.use(_program)
		_program.uniform('matrix', _matrix)
		_program.uniform('tex', _img)

def matrix(m):
	global _matrix, _program
	_matrix = m
	_program.uniform('matrix', m)

def _expand_color(c):
	if len(c) == 1:
		return glm.vec4(c[0], c[0], c[0], 1)
	elif len(c) == 2:
		return glm.vec4(c[0], c[0], c[0], c[1])
	elif len(c) == 3:
		return glm.vec4(c[0], c[1], c[2], 1)
	elif len(c) == 4:
		return c
	return glm.vec4(1)

def color(p1 = None, p2 = None, p3 = None, p4 = None):
	global _color
	if p1 is None:
		_color = glm.vec4(1)
		_vbo.data(bytes(_color) * 4, s_f2_4)
	elif p2 is None:
		_color = _expand_color(p1)
		_vbo.data(bytes(_color) * 4, s_f2_4)
	elif p3 is None or p4 is None:
		raise Exception('Color must specify 1 point or all four.')
	else:
		_vbo.data(bytes(_expand_color(p1)) + bytes(_expand_color(p2)) + bytes(_expand_color(p3)) + bytes(_expand_color(p4)), s_f2_4)

def texcoord(p1 = None, p2 = None, p3 = None, p4 = None):
	global _texcoordsdirty
	
	if p1 is None:
		p1 = glm.vec2(0)
		p2 = glm.vec2(0, 1)
		p3 = glm.vec2(1)
		p4 = glm.vec2(1, 0)
	elif p2 is None:
		p2 = p3 = p4 = p1
	elif p3 is None:
		p3 = p2
		p4 = glm.vec2(p3.x, p1.y)
		p2 = glm.vec2(p1.x, p3.y)
	else:
		raise Exception('Texcoord accepts zero, one, two, or four arguments.')
	
	_texcoordsdirty = True
	_vbo.data(bytes(p1) + bytes(p2) + bytes(p3) + bytes(p4), s_f6_4)

def quad(a, b):
	_vbo.data(struct.pack('ffffffff', a.x, a.y, a.x, b.y, b.x, b.y, b.x, a.y))
	_wd.draw('triangle fan', 4)

def triangle(a, b, c):
	_vbo.data(bytes(a) + bytes(b) + bytes(c), 0)
	_wd.draw('triangle', 3)

def line(a, b):
	_vbo.data(bytes(a) + bytes(b), 0)
	_wd.draw('line', 2)

def point(a):
	_vbo.data(bytes(a), 0)
	_wd.draw('point', 1)

def image(img = None):
	global _img
	_img = (img if img else _white)
	_program.uniform('tex', _img)

_itc = struct.pack('ffffffff', 0, 0, 0, 1, 1, 1, 1, 0)

def draw(p = None, s = None, r = None, o = None):
	if r is None: r = 0
	if p is None: p = glm.vec2(0)
	if s is None: s = glm.vec2(1)
	if o is None: o = glm.vec2(0)
	global _texcoordsdirty
	
	_program.uniform('matrix', _matrix.translate(glm.vec3(p, 0)).scale(glm.vec3(s, 1)).rotate(r, glm.vec3(0, 0, 1)).translate(glm.vec3(-o, 0)).scale(glm.vec3(_img.size, 1)))
	q = p + glm.vec2(_img.size) * s
	if _texcoordsdirty:
		_vbo.data(_itc, s_f8_4)
		_texcoordsdirty = False
	_vbo.data(_itc, 0)
	_wd.draw('triangle fan', 4)
	
	_program.uniform('matrix', _matrix)

def write(fnt, sttr, p = None, mw = None):
	if p is None: p = glm.vec2(0)
	s = glm.vec2(p)
	_wd.use(_font_program)
	_font_program.uniform('matrix', _matrix)
	_font_program.uniform('color', _color)
	_wd.use(_font_vao)

	_program.font('tex')
	lc = None
	for c in sttr:
		if lc:
			a = fnt.advance(lc, c)
		else:
			a = 0
		
		if c == '\n' or mw and p.x + a + fnt.advance(c) > mw:
			p.x = s.x
			p.y -= fnt.height()
			lc = None
			continue
		
		g = fnt.glyph(c)
		
		p.x += a

		_font_vbo.data(struct.pack('ffffffffffffffff',
			g.vertices.x1 + p.x,
			g.vertices.y1 + p.y,
			g.texcoords.x1,
			g.texcoords.y1,

			g.vertices.x1 + p.x,
			g.vertices.y2 + p.y,
			g.texcoords.x1,
			g.texcoords.y2,

			g.vertices.x2 + p.x,
			g.vertices.y2 + p.y,
			g.texcoords.x2,
			g.texcoords.y2,

			g.vertices.x2 + p.x,
			g.vertices.y1 + p.y,
			g.texcoords.x2,
			g.texcoords.y1,
		), 0)

		_wd.draw('triangle fan', 4)
		
		lc = c

	_wd.use(_vao)
	_wd.use(_program)

def advance(fnt, sttr):
	lc = None
	a = 0
	for c in sttr:
		g = fnt.glyph(c)
		if lc:
			a += fnt.advance(lc, c)
		lc = c
	
	return a
