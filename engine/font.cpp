void setdown_font() {
	delete font_library;
}

void setup_font() {
	if(glfw_state == 0)
		graphics();
	font_library = new Shikoba::Library();
	
	atexit(setdown_font);
}

font::font(const char* a, unsigned int s) {
	if(font_library == NULL)
		setup_font();
	
	data = new Shikoba::Face(font_library, a);
	
	if(s)
		data->size(s);
}

font::~font() {
	delete data;
}

void font::size(unsigned int s) {
	data->size(s);
}

float font::height() {
	return data->height();
}

float font::ascent() {
	return data->ascender();
}

float font::descent() {
	return data->descender();
}

float font::advance(const char* a, const char* b) {
	if(*a == 0x0 || (b && *b == 0x0))
		return 0.0;

	return data->advance(utf8::unchecked::next(a), b ? utf8::unchecked::next(b) : 0);
}

glyphmetrics font::glyph(const char* a) {
	glyphmetrics g;
	
	if(*a == 0x0)
		return g;
	
	const Shikoba::Glyph * s = data->glyph(utf8::unchecked::next(a));

	// XXX FIXME //
	g.vertices.x1 = s->vertices[0];
	g.vertices.x2 = s->vertices[1];
	g.vertices.y1 = s->vertices[2];
	g.vertices.y2 = s->vertices[3];
	g.texcoords.x1 = s->texcoords[0];
	g.texcoords.x2 = s->texcoords[1];
	g.texcoords.y1 = s->texcoords[2];
	g.texcoords.y2 = s->texcoords[3];

	return g;
}
