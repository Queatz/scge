GLFWwindow glfw_window = NULL;
int glfw_state = 0;
Shikoba::Library * font_library = NULL;

void setdown_font() {
	delete font_library;
}

void setup_font() {
	if(glfw_state == 0)
		graphics();
	font_library = new Shikoba::Library();
	atexit(setdown_font);
}

void glfw_error(int code, const char* string) {
}

char key_int_to_char(int a) {
	if(a < 256) return (char) a;
	return 0;
}

const char* int_to_button_name(int a) {
	if(a == GLFW_MOUSE_BUTTON_LEFT)
		return "left";
	if(a == GLFW_MOUSE_BUTTON_MIDDLE)
		return "middle";
	if(a == GLFW_MOUSE_BUTTON_RIGHT)
		return "right";
	return "";
}

const char* int_to_key_name(int a) {
	switch(a) {
	case 'A': return "a";
	case 'B': return "b";
	case 'C': return "c";
	case 'D': return "d";
	case 'E': return "e";
	case 'F': return "f";
	case 'G': return "g";
	case 'H': return "h";
	case 'I': return "i";
	case 'J': return "j";
	case 'K': return "k";
	case 'L': return "l";
	case 'M': return "m";
	case 'N': return "n";
	case 'O': return "o";
	case 'P': return "p";
	case 'Q': return "q";
	case 'R': return "r";
	case 'S': return "s";
	case 'T': return "t";
	case 'U': return "u";
	case 'V': return "v";
	case 'W': return "w";
	case 'X': return "x";
	case 'Y': return "y";
	case 'Z': return "z";
	case '1': return "1";
	case '2': return "2";
	case '3': return "3";
	case '4': return "4";
	case '5': return "5";
	case '6': return "6";
	case '7': return "7";
	case '8': return "8";
	case '9': return "9";
	case '0': return "0";
	case '-': return "-";
	case '=': return "=";
	case '[': return "[";
	case ']': return "]";
	case '\\': return "\\";
	case ';': return ";";
	case '\'': return "'";
	case ',': return ",";
	case '.': return ".";
	case '/': return "/";
	case '`': return "`";
	
	case GLFW_KEY_SPACE: return "space";
	case GLFW_KEY_ESC: return "escape";
	case GLFW_KEY_F1: return "f1";
	case GLFW_KEY_F2: return "f2";
	case GLFW_KEY_F3: return "f3";
	case GLFW_KEY_F4: return "f4";
	case GLFW_KEY_F5: return "f5";
	case GLFW_KEY_F6: return "f6";
	case GLFW_KEY_F7: return "f7";
	case GLFW_KEY_F8: return "f8";
	case GLFW_KEY_F9: return "f9";
	case GLFW_KEY_F10: return "f10";
	case GLFW_KEY_F11: return "f11";
	case GLFW_KEY_F12: return "f12";
	case GLFW_KEY_UP: return "up";
	case GLFW_KEY_DOWN: return "down";
	case GLFW_KEY_LEFT: return "left";
	case GLFW_KEY_RIGHT: return "right";
	case GLFW_KEY_LSHIFT: return "left shift";
	case GLFW_KEY_RSHIFT: return "right shift";
	case GLFW_KEY_LCTRL: return "left ctrl";
	case GLFW_KEY_RCTRL: return "right ctrl";
	case GLFW_KEY_LALT: return "left alt";
	case GLFW_KEY_RALT: return "right alt";
	case GLFW_KEY_TAB: return "tab";
	case GLFW_KEY_ENTER: return "enter";
	case GLFW_KEY_BACKSPACE: return "backspace";
	case GLFW_KEY_INSERT: return "insert";
	case GLFW_KEY_DEL: return "delete";
	case GLFW_KEY_PAGEUP: return "page up";
	case GLFW_KEY_PAGEDOWN: return "page down";
	case GLFW_KEY_HOME: return "home";
	case GLFW_KEY_END: return "end";
	case GLFW_KEY_KP_1: return "kp 1";
	case GLFW_KEY_KP_2: return "kp 2";
	case GLFW_KEY_KP_3: return "kp 3";
	case GLFW_KEY_KP_4: return "kp 4";
	case GLFW_KEY_KP_5: return "kp 5";
	case GLFW_KEY_KP_6: return "kp 6";
	case GLFW_KEY_KP_7: return "kp 7";
	case GLFW_KEY_KP_8: return "kp 8";
	case GLFW_KEY_KP_9: return "kp 9";
	case GLFW_KEY_KP_0: return "kp 0";
	case GLFW_KEY_KP_DIVIDE: return "kp divide";
	case GLFW_KEY_KP_MULTIPLY: return "kp multiply";
	case GLFW_KEY_KP_SUBTRACT: return "kp subtract";
	case GLFW_KEY_KP_ADD: return "kp add";
	case GLFW_KEY_KP_DECIMAL: return "kp decimal";
	case GLFW_KEY_KP_EQUAL: return "kp equal";
	case GLFW_KEY_KP_ENTER: return "kp enter";
	case GLFW_KEY_KP_NUM_LOCK: return "num lock";
	case GLFW_KEY_CAPS_LOCK: return "caps lock";
	case GLFW_KEY_SCROLL_LOCK: return "scroll lock";
	case GLFW_KEY_PAUSE: return "pause";
	case GLFW_KEY_LSUPER: return "right super";
	case GLFW_KEY_RSUPER: return "left super";
	case GLFW_KEY_MENU: return "menu";
	}
	return "unknown";
}

bool graphics() {
	glfwSetErrorCallback(glfw_error);
	
	if(glfwInit() != GL_TRUE) {
		err("graphics", "could not");
		return false;
	}
	
	FreeImage_Initialise();
		
	glfw_state = 1;
	return true;
}

void graphics_off() {
	FreeImage_DeInitialise();
	glfwTerminate();
}

FREE_IMAGE_FORMAT fif_from_string(const char* a = NULL, const char* b = NULL) {
	if(b) {
		if(!strcmp(b, "png"))
			return FIF_PNG;
		else if(!strcmp(b, "jpg"))
			return FIF_JPEG;
		else if(!strcmp(b, "bmp"))
			return FIF_BMP;
	}
	
	if(a) {
		FREE_IMAGE_FORMAT t = FreeImage_GetFileType(a, 0);
	
		if(t == FIF_UNKNOWN)
			t = FreeImage_GetFIFFromFilename(a);
		return t;
	}
	
	return FIF_UNKNOWN;
}

int default_from_fif(FREE_IMAGE_FORMAT a) {
	if(a == FIF_PNG)
		return PNG_DEFAULT;
	else if(a == FIF_JPEG)
		return JPEG_QUALITYSUPERB;
	else if(a == FIF_BMP)
		return BMP_DEFAULT;
	else
		return 0;
}

void key_repeat(bool a) {
	glfwSetInputMode(glfw_window, GLFW_KEY_REPEAT, a ? GL_TRUE : GL_FALSE);
}

bool window(const char* title, int x, int y, bool fullscreen, bool resizeable, int fsaa) {
	if(glfw_state == 0)
		graphics();
	
	if(!x || !y) {
		if(fullscreen) {
			GLFWvidmode d;
			glfwGetDesktopMode(&d);
			x = d.width;
			y = d.height;
		}
		else {
			x = 320;
			y = 240;
		}
	}
	
	if(fsaa)
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, fsaa);
	
	glfwOpenWindowHint(GLFW_WINDOW_RESIZABLE, resizeable ? GL_TRUE : GL_FALSE);

#ifdef _WIN32
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
#else
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	glfw_window = glfwOpenWindow(x, y, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOWED), title, NULL);
	if(!glfw_window) {
		err("window", "could not initiate window");
		return false;
	}

#ifdef _WIN32
	if(glewInit() != GLEW_OK)
		err("window", "(windows) extensions unsupported");
#endif

	glfw_state = 2;
	
	glfwSwapInterval(0);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	glViewport(0, 0, w, h);
	
	return true;
}

void close_window() {
	glfwCloseWindow(glfw_window);
	glfw_state = 1;
}

void window_title(const char* a) {
	glfwSetWindowTitle(glfw_window, a);
}

bool window_opened() {
	if(glfwIsWindow(glfw_window))
		return true;
	return false;
}

bool window_active() {
	if(glfwGetWindowParam(glfw_window, GLFW_ACTIVE))
		return true;
	return false;
}

std::string display_modes() {
	if(glfw_state == 0)
		graphics();
	
	GLFWvidmode m[128] = {0};
	std::ostringstream s;
	std::string l;
	int i, c;
	
	c = glfwGetVideoModes(m, 128);
	
	for(i = 0; i < c; i++) {
		if(i > 0) s << " ";
		s << m[i].width << "x" << m[i].height;
	}
	
	l = s.str();
	return l;
}

glm::ivec2 display_dimensions() {
	if(glfw_state == 0)
		graphics();
	
	GLFWvidmode a;
	glfwGetDesktopMode(&a);
	
	return glm::ivec2(a.width, a.height);
}

glm::ivec2 window_dimensions() {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	
	return glm::ivec2(w, h);
}

void window_size(int w, int h) {
	glfwSetWindowSize(glfw_window, w, h);
}

void position_window(int x, int y) {
	glfwSetWindowPos(glfw_window, x, y);
}

glm::ivec2 window_position() {
	glm::ivec2 w;
	glfwGetWindowPos(glfw_window, &w.x, &w.y);
	return w;
}

void vsync(bool a) {	
	glfwSwapInterval(a ? 1 : 0);
}

void swap() {
	glfwSwapBuffers();
}

void poll() {
	glfwPollEvents();
}

bool screenshot(const char* a, const char* b) {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);

	
	pixelcache p(w, h);
	
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, p.data);
	
	if (p.save(a, b))
		return true;
	err("screenshot", "could not save");
	return false;

}

void viewport(int x, int y, int w, int h) {
	glViewport(x, y, w, h);
}

void polygon_mode(const char* a) {
	int c = -1;
	
	if (!strcmp(a, "dot"))
		c = GL_POINT;
	else if (!strcmp(a, "outline"))
		c = GL_LINE;
	else if (!strcmp(a, "fill"))
		c = GL_FILL;
	
	if(c > -1)
		glPolygonMode(GL_FRONT_AND_BACK, c);
	else
		err("polygon_mode", "invalid option");
}

void enable(const char* a, bool b) {
	GLint c = -1;
	
	if (!strcmp(a, "scissor"))
		c = GL_SCISSOR_TEST;
	else if (!strcmp(a, "polygon smooth"))
		c = GL_POLYGON_SMOOTH;
	else if (!strcmp(a, "line smooth"))
		c = GL_LINE_SMOOTH;
	else if (!strcmp(a, "blend"))
		c = GL_BLEND;
	else if (!strcmp(a, "depth"))
		c = GL_DEPTH_TEST;
	else if (!strcmp(a, "stencil"))
		c = GL_STENCIL_TEST;
	else if (!strcmp(a, "polygon depth offset"))
		c = GL_POLYGON_OFFSET_FILL;
	else if (!strcmp(a, "line depth offset"))
		c = GL_POLYGON_OFFSET_LINE;
	else if (!strcmp(a, "point depth offset"))
		c = GL_POLYGON_OFFSET_POINT;
	else if (!strcmp(a, "cull"))
		c = GL_CULL_FACE;
	else if (!strcmp(a, "program point size"))
		c = GL_PROGRAM_POINT_SIZE;
	else if (!strcmp(a, "multisample"))
		c = GL_MULTISAMPLE;
	
	if(c > -1) {
		if(b) glEnable(c);
		else glDisable(c);
	}
	else
		err("enable", "invalid option");
}

void cull(const char* a) {
	GLint c = -1;
	
	if (!strcmp(a, "front"))
		c = GL_FRONT;
	else if (!strcmp(a, "back"))
		c = GL_BACK;
	else if (!strcmp(a, "all"))
		c = GL_FRONT_AND_BACK;
	
	if(c > -1)
		glCullFace(c);
	else
		err("cull", "invalid option");
}

void clear_color(float a, float b, float c, float d) {
	glClearColor(a, b, c, d);
}

void clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void depth_clear() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

GLenum comparison_string_to_gl(const char* a) {
	if(!strcmp(a, "=="))
		return GL_EQUAL;
	else if(!strcmp(a, "!="))
		return GL_NOTEQUAL;
	else if(!strcmp(a, "<"))
		return GL_LESS;
	else if(!strcmp(a, ">"))
		return GL_GREATER;
	else if(!strcmp(a, "<="))
		return GL_LEQUAL;
	else if(!strcmp(a, ">="))
		return GL_GEQUAL;
	else if(!strcmp(a, "always"))
		return GL_ALWAYS;
	else if(!strcmp(a, "never"))
		return GL_NEVER;
	
	return 0;
}

void depth_test(const char* a) {
	glDepthFunc(comparison_string_to_gl(a));
}

void depth_op(bool a) {
	glDepthMask(a ? GL_TRUE : GL_FALSE);
}

void  point_size(float a) {
	glPointSize(a);
}

void  line_width(float a) {
	glLineWidth(a);
}

void polygon_depth_offset(float a, float b) {
	glPolygonOffset(a, b);
}

void color_mask(bool r, bool g, bool b, bool a) {
	glColorMask(r, g, b, a);
}

void color_mask(bool a) {
	glColorMask(a, a, a, a);
}

void use_fbo(fbo* a) {
	glBindFramebuffer(GL_FRAMEBUFFER, a->id);
}

void use_fbo() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void use_vbo(vbo* a) {
	glBindBuffer(GL_ARRAY_BUFFER, a->id);
}

void use_vbo() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void use_vao(vao* a) {
	glBindVertexArray(a->id);
}

void use_vao() {
	glBindVertexArray(0);
}

void use_ibo(ibo* a) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a->id);
}

void use_ibo() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void scissor(int a, int b, int c, int d) {
	glScissor(a, b, c, d);
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
		else if (!strcmp(a, "color"))
			glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
		else if (!strcmp(a, "multiply"))
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		else if(!strcmp(a, "mix"))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else if(!strcmp(a, "color"))
			glBlendFunc(GL_ONE, GL_ZERO);
		else if(!strcmp(a, "saturate"))
			glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
		else
			err("blend_mode", "invalid value");
	}
}

void blend_mode(const char* a) {
	display_set_blend_mode_from_string(a);
}

void use_program(program* a) {
	glUseProgram(a->id);
}

void use_program() {
	glUseProgram(0);
}

GLenum primitive_from_string(const char* a) {
		if (!strcmp(a, "triangle"))
			return GL_TRIANGLES;
		else if (!strcmp(a, "triangle strip"))
			return GL_TRIANGLE_STRIP;
		else if (!strcmp(a, "triangle fan"))
			return GL_TRIANGLE_FAN;
		else if (!strcmp(a, "line"))
			return GL_LINES;
		else if (!strcmp(a, "line strip"))
			return GL_LINE_STRIP;
		else if (!strcmp(a, "line loop"))
			return GL_LINE_LOOP;
		else if (!strcmp(a, "point"))
			return GL_POINTS;
		else
			err("primitive", "unknown type");
		return 0;
}

glm::vec4 pixel(int x, int y, const char* f) {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	
	if(x < 0 || x >= w || y < 0 || y >= h)
		return glm::vec4(0.0, 0.0, 0.0, 1.0);
	
	GLenum fmt = GL_RGB;
	if(w) {
		if(!strcmp(f, "depth"))
			fmt = GL_DEPTH_COMPONENT;
		else if(!strcmp(f, "stencil"))
			fmt = GL_STENCIL_INDEX;
	}
	
	GLfloat c[3];
	glReadPixels(x, y, 1, 1, fmt, GL_FLOAT, c);
	return glm::vec4(c[0], c[1], c[2], 1.0);
}

void stencil_clear(int a) {
	glClearStencil(a);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void stencil_test(const char* a, int b) {
	glStencilFunc(comparison_string_to_gl(a), b, 1);
}

void stencil_op(const char* a) {
	GLenum f;
	if(!strcmp(a, "keep"))
		f = GL_KEEP;
	else if(!strcmp(a, "zero"))
		f = GL_ZERO;
	else if(!strcmp(a, "replace"))
		f = GL_REPLACE;
	else if(!strcmp(a, "increment"))
		f = GL_INCR;
	else if(!strcmp(a, "decrement"))
		f = GL_DECR;
	
	glStencilOp(f, f, f);
}

glm::vec2 mouse_position() {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	
	int x, y;
	glm::vec2 r;
	glfwGetMousePos(glfw_window, &x, &y);
	r = glm::vec2(x, h - y);
	return r;
}

void mouse(const char* a) {
	if(!strcmp(a, "show"))
		glfwSetInputMode(glfw_window, GLFW_CURSOR_MODE, GLFW_CURSOR_NORMAL);
	else if(!strcmp(a, "hide"))
		glfwSetInputMode(glfw_window, GLFW_CURSOR_MODE, GLFW_CURSOR_HIDDEN);
	else if(!strcmp(a, "capture"))
		glfwSetInputMode(glfw_window, GLFW_CURSOR_MODE, GLFW_CURSOR_CAPTURED);
	else
		err("mouse", "invalid option");
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
	
	return glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_1 + a - 1) == GLFW_PRESS;
}

float wheel() {
	double x, y;
	glfwGetScrollOffset(glfw_window, &x, &y);
	return y;
}

void move_mouse(int a, int b) {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	
	glfwSetMousePos(glfw_window, a, h - b);
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
	|| !strcmp(a, "-")
	|| !strcmp(a, "=")
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
	else if(!strcmp(a, "escape")) return GLFW_KEY_ESC;
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
	else if(!strcmp(a, "left control")) return GLFW_KEY_LCTRL;
	else if(!strcmp(a, "right control")) return GLFW_KEY_RCTRL;
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
	else if(!strcmp(a, "num lock")) return GLFW_KEY_KP_NUM_LOCK;
	else if(!strcmp(a, "caps lock")) return GLFW_KEY_CAPS_LOCK;
	else if(!strcmp(a, "scroll lock")) return GLFW_KEY_SCROLL_LOCK;
	else if(!strcmp(a, "pause")) return GLFW_KEY_PAUSE;
	else if(!strcmp(a, "left super")) return GLFW_KEY_LSUPER;
	else if(!strcmp(a, "right super")) return GLFW_KEY_RSUPER;
	else if(!strcmp(a, "menu")) return GLFW_KEY_MENU;
	return -1;
}

bool key(const char* a) {
	if(!strcmp(a, "shift")) {
		if(glfwGetKey(glfw_window, keyboard_key_string_to_int("left shift")) || glfwGetKey(glfw_window, keyboard_key_string_to_int("right shift")))
			return true;
		return false;
	}
	if(!strcmp(a, "ctrl")) {
		if(glfwGetKey(glfw_window, keyboard_key_string_to_int("left ctrl")) || glfwGetKey(glfw_window, keyboard_key_string_to_int("right ctrl")))
			return true;
		return false;
	}
	if(!strcmp(a, "alt")) {
		if(glfwGetKey(glfw_window, keyboard_key_string_to_int("left alt")) || glfwGetKey(glfw_window, keyboard_key_string_to_int("right alt")))
			return true;
		return false;
	}
	if(!strcmp(a, "super")) {
		if(glfwGetKey(glfw_window, keyboard_key_string_to_int("left super")) || glfwGetKey(glfw_window, keyboard_key_string_to_int("right super")))
			return true;
		return false;
	}
	
	if(glfwGetKey(glfw_window, keyboard_key_string_to_int(a)))
		return true;
	return false;
}

bool key_state(const char* a) {
	//if(glfwGetKeyState(glfw_window, keyboard_key_string_to_int(a)))
	//	return true;
	return false;
}

pixelcache::pixelcache(const char* a) {
	FIBITMAP* bm = FreeImage_Load(fif_from_string(a), a, 0);
	
	if(!bm) {
		err("pixelcache", "could not load");
		return;
	}
	
	colors = FreeImage_IsTransparent(bm) ? 4 : 3;
	
	BYTE *bits = FreeImage_GetBits(bm);
	int bpp = FreeImage_GetBPP(bm);
	
	width = FreeImage_GetWidth(bm);
	height = FreeImage_GetHeight(bm);
	
	int i, j, s = height*width;
	RGBQUAD pix;
	data = new GLubyte[s*colors];
	for(i = 0; i < width; i++)
	for(j = 0; j < height; j++) {
		FreeImage_GetPixelColor(bm, i, j, &pix);
		data[(j*width+i)*colors] = pix.rgbRed;
		data[(j*width+i)*colors+1] = pix.rgbGreen;
		data[(j*width+i)*colors+2] = pix.rgbBlue;
		if(colors == 4)
			data[(j*width+i)*colors+3] = pix.rgbReserved;
	}
	
	FreeImage_Unload(bm);
}

pixelcache::pixelcache(const pixelcache& a) {
	width = a.width;
	height = a.height;
	colors = a.colors;
	
	int s = height*width*colors;
	data = new GLubyte[s];
	memcpy(data, a.data, sizeof(GLubyte)*s);
}

pixelcache::pixelcache(int w, int h, bool a) {
	width = w;
	height = h;
	colors = a ? 4 : 3;
	data = new GLubyte[height*width*colors];
}

pixelcache::~pixelcache() {
	if(data)
		delete data;
}

glm::vec4 pixelcache::pixel(int x, int y) {
	int h;
	
	if(x < 0 || x >= width || y < 0 || y >= height)
		return glm::vec4(0.0, 0.0, 0.0, colors == 4 ? 0.0 : 1.0);
	
	h = y * width * colors + x * colors;
	return glm::vec4(static_cast<float>(data[h + 0]) / 255.0, static_cast<float>(data[h + 1]) / 255.0, static_cast<float>(data[h + 2]) / 255.0, colors == 4 ? static_cast<float>(data[h + 3]) / 255.0 : 1.0);
}

void pixelcache::set_pixel(int x, int y, glm::vec4 c) {
	int h;
	if(x < 0 || x >= width || y < 0 || y >= height) {
		err("pixelcache", "set_pixel", "out of bounds");
		return;
	}
	
	h = y * width * colors + x * colors;
	data[h + 0] = static_cast<GLubyte>(c.r * 255.0);
	data[h + 1] = static_cast<GLubyte>(c.g * 255.0);
	data[h + 2] = static_cast<GLubyte>(c.b * 255.0);
	if(colors == 4)
		data[h + 3] = static_cast<GLubyte>(c.a * 255.0);
}

bool pixelcache::save(const char* a, const char* b) {
	if(!data)
		return false;
	
	FIBITMAP* c = FreeImage_Allocate(width, height, colors == 4 ? 32 : 24);
	RGBQUAD color;
	
	for(unsigned int x = 0; x < width; x++)
		for(unsigned int y = 0; y < height; y++) {
			int h = y * width * colors + x * colors;
			color.rgbRed = data[h + 0];
			color.rgbGreen = data[h + 1];
			color.rgbBlue = data[h + 2];
			if(colors == 4)
				color.rgbReserved = data[h + 3];
			FreeImage_SetPixelColor(c, x, y, &color);
		}
	
	if (FreeImage_Save(fif_from_string(a, b), c, a, default_from_fif(fif_from_string(a, b))))
		return true;
	return false;
	
	FreeImage_Unload(c);
}

image::image(const char* a, bool m) {
	if(glfw_state == 0)
		graphics();

	FIBITMAP *bm = FreeImage_Load(fif_from_string(a), a, 0);
	
	if(!bm) {
		err("image", "could not load");
		return;
	}
	
	bool has_alpha = FreeImage_IsTransparent(bm);
	
	FIBITMAP *tmp = bm;
	if(has_alpha)
		bm = FreeImage_ConvertTo32Bits(bm);
	else
		bm = FreeImage_ConvertTo24Bits(bm);
	FreeImage_Unload(tmp);
	
	
	BYTE *bits = new BYTE[FreeImage_GetHeight(bm) * FreeImage_GetPitch(bm)];
	
	FreeImage_ConvertToRawBits(bits, bm, FreeImage_GetPitch(bm), (has_alpha ? 32 : 24), FI_RGBA_BLUE_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_RED_MASK, FALSE);
	
	GLint last;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last);
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(bm), FreeImage_GetHeight(bm), 0, (has_alpha ? GL_BGRA : GL_BGR), GL_UNSIGNED_BYTE, bits);
	
	if(m)
		glGenerateMipmap(GL_TEXTURE_2D);
	
	FreeImage_Unload(bm);
	
	external_cache = false;
	cache = NULL;
	mipmaps = m;
	
	if(id == 0) {
		err("image", "could not load");
		return;
	}
	
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	
	if(m) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	
	glBindTexture(GL_TEXTURE_2D, last);
}

image::image(int a, int b, bool alpha, bool quality) {
	if(glfw_state == 0)
		graphics();
	
	cache = NULL;
	mipmaps = false;
	external_cache = false;
	
	width = a;
	height = b;
	
	GLint last;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last);
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? (quality ? GL_RGBA12 : GL_RGBA8) : (quality ? GL_RGB12 : GL_RGB8), a, b, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, last);
}

image::image(int a, int b, const char* f) {
	if(glfw_state == 0)
		graphics();
	
	cache = NULL;
	mipmaps = false;
	external_cache = false;
	
	width = a;
	height = b;
	
	GLint last;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last);
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	GLenum fmt, ifmt;
	if(!strcmp(f, "r")) {
		fmt = GL_RED;
		ifmt = GL_R8;
	}
	else if(!strcmp(f, "rg")) {
		fmt = GL_RG;
		ifmt = GL_RG8;
	}
	else if(!strcmp(f, "rgb")) {
		fmt = GL_RGB;
		ifmt = GL_RGB8;
	}
	else if(!strcmp(f, "rgba")) {
		fmt = GL_RGBA;
		ifmt = GL_RGBA8;
	}
	else if(!strcmp(f, "depth")) {
		fmt = GL_DEPTH_COMPONENT;
		ifmt = GL_DEPTH_COMPONENT24;
	}
	else if(!strcmp(f, "depth stencil")) {
		fmt = GL_DEPTH_STENCIL;
		ifmt = GL_DEPTH24_STENCIL8;
	}
	else {
		err("image", "invalid format");
		fmt = GL_RED;
		ifmt = GL_R8;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, ifmt, a, b, 0, fmt, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, last);
}

image::image(pixelcache* p) {
	if(glfw_state == 0)
		graphics();
	
	mipmaps = false;
	external_cache = true;
	cache = p;
	
	width = p->width;
	height = p->height;
	colors = p->colors;
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, p->colors == 4 ? GL_RGBA8 : GL_RGB8, width, height, 0, p->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	from_pixelcache();
}

image::~image() {
	if(!external_cache && cache)
		delete cache;
}

void image::set(const char* a) {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	if (!strcmp(a, "linear")) {
		if(mipmaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else if (!strcmp(a, "nearest")) {
		if(mipmaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}  else if (!strcmp(a, "nearest mipmap")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else if (!strcmp(a, "mirrored repeat")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	} else if (!strcmp(a, "repeat")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else if (!strcmp(a, "clamp to edge")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
		err("image", "set", "invalid value");
	
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
	
	// Reset to the original texture
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
	if(!cache) {
		err("image", "from_pixelcache", "no cache");
		return;
	}
	
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, cache->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(int x, int y, int w, int h) {
	if(!cache) {
		err("image", "from_pixelcache", "no cache");
		return;
	}
	
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, cache->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
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
	
	glGetTexImage(GL_TEXTURE_2D, 0, cache->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::discard_pixel_cache() {
	if(!external_cache && cache)
		delete cache;
	cache = NULL;
}

glm::vec4 image::pixel(int x, int y) {
	if(!cache)
		refresh_pixel_cache();
	
	return cache->pixel(x, y);
}

bool image::save(const char* a, const char* b) {
	if(!cache)
		refresh_pixel_cache();
	
	return cache->save(a, b);
}

fontface::fontface(const char* a) {
	if(font_library == NULL)
		setup_font();
	
	data = new Shikoba::Face(font_library, a);
}

fontface::~fontface() {
	delete data;
}

void font_face(fontface* f) {
	if(font_library == NULL)
		setup_font();

	font_library->setFace(f->data);
}

void font_size(unsigned int s) {
	if(font_library == NULL)
		setup_font();

	font_library->setSize(s);
}

float line_height() {
	if(font_library == NULL)
		setup_font();

	return font_library->height();
}

float ascent() {
	if(font_library == NULL)
		setup_font();

	return font_library->ascender();
}

float descent() {
	if(font_library == NULL)
		setup_font();

	return font_library->descender();
}

float advance(const char* a, const char* b) {
	if(font_library == NULL)
		setup_font();

	return font_library->advance(utf8::unchecked::next(a), utf8::unchecked::next(b));
}

glyphmetrics glyph(const char* a) {
	if(font_library == NULL)
		setup_font();

	glyphmetrics g;
	const Shikoba::Glyph * s = font_library->glyph(utf8::unchecked::next(a));

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

shader::shader(const char* a, const char* b, bool isfile) {
	if(glfw_state < 2) {
		id = 0;
		err("shader", "window needs to be open");
		return;
	}
	
	if(!strcmp(a, "vertex"))
		id = glCreateShader(GL_VERTEX_SHADER);
	else if(!strcmp(a, "fragment"))
		id = glCreateShader(GL_FRAGMENT_SHADER);
	else {
		id = 0;
		err("shader", "invalid type");
		return;
	}
	if(isfile) {
		const char* c = read_file(b);
		if(c) {
			glShaderSource(id, 1, &c, NULL);
			
			delete c;
		}
		else
			err("shader", "could not load file");
	}
	else
		glShaderSource(id, 1, &b, NULL);
	
	glCompileShader(id);
	
	int compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	
	if(compiled != GL_TRUE) {
		GLchar* infoLog;
		GLint length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		infoLog = new GLchar[length];
		
		glGetShaderInfoLog(id, length, &length, infoLog);
		
		err("shader", "compile", "error");
		std::cout << infoLog << std::endl;
		
		delete infoLog;
	}
}

shader::~shader() {
	if(id)
		glDeleteShader(id);
}

program::program() {
	if(glfw_state < 2) {
		id = 0;
		err("program", "window needs to be open");
		return;
	}
	
	id = glCreateProgram();
}

program::~program() {
	glDeleteProgram(id);
}

void program::uniform(const char* a, int b) {
	glUniform1i(glGetUniformLocation(id, a), b);
}

void program::uniform(const char* a, float b) {
	glUniform1f(glGetUniformLocation(id, a), b);
}
/*$ PROGRAM::UNIFORM $*/

void program::uniform(const char* a, glm::${p}${type}${n} m) {
	glUniform${'Matrix' if type == 'mat' else ''}${n}${p if p else 'f'}v(glGetUniformLocation(id, a), 1${', GL_FALSE' if type == 'mat' else ''}, glm::value_ptr(m));
}
/*$ $*/

void program::uniform(const char* a, image* c, int b) {
	GLint act;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_2D, c->id);
	
	uniform(a, b);
	
	// Reset to the original texture XXX FIXME don't have to?
	if(act != GL_TEXTURE0 + b) glActiveTexture(act);
}

void program::bind_font(const char* a, int b) {
	if(!font_library)
		setup_font();

	GLint act;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_2D, font_library->texture());
	
	uniform(a, b);
	
	// Reset to the original texture XXX FIXME don't have to?
	if(act != GL_TEXTURE0 + b) glActiveTexture(act);
}

void program::attribute(int b, const char* a) {
	if(id)
		glBindAttribLocation(id, b, a);
}

void program::attach(shader* a) {
	if(id)
		glAttachShader(id, a->id);
}

void program::link() {
	if(id)
		glLinkProgram(id);
}

fbo::fbo(image* a) {
	if(glfw_state == 0)
		graphics();
	
	GLint last, lastt;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastt);
	
	glGenFramebuffers(1, &id);
	
	depth = NULL;
	buffer = a;
	buffer_is_mine = false;
	
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	
	glBindTexture(GL_TEXTURE_2D, buffer->id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->id, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, lastt);
	glBindFramebuffer(GL_FRAMEBUFFER, last);
}

fbo::fbo(int b, int c, bool alpha, bool quality, bool ds, int multisample) {
	if(glfw_state == 0)
		graphics();
	
	glGenFramebuffers(1, &id);
	
	GLint last, lastt;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastt);
	
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	
	buffer_is_mine = true;
	buffer = new image(b, c, alpha, quality);
	
	glBindTexture(GL_TEXTURE_2D, buffer->id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->id, 0);
	
	if(ds) {
		depth = new image(b, c, "depth");
	
		glBindTexture(GL_TEXTURE_2D, depth->id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->id, 0);
	}
	else {
		depth = NULL;
	}
	
	glBindTexture(GL_TEXTURE_2D, lastt);
	glBindFramebuffer(GL_FRAMEBUFFER, last);
}

fbo::~fbo() {
	if(buffer_is_mine)
		delete buffer;
	if(depth)
		delete depth;
	glDeleteFramebuffers(1, &id);
}

GLenum buffer_usage_from_string(const char* t) {
	if(!strcmp(t, "static draw"))
		return GL_STATIC_DRAW;
	else if(!strcmp(t, "static read"))
		return GL_STATIC_READ;
	else if(!strcmp(t, "static copy"))
		return GL_STATIC_COPY;
	else if(!strcmp(t, "stream draw"))
		return GL_STREAM_DRAW;
	else if(!strcmp(t, "stream read"))
		return GL_STREAM_READ;
	else if(!strcmp(t, "stream copy"))
		return GL_STREAM_COPY;
	else if(!strcmp(t, "dynamic draw"))
		return GL_DYNAMIC_DRAW;
	else if(!strcmp(t, "dynamic read"))
		return GL_DYNAMIC_READ;
	else if(!strcmp(t, "dynamic copy"))
		return GL_DYNAMIC_COPY;
	return GL_STATIC_DRAW;
}

GLenum common_type_from_string(const char* a) {
	if(!strcmp(a, "float"))
		return GL_FLOAT;
	else if(!strcmp(a, "byte"))
		return GL_BYTE;
	else if(!strcmp(a, "unsigned byte"))
		return GL_UNSIGNED_BYTE;
	else if(!strcmp(a, "short"))
		return GL_SHORT;
	else if(!strcmp(a, "unsigned short"))
		return GL_UNSIGNED_SHORT;
	else if(!strcmp(a, "int"))
		return GL_INT;
	else if(!strcmp(a, "unsigned int"))
		return GL_UNSIGNED_INT;
	else if(!strcmp(a, "double"))
		return GL_DOUBLE;
	else {
		err("type", "invalid");
		return -1;
	}
}

vbo::vbo(int l, const char* t) {
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	glGenBuffers(1, &id);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, l, NULL, buffer_usage_from_string(t));
	
	glBindBuffer(GL_ARRAY_BUFFER, last);
	
	size = l;
}

void vbo::data(const void* data, int offset, int size) {
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	
	if(last != id)
		glBindBuffer(GL_ARRAY_BUFFER, last);
}

#ifdef WITH_PYTHON
vbo::vbo(PyObject* o, const char* t) {
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	glGenBuffers(1, &id);
	
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)), buffer_usage_from_string(t));
	
	glBindBuffer(GL_ARRAY_BUFFER, last);
	
	size = PyBytes_Size(o);
}

void vbo::data(PyObject* o, int offset) {
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ARRAY_BUFFER, offset, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)));
	
	if(last != id)
		glBindBuffer(GL_ARRAY_BUFFER, last);
}
#endif

void vbo::data(vbo* o, int offset, int copy_offset, int l) {
	glBindBuffer(GL_COPY_READ_BUFFER, o->id);
	glBindBuffer(GL_COPY_WRITE_BUFFER, id);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset, copy_offset, l >= 0 ? l : o->size);
}

vbo::~vbo() {
	glDeleteBuffers(1, &id);
}

void draw(const char* a, unsigned int count, unsigned int first) {
	glDrawArrays(primitive_from_string(a), first, count);
}

ibo::ibo(int l, const char* s, const char* t) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	glGenBuffers(1, &id);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, l, NULL, buffer_usage_from_string(t));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
	
	storage = common_type_from_string(s);
	
	size = l;
}

void ibo::data(const void* data, int offset, int size) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
}

ibo::~ibo() {
	glDeleteBuffers(1, &id);
}

#ifdef WITH_PYTHON
ibo::ibo(PyObject* o, const char* s, const char* t) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	glGenBuffers(1, &id);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)), buffer_usage_from_string(t));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
	
	storage = common_type_from_string(s);
	size = PyBytes_Size(o);
}

void ibo::data(PyObject * o, int offset) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)));
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
}
#endif

void ibo::data(ibo* o, int offset, int copy_offset, int l) {
	glBindBuffer(GL_COPY_READ_BUFFER, o->id);
	glBindBuffer(GL_COPY_WRITE_BUFFER, id);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset, copy_offset, l >= 0 ? l : o->size);
}

void ibo::draw(const char* a, unsigned int count, unsigned int first) {
	glDrawElements(primitive_from_string(a), (GLsizei)count, storage, reinterpret_cast<const GLvoid*>(first));
}

vao::vao() {
	GLint last;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	
	glGenVertexArrays(1, &id);
	
	glBindVertexArray(last);
}

vao::~vao() {
	glDeleteVertexArrays(1, &id);
}

void vao::enable(unsigned int i, bool e) {
	GLint last;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	
	if(last != id)
		glBindVertexArray(id);
	
	if(e)
		glEnableVertexAttribArray(i);
	else
		glDisableVertexAttribArray(i);
	
	if(last != id)
		glBindVertexArray(last);
}

void vao::divisor(unsigned int i, unsigned int d) {
	GLint last;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	
	if(last != id)
		glBindVertexArray(id);
	
	glVertexAttribDivisor(i, d);
	
	if(last != id)
		glBindVertexArray(last);
}

void vao::attribute(unsigned int index, vbo* v, const char* type, int size, int offset, int stride, bool normalized, bool integers) {
	GLint last, lastb;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastb);
	
	if(last != id)
		glBindVertexArray(id);
	if(lastb != v->id)
		glBindBuffer(GL_ARRAY_BUFFER, v->id);
	
	if(integers)
		glVertexAttribIPointer(index, size, common_type_from_string(type), (GLsizei)stride, reinterpret_cast<const GLvoid*>(offset));
	else
		glVertexAttribPointer(index, size, common_type_from_string(type), normalized, (GLsizei)stride, reinterpret_cast<const GLvoid*>(offset));
	
	if(lastb != v->id)
		glBindBuffer(GL_ARRAY_BUFFER, lastb);
	if(last != id)
		glBindVertexArray(last);
}
