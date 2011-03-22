std::stack<fbo*, std::list<fbo*> > fbo_stack;
std::stack<image*, std::list<image*> > image_stack;
std::stack<font*, std::list<font*> > font_stack;
std::stack<program*, std::list<program*> > program_stack;
std::stack<rgba, std::list<rgba> > color_stack;
std::stack<crop, std::list<crop> > image_crop_stack;
std::stack<icrop, std::list<icrop> > scissor_stack;
std::stack<std::string, std::list<std::string> > blend_mode_stack;
	
int width = 0, height = 0, glfw_state = 0;

bool initiate() {
	if(glfwInit() != GL_TRUE) {
		err("initiate", "could not");
		return false;
	}
	glfw_state = 1;
	return true;
}

void done() {
	glfwTerminate();
}

bool window(int x, int y, bool fullscreen) {
	if(glfw_state == 0)
		initiate();
	
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	
	if(glfwOpenWindow(x, y, 0, 0, 0, 0, 0, 0, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW)) != GL_TRUE) {
		err("window", "could not initiate window");
		return false;
	}
	
	glfwSwapInterval(0);
	
	glfwGetWindowSize(&width, &height);
	
	GLenum err = glewInit();
	if(err != GLEW_OK)
		note("window", "extentions unsupported");
	
	// ...should these be here...
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1.0, 1.0);
		
	fbo_stack.push(NULL);
	image_stack.push(NULL);
	font_stack.push(NULL);
	program_stack.push(NULL);
	color_stack.push(rgba(1, 1, 1, 1));
	image_crop_stack.push(crop(false));
	scissor_stack.push(icrop(false));
	blend_mode_stack.push("mix");
	
	return true;
}

void close_window() {
	glfwCloseWindow();
}

void window_title(const char* a) {
	glfwSetWindowTitle(a);
}

bool window_opened() {
	if(glfwGetWindowParam(GLFW_OPENED))
		return true;
	return false;
}

bool window_active() {
	if(glfwGetWindowParam(GLFW_ACTIVE))
		return true;
	return false;
}

float window_timer() {
	return glfwGetTime();
}

void rest(float a) {
	glfwSleep(a);
}

ibox display_dimensions() {
	GLFWvidmode a;
	glfwGetDesktopMode(&a);
	
	return ibox(a.Width, a.Height);
}

ibox window_dimensions() {
	int w, h;
	glfwGetWindowSize(&w, &h);
	
	return ibox(w, h);
}

void position_window(int x, int y) {
	glfwSetWindowPos(x, y);
}

void vsync(bool a) {	
	glfwSwapInterval(a);
}

void swap() {
	glfwSwapBuffers();
}

void poll() {
	glfwPollEvents();
}

void screenshot(const char* a) {
	if(!SOIL_save_screenshot(a, SOIL_SAVE_TYPE_TGA, 0, 0, width, height))
		err("screenshot", "could not save");
}

void reset_matrix() {
	glLoadIdentity();
}

void orthographic(float a, float b, float c, float d, float e, float f) {
	glOrtho(a, b, c, d, e, f);
}

void perspective(float a, float b, float c, float d, float e, float f) {
	glFrustum(a, b, c, d, e, f);
}

void polygon_mode(const char* a) {
	int c;
	
	if (!strcmp(a, "dot"))
		c = GL_POINT;
	else if (!strcmp(a, "outline"))
		c = GL_LINE;
	else if (!strcmp(a, "fill"))
		c = GL_FILL;
	
	if(c > -1)
		glPolygonMode(GL_FRONT_AND_BACK, c);
	else
		err(" polygon_mode", "invalid option");
}

void enable(const char* a, bool b) {
	int c = -1;
	
	if (!strcmp(a, "texture"))
		c = GL_TEXTURE_2D;
	else if (!strcmp(a, "point smooth"))
		c = GL_POINT_SMOOTH;
	else if (!strcmp(a, "polygon smooth"))
		c = GL_POLYGON_SMOOTH;
	else if (!strcmp(a, "line smooth"))
		c = GL_LINE_SMOOTH;
	else if (!strcmp(a, "blend"))
		c = GL_BLEND;
	else if (!strcmp(a, "depth"))
		c = GL_DEPTH_TEST;
	
	if(c > -1) {
		if(b) glEnable(c);
		else glDisable(c);
	}
	else
		err("enable", "invalid option");
}



void clear_color(float a, float b, float c, float d) {
	glClearColor(a, b, c, d);
}

void clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void  point_size(float a) {
	glPointSize(a);
}

void  line_width(float a) {
	glLineWidth(a);
}

void line_stipple(int a, const char* b) {
	GLshort c = 0x1111;
	if(b != NULL) {
		if(!strcmp(b, "dotted"))
			c = 0x0101;
		else if(!strcmp(b, "dashed"))
			c = 0x00FF;
	}
	glLineStipple(a, c);
}


void blend_color(float r, float g, float b, float a) {
	glBlendColor(r, g, b, a);
}



void color(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void color(rgba a) {
	glColor4f(a.r, a.g, a.b, a.a);
}

void push_color() {
	GLfloat c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);
	color_stack.top() = rgba(c[0], c[1], c[2], c[3]);
	
	color_stack.push(color_stack.top());
}

void pop_color() {
	if(color_stack.size() > 1)
		color_stack.pop();
	else
		err("pop_color", "already at begining of stack");
	
	color(color_stack.top());
}

rgba get_color() {
	GLfloat c[4];
	glGetFloatv(GL_CURRENT_COLOR, c);
	return rgba(c[0], c[1], c[2], c[3]);
}


void use_fbo(fbo* a) {
	fbo_stack.top() = a;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, a->id);
}

void use_fbo() {
	fbo_stack.top() = NULL;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void push_fbo() {
	fbo_stack.push(fbo_stack.top());
}

void pop_fbo() {
	if(fbo_stack.size() > 1)
		fbo_stack.pop();
	else
		err("pop_fbo", "already at begining of stack");
	
	if(fbo_stack.top() == NULL)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	else
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_stack.top()->id);
}

fbo* get_fbo() {
	return fbo_stack.top();
}


void use_image(image* a) {
	image_stack.top() = a;
	glBindTexture(GL_TEXTURE_2D, a->id);
}

void push_image() {
	image_stack.push(image_stack.top());
}

void pop_image() {
	if(image_stack.size() > 1)
		image_stack.pop();
	else
		err("pop_image", "already at begining of stack");
	
	if(image_stack.top() != NULL)
		glBindTexture(GL_TEXTURE_2D, image_stack.top()->id);
}

image* get_image() {
	return image_stack.top();
}

void use_font(font* a) {
	font_stack.top() = a;
}

void push_font() {
	font_stack.push(font_stack.top());
}

void pop_font() {
	if(font_stack.size() > 1)
		font_stack.pop();
	else
		err("pop_font", "already at begining of stack");
}

font* get_font() {
	return font_stack.top();
}


void image_crop(float x, float y, float w, float h) {
	image_crop_stack.top() = crop(true, x, y, w, h);
}

void image_crop() {
	image_crop_stack.top() = crop(false);
}

void push_image_crop() {
	image_crop_stack.push(image_crop_stack.top());
}

void pop_image_crop() {
	if(image_crop_stack.size() > 1)
		image_crop_stack.pop();
	else
		err("pop_image_crop", "already at begining of stack");
}

crop get_image_crop() {
	return image_crop_stack.top();
}


void scissor(int a, int b, int c, int d) {
	if(glIsEnabled(GL_SCISSOR_TEST) == GL_FALSE)
		glEnable(GL_SCISSOR_TEST);
	glScissor(a, b, c, d);
}

void scissor() {
	if(glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE)
		glDisable(GL_SCISSOR_TEST);
}

void push_scissor() {
	GLfloat s[4];
	GLboolean u;
	glGetBooleanv(GL_SCISSOR_TEST, &u);
	glGetFloatv(GL_SCISSOR_BOX, s);
	scissor_stack.top() = icrop(u, s[0], s[1], s[2], s[3]);
	
	scissor_stack.push(scissor_stack.top());
}

void pop_scissor() {
	if(scissor_stack.size() > 1)
		scissor_stack.pop();
	else
		err("pop_scissor", "already at begining of stack");
	
	if(!scissor_stack.top().use && glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE)
		glDisable(GL_SCISSOR_TEST);
	else if(scissor_stack.top().use && glIsEnabled(GL_SCISSOR_TEST) == GL_FALSE)
		glEnable(GL_SCISSOR_TEST);
	
	glScissor(scissor_stack.top().x, scissor_stack.top().y, scissor_stack.top().w, scissor_stack.top().h);
}

icrop get_scissor() {
	GLfloat s[4];
	GLboolean u;
	glGetBooleanv(GL_SCISSOR_TEST, &u);
	glGetFloatv(GL_SCISSOR_BOX, s);
	
	return icrop(u, s[0], s[1], s[2], s[3]);
}



void display_set_blend_mode_from_string(const char* a) {
	if(!strcmp(a, "subtract")) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	}
	else {
		glBlendEquation(GL_FUNC_ADD);

		if(!strcmp(a, "add"))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		else if (!strcmp(a, "multiply"))
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		else if(!strcmp(a, "mix"))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else if(!strcmp(a, "none"))
			glBlendFunc(GL_SRC_COLOR, GL_ZERO);
		else
			;//err("blend_mode", "invalid value");
	}
}

void blend_mode(const char* a) {
	blend_mode_stack.top() = std::string(a);
	
	display_set_blend_mode_from_string(a);
}

void blend_mode() {
	blend_mode_stack.top() = std::string("mix");
	display_set_blend_mode_from_string("mix");
}

void push_blend_mode() {
	blend_mode_stack.push(blend_mode_stack.top());
}


void pop_blend_mode() {
	if(blend_mode_stack.size() > 1)
		blend_mode_stack.pop();
	else
		err("pop_blend_mode", "already at begining of stack");
	
	display_set_blend_mode_from_string(blend_mode_stack.top().c_str());
}

const char* get_blend_mode() {
	return blend_mode_stack.top().c_str();
}


void use_program(program* a) {
	program_stack.top() = a;
	glUseProgramObjectARB(a->id);
}

void use_program() {
	program_stack.top() = NULL;
	glUseProgramObjectARB(0);
}

void push_program() {
	program_stack.push(program_stack.top());
}

void pop_program() {
	if(program_stack.size() > 1)
		program_stack.pop();
	else
		err("pop_program", "already at begining of stack");
	
	if(program_stack.top() == NULL)
		glUseProgramObjectARB(0);
	else
		glUseProgramObjectARB(program_stack.top()->id);
}

program* get_program() {
	return program_stack.top();
}


void push_matrix() {
	glPushMatrix();
}

void pop_matrix() {
	glPopMatrix();
}

void translate(float a, float b) {
	glTranslatef(a, b, 0);
}

void rotate(float a) {
	glRotatef(a, 0, 0, 1);
}

void scale(float a, float b) {
	glScalef(a, b, 1);
}


void begin(const char* a) {
	if (!strcmp(a, "image") || !strcmp(a, "quad"))
		glBegin(GL_QUADS);
	else if (!strcmp(a, "triangle"))
		glBegin(GL_TRIANGLES);
	else if (!strcmp(a, "line"))
		glBegin(GL_LINES);
	else if (!strcmp(a, "line strip"))
		glBegin(GL_LINE_STRIP);
	else if (!strcmp(a, "line loop"))
		glBegin(GL_LINE_LOOP);
	else if (!strcmp(a, "point"))
		glBegin(GL_POINTS);
	else if (!strcmp(a, "polygon"))
		glBegin(GL_POLYGON);
	else
		return err("display", "begin", "unknown type");
	
}

void end() {
	glEnd();
}

void draw(float x, float y, float sx, float sy, float r, float xo, float yo) {
	glBegin(GL_QUADS);
	idraw(x, y, sx, sy, r, xo, yo);
	glEnd();
}

void idraw(float x, float y, float sx, float sy, float r, float xo, float yo) {
	if(!sx) sx = 1;
	if(!sy) sy = sx;
	
	float co_x = 0, co_y = 0, co_x2 = 1, co_y2 = 1, w, h;
	
	image* ci = image_stack.top();
	crop* cr = &image_crop_stack.top();
	
	if(cr->use) {
		co_x = cr->x / ci->width;
		co_y = cr->y / ci->height;
		co_x2 = (cr->x + cr->w) / ci->width;
		co_y2 = (cr->y + cr->h) / ci->height;
		
		w = cr->w;
		h = cr->h;
	}
	else {
		w = ci->width;
		h = ci->height;
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

void write(const char* b, float x, float y, bool invert_y) {
	if(invert_y) {
		glPushMatrix();
		glScalef(1, -1, 1);
	}
	font_stack.top()->data->Render(b, -1, FTPoint(x, invert_y ? -y : y));
	if(invert_y)
		glPopMatrix();
}

void background_image(float x, float y, float s) {
	x /= image_stack.top()->width;
	y /= image_stack.top()->height;
	
	float sx = width / image_stack.top()->width / s, sy = height / image_stack.top()->height / s;
	
	glTexCoord2f(x, y); glVertex2i(0, 0);
	glTexCoord2f(x + sx, y); glVertex2i(width, 0);
	glTexCoord2f(x + sx, y + sy); glVertex2i(width, height);
	glTexCoord2f(x, y + sy); glVertex2i(0, height);
}

void quad(float x, float y, float x2, float y2) {
	glBegin(GL_QUADS);
	iquad(x, y, x2, y2);
	glEnd();
}

void quad() {
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(width, 0);
	glVertex2i(width, height);
	glVertex2i(0, height);
	glEnd();
}

void mquad(float x, float y, float x2, float y2) {
	glBegin(GL_QUADS);
	imquad(x, y, x2, y2);
	glEnd();
}

void mquad() {
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2i(0, 0);
	glTexCoord2f(1, 0); glVertex2i(width, 0);
	glTexCoord2f(1, 1); glVertex2i(width, height);
	glTexCoord2f(0, 1); glVertex2i(0, height);
	glEnd();
}

void iquad(float x, float y, float x2, float y2) {
	glVertex2i(x, y);
	glVertex2i(x2, y);
	glVertex2i(x2, y2);
	glVertex2i(x, y2);
}

void imquad(float x, float y, float x2, float y2) {
	glTexCoord2f(0, 0); glVertex2i(x, y);
	glTexCoord2f(1, 0); glVertex2i(x2, y);
	glTexCoord2f(1, 1); glVertex2i(x2, y2);
	glTexCoord2f(0, 1); glVertex2i(x, y2);
}

void point(float x, float y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void line(float a, float b, float c, float d) {
	glBegin(GL_LINES);
	glVertex2i(a, b);
	glVertex2i(c, d);
	glEnd();
}

void ipoint(float x, float y) {
	glVertex2i(x, y);
}

void iline(float a, float b, float c, float d) {
	glVertex2i(a, b);
	glVertex2i(c, d);
}

void portion(int x, int y, int x2, int y2) {
	image* ci = image_stack.top();
	
	glBegin(GL_QUADS);
	glTexCoord2f(x / ci->width, y / ci->height); glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0); glVertex2i(x, y);
	glTexCoord2f(x2 / ci->width, y / ci->height); glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0); glVertex2i(x2, y);
	glTexCoord2f(x2 / ci->width, y2 / ci->height); glMultiTexCoord2fARB(GL_TEXTURE1, 1, 1); glVertex2i(x2, y2);
	glTexCoord2f(x / ci->width, y2 / ci->height); glMultiTexCoord2fARB(GL_TEXTURE1, 0, 1); glVertex2i(x, y2);
	glEnd();
}

rgba pixel(int x, int y) {
	GLfloat c[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, c);
	return rgba(c[0], c[1], c[2]);
}

offset mouse_position() {
	int x, y;
	glfwGetMousePos(&x, &y);
	return offset(x, y);
}

void mouse(bool a) {
	if(a)
		glfwEnable(GLFW_MOUSE_CURSOR);
	else
		glfwDisable(GLFW_MOUSE_CURSOR);
}

// Utility function
int mouse_button_string_to_int(const char* a) {
	if(!strcmp(a, "left"))
		return GLFW_MOUSE_BUTTON_LEFT + 1;
	if(!strcmp(a, "middle"))
		return GLFW_MOUSE_BUTTON_MIDDLE + 1;
	if(!strcmp(a, "right"))
		return GLFW_MOUSE_BUTTON_RIGHT + 1;
	return -1;
}

bool button(const char* a) {
	return button(mouse_button_string_to_int(a));
}

bool button(short a) {
	if(a < 1 || a > 8)
		return false;
	
	return glfwGetMouseButton(GLFW_MOUSE_BUTTON_1 + a - 1) == GLFW_PRESS;
}

int wheel() {
	return glfwGetMouseWheel();
}

void move_mouse(int a, int b) {
	glfwSetMousePos(a, b);
}

int keyboard_key_string_to_int(const char* a) {
	if(!strcmp(a, "a")
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
	|| !strcmp(a, "`"))
		return toupper(a[0]);

	if(!strcmp(a, "space")) return GLFW_KEY_SPACE;
	else if(!strcmp(a, "esc") || !strcmp(a, "escape")) return GLFW_KEY_ESC;
	else if(!strcmp(a, "f1")) return GLFW_KEY_F1;
	else if(!strcmp(a, "f2")) return GLFW_KEY_F2;
	else if(!strcmp(a, "f3")) return GLFW_KEY_F3;
	else if(!strcmp(a, "f4")) return GLFW_KEY_F4;
	else if(!strcmp(a, "f5")) return GLFW_KEY_F5;
	else if(!strcmp(a, "f6")) return GLFW_KEY_F6;
	else if(!strcmp(a, "f7")) return GLFW_KEY_F7;
	else if(!strcmp(a, "f8")) return GLFW_KEY_F8;
	else if(!strcmp(a, "f9")) return GLFW_KEY_F9;
	else if(!strcmp(a, "f10")) return GLFW_KEY_F10;
	else if(!strcmp(a, "f11")) return GLFW_KEY_F11;
	else if(!strcmp(a, "f12")) return GLFW_KEY_F12;
	else if(!strcmp(a, "up")) return GLFW_KEY_UP;
	else if(!strcmp(a, "down")) return GLFW_KEY_DOWN;
	else if(!strcmp(a, "left")) return GLFW_KEY_LEFT;
	else if(!strcmp(a, "right")) return GLFW_KEY_RIGHT;
	else if(!strcmp(a, "left shift")) return GLFW_KEY_LSHIFT;
	else if(!strcmp(a, "right shift")) return GLFW_KEY_RSHIFT;
	else if(!strcmp(a, "left ctrl")) return GLFW_KEY_LCTRL;
	else if(!strcmp(a, "right ctrl")) return GLFW_KEY_RCTRL;
	else if(!strcmp(a, "left alt")) return GLFW_KEY_LALT;
	else if(!strcmp(a, "right alt")) return GLFW_KEY_RALT;
	else if(!strcmp(a, "tab")) return GLFW_KEY_TAB;
	else if(!strcmp(a, "enter") || !strcmp(a, "return")) return GLFW_KEY_ENTER;
	else if(!strcmp(a, "backspace")) return GLFW_KEY_BACKSPACE;
	else if(!strcmp(a, "insert")) return GLFW_KEY_INSERT;
	else if(!strcmp(a, "delete")) return GLFW_KEY_DEL;
	else if(!strcmp(a, "page up")) return GLFW_KEY_PAGEUP;
	else if(!strcmp(a, "page down")) return GLFW_KEY_PAGEDOWN;
	else if(!strcmp(a, "home")) return GLFW_KEY_HOME;
	else if(!strcmp(a, "end")) return GLFW_KEY_END;
	else if(!strcmp(a, "kp 1")) return GLFW_KEY_KP_1;
	else if(!strcmp(a, "kp 2")) return GLFW_KEY_KP_2;
	else if(!strcmp(a, "kp 3")) return GLFW_KEY_KP_3;
	else if(!strcmp(a, "kp 4")) return GLFW_KEY_KP_4;
	else if(!strcmp(a, "kp 5")) return GLFW_KEY_KP_5;
	else if(!strcmp(a, "kp 6")) return GLFW_KEY_KP_6;
	else if(!strcmp(a, "kp 7")) return GLFW_KEY_KP_7;
	else if(!strcmp(a, "kp 8")) return GLFW_KEY_KP_8;
	else if(!strcmp(a, "kp 9")) return GLFW_KEY_KP_9;
	else if(!strcmp(a, "kp 0")) return GLFW_KEY_KP_0;
	else if(!strcmp(a, "kp divide")) return GLFW_KEY_KP_DIVIDE;
	else if(!strcmp(a, "kp multiply")) return GLFW_KEY_KP_MULTIPLY;
	else if(!strcmp(a, "kp subtract")) return GLFW_KEY_KP_SUBTRACT;
	else if(!strcmp(a, "kp add")) return GLFW_KEY_KP_ADD;
	else if(!strcmp(a, "kp decimal")) return GLFW_KEY_KP_DECIMAL;
	else if(!strcmp(a, "kp equal")) return GLFW_KEY_KP_EQUAL;
	else if(!strcmp(a, "kp enter")) return GLFW_KEY_KP_ENTER;
	return -1;
}

bool key(const char* a) {
	if(!strcmp(a, "shift")) {
		if(glfwGetKey(keyboard_key_string_to_int("left shift")) || glfwGetKey(keyboard_key_string_to_int("right shift")))
			return true;
		return false;
	}
	if(!strcmp(a, "ctrl")) {
		if(glfwGetKey(keyboard_key_string_to_int("left ctrl")) || glfwGetKey(keyboard_key_string_to_int("right ctrl")))
			return true;
		return false;
	}
	if(!strcmp(a, "alt")) {
		if(glfwGetKey(keyboard_key_string_to_int("left alt")) || glfwGetKey(keyboard_key_string_to_int("right alt")))
			return true;
		return false;
	}
	
	if(glfwGetKey(keyboard_key_string_to_int(a)))
		return true;
	return false;
}
