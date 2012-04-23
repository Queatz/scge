void setdown_font() {
	delete font_library;
}

void setup_font() {
	if(glfw_state == 0)
		graphics();
	font_library = new Shikoba::Library();
	atexit(setdown_font);
}

font::font(const char* a) {
	if(font_library == NULL)
		setup_font();
	
	data = new Shikoba::Face(font_library, a);
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
	return data->advance(utf8::unchecked::next(a), utf8::unchecked::next(b));
}

glyphmetrics font::glyph(const char* a) {
	glyphmetrics g;
	const Shikoba::Glyph * s = data->glyph(utf8::unchecked::next(a));

	// XXX FIXME //
	g.vertices.x1 = s->vertices.x1;
	g.vertices.x2 = s->vertices.x2;
	g.vertices.y1 = s->vertices.y1;
	g.vertices.y2 = s->vertices.y2;
	g.texcoords.x1 = s->texcoords.x1;
	g.texcoords.x2 = s->texcoords.x2;
	g.texcoords.y1 = s->texcoords.y1;
	g.texcoords.y2 = s->texcoords.y2;

	return g;
}
