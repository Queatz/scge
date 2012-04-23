window::window(const char* title, int x, int y, bool fullscreen, bool resizeable, int fsaa) : win(NULL) {
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
	
	if(!y)
		y = x;
	
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

	win = glfwOpenWindow(x, y, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOWED), title, NULL);
	
	if(!win) {
		err("window", "could not initiate window");
		return;
	}
	
	glfw_window = win;

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

void window::size(int w, int h) {
	glfwSetWindowSize(win, w, h);
}

void window::position(int x, int y) {
	glfwSetWindowPos(win, x, y);
}

glm::ivec2 window::position() {
	glm::ivec2 w;
	glfwGetWindowPos(win, &w.x, &w.y);
	return w;
}

