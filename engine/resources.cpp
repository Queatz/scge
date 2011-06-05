/* * Resources
buffer
A sound buffer.

C++
buffer a("powerup.ogg");

Python
a = buffer('powerup.ogg')

see:sound
* */

buffer::buffer() {
	buf = 0;
}

buffer::buffer(const char* a) {
	if(alure_state == 0)
		audio();
	
	buf = alureCreateBufferFromFile(a);
	if(buf == AL_NONE) {
		buf = 0;
		err("sound", "could not load");
	}
	else
		buffer_loaded(buf);
}

buffer::~buffer() {
	clear();
}

void buffer::clear() {
	if(buf) {
		alDeleteBuffers(1, &buf);
		buffer_unloaded(buf);
	}
	buf = 0;
}


/* *
sound
A sound.

	play(sound, unsigned int repeats = 0, bool wait = false, bool dont = false)
		play the sound, ptionally waiting until it's finished, or just don't play it if it's already playing
	stop()
		stop it
	pause()
		pause the sound
	resume()
		resume the sound
	gain(float)
		set the gain of the sound
	pitch(float)
		set the pitch of the sound
	pan(float)
		set the pan of the sound
	repeat(bool)
		turn on and off repeating of the sound
	seek(float = NULL, string specifier = "")
		set the offset at which to play the sound. The specifier can be:
		"second" (default)
		"byte"
		"sample"
	playing()
		returns true if the sound is currently being played
	get(string)
		returns an int relating to:
		"frequency"
		"channels"
		"bytes"
	get_offset(string) as:
		returns a float of:
		"second"
		"byte"
		"sample"
	font(string)
		set the soundfont used for this sound

C++
sound a("click.ogg");
a.pan(-1.0); //pan fully to the left

sound b("music.ogg", true);
b.repeat(true); //set the music to repeat itself

Python
a = sound('click.ogg')
a.pan(-1.0) #pan fully to the left

b = sound('music.ogg', True)
b.repeat(True) #set the music to repeat itself

see:buffer
* */

void reset_sound(sound* a) {
	a->is_stream = false;
	a->looping = false;
	a->pending = 0;
	a->data = NULL;
	a->source = 0;
	
	int i;
	for(i = 0; i < NUM_BUFS; i++)
		a->bufs[i] = NULL;
}

sound::sound() {
	if(alure_state == 0)
		audio();
	
	reset_sound(this);
	
	alGenSources(1, &source);
	sound_loaded(source);
}

sound::sound(buffer* a) {
	if(alure_state == 0)
		audio();
	
	reset_sound(this);
	
	alGenSources(1, &source);
	sound_loaded(source);
	
	data = a;
	alSourcei(source, AL_BUFFER, data->buf);
}

sound::sound(const char* a, bool b) {
	if(alure_state == 0)
		audio();
	
	reset_sound(this);
	
	is_stream = b;

	if(is_stream) {
		stream = alureCreateStreamFromFile(a, 19200, NUM_BUFS, bufs);
		if(!stream)
			err("sound", "could not load");
	} else {
		bufs[0] = alureCreateBufferFromFile(a);
		if(bufs[0] == AL_NONE)
			err("sound", "could not load");
	}
	
	alGenSources(1, &source);
	sound_loaded(source);
	
	//alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
	
	if(!is_stream)
		alSourcei(source, AL_BUFFER, bufs[0]);
}

sound::~sound() {
	clear();
}

void sound::clear() {
	alureStopSource(source, AL_FALSE);
	
	if(is_stream)
		alureDestroyStream(stream, NUM_BUFS, bufs);
	
	if(source) {
		alSourcei(source, AL_BUFFER, NULL);
		alDeleteSources(1, &source);
		sound_unloaded(source);
	}
	
	reset_sound(this);
	
	alDeleteBuffers(NUM_BUFS, bufs);
}

void play_ended(void* userdata, ALuint source) {
	sound* a = (sound*)userdata;
	
	if(a->pending > 0) {
		a->pending--;
		alurePlaySource(a->source, play_ended, a);
	}
}

void sound::play(unsigned int repeats, bool b, bool c) {
	ALint d;
	alGetSourcei(source, AL_SOURCE_STATE, &d);
	
	if(b) {
		if(looping)
			return;
		
		if(d == AL_PLAYING || pending > 0) {
			if(!c)
				pending += repeats + 1;
			return;
		}
	}
	
	if(is_stream) {
		alureRewindStream(stream);
		alurePlaySourceStream(source, stream, NUM_BUFS, (looping ? -1 : repeats), NULL, NULL);
	} else {
		pending = repeats;
		if(d == AL_PLAYING)
			alureStopSource(source, AL_FALSE);
		alurePlaySource(source, play_ended, this);
	}	
}

void sound::stop() {
	alureStopSource(source, AL_FALSE);
	pending = 0;
}

void sound::pause() {
	alurePauseSource(source);
}

void sound::resume() {
	alureResumeSource(source);
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

void sound::seek(float a, const char* b) {
	if(a) {
		if(strcmp(b, "second"))
			alSourcef(source, AL_SEC_OFFSET, a);
		else if(strcmp(b, "sample"))
			alSourcef(source, AL_SAMPLE_OFFSET, a);
		else if(strcmp(b, "byte"))
			alSourcef(source, AL_BYTE_OFFSET, a);
		else
			err("sound", "seek", "invalid specifier");
	}
	else
		alSourceRewind(source);
}

bool sound::playing() {
	ALint a;
	alGetSourcei(source, AL_SOURCE_STATE, &a);
	return a == AL_PLAYING;
}

int sound::get(const char* b) {
	ALint a;
	if(strcmp(b, "bits"))
		alGetSourcei(bufs[0], AL_BITS, &a);
	else if(strcmp(b, "channels"))
		alGetSourcei(bufs[0], AL_CHANNELS, &a);
	else if(strcmp(b, "frequency"))
		alGetSourcei(bufs[0], AL_FREQUENCY, &a);
	else if(strcmp(b, "bytes"))
		alGetSourcei(bufs[0], AL_SIZE, &a);
	else {
		err("sound", "get", "invalid request");
		return 0;
	}
	return a;
}

float sound::get_offset(const char* b) {
	ALfloat a;
	if(strcmp(b, "second"))
		alGetSourcef(source, AL_SEC_OFFSET, &a);
	else if(strcmp(b, "sample"))
		alGetSourcef(source, AL_SAMPLE_OFFSET, &a);
	else if(strcmp(b, "byte"))
		alGetSourcef(source, AL_BYTE_OFFSET, &a);
	else {
		err("sound", "get", "invalid request");
		return 0.0;
	}
	return a;
}

void sound::font(const char* a) {
	if(stream)
		alureSetStreamPatchset(stream, a);
}

/* *
pixelcache
A cache of pixels.

	width int
		the width of the cache
	height int
		the height of the cache
	pixel(int x, int y)
		get a pixel as an rgba
	set_pixel(int, int, rgba)
		set a pixel in the cache
	data GLubyte
		the cache data

C++
pixelcache a(24, 24);

pixelcache b(a); //copy a

Python
a = pixelcache(24, 24)

b = pixelcache(a) #copy a

see:image.cache
* */
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
	
	data = new GLubyte[height*width*3];
}

pixelcache::~pixelcache() {
	if(data)
		delete data;
}

rgba pixelcache::pixel(int x, int y) {
	int h;
	
	if(x < 0 || x >= width || y < 0 || y >= height)
		return rgba(0.0, 0.0, 0.0);
	
	h = y * width * 3 + x * 3;
	return rgba(static_cast<float>(data[h + 0]) / 255.0, static_cast<float>(data[h + 1]) / 255.0, static_cast<float>(data[h + 2]) / 255.0);
}

void pixelcache::set_pixel(int x, int y, rgba c) {
	int h;
	if(x < 0 || x >= width || y < 0 || y >= height) {
		err("pixelcache", "set_pixel", "out of bounds");
		return;
	}
	
	h = y * width * 3 + x * 3;
	data[h + 0] = static_cast<GLubyte>(c.r * 255.0);
	data[h + 1] = static_cast<GLubyte>(c.g * 255.0);
	data[h + 2] = static_cast<GLubyte>(c.b * 255.0);
}

/* *
image
An image.

	width float
		the width of the image
	height float
		the height of the image
	cache pixelcache
		the image's cache
	set(string)
		set options on the image:
		"linear" - draw the image smoothly
		"nearest" - draw the image pixelated
		"mirrored repeat" - repeat the image coordinates, mirrored
		"repeat" - repeat the image coordinates
		"clamp" - clamp to image coordinates
		"clamp to edge" - clamp min/max texel
	from_pixelcache()
		restore the image to it's cached state
	from_pixelcache(pixelcache)
		overwrites pixels with data from a pixelcache
	from_pixelcache(int x, int y, int w, int h)
		restore a portion of the image to it's cached state
	from_pixelcache(pixelcache, int x, int y, int w, int h)
		overwrites a portion of pixels with data from a pixelcache
	refresh_pixel_cache()
		caches the images current state
	pixel(int x, int y)
		returns an rgba containing the pixel color of x and y

C++
image a("tux.png");
a.set("linear");
rgba b();
b = a.pixel(0, 0);

image c(64, 64, false); //creates a blank image sized 64 by 64, and not high quality

Python
a = image('tux.png')
a.set('linear')
b = a.pixel(0, 0)

c = image(64, 64, False); #creates a blank image sized 64 by 64, and not high quality

see:use_image
* */
image::image(const char* a) {
	if(glfw_state == 0)
		graphics();
	
	id = SOIL_load_OGL_texture(a, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); //NULL
	
	cache = NULL;
	
	if(id == 0) {
		err("image", "could not load");
		return;
	}
	
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

image::image(int a, int b, bool quality) {
	if(glfw_state == 0)
		graphics();
	
	cache = NULL;
	
	width = a;
	height = b;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, (quality ? GL_RGB12 : GL_RGB8), a, b, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
	} else if (!strcmp(a, "clamp to edge")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, a->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(pixelcache* a, int x, int y, int w, int h) {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, a->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache() {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(int x, int y, int w, int h) {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
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
	
	if(x < 0 || x >= w || y < 0 || y >= h)
		return rgba(0.0, 0.0, 0.0);
	
	if(!cache)
		refresh_pixel_cache();
	
	h = y * w * 3 + x * 3;
	return rgba(static_cast<float>(cache->data[h + 0]) / 255.0, static_cast<float>(cache->data[h + 1]) / 255.0, static_cast<float>(cache->data[h + 2]) / 255.0);
}

/* *
font
A font.

	width_of(string)
		returns a float representing the width of the computed string
	height_of(string)
		returns the caclulated height of the string as a float
	size(float)
		set the font size
		!It is better to create two fonts of different sizes rather than changing the font size each frame because the font has to be regenerated.
	push_size()
		store the current font size into a stack
	pop_size()
		reset the font size to the last stored value
	get_size()
		get the size of the font
	outset(float[, float])
		set the outset of the font, for use in extrude fonts
	depth(float)
		set the depth of the font, for use in extrude fonts
	line_height()
		set the line height
	ascent()
		get the ascent of the font
	descent()
		get the decent of the font
	advance(string)
		get the advance of the string, where the next character should be placed

C++
font a("sans.ttf");

font b("serif.ttf", 16.0); //default to size 16 instead of 12, the default
b.push_size();
b.size(12.0);
b.pop_size();

font c("sans.ttf", 12, "outline"); //create an outline font

Python
a = font('sans.ttf')

b = font('serif.ttf', 16.0) #default to size 16 instead of 12, the default
b.push_size()
b.size(12.0)
b.pop_size()

c = font('sans.ttf', 12, 'outline') #create an outline font

see: use_font, write

* */
font::font(const char* a, float b, const char* c) {
	if(glfw_state == 0)
		graphics();
	
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
	
	if(data->Error()) {
		err("font", "could not load");
		delete data;
		data = NULL;
		return;
	}
	data->FaceSize(b);
	
	size_default = b;
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

/* *
paper
A paper.

	align(string)
		set the alignment of writing: left, center, right, justify
	get_align()
		returns the aligmnent method currently used
	pen(font)
		set the font used on this paper
	get_pen()
		get the font used on this paper
	line_spacing(float)
		set the line spacing
	get_line_spacing()
		get the line spacing
	width(float)
		set the width of the paper
	get_width()
		get the width of the paper
	width_of(string)
		returns a float representing the width of the computed string
	height_of(string)
		returns the caclulated height of the string as a float
	write(string[, float, float, bool])
		write some text using this paper, at x, y position, optionally flipped.

C++
paper a();

font b("sans.ttf");
paper b(b, 120.0, "center");

Python
a = paper()

b = font('sans.ttf')
b = paper(f, 120.0, 'center')

see: font
* */
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

paper::paper(font* f, float a, const char* b) {
	data = new FTSimpleLayout();
	data->SetLineLength(a);
	data->SetAlignment(paper_align_from_string(b));
	data_font = f;
	data->SetFont(data_font->data);
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
	
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	
	data->Render(b);
	
	// Reset to the origional texture
	glBindTexture(GL_TEXTURE_2D, bind);
	
	glPopMatrix();
}

/* *
shader
A shader object.

	compile()
		compile the shader

C++
shader a("fragment", "burgundy.frag"); //create a fragment shader from a file
a.compile();

shader b("vertex", "jitter.vert");

Python
a = shader('fragment', 'burgundy.frag') #create a fragment shader from a file
a.compile()

b = shader('vertex', 'jitter.vert')

!Vertex shaders are not very useful yet. Fragment shaders are the ones you would use for post-processed image effects.
* */
shader::shader(const char* a, const char* b) {
	if(glfw_state < 2) {
		id = 0;
		err("shader", "window needs to be open");
		return;
	}
	
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
	if(c)
		glShaderSourceARB(id, 1, &c, NULL);
}

shader::~shader() {
	if(id)
		glDeleteObjectARB(id);
}

void shader::compile() {
	if(!id)
		return;
	
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

/* *
program
A program that processes draw operations.

	attach(shader)
		attaches a shader to this program
	link()
		links and completes the program, making it ready for use
	uniform_int(string, int)
		set a uniform integer on the program
	uniform_float(string, float, ...)
		set a uniform float on the program
	uniform_image(string, int, image)
		set a uniform sampler2D with a multitexture ID

C++
program a();

Python
a = program()

see: use_program
* */
program::program() {
	if(glfw_state < 2) {
		id = 0;
		err("program", "window needs to be open");
		return;
	}
	
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
	
	// Reset to the original texture
	if(act != GL_TEXTURE0 + b) glActiveTexture(act);
}

void program::attach(shader* a) {
	if(id)
		glAttachObjectARB(id, a->id);
}

void program::link() {
	if(id)
		glLinkProgramARB(id);
}

/* *
fbo
A frame buffer object.

C++
fbo a(320, 240);

fbo b(320, 240, true); //high quality

Python
a = fbo(320, 240)

b = fbo(320, 240, true) #high quality

see: use_fbo
* */
fbo::fbo(image* a) {
	if(glfw_state == 0)
		graphics();
	
	glGenFramebuffersEXT(1, &id);
	
	GLint last;
	glGetIntegerv(GL_FRAMEBUFFER_EXT, &last);
	
	buffer = a;
	buffer_is_mine = false;
	depth_stencil = false;
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, buffer->id, 0);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, last);
}

fbo::fbo(int b, int c, bool quality, bool ds) {
	if(glfw_state == 0)
		graphics();
	
	glGenFramebuffersEXT(1, &id);
	
	GLint last, lastr;
	glGetIntegerv(GL_FRAMEBUFFER_EXT, &last);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	
	buffer = new image(b, c, quality);
	buffer_is_mine = true;
	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, buffer->id, 0);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	
	if(ds) {
		depth_stencil = true;
		glGenRenderbuffersEXT(1, &depth_stencil_id);
		
		glGetIntegerv(GL_RENDERBUFFER_EXT, &lastr);
		
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_stencil_id);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8_EXT, b, c);
		
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_stencil_id);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_stencil_id);
		
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, lastr);
	}
	else
		depth_stencil = false;
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, last);
}

fbo::~fbo() {
	if(buffer_is_mine)
		delete buffer;
	if(depth_stencil_id)
		glDeleteRenderbuffersEXT(1, &depth_stencil_id);
	glDeleteFramebuffersEXT(1, &id);
}
