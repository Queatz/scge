sound::sound(const char* a, bool b) {
	is_stream = b;
	is_playing = false;
	looping = false;
	
	source = NULL;
	int i;
	for(i = 0; i < NUM_BUFS; i++)
		buffer[i] = NULL;
	
	if(is_stream)
		stream = alureCreateStreamFromFile(a, 19200, NUM_BUFS, buffer);
	else {
		buffer[0] = alureCreateBufferFromFile(a);
		if(buffer[0] == AL_NONE)
			err("sound", "could not load");
	}
	
	alGenSources(1, &source);
	
	if(!is_stream)
		alSourcei(source, AL_BUFFER, buffer[0]);
	
	//alSourcef(source, AL_GAIN, 1.0);
	//alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
	//alSourcef(source, AL_ROLLOFF_FACTOR, 0.0);
}

sound::~sound() {
	ALint queued;
	alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
	alSourceUnqueueBuffers(source, queued, buffer);
	alSourcei(source, AL_BUFFER, NULL);
	alDeleteSources(1, &source);
	alDeleteBuffers(NUM_BUFS, buffer);
	is_playing = false;
	looping = false;
}

void sound::gain(float a) {
	alSourcef(source, AL_GAIN, a);
}

void sound::pitch(float a) {
	alSourcef(source, AL_PITCH, a);
}

void sound::pan(float a) {
	alSource3f(source, AL_POSITION, a, 0.0, 0.0);
}

void sound::repeat(bool a) {
	looping = a;
	if(!is_stream)
		alSourcei(source, AL_LOOPING, (a ? AL_TRUE : AL_FALSE));
}

void sound::update() {
	if(!is_stream)
		return;
	
	ALint state = AL_PLAYING;
	ALint processed = 0;

	alGetSourcei(source, AL_BUFFERS_QUEUED, &processed);
	
	// Just started
	if(processed <= 0)
		alSourceQueueBuffers(source, NUM_BUFS, buffer);
	
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	
	if(processed > 0) {
		alSourceUnqueueBuffers(source, processed, buffer);
		
		ALint added = alureBufferDataFromStream(stream, processed, buffer);
		if(added <= 0) {
			if(looping) {
				alureRewindStream(stream);
				alureBufferDataFromStream(stream, processed, buffer);
			} else
				is_playing = false;
		}

		alSourceQueueBuffers(source, processed, buffer);
	}
	
	if(is_playing and state != AL_PLAYING)
		alSourcePlay(source);
	
	return;
}

pixelcache::pixelcache(const pixelcache& a) {
	width = a.width;
	height = a.height;
	
	int s = height*width*3;
	data = new GLubyte[s];
	memcpy(data, a.data, sizeof(GLubyte)*s);
}

pixelcache::pixelcache(int w, int h) {
	width = w;
	height = h;
	
	data = new GLubyte[(int)height*(int)width*3];
}

pixelcache::~pixelcache() {
	if(data)
		delete data;
}

image::image(const char* a) {
	id = SOIL_load_OGL_texture(a, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); //NULL
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	cache = NULL;
}

image::image(int a, int b, bool quality) {
	width = a;
	height = b;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, (quality ? GL_RGB12 : GL_RGB8), a, b, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	cache = NULL;
}

/*void image::save(const char* a) {
	unsigned char b[1] = {id};
	if(!SOIL_save_image(a, SOIL_SAVE_TYPE_DDS, width, height, 3, b))
		err("image", "save", "could not save image");
	
}*/

image::~image() {
	if(cache)
		delete cache;
}

void image::set(const char* a) {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	if (!strcmp(a, "linear")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else if (!strcmp(a, "nearest")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else if (!strcmp(a, "mirrored repeat")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	} else if (!strcmp(a, "repeat")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else if (!strcmp(a, "clamp")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(pixelcache* a) {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	if(a)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, a->data);
	else if(cache)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	else
		err("image", "from_pixelcache", "nothing to copy");
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::refresh_pixel_cache() {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	if(!cache)
		cache = new pixelcache(width, height);
	
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

rgba image::pixel(int x, int y) {
	int h = height, w = width;
	
	if(x < 0 || x >= w || y < 0 || y >= h) {
		err("image", "pixel", "out of bounds");
		return rgba(0, 0, 0, 0);
	}
	
	if(!cache)
		refresh_pixel_cache();
	
	h = y * w * 4 + x * 4;
	return rgba(cache->data[h + 0], cache->data[h + 1], cache->data[h + 2], cache->data[h + 3]);
}


font::font(const char* a, float b, const char* c) {
	if(!b)
		b = 12;
	if(!strcmp("bitmap", c))
		data = new FTBitmapFont(a);
	else if(!strcmp("pixmap", c))
		data = new FTPixmapFont(a);
	else if(!strcmp("outline", c))
		data = new FTOutlineFont(a);
	else if(!strcmp("polygon", c))
		data = new FTPolygonFont(a);
	else if(!strcmp("buffer", c))
		data = new FTBufferFont(a);
	else if(!strcmp("extrude", c))
		data = new FTExtrudeFont(a);
	else if(!strcmp("texture", c))
		data = new FTTextureFont(a);
	else {
		err("font", "invalid type");
		return;
	}
	data->FaceSize(b);
	
	size_default = b;
	
	if(data->Error())
		err("font", "could not load font");
}

void font::size(float a) {
	size_stack.top() = a;
	data->FaceSize(a);
}

void font::depth(float a) {
	data->Depth(a);
}

void font::outset(float a) {
	data->Outset(a);
}

void font::outset(float a, float b) {
	data->Outset(a, b);
}

font::~font() {
	delete data;
}

void font::push_size() {
	if(size_stack.empty())
		size_stack.push(size_default);
	else
		size_stack.push(size_stack.top());
}

void font::pop_size() {
	if(size_stack.size() > 1)
		size_stack.pop();
	else
		err("font", "pop_size", "already at begining of stack");
	
	if(size_stack.empty())
		data->FaceSize(size_default);
	else
		data->FaceSize(size_stack.top());
}

float font::get_size() {
	return data->FaceSize();
}

float font::line_height() {
	return data->LineHeight();
}

float font::ascent() {
	return data->Ascender();
}

float font::descent() {
	return data->Descender();
}

float font::advance(const char* a) {
	return data->Advance(a);
}

float font::width_of(const char* a) {
	FTBBox b = data->BBox(a);
	return b.Upper().Xf() - b.Lower().Xf();
}

float font::height_of(const char* a) {
	FTBBox b = data->BBox(a);
	return b.Upper().Yf() - b.Lower().Yf();
}

paper::paper() {
	data = new FTSimpleLayout();
}

FTGL::TextAlignment paper_align_from_string(const char* a) {
	if(!strcmp("left", a))
		return FTGL::ALIGN_LEFT;
	if(!strcmp("center", a))
		return FTGL::ALIGN_CENTER;
	if(!strcmp("right", a))
		return FTGL::ALIGN_RIGHT;
	if(!strcmp("justify", a))
		return FTGL::ALIGN_JUSTIFY;
	
	err("paper", "align", "unknown method");
	return FTGL::ALIGN_LEFT;
}

paper::paper(float a, const char* b) {
	data = new FTSimpleLayout();
	data->SetLineLength(a);
	data->SetAlignment(paper_align_from_string(b));
	
}

paper::~paper() {
	delete data;
}

void paper::align(const char* a) {
	data->SetAlignment(paper_align_from_string(a));
}

const char* paper::get_align() {
	if(data->GetAlignment() == FTGL::ALIGN_LEFT)
		return "left";
	if(data->GetAlignment() == FTGL::ALIGN_CENTER)
		return "center";
	if(data->GetAlignment() == FTGL::ALIGN_RIGHT)
		return "right";
	if(data->GetAlignment() == FTGL::ALIGN_JUSTIFY)
		return "justify";
}

void paper::pen(font* a) {
	data_font = a;
	data->SetFont(data_font->data);
}

font* paper::get_pen() {
	return data_font;
}

void paper::line_spacing(float a) {
	data->SetLineSpacing(a);
}

float paper::get_line_spacing() {
	return data->GetLineSpacing();
}

void paper::width(float a) {
	data->SetLineLength(a);
}

float paper::get_width() {
	return data->GetLineLength();
}

float paper::width_of(const char* a) {
	FTBBox b = data->BBox(a);
	return b.Upper().Xf() - b.Lower().Xf();
}

float paper::height_of(const char* a) {
	FTBBox b = data->BBox(a);
	return b.Upper().Yf() - b.Lower().Yf();
}

void paper::write(const char* b, float x, float y, bool invert_y) {
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	if(invert_y) {
		glScalef(1.0, -1.0, 1.0);
	}
	data->Render(b);
	
	glPopMatrix();
}

shader::shader(const char* a, const char* b) {
	glGetError();
	if(!strcmp(a, "vertex"))
		id = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	else if(!strcmp(a, "fragment"))
		id = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	else {
		err("shader", "invalid type");
		return;
	}
	const char* c = read_file(b);
	glShaderSourceARB(id, 1, &c, NULL);
	
}

shader::~shader() {
	glDeleteObjectARB(id);
}

void shader::compile() {
	glCompileShaderARB(id);
	
	GLchar infoLog[2056]; int length;
	glGetShaderInfoLog(id, 2056, &length, infoLog);
	
	int compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	
	if(compiled != GL_TRUE) {
		err("shader", "compile", "error");
		std::cout << infoLog << std::endl;
	}
}

program::program() {
	id = glCreateProgramObjectARB();
}

program::~program() {
	glDeleteObjectARB(id);
}

void program::uniform_int(const char* a, int b) {
	glUniform1iARB(glGetUniformLocationARB(id, a), b);
}

void program::uniform_float(const char* a, float b) {
	glUniform1fARB(glGetUniformLocationARB(id, a), b);
}

void program::uniform_float(const char* a, float b, float c) {
	glUniform2fARB(glGetUniformLocationARB(id, a), b, c);
}

void program::uniform_float(const char* a, float b, float c, float d) {
	glUniform3fARB(glGetUniformLocationARB(id, a), b, c, d);
}

void program::uniform_float(const char* a, float b, float c, float d, float e) {
	glUniform4fARB(glGetUniformLocationARB(id, a), b, c, d, e);
}

void program::uniform_image(const char* a, int b, image* c) {
	GLint act;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_2D, c->id);
	uniform_int(a, b);
	
	// Reset to the origional texture
	if(act != GL_TEXTURE0 + b)
		glActiveTexture(act);
}

void program::attach(shader* a) {
	glAttachObjectARB(id, a->id);
}

void program::link() {
	glLinkProgramARB(id);
}

fbo::fbo(int b, int c, bool quality) {
	glGenFramebuffersEXT(1, &id);
	
	GLint last;
	glGetIntegerv(GL_FRAMEBUFFER_EXT, &last);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	
	buffer = new image(b, c, quality);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, buffer->id, 0);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, last);
}

fbo::~fbo() {
	glDeleteFramebuffersEXT(1, &id);
}
