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

int mouse_button_string_to_int(const char* a) {
	if(!strcmp(a, "left"))
		return GLFW_MOUSE_BUTTON_LEFT + 1;
	if(!strcmp(a, "middle"))
		return GLFW_MOUSE_BUTTON_MIDDLE + 1;
	if(!strcmp(a, "right"))
		return GLFW_MOUSE_BUTTON_RIGHT + 1;
	return -1;
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

void window::mouse(int a, int b) {
	int w, h;
	glfwGetWindowSize(win, &w, &h);
	
	glfwSetMousePos(win, a, h - b);
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
