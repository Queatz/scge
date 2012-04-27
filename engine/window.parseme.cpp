window::window(const char* title, glm::ivec2 s, bool fullscreen, bool resizeable, int fsaa) : win(NULL) {
#ifdef WITH_PYTHON
/*$ CALLBACK $*/
	_${n}_callback = NULL;
/*$ $*/
#endif

	if(glfw_state == 0)
		graphics();
	
	if(!s.x || !s.y) {
		if(fullscreen) {
			GLFWvidmode d;
			glfwGetDesktopMode(&d);
			s.x = d.width;
			s.y = d.height;
		}
		else {
			s.x = 320;
			s.y = 240;
		}
	}
	
	if(fsaa)
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, fsaa);
	
	glfwOpenWindowHint(GLFW_WINDOW_RESIZABLE, resizeable ? GL_TRUE : GL_FALSE);

#ifndef _WIN32
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	win = glfwOpenWindow(s.x, s.y, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOWED), title, NULL);
	
	if(!win) {
		err("window", "could not initiate window");
		return;
	}
	
	glfwSetWindowUserPointer(win, this);
	
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
	glfwGetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);
}

window::~window() {
	if(win)
		close();
}

void window::close() {
	glfwIsWindow(win);
	win = NULL;
}

void window::title(const char* a) {
	glfwSetWindowTitle(win, a);
}

bool window::opened() {
	return glfwIsWindow(win) ? true : false;
}

bool window::active() {
	return glfwGetWindowParam(win, GLFW_ACTIVE) ? true : false;
}

bool window::iconified() {
	return glfwGetWindowParam(win, GLFW_ICONIFIED) ? true : false;
}

void window::iconify(bool i) {
	if(i)
		glfwIconifyWindow(win);
	else
		glfwRestoreWindow(win);
}

glm::ivec2 window::size() {
	glm::ivec2 w;
	glfwGetWindowSize(win, &w.x, &w.y);
	
	return w;
}

void window::size(glm::ivec2 a) {
	glfwSetWindowSize(win, a.x, a.y);
}

void window::position(glm::ivec2 a) {
	glfwSetWindowPos(win, a.x, a.y);
}

glm::ivec2 window::position() {
	glm::ivec2 w;
	glfwGetWindowPos(win, &w.x, &w.y);
	return w;
}

void window::set(const char* a, bool b) {
	if(!strcmp(a, "key repeat"))
		glfwSetInputMode(win, GLFW_KEY_REPEAT, b ? GL_TRUE : GL_FALSE);
	else
		err("window", "set", "unknown parameter");
}

glm::vec2 window::mouse() {
	int w, h;
	glfwGetWindowSize(win, &w, &h);
	
	int x, y;
	glm::vec2 r;
	glfwGetMousePos(win, &x, &y);
	r = glm::vec2(x, h - y);
	return r;
}

void window::mouse(const char* a) {
	if(!strcmp(a, "show"))
		glfwSetInputMode(win, GLFW_CURSOR_MODE, GLFW_CURSOR_NORMAL);
	else if(!strcmp(a, "hide"))
		glfwSetInputMode(win, GLFW_CURSOR_MODE, GLFW_CURSOR_HIDDEN);
	else if(!strcmp(a, "capture"))
		glfwSetInputMode(win, GLFW_CURSOR_MODE, GLFW_CURSOR_CAPTURED);
	else
		err("window", "mouse", "invalid option");
}

void window::mouse(glm::ivec2 a) {
	int w, h;
	glfwGetWindowSize(win, &w, &h);
	
	glfwSetMousePos(win, a.x, h - a.y);
}

bool window::button(const char* a) {
	return button(mouse_button_string_to_int(a));
}

bool window::button(short a) {
	if(a < 1 || a > 8)
		return false;
	
	return glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1 + a - 1) == GLFW_PRESS;
}

glm::vec2 window::scroll() {
	double x, y;
	glfwGetScrollOffset(win, &x, &y);
	return glm::vec2((float) x, (float) y);
}

bool window::key(const char* a) {
	if(!strcmp(a, "shift")) {
		if(glfwGetKey(win, keyboard_key_string_to_int("left shift")) || glfwGetKey(win, keyboard_key_string_to_int("right shift")))
			return true;
		return false;
	}
	if(!strcmp(a, "ctrl")) {
		if(glfwGetKey(win, keyboard_key_string_to_int("left ctrl")) || glfwGetKey(win, keyboard_key_string_to_int("right ctrl")))
			return true;
		return false;
	}
	if(!strcmp(a, "alt")) {
		if(glfwGetKey(win, keyboard_key_string_to_int("left alt")) || glfwGetKey(win, keyboard_key_string_to_int("right alt")))
			return true;
		return false;
	}
	if(!strcmp(a, "super")) {
		if(glfwGetKey(win, keyboard_key_string_to_int("left super")) || glfwGetKey(win, keyboard_key_string_to_int("right super")))
			return true;
		return false;
	}
	
	if(glfwGetKey(win, keyboard_key_string_to_int(a)))
		return true;
	return false;
}

bool window::key_state(const char* a) {
	//if(glfwGetKeyState(win, keyboard_key_string_to_int(a)))
	//	return true;
	return false;
}

glm::vec4 window::pixel(glm::ivec2 a, const char* f) {
	glfwMakeContextCurrent(win);
	
	int w, h;
	glfwGetWindowSize(win, &w, &h);
	
	if(a.x < 0 || a.x >= w || a.y < 0 || a.y >= h)
		return glm::vec4(0.0, 0.0, 0.0, 1.0);
	
	GLenum fmt = GL_RGB;
	if(w) {
		if(!strcmp(f, "depth"))
			fmt = GL_DEPTH_COMPONENT;
		else if(!strcmp(f, "stencil"))
			fmt = GL_STENCIL_INDEX;
	}
	
	GLfloat c[3];
	glReadPixels(a.x, a.y, 1, 1, fmt, GL_FLOAT, c);
	return glm::vec4(c[0], c[1], c[2], 1.0);
}

bool window::screenshot(const char* a, const char* b) {
	glfwMakeContextCurrent(win);
	
	int w, h;
	glfwGetWindowSize(win, &w, &h);
	
	pixelcache p(glm::ivec2(w, h));
	
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, p.data);
	
	if (p.save(a, b))
		return true;
	err("screenshot", "could not save");
	return false;
}

#ifdef WITH_PYTHON
void window::callback(const char* e, PyObject* o) {
/*$ CALLBACK $*/
	${'else ' if I > 0 else ''}if(!strcmp(e, "${n}")) {
		if(_${n}_callback)
			Py_DECREF(_${n}_callback);
		_${n}_callback = o;
		Py_INCREF(o);
	}
/*$ $*/
	else
		err("window", "callback", "not understood");
}
#endif
