#include "engine.hpp"

// Utilities

const char* read_file(const char* a) {
	FILE *fp;
	long len;
	char *buf;
	
	fp = fopen(a, "rb");
	fseek(fp, 0, SEEK_END); // go to end
	len = ftell(fp); // get position at end (length)
	fseek(fp, 0, SEEK_SET); // go to beg.
	buf = (char *) malloc(len + 1); // malloc buffer
	buf[len] = 0; // append a null char
	fread(buf, 1, len, fp); // read into buffer
	fclose(fp);
	
	return buf;
}

// Types

sound::sound(const char* a, bool b) {
	is_stream = b;
	is_playing = false;
	
	source = NULL;
	int i;
	for(i = 0; i < NUM_BUFS; i++)
		buffer[i] = NULL;
	
	if(is_stream)
		stream = alureCreateStreamFromFile(a, 19200, NUM_BUFS, buffer);
	else
		buffer[0] = alureCreateBufferFromFile(a);
	
	alGenSources(1, &source);
	
	if(!is_stream)
		alSourcei(source, AL_BUFFER, buffer[0]);
	
	//alSourcef(source, AL_GAIN, 1.0);
	//alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
	//alSourcef(source, AL_ROLLOFF_FACTOR, 0.0);
}

sound::~sound() {
	alDeleteBuffers(1, buffer);
	alDeleteSources(1, &source);
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
	ALint state = AL_PLAYING;
	ALint processed = 0;

	alGetSourcei(source, AL_BUFFERS_QUEUED, &processed);
	
	// Just started
	if(processed <= 0) {
		alSourceQueueBuffers(source, NUM_BUFS, buffer);
	}
	
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
	
	if(is_playing && state != AL_PLAYING)
		alSourcePlay(source);
	
	return;
}

image::image(const char* a) {
	id = SOIL_load_OGL_texture(a, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

image::image(int a, int b) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,  a, b, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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

font::font(const char* a, float s) {
	f = new FTTextureFont(a);
	f->FaceSize(s);
}

font::~font() {
	delete f;
}

shader::shader(const char* a, const char* b) {
	glGetError();//XXX
	if(!strcmp(a, "vertex"))
		s = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	else if(!strcmp(a, "fragment"))
		s = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	
	const char* c = read_file(b);
	glShaderSourceARB(s, 1, &c, NULL);
	
	//XXX
	/*char infoLog; int length;
	glGetShaderInfoLog(s, 2000, &length, &infoLog);
	
	int params;
	glGetShaderiv(s, GL_COMPILE_STATUS, &params);
	
	cout << params << ": "<< infoLog << endl;
	*/
}

shader::~shader() {
	glDeleteObjectARB(s);
}

void shader::compile() {
	glCompileShaderARB(s);
}

program::program() {
	p = glCreateProgramObjectARB();
}

program::~program() {
	glDeleteObjectARB(p);
}

void program::uniform1i(const char* a, int b) {
	glUniform1iARB(glGetUniformLocationARB(p, a), b);
}

void program::uniform1f(const char* a, float b) {
	glUniform1fARB(glGetUniformLocationARB(p, a), b);
}

void program::uniform2f(const char* a, float b, float c) {
	glUniform2fARB(glGetUniformLocationARB(p, a), b, c);
}

void program::uniform3f(const char* a, float b, float c, float d) {
	glUniform3fARB(glGetUniformLocationARB(p, a), b, c, d);
}

void program::uniform4f(const char* a, float b, float c, float d, float e) {
	glUniform4fARB(glGetUniformLocationARB(p, a), b, c, d, e);
}

void program::bind_image(const char* a, int b, image* c) {
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_2D, c->id);
	uniform1i(a, b);

}

void program::attach(shader* a) {
	glAttachObjectARB(p, a->s);
}

void program::link() {
	glLinkProgramARB(p);
}

void font::size(float a) {
	f->FaceSize(a);
}

float font::width(const char * a) {
	FTBBox b = f->BBox(a);
	return b.Upper().Xf() - b.Lower().Xf();
}

float font::height(const char * a) {
	FTBBox b = f->BBox(a);
	return b.Upper().Yf() - b.Lower().Yf();
}

fbo::fbo(display* a) {
	d = a;
	glGenFramebuffersEXT(1, &id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	
	i = new image(d->w, d->h);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, i->id, 0);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
}

fbo::~fbo() {
	glDeleteFramebuffersEXT(1, &id);
}

// Display

display::display(const char* a, int b, int c) {
	fs = (!strcmp(a, "window") ? false : true);
	w = b;
	h = c;
	
	framebuffer = NULL;
	filter = NULL;
	
	set_image_crop();
}

display::~display() {
	off();
}

bool display::on(const char* a) {
	if(glfwInit() != GL_TRUE) {
		cout << "Could not initiate window manager." << endl;
		return false;
	}

	GLFWvidmode m;
	glfwGetDesktopMode(&m);
	
	if(!w)
		w = m.Width;
	
	if(!h)
		h = m.Height;
	
	if(glfwOpenWindow(w, h, 0, 0, 0, 0, 0, 0, (fs ? GLFW_FULLSCREEN : GLFW_WINDOW)) != GL_TRUE) {
		cout << "Could not initiate window." << endl;
		return false;
	}
	
	GLenum err = glewInit();
	if(err != GLEW_OK)
		cout << "Could not initiate GL extentions, passing." << endl;
	
	if(a)
		glfwSetWindowTitle(a);
	
	glfwSwapInterval(1);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, m.Width, m.Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m.Width, 0, m.Height, -1, 1);
	
	return true;
}

void display::off() {
	glfwTerminate();
}

void display::title(const char* a) {
	glfwSetWindowTitle(a);
}

bool display::opened() {
	if(glfwGetWindowParam(GLFW_OPENED))
		return true;

	return false;
}

float display::timer() {
	return glfwGetTime();
}

Size display::dimensions() {
	GLFWvidmode m;
	glfwGetDesktopMode(&m);

	Size a;
	a.w = m.Width;
	a.h = m.Height;

	return a;
}

void display::clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void display::update() {
	if(framebuffer) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
		glBindTexture(GL_TEXTURE_2D, framebuffer->i->id);
	
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		color(1, 1, 1);
		
		if(filter) {
			use_program(filter);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, framebuffer->i->id);
			filter->uniform1i("buffer", 0);
		}
		
		begin("quads"); mquad(0, 0, w, h); end();
		
		if(filter)
			use_program();
		glDisable(GL_TEXTURE_2D);
	}
	
	glfwSwapBuffers();
	
	if(framebuffer)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer->id);
}

void display::use_image(image* a) {
	current_image = a;
	glBindTexture(GL_TEXTURE_2D, a->id);
}

void display::set_image_crop(int x, int y, int w, int h) {
	if(!w && !h)
		image_crop.use = false;
	else {
		image_crop.use = true;
		image_crop.x = x;
		image_crop.y = y;
		image_crop.w = w;
		image_crop.h = h;
	}
}

void display::use_font(font* a) {
	current_font = a;
}

void display::draw(float x, float y, float sx, float sy, float r, float xo, float yo) {
	if(!sx) sx = 1;
	if(!sy) sy = sx;
	if(!yo) yo = xo;
	
	float co_x = 0, co_y = 0, co_x2 = 1, co_y2 = 1, w, h;
	
	if(image_crop.use) {
		co_x = image_crop.x / current_image->w;
		co_y = image_crop.y / current_image->h;
		co_x2 = (image_crop.x + image_crop.w) / current_image->w;
		co_y2 = (image_crop.y + image_crop.h) / current_image->h;
		w = image_crop.w;
		h = image_crop.h;
	}
	else {
		w = current_image->w;
		h = current_image->h;
	}
	
	if(r) {
		float rc = cos(r), rs = sin(r);
		glTexCoord2f(co_x, co_y); glVertex2i(x + (-xo * rc - -yo * rs) * sx, y + (-yo * rc + -xo * rs) * sy);
		glTexCoord2f(co_x2, co_y); glVertex2i(x + ((w - xo) * rc - -yo * rs) * sx, y + (-yo * rc + (w - xo) * rs) * sy);
		glTexCoord2f(co_x2, co_y2); glVertex2i(x + ((w - xo) * rc - (h - yo) * rs) * sx, y + ((h - yo) * rc + (w - xo) * rs) * sy);
		glTexCoord2f(co_x, co_y2); glVertex2i(x + (-xo * rc - (h - yo) * rs) * sx, y + ((h - yo) * rc + -xo * rs) * sy);
	}
	else {
		glTexCoord2f(co_x, co_y); glVertex2i(x - xo * sx, y - yo * sy);
		glTexCoord2f(co_x2, co_y); glVertex2i(x + (w - xo) * sx, y - yo * sy);
		glTexCoord2f(co_x2, co_y2); glVertex2i(x + (w - xo) * sx, y + (h - yo) * sy);
		glTexCoord2f(co_x, co_y2); glVertex2i(x - xo * sx, y + (h - yo) * sy);
	}
}

void display::background_image(float x, float y, float s) {
	x /= current_image->w;
	y /= current_image->h;
	
	glTexCoord2f(x, y); glVertex2i(0, 0);
	glTexCoord2f(x + s, y); glVertex2i(w, 0);
	glTexCoord2f(x + s, y + s); glVertex2i(w, h);
	glTexCoord2f(x, y + s); glVertex2i(0, h);
}

void display::write(string b, float x, float y) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	current_font->f->Render(b.c_str());
	glPopMatrix();
	
	// This doesn't work for some reason:
	//FTPoint c(x, y);
	//a->f->Render(b, NULL, c);
}

void display::color(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void display::blend_mode(const char* a) {
	if(!strcmp(a, "subtract")) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	}
	else {
		glBlendEquation(GL_FUNC_ADD);
		
		if(!strcmp(a, "add")) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_COLOR);
		}
		else if (!strcmp(a, "multiply")) {
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		}
		else if(!strcmp(a, "mix")) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else if(!strcmp(a, "none")) {
			glBlendFunc(GL_SRC_COLOR, GL_ZERO);
		}
	}
}

void display::set(const char* a, bool b) {
	int c;
	if (!strcmp(a, "texture")) {
		c = GL_TEXTURE_2D;
	}
	else if (!strcmp(a, "point smooth")) {
		c = GL_POINT_SMOOTH;
	}
	
	if(c) {
		if(b) glEnable(c);
		else glDisable(c);
	}
}

void  display::point_size(float a) {
	glPointSize(a);
}

void display::begin(const char* a) {
	if (!strcmp(a, "images"))
		glBegin(GL_QUADS);
	else if (!strcmp(a, "quads"))
		glBegin(GL_QUADS);
	else if (!strcmp(a, "lines"))
		glBegin(GL_LINES);
	else if (!strcmp(a, "points"))
		glBegin(GL_POINTS);
}

void display::end() {
	glEnd();
}

void display::quad(float x, float y, float x2, float y2) {
	glVertex2i(x, y);
	glVertex2i(x2, y);
	glVertex2i(x2, y2);
	glVertex2i(x, y2);
}

void display::mquad(float x, float y, float x2, float y2) {
	glTexCoord2i(0, 0); glVertex2i(x, y);
	glTexCoord2i(1, 0); glVertex2i(x2, y);
	glTexCoord2i(1, 1); glVertex2i(x2, y2);
	glTexCoord2i(0, 1); glVertex2i(x, y2);
}

void display::point(float x, float y) {
	glVertex2i(x, y);
}

void display::use_program(program* a) {
	glUseProgramObjectARB(a->p);
}

void display::apply_filter(program* a) {
	filter = a;
}

void display::apply_filter() {
	filter = NULL;
}

void display::use_program() {
	glUseProgramObjectARB(0);
}

void display::sleep(float a) {
	glfwSleep(a);
}

float display::time() {
	return glfwGetTime();
}

timer::timer() {
	l = glfwGetTime();
	s = l;
	t = 0;
}

void timer::update() {
	f = f * 0.9 + (1.0 / delta()) * 0.1;
	l = glfwGetTime();
	t++;
}

float timer::delta() {
	return glfwGetTime() - l;
}

int timer::tick() {
	return t;
}

float timer::total() {
	return glfwGetTime() - s;
}

int timer::fps() {
	return (int) round(f);
}

// Speaker

speaker::~speaker() {
	off();
}

void speaker::on() {
	alureInitDevice(NULL, NULL);
	alListener3f(AL_POSITION, 0.0, 0.0, 0.0);
}

void speaker::off() {
	alureShutdownDevice();
}

void speaker::play(sound* a) {
	if(a->is_stream)
		a->is_playing = true;
	else{
		alSourcePlay(a->source);
}}

void speaker::stop(sound* a) {
	if(a->is_stream) {
		a->is_playing = false;
		alureRewindStream(a->stream);
	}
	alSourceStop(a->source);
}

void speaker::pause(sound* a) {
	if(a->is_stream)
		a->is_playing = false;
}

// Mouse

mouse::mouse() {
	update();
}

void mouse::hide() {
	glfwDisable(GLFW_MOUSE_CURSOR);
}

void mouse::show() {
	glfwEnable(GLFW_MOUSE_CURSOR);
}

bool mouse::clicked(const char* a) {
	int b;
	if(!strcmp(a, "left"))
		b = GLFW_MOUSE_BUTTON_LEFT;
	else if(!strcmp(a, "middle"))
		b = GLFW_MOUSE_BUTTON_MIDDLE;
	else if(!strcmp(a, "right"))
		b = GLFW_MOUSE_BUTTON_RIGHT;
	else
		return false;
	
	return glfwGetMouseButton(b) == GLFW_PRESS;
}

bool mouse::clicked(short a) {
	if(a < 1 || a > 8)
		return false;
	
	return glfwGetMouseButton(GLFW_MOUSE_BUTTON_1 + --a) == GLFW_PRESS;
}

int mouse::wheel() {
	return glfwGetMouseWheel();
}

int mouse::scroll() {
	return glfwGetMouseWheel() - wheel_pos;
}

void mouse::update() {
	wheel_pos = glfwGetMouseWheel();
	glfwGetMousePos(&x, &y);
}

bool keyboard::key(const char* a) {
	if(glfwInit() != GL_TRUE) {
		cout << "Could not initiate window manager." << endl;
		return false;
	}
	if (!strcmp(a, "a")
	|| !strcmp(a, "b")
	|| !strcmp(a, "c")
	|| !strcmp(a, "d")
	|| !strcmp(a, "e")
	|| !strcmp(a, "f")
	|| !strcmp(a, "g")
	|| !strcmp(a, "h")
	|| !strcmp(a, "i")
	|| !strcmp(a, "j")
	|| !strcmp(a, "k")
	|| !strcmp(a, "l")
	|| !strcmp(a, "m")
	|| !strcmp(a, "n")
	|| !strcmp(a, "o")
	|| !strcmp(a, "p")
	|| !strcmp(a, "q")
	|| !strcmp(a, "r")
	|| !strcmp(a, "s")
	|| !strcmp(a, "t")
	|| !strcmp(a, "u")
	|| !strcmp(a, "v")
	|| !strcmp(a, "w")
	|| !strcmp(a, "x")
	|| !strcmp(a, "y")
	|| !strcmp(a, "z")
	|| !strcmp(a, "1")
	|| !strcmp(a, "2")
	|| !strcmp(a, "3")
	|| !strcmp(a, "4")
	|| !strcmp(a, "5")
	|| !strcmp(a, "6")
	|| !strcmp(a, "7")
	|| !strcmp(a, "8")
	|| !strcmp(a, "9")
	|| !strcmp(a, "0")
	|| !strcmp(a, "[")
	|| !strcmp(a, "]")
	|| !strcmp(a, "\\")
	|| !strcmp(a, ";")
	|| !strcmp(a, "'")
	|| !strcmp(a, ",")
	|| !strcmp(a, ".")
	|| !strcmp(a, "/")
	|| !strcmp(a, "`")) {
		if(glfwGetKey(toupper(a[0])))
			return true;

		return false;
	}

	int b;

	if(!strcmp(a, "space")) b = GLFW_KEY_SPACE;
	else if(!strcmp(a, "esc")) b = GLFW_KEY_ESC;
	else if(!strcmp(a, "F1")) b = GLFW_KEY_F1;
	else if(!strcmp(a, "F2")) b = GLFW_KEY_F2;
	else if(!strcmp(a, "F3")) b = GLFW_KEY_F3;
	else if(!strcmp(a, "F4")) b = GLFW_KEY_F4;
	else if(!strcmp(a, "F5")) b = GLFW_KEY_F5;
	else if(!strcmp(a, "F6")) b = GLFW_KEY_F6;
	else if(!strcmp(a, "F7")) b = GLFW_KEY_F7;
	else if(!strcmp(a, "F8")) b = GLFW_KEY_F8;
	else if(!strcmp(a, "F9")) b = GLFW_KEY_F9;
	else if(!strcmp(a, "F10")) b = GLFW_KEY_F10;
	else if(!strcmp(a, "F11")) b = GLFW_KEY_F11;
	else if(!strcmp(a, "F12")) b = GLFW_KEY_F12;
	else if(!strcmp(a, "up")) b = GLFW_KEY_UP;
	else if(!strcmp(a, "down")) b = GLFW_KEY_DOWN;
	else if(!strcmp(a, "left")) b = GLFW_KEY_LEFT;
	else if(!strcmp(a, "right")) b = GLFW_KEY_RIGHT;
	else if(!strcmp(a, "left shift")) b = GLFW_KEY_LSHIFT;
	else if(!strcmp(a, "right shift")) b = GLFW_KEY_RSHIFT;
	else if(!strcmp(a, "left ctrl")) b = GLFW_KEY_LCTRL;
	else if(!strcmp(a, "right ctrl")) b = GLFW_KEY_RCTRL;
	else if(!strcmp(a, "left alt")) b = GLFW_KEY_LALT;
	else if(!strcmp(a, "right alt")) b = GLFW_KEY_RALT;
	else if(!strcmp(a, "tab")) b = GLFW_KEY_TAB;
	else if(!strcmp(a, "enter")) b = GLFW_KEY_ENTER;
	else if(!strcmp(a, "backspace")) b = GLFW_KEY_BACKSPACE;
	else if(!strcmp(a, "insert")) b = GLFW_KEY_INSERT;
	else if(!strcmp(a, "delete")) b = GLFW_KEY_DEL;
	else if(!strcmp(a, "page up")) b = GLFW_KEY_PAGEUP;
	else if(!strcmp(a, "page down")) b = GLFW_KEY_PAGEDOWN;
	else if(!strcmp(a, "home")) b = GLFW_KEY_HOME;
	else if(!strcmp(a, "end")) b = GLFW_KEY_END;
	else if(!strcmp(a, "kp 1")) b = GLFW_KEY_KP_1;
	else if(!strcmp(a, "kp 2")) b = GLFW_KEY_KP_2;
	else if(!strcmp(a, "kp 3")) b = GLFW_KEY_KP_3;
	else if(!strcmp(a, "kp 4")) b = GLFW_KEY_KP_4;
	else if(!strcmp(a, "kp 5")) b = GLFW_KEY_KP_5;
	else if(!strcmp(a, "kp 6")) b = GLFW_KEY_KP_6;
	else if(!strcmp(a, "kp 7")) b = GLFW_KEY_KP_7;
	else if(!strcmp(a, "kp 8")) b = GLFW_KEY_KP_8;
	else if(!strcmp(a, "kp 9")) b = GLFW_KEY_KP_9;
	else if(!strcmp(a, "kp 0")) b = GLFW_KEY_KP_0;
	else if(!strcmp(a, "kp divide")) b = GLFW_KEY_KP_DIVIDE;
	else if(!strcmp(a, "kp multiply")) b = GLFW_KEY_KP_MULTIPLY;
	else if(!strcmp(a, "kp subtract")) b = GLFW_KEY_KP_SUBTRACT;
	else if(!strcmp(a, "kp add")) b = GLFW_KEY_KP_ADD;
	else if(!strcmp(a, "kp decimal")) b = GLFW_KEY_KP_DECIMAL;
	else if(!strcmp(a, "kp equal")) b = GLFW_KEY_KP_EQUAL;
	else if(!strcmp(a, "kp enter")) b = GLFW_KEY_KP_ENTER;

	if(b)
		if(glfwGetKey(b))
			return true;

	return false;
}

#ifdef WITH_NETWORK

// Server & Client

connection::~connection() {
	off();
}

void connection::on() {
	enet_initialize();
}

void connection::off() {
	enet_deinitialize();
}

server::server(int a, int b, int c, int d, int e) {
	address.host = ENET_HOST_ANY;
	address.port = a;
	
	host = enet_host_create(&address, b, c, d, e);
}

server::~server() {
	enet_host_destroy(host);
}

ENetEvent server::service(int a) {
	ENetEvent b;
	
	if(enet_host_service(host, &b, a) > 0)
		return b;
}

client::client(int a, int b, int c, int d) {
	host = enet_host_create(NULL, a, b, c, d);
}

client::~client() {
	enet_host_destroy(host);
}

ENetEvent client::service(int a) {
	ENetEvent b;
	
	if(enet_host_service(host, &b, a) > 0)
		return b;
}

void client::send(const char* b, int c) {
	ENetPacket* d = enet_packet_create(b, strlen(b) + 1, ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(peer, c, d);
}

void client::commune() {
	enet_host_flush(host);
}

void client::connect(const char* a, int b, int c) {
	ENetAddress d;

	enet_address_set_host(&d, a);
	d.port = b;

	peer = enet_host_connect(host, &d, c, 0);    
}

void client::disconnect() {
	enet_peer_disconnect(peer, 0);
}

#endif

// Functions

void seed_rnd(int a) {
	srand(a);
}

float rnd() {
	return rand() / (float) RAND_MAX;
}
