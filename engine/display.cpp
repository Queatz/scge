GLFWwindow glfw_window = NULL;

Shikoba::Library * font_library = NULL;

int width = 0, height = 0, glfw_state = 0;
bool fullscreened;

void setdown_font() {
	delete font_library;
}

void setup_font() {
	font_library = new Shikoba::Library();
	atexit(setdown_font);
}

void glfw_error(int code, const char* string) {
}

void _size_callback_default(GLFWwindow w, int x, int y) {
	width = x;
	height = y;
}

char key_int_to_char(int a) {
	if(a < 256) return (char) a;
	return NULL;
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

/* * General Functions
graphics()
Initiate the context.  Returns true if it could happen, otherwise false.
#window calls this function if you have not.  It is mainly used when you need preperations before opening the window.

C++
graphics();

Python
graphics()

see:window, key, button
* */

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

/* *
graphics_off()
Ends graphic abilities.

C++
graphics_off();

Python
graphics_off()
* */
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



/* *
key_repeat(bool)
Sets whether or not to repeat key presses.  Normally they do not.

C++
key_repeat();

Python
key_repeat()
* */
void key_repeat(bool a) {
	if(a)
		glfwEnable(glfw_window, GLFW_KEY_REPEAT);
	else
		glfwDisable(glfw_window, GLFW_KEY_REPEAT);
}

/* *
window(string "title", int w, int h, bool fullscreen = false, bool resizable = False, int fsaa = 0)
Open a window, optionally fullscreen. Returns true if the window was able to be opened, else false.
#window sets up a default matrix with 0, 0 in the bottom-left.  If you want a different one you can call reset_matrix first.

C++
window("Window", 320, 240);

window("Window", 320, 240, true); //fullscreen

Python
window("Window", 320, 240)

window("Window", 320, 240, True) //fullscreen

see:swap, poll
* */
bool window(const char* title, int x, int y, bool fullscreen, bool resizeable, int fsaa) {
	if(glfw_state == 0)
		graphics();
	
	if(!x || !y) {
		GLFWvidmode d;
		glfwGetDesktopMode(&d);
		x = d.width;
		y = d.height;
	}
	
	if(fsaa)
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, fsaa);
	
	glfwOpenWindowHint(GLFW_WINDOW_RESIZABLE, resizeable ? GL_TRUE : GL_FALSE);
	
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfw_window = glfwOpenWindow(x, y, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOWED), title, NULL);
	if(!glfw_window) {
		err("window", "could not initiate window");
		return false;
	}
	
	glfw_state = 2;
	
	//GLenum err = glewInit();
	//if(err != GLEW_OK)
	//	note("window", "extentions unsupported");
	
	glfwSwapInterval(0);
	
	//glfwGetWindowSize(glfw_window, &width, &height);
	width = x;
	height = y;
	
	glfwSetWindowSizeCallback(_size_callback_default);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0, 0, width, height);
	
	fullscreened = fullscreen;
	return true;
}

/* *
close_window()
Close the window.

C++
close_window();

Python
close_window()

see:window
* */
void close_window() {
	glfwCloseWindow(glfw_window);
	glfw_state = 1;
}

/* *
window_title(string)
Set the title of the window.

C++
window_title("Hello World");

Python
window_title('Hello World')
* */
void window_title(const char* a) {
	glfwSetWindowTitle(glfw_window, a);
}

/* *
window_opened()
Returns true if the window is still open.

C++
bool a = window_opened();

Python
a = window_opened()
* */
bool window_opened() {
	if(glfwIsWindow(glfw_window))
		return true;
	return false;
}

/* *
window_active()
Returns true if the window has focus.

C++
bool a = window_active();

Python
a = window_active()
* */
bool window_active() {
	if(glfwGetWindowParam(glfw_window, GLFW_ACTIVE))
		return true;
	return false;
}

/* *
window_timer()
Returns a float of the time since the window was opened.

C++
float a = window_timer();

Python
a = window_timer()
* */
float window_timer() {
	return glfwGetTime();
}

/* *
display_modes()
!- return type to be changed -
Returns a space-seperated string of possible video modes for the display, in the format: WxH.

C++
std::string s = display_modes();

Python
s = display_modes()
* */
std::string display_modes() {
	if(glfw_state == 0)
		graphics();
	
	GLFWvidmode m[128] = {NULL};
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

/* *
display_dimensions()
Returns an ivec2 filled out with the resolution of the display.

C++
glm::ivec2 a = display_dimentions();

Python
a = display_dimentions()
* */
glm::ivec2 display_dimensions() {
	if(glfw_state == 0)
		graphics();
	
	GLFWvidmode a;
	glfwGetDesktopMode(&a);
	
	return glm::ivec2(a.width, a.height);
}

/* *
window_dimensions()
Returns an ivec2 filled out with the resolution of the window.

C++
glm::ivec2 a = window_dimentions();

Python
a = window_dimentions()
* */
glm::ivec2 window_dimensions() {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	
	return glm::ivec2(w, h);
}

/* *
position_window(int x, int y)
Move the window around.

C++
position_window(12, 12);

Python
position_window(12, 12)
* */
void position_window(int x, int y) {
	glfwSetWindowPos(glfw_window, x, y);
}

/* *
window_position()
Get the position of the window as an ivec2.

C++
glm::ivec2 a = window_position();

Python
a = window_position()
* */
glm::ivec2 window_position() {
	glm::ivec2 w;
	glfwGetWindowPos(glfw_window, &w.x, &w.y);
	return w;
}

/* *
vsync(bool)
Sync the display refresh to the monitor refresh rate, which is usually around 60, or not.

C++
vsync(false); //framerate unlimited
vsync(); //framerate 60

Python
vsync(False) #framerate unlimited
vsync(); #framerate 60
* */
void vsync(bool a) {	
	glfwSwapInterval(a);
}

/* *
swap()
Show all the drawn things on the screen.

C++
swap();

Python
swap()

see:poll
* */
void swap() {
	glfwSwapBuffers();
}

/* *
poll()
Update the state of the keyboard, mouse, and window.

C++
poll();

Python
poll()

see:swap
* */
void poll() {
	glfwPollEvents();
}

/* *
screenshot(string, type)
Save a screenshot.

C++
screenshot("outcome.png");

Python
screenshot('outcome.png')
* */
bool screenshot(const char* a, const char* b) {
	pixelcache p(width, height);
	
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, p.data);
	
	if (p.save(a, b))
		return true;
	err("screenshot", "could not save");
	return false;

}

/* *
viewport(int x, int y, int w, int h)
Set the screen viewport size.

C++
viewport(0, 0, 320, 240);

Python
viewport(0, 0, 320, 240)
* */
void viewport(int x, int y, int w, int h) {
	glViewport(x, y, w, h);
}

/* *
polygon_mode(string)
Set the draw type of polygons; either
"dot",
"outline", or
"fill", the default.

C++
polygon_mode("outline");

Python
polygon_mode('outline')
* */
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

/* *
enable(string, bool = true)
Enable or disable different things:
"texture" - use texure when drawing
"scissor" - scissor testing
"polygon smooth" - draw polygons smoothly
"line smooth" - draw lines smoothly
"blend" - blending, such as with alpha
"depth" - depth testing
"cull" - cull
"stencil" - stencil testing
"polygon depth offset" - offsetting the depth values of polygons
"line depth offset" - offsetting the depth values of lines
"point depth offset" - offsetting the depth values of points

C++
enable("texture");

enable("texture", false);

Python
enable('texture')

enable('texture', False)
* */
void enable(const char* a, bool b) {
	GLint c = -1;
	
	if (!strcmp(a, "texture"))
		c = GL_TEXTURE_2D;
	else if (!strcmp(a, "scissor"))
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
	
	if(c > -1) {
		if(b) glEnable(c);
		else glDisable(c);
	}
	else
		err("enable", "invalid option");
}

/* *
cull(string)
Set the faces to cull:
"back" - cull faces that face away
"front" - cull faces that face the camera
"all" - cull all faces

C++
cull("back");

Python
cull('back')

* */
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

/* *
clear_color(float, float, float, float = 1.0)
Set the color used by clear().

C++
clear_color(1.0, 1.0, 1.0);

Python
clear_color(1.0, 1.0, 1.0)

see: clear
* */
void clear_color(float a, float b, float c, float d) {
	glClearColor(a, b, c, d);
}

/* *
clear()
Clear the window.

C++
clear();

Python
clear()

see: clear_color
* */
void clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

/* *
depth_clear()
Clear the depth buffer.

C++
depth_clear();

Python
depth_clear()

see: enable
* */
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
	
	return NULL;
}

/* *
depth_test(const char* = "<")
How to compare depth values to decide if the fragment should be discarded.

C++
depth_test();

Python
depth_test()

see: enable
* */
void depth_test(const char* a) {
	glDepthFunc(comparison_string_to_gl(a));
}

/* *
depth_op(bool)
Disable or enable writing to the depth buffer.

C++
depth_op(false);

Python
depth_op(False) 
* */
void depth_op(bool a) {
	glDepthMask(a ? GL_TRUE : GL_FALSE);
}

/* *
point_size(float)
Set the size of points are to be drawn with.

C++
point_size(2.0);

Python
point_size(2.0)

see:point
* */
void  point_size(float a) {
	glPointSize(a);
}

/* *
line_width(float)
Set the width lines ought to be drawn with.

C++
line_width(0.5);

Python
line_width(.5)

see:line
* */
void  line_width(float a) {
	glLineWidth(a);
}

GLubyte stipple_none[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

GLubyte stipple_halftone[] = {
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55};

GLubyte stipple_quarttone[] = {
0x88,0x88,0x88,0x88,0x00,0x00,0x00,0x00,
0x22,0x22,0x22,0x22,0x00,0x00,0x00,0x00,
0x88,0x88,0x88,0x88,0x00,0x00,0x00,0x00,
0x22,0x22,0x22,0x22,0x00,0x00,0x00,0x00,
0x88,0x88,0x88,0x88,0x00,0x00,0x00,0x00,
0x22,0x22,0x22,0x22,0x00,0x00,0x00,0x00,
0x88,0x88,0x88,0x88,0x00,0x00,0x00,0x00,
0x22,0x22,0x22,0x22,0x00,0x00,0x00,0x00,
0x88,0x88,0x88,0x88,0x00,0x00,0x00,0x00,
0x22,0x22,0x22,0x22,0x00,0x00,0x00,0x00,
0x88,0x88,0x88,0x88,0x00,0x00,0x00,0x00,
0x22,0x22,0x22,0x22,0x00,0x00,0x00,0x00,
0x88,0x88,0x88,0x88,0x00,0x00,0x00,0x00,
0x22,0x22,0x22,0x22,0x00,0x00,0x00,0x00,
0x88,0x88,0x88,0x88,0x00,0x00,0x00,0x00,
0x22,0x22,0x22,0x22,0x00,0x00,0x00,0x00};

GLubyte stipple_screen[] = {
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00,
0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00};

GLubyte stipple_horizontal[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};

GLubyte stipple_vertical[] = {
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

GLubyte stipple_diagonal1[] = {
0x77, 0x77, 0x77, 0x77,
0xBB, 0xBB, 0xBB, 0xBB,
0xDD, 0xDD, 0xDD, 0xDD,
0xEE, 0xEE, 0xEE, 0xEE,
0x77, 0x77, 0x77, 0x77,
0xBB, 0xBB, 0xBB, 0xBB,
0xDD, 0xDD, 0xDD, 0xDD,
0xEE, 0xEE, 0xEE, 0xEE,
0x77, 0x77, 0x77, 0x77,
0xBB, 0xBB, 0xBB, 0xBB,
0xDD, 0xDD, 0xDD, 0xDD,
0xEE, 0xEE, 0xEE, 0xEE,
0x77, 0x77, 0x77, 0x77,
0xBB, 0xBB, 0xBB, 0xBB,
0xDD, 0xDD, 0xDD, 0xDD,
0xEE, 0xEE, 0xEE, 0xEE,
0x77, 0x77, 0x77, 0x77,
0xBB, 0xBB, 0xBB, 0xBB,
0xDD, 0xDD, 0xDD, 0xDD,
0xEE, 0xEE, 0xEE, 0xEE,
0x77, 0x77, 0x77, 0x77,
0xBB, 0xBB, 0xBB, 0xBB,
0xDD, 0xDD, 0xDD, 0xDD,
0xEE, 0xEE, 0xEE, 0xEE,
0x77, 0x77, 0x77, 0x77,
0xBB, 0xBB, 0xBB, 0xBB,
0xDD, 0xDD, 0xDD, 0xDD,
0xEE, 0xEE, 0xEE, 0xEE,
0x77, 0x77, 0x77, 0x77,
0xBB, 0xBB, 0xBB, 0xBB,
0xDD, 0xDD, 0xDD, 0xDD,
0xEE, 0xEE, 0xEE, 0xEE};

GLubyte stipple_diagonal2[] = {
0xEE, 0xEE, 0xEE, 0xEE,
0xDD, 0xDD, 0xDD, 0xDD,
0xBB, 0xBB, 0xBB, 0xBB,
0x77, 0x77, 0x77, 0x77,
0xEE, 0xEE, 0xEE, 0xEE,
0xDD, 0xDD, 0xDD, 0xDD,
0xBB, 0xBB, 0xBB, 0xBB,
0x77, 0x77, 0x77, 0x77,
0xEE, 0xEE, 0xEE, 0xEE,
0xDD, 0xDD, 0xDD, 0xDD,
0xBB, 0xBB, 0xBB, 0xBB,
0x77, 0x77, 0x77, 0x77,
0xEE, 0xEE, 0xEE, 0xEE,
0xDD, 0xDD, 0xDD, 0xDD,
0xBB, 0xBB, 0xBB, 0xBB,
0x77, 0x77, 0x77, 0x77,
0xEE, 0xEE, 0xEE, 0xEE,
0xDD, 0xDD, 0xDD, 0xDD,
0xBB, 0xBB, 0xBB, 0xBB,
0x77, 0x77, 0x77, 0x77,
0xEE, 0xEE, 0xEE, 0xEE,
0xDD, 0xDD, 0xDD, 0xDD,
0xBB, 0xBB, 0xBB, 0xBB,
0x77, 0x77, 0x77, 0x77,
0xEE, 0xEE, 0xEE, 0xEE,
0xDD, 0xDD, 0xDD, 0xDD,
0xBB, 0xBB, 0xBB, 0xBB,
0x77, 0x77, 0x77, 0x77,
0xEE, 0xEE, 0xEE, 0xEE,
0xDD, 0xDD, 0xDD, 0xDD,
0xBB, 0xBB, 0xBB, 0xBB,
0x77, 0x77, 0x77, 0x77};

GLubyte stipple_diamond1[] = {
0xdd,0xdd,0xdd,0xdd,0xaa,0xaa,0xaa,0xaa,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xaa,0xaa,0xaa,0xaa,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xaa,0xaa,0xaa,0xaa,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xaa,0xaa,0xaa,0xaa,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xaa,0xaa,0xaa,0xaa,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xaa,0xaa,0xaa,0xaa,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xaa,0xaa,0xaa,0xaa,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xaa,0xaa,0xaa,0xaa,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa};

GLubyte stipple_diamond2[] = {
0xbb,0xbb,0xbb,0xbb,0xee,0xee,0xee,0xee,
0xbb,0xbb,0xbb,0xbb,0x55,0x55,0x55,0x55,
0xbb,0xbb,0xbb,0xbb,0xee,0xee,0xee,0xee,
0xbb,0xbb,0xbb,0xbb,0x55,0x55,0x55,0x55,
0xbb,0xbb,0xbb,0xbb,0xee,0xee,0xee,0xee,
0xbb,0xbb,0xbb,0xbb,0x55,0x55,0x55,0x55,
0xbb,0xbb,0xbb,0xbb,0xee,0xee,0xee,0xee,
0xbb,0xbb,0xbb,0xbb,0x55,0x55,0x55,0x55,
0xbb,0xbb,0xbb,0xbb,0xee,0xee,0xee,0xee,
0xbb,0xbb,0xbb,0xbb,0x55,0x55,0x55,0x55,
0xbb,0xbb,0xbb,0xbb,0xee,0xee,0xee,0xee,
0xbb,0xbb,0xbb,0xbb,0x55,0x55,0x55,0x55,
0xbb,0xbb,0xbb,0xbb,0xee,0xee,0xee,0xee,
0xbb,0xbb,0xbb,0xbb,0x55,0x55,0x55,0x55,
0xbb,0xbb,0xbb,0xbb,0xee,0xee,0xee,0xee,
0xbb,0xbb,0xbb,0xbb,0x55,0x55,0x55,0x55};

GLubyte stipple_diamond3[] = {
0x5b,0x5b,0x5b,0x5b,0xbf,0xbf,0xbf,0xbf,
0xee,0xee,0xee,0xee,0xfb,0xfb,0xfb,0xfb,
0xb5,0xb5,0xb5,0xb5,0xfb,0xfb,0xfb,0xfb,
0xee,0xee,0xee,0xee,0xbf,0xbf,0xbf,0xbf,
0x5b,0x5b,0x5b,0x5b,0xbf,0xbf,0xbf,0xbf,
0xee,0xee,0xee,0xee,0xfb,0xfb,0xfb,0xfb,
0xb5,0xb5,0xb5,0xb5,0xfb,0xfb,0xfb,0xfb,
0xee,0xee,0xee,0xee,0xbf,0xbf,0xbf,0xbf,
0x5b,0x5b,0x5b,0x5b,0xbf,0xbf,0xbf,0xbf,
0xee,0xee,0xee,0xee,0xfb,0xfb,0xfb,0xfb,
0xb5,0xb5,0xb5,0xb5,0xfb,0xfb,0xfb,0xfb,
0xee,0xee,0xee,0xee,0xbf,0xbf,0xbf,0xbf,
0x5b,0x5b,0x5b,0x5b,0xbf,0xbf,0xbf,0xbf,
0xee,0xee,0xee,0xee,0xfb,0xfb,0xfb,0xfb,
0xb5,0xb5,0xb5,0xb5,0xfb,0xfb,0xfb,0xfb,
0xee,0xee,0xee,0xee,0xbf,0xbf,0xbf,0xbf};

GLubyte stipple_diamond4[] = {
0x52,0x52,0x52,0x52,0xa8,0xa8,0xa8,0xa8,
0x75,0x75,0x75,0x75,0xa8,0xa8,0xa8,0xa8,
0x52,0x52,0x52,0x52,0x27,0x27,0x27,0x27,
0x8f,0x8f,0x8f,0x8f,0x27,0x27,0x27,0x27,
0x52,0x52,0x52,0x52,0xa8,0xa8,0xa8,0xa8,
0x75,0x75,0x75,0x75,0xa8,0xa8,0xa8,0xa8,
0x52,0x52,0x52,0x52,0x27,0x27,0x27,0x27,
0x8f,0x8f,0x8f,0x8f,0x27,0x27,0x27,0x27,
0x52,0x52,0x52,0x52,0xa8,0xa8,0xa8,0xa8,
0x75,0x75,0x75,0x75,0xa8,0xa8,0xa8,0xa8,
0x52,0x52,0x52,0x52,0x27,0x27,0x27,0x27,
0x8f,0x8f,0x8f,0x8f,0x27,0x27,0x27,0x27,
0x52,0x52,0x52,0x52,0xa8,0xa8,0xa8,0xa8,
0x75,0x75,0x75,0x75,0xa8,0xa8,0xa8,0xa8,
0x52,0x52,0x52,0x52,0x27,0x27,0x27,0x27,
0x8f,0x8f,0x8f,0x8f,0x27,0x27,0x27,0x27};

GLubyte stipple_bricks[] = {
0x00,0x00,0x00,0x00,0x7f,0x7f,0x7f,0x7f,
0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
0x00,0x00,0x00,0x00,0xf7,0xf7,0xf7,0xf7,
0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,
0x00,0x00,0x00,0x00,0x7f,0x7f,0x7f,0x7f,
0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
0x00,0x00,0x00,0x00,0xf7,0xf7,0xf7,0xf7,
0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,
0x00,0x00,0x00,0x00,0x7f,0x7f,0x7f,0x7f,
0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
0x00,0x00,0x00,0x00,0xf7,0xf7,0xf7,0xf7,
0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,
0x00,0x00,0x00,0x00,0x7f,0x7f,0x7f,0x7f,
0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
0x00,0x00,0x00,0x00,0xf7,0xf7,0xf7,0xf7,
0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7,0xf7};

GLubyte stipple_bricks2[] = {
0xaa,0xaa,0xaa,0xaa,0x7f,0x7f,0x7f,0x7f,
0xff,0xff,0xff,0xff,0x7f,0x7f,0x7f,0x7f,
0xaa,0xaa,0xaa,0xaa,0xf7,0xf7,0xf7,0xf7,
0xff,0xff,0xff,0xff,0xf7,0xf7,0xf7,0xf7,
0xaa,0xaa,0xaa,0xaa,0x7f,0x7f,0x7f,0x7f,
0xff,0xff,0xff,0xff,0x7f,0x7f,0x7f,0x7f,
0xaa,0xaa,0xaa,0xaa,0xf7,0xf7,0xf7,0xf7,
0xff,0xff,0xff,0xff,0xf7,0xf7,0xf7,0xf7,
0xaa,0xaa,0xaa,0xaa,0x7f,0x7f,0x7f,0x7f,
0xff,0xff,0xff,0xff,0x7f,0x7f,0x7f,0x7f,
0xaa,0xaa,0xaa,0xaa,0xf7,0xf7,0xf7,0xf7,
0xff,0xff,0xff,0xff,0xf7,0xf7,0xf7,0xf7,
0xaa,0xaa,0xaa,0xaa,0x7f,0x7f,0x7f,0x7f,
0xff,0xff,0xff,0xff,0x7f,0x7f,0x7f,0x7f,
0xaa,0xaa,0xaa,0xaa,0xf7,0xf7,0xf7,0xf7,
0xff,0xff,0xff,0xff,0xf7,0xf7,0xf7,0xf7};

GLubyte stipple_pattern1[] = {
0xfd,0xfd,0xfd,0xfd,0xaf,0xaf,0xaf,0xaf,
0x57,0x57,0x57,0x57,0xad,0xad,0xad,0xad,
0x57,0x57,0x57,0x57,0xaf,0xaf,0xaf,0xaf,
0xfd,0xfd,0xfd,0xfd,0xda,0xda,0xda,0xda,
0xfd,0xfd,0xfd,0xfd,0xaf,0xaf,0xaf,0xaf,
0x57,0x57,0x57,0x57,0xad,0xad,0xad,0xad,
0x57,0x57,0x57,0x57,0xaf,0xaf,0xaf,0xaf,
0xfd,0xfd,0xfd,0xfd,0xda,0xda,0xda,0xda,
0xfd,0xfd,0xfd,0xfd,0xaf,0xaf,0xaf,0xaf,
0x57,0x57,0x57,0x57,0xad,0xad,0xad,0xad,
0x57,0x57,0x57,0x57,0xaf,0xaf,0xaf,0xaf,
0xfd,0xfd,0xfd,0xfd,0xda,0xda,0xda,0xda,
0xfd,0xfd,0xfd,0xfd,0xaf,0xaf,0xaf,0xaf,
0x57,0x57,0x57,0x57,0xad,0xad,0xad,0xad,
0x57,0x57,0x57,0x57,0xaf,0xaf,0xaf,0xaf,
0xfd,0xfd,0xfd,0xfd,0xda,0xda,0xda,0xda};

GLubyte stipple_pattern2[] = {
0xfe,0xfe,0xfe,0xfe,0x82,0x82,0x82,0x82,
0xba,0xba,0xba,0xba,0xaa,0xaa,0xaa,0xaa,
0xba,0xba,0xba,0xba,0x82,0x82,0x82,0x82,
0xfe,0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00,
0xfe,0xfe,0xfe,0xfe,0x82,0x82,0x82,0x82,
0xba,0xba,0xba,0xba,0xaa,0xaa,0xaa,0xaa,
0xba,0xba,0xba,0xba,0x82,0x82,0x82,0x82,
0xfe,0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00,
0xfe,0xfe,0xfe,0xfe,0x82,0x82,0x82,0x82,
0xba,0xba,0xba,0xba,0xaa,0xaa,0xaa,0xaa,
0xba,0xba,0xba,0xba,0x82,0x82,0x82,0x82,
0xfe,0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00,
0xfe,0xfe,0xfe,0xfe,0x82,0x82,0x82,0x82,
0xba,0xba,0xba,0xba,0xaa,0xaa,0xaa,0xaa,
0xba,0xba,0xba,0xba,0x82,0x82,0x82,0x82,
0xfe,0xfe,0xfe,0xfe,0x00,0x00,0x00,0x00};

GLubyte stipple_pattern3[] = {
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xe0,0xe0,0xe0,0xe0,0x1c,0x1c,0x1c,0x1c,
0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
0xc1,0xc1,0xc1,0xc1,0x0e,0x0e,0x0e,0x0e,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xe0,0xe0,0xe0,0xe0,0x1c,0x1c,0x1c,0x1c,
0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
0xc1,0xc1,0xc1,0xc1,0x0e,0x0e,0x0e,0x0e,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xe0,0xe0,0xe0,0xe0,0x1c,0x1c,0x1c,0x1c,
0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
0xc1,0xc1,0xc1,0xc1,0x0e,0x0e,0x0e,0x0e,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xe0,0xe0,0xe0,0xe0,0x1c,0x1c,0x1c,0x1c,
0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
0xc1,0xc1,0xc1,0xc1,0x0e,0x0e,0x0e,0x0e};

GLubyte stipple_pattern4[] = {
0xea,0xea,0xea,0xea,0xae,0xae,0xae,0xae,
0xc0,0xc0,0xc0,0xc0,0x0c,0x0c,0x0c,0x0c,
0xd5,0xd5,0xd5,0xd5,0x5d,0x5d,0x5d,0x5d,
0xc0,0xc0,0xc0,0xc0,0x0c,0x0c,0x0c,0x0c,
0xea,0xea,0xea,0xea,0xae,0xae,0xae,0xae,
0xc0,0xc0,0xc0,0xc0,0x0c,0x0c,0x0c,0x0c,
0xd5,0xd5,0xd5,0xd5,0x5d,0x5d,0x5d,0x5d,
0xc0,0xc0,0xc0,0xc0,0x0c,0x0c,0x0c,0x0c,
0xea,0xea,0xea,0xea,0xae,0xae,0xae,0xae,
0xc0,0xc0,0xc0,0xc0,0x0c,0x0c,0x0c,0x0c,
0xd5,0xd5,0xd5,0xd5,0x5d,0x5d,0x5d,0x5d,
0xc0,0xc0,0xc0,0xc0,0x0c,0x0c,0x0c,0x0c,
0xea,0xea,0xea,0xea,0xae,0xae,0xae,0xae,
0xc0,0xc0,0xc0,0xc0,0x0c,0x0c,0x0c,0x0c,
0xd5,0xd5,0xd5,0xd5,0x5d,0x5d,0x5d,0x5d,
0xc0,0xc0,0xc0,0xc0,0x0c,0x0c,0x0c,0x0c};

GLubyte stipple_pattern5[] = {
0x9c,0x9c,0x9c,0x9c,0x22,0x22,0x22,0x22,
0x49,0x49,0x49,0x49,0x55,0x55,0x55,0x55,
0x49,0x49,0x49,0x49,0x22,0x22,0x22,0x22,
0x9c,0x9c,0x9c,0x9c,0x41,0x41,0x41,0x41,
0x9c,0x9c,0x9c,0x9c,0x22,0x22,0x22,0x22,
0x49,0x49,0x49,0x49,0x55,0x55,0x55,0x55,
0x49,0x49,0x49,0x49,0x22,0x22,0x22,0x22,
0x9c,0x9c,0x9c,0x9c,0x41,0x41,0x41,0x41,
0x9c,0x9c,0x9c,0x9c,0x22,0x22,0x22,0x22,
0x49,0x49,0x49,0x49,0x55,0x55,0x55,0x55,
0x49,0x49,0x49,0x49,0x22,0x22,0x22,0x22,
0x9c,0x9c,0x9c,0x9c,0x41,0x41,0x41,0x41,
0x9c,0x9c,0x9c,0x9c,0x22,0x22,0x22,0x22,
0x49,0x49,0x49,0x49,0x55,0x55,0x55,0x55,
0x49,0x49,0x49,0x49,0x22,0x22,0x22,0x22,
0x9c,0x9c,0x9c,0x9c,0x41,0x41,0x41,0x41};

GLubyte stipple_checkerboard1[] = {
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc};

GLubyte stipple_checkerboard2[] = {
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f};

GLubyte stipple_checkerboard3[] = {
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff};

GLubyte stipple_checkerboard4[] = {
0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,
0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,
0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,
0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,
0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,
0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,
0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,
0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff};

GLubyte stipple_tiles[] = {
0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x77,
0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x77,
0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x77,
0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x77,
0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x77,
0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x77,
0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x77,
0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x77,
0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77};

GLubyte stipple_zigzag1[] = {
0x77,0x77,0x77,0x77,0xbb,0xbb,0xbb,0xbb,
0xdd,0xdd,0xdd,0xdd,0xbb,0xbb,0xbb,0xbb,
0x77,0x77,0x77,0x77,0xbb,0xbb,0xbb,0xbb,
0xdd,0xdd,0xdd,0xdd,0xbb,0xbb,0xbb,0xbb,
0x77,0x77,0x77,0x77,0xbb,0xbb,0xbb,0xbb,
0xdd,0xdd,0xdd,0xdd,0xbb,0xbb,0xbb,0xbb,
0x77,0x77,0x77,0x77,0xbb,0xbb,0xbb,0xbb,
0xdd,0xdd,0xdd,0xdd,0xbb,0xbb,0xbb,0xbb,
0x77,0x77,0x77,0x77,0xbb,0xbb,0xbb,0xbb,
0xdd,0xdd,0xdd,0xdd,0xbb,0xbb,0xbb,0xbb,
0x77,0x77,0x77,0x77,0xbb,0xbb,0xbb,0xbb,
0xdd,0xdd,0xdd,0xdd,0xbb,0xbb,0xbb,0xbb,
0x77,0x77,0x77,0x77,0xbb,0xbb,0xbb,0xbb,
0xdd,0xdd,0xdd,0xdd,0xbb,0xbb,0xbb,0xbb,
0x77,0x77,0x77,0x77,0xbb,0xbb,0xbb,0xbb,
0xdd,0xdd,0xdd,0xdd,0xbb,0xbb,0xbb,0xbb};

GLubyte stipple_zigzag2[] = {
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xff,0xff,0xff,0xff,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xff,0xff,0xff,0xff,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xff,0xff,0xff,0xff,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xff,0xff,0xff,0xff,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xff,0xff,0xff,0xff,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xff,0xff,0xff,0xff,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xff,0xff,0xff,0xff,
0x77,0x77,0x77,0x77,0xaa,0xaa,0xaa,0xaa,
0xdd,0xdd,0xdd,0xdd,0xff,0xff,0xff,0xff};

GLubyte stipple_zigzag3[] = {
0x5f,0x5f,0x5f,0x5f,0xae,0xae,0xae,0xae,
0x55,0x55,0x55,0x55,0xea,0xea,0xea,0xea,
0xf5,0xf5,0xf5,0xf5,0xbb,0xbb,0xbb,0xbb,
0xee,0xee,0xee,0xee,0xbb,0xbb,0xbb,0xbb,
0x5f,0x5f,0x5f,0x5f,0xae,0xae,0xae,0xae,
0x55,0x55,0x55,0x55,0xea,0xea,0xea,0xea,
0xf5,0xf5,0xf5,0xf5,0xbb,0xbb,0xbb,0xbb,
0xee,0xee,0xee,0xee,0xbb,0xbb,0xbb,0xbb,
0x5f,0x5f,0x5f,0x5f,0xae,0xae,0xae,0xae,
0x55,0x55,0x55,0x55,0xea,0xea,0xea,0xea,
0xf5,0xf5,0xf5,0xf5,0xbb,0xbb,0xbb,0xbb,
0xee,0xee,0xee,0xee,0xbb,0xbb,0xbb,0xbb,
0x5f,0x5f,0x5f,0x5f,0xae,0xae,0xae,0xae,
0x55,0x55,0x55,0x55,0xea,0xea,0xea,0xea,
0xf5,0xf5,0xf5,0xf5,0xbb,0xbb,0xbb,0xbb,
0xee,0xee,0xee,0xee,0xbb,0xbb,0xbb,0xbb};

GLubyte stipple_shapes[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,
0x3f,0x81,0x40,0x00,0x3f,0x80,0x80,0x00,
0x31,0x85,0x50,0x10,0x31,0x8a,0xa8,0x28,
0x31,0x85,0x50,0x54,0x3f,0x80,0x80,0x8a,
0x3f,0x81,0x41,0xff,0x00,0x00,0x80,0x00,
0x00,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,
0x1e,0x00,0x00,0x00,0x35,0x00,0x00,0x00,
0x3d,0x00,0x00,0x00,0x21,0x00,0x00,0x00,
0x1e,0x00,0x00,0x00,0x00,0x01,0x02,0x00,
0x00,0x21,0x02,0x00,0x00,0x21,0x01,0x04,
0x10,0x21,0x01,0x04,0x10,0x23,0x01,0x0c,
0x10,0x22,0x01,0x08,0x08,0x1a,0x01,0x08,
0x08,0x1e,0x01,0x0c,0x3c,0x25,0x87,0x8f,
0x36,0x6f,0x85,0xcd,0xfb,0x8c,0xcd,0x6f,
0x98,0x42,0x7b,0x38,0x44,0x10,0x81,0x0c,
0x30,0x42,0x03,0x40,0x00,0x00,0x10,0x09};

GLubyte* polygon_stipple_from_name(const char* a) {
	if(!a)
		return NULL;
	else if(!strcmp(a, "halftone"))
		return stipple_halftone;
	else if(!strcmp(a, "quarttone"))
		return stipple_quarttone;
	else if(!strcmp(a, "screen"))
		return stipple_screen;
	else if(!strcmp(a, "horizontal"))
		return stipple_horizontal;
	else if(!strcmp(a, "vertical"))
		return stipple_vertical;
	else if(!strcmp(a, "diagonal1"))
		return stipple_diagonal1;
	else if(!strcmp(a, "diagonal2"))
		return stipple_diagonal2;
	else if(!strcmp(a, "diamond1"))
		return stipple_diamond1;
	else if(!strcmp(a, "diamond2"))
		return stipple_diamond2;
	else if(!strcmp(a, "diamond3"))
		return stipple_diamond3;
	else if(!strcmp(a, "diamond4"))
		return stipple_diamond4;
	else if(!strcmp(a, "bricks1"))
		return stipple_bricks;
	else if(!strcmp(a, "bricks2"))
		return stipple_bricks2;
	else if(!strcmp(a, "pattern1"))
		return stipple_pattern1;
	else if(!strcmp(a, "pattern2"))
		return stipple_pattern2;
	else if(!strcmp(a, "pattern3"))
		return stipple_pattern3;
	else if(!strcmp(a, "pattern4"))
		return stipple_pattern4;
	else if(!strcmp(a, "pattern5"))
		return stipple_pattern5;
	else if(!strcmp(a, "checkerboard1"))
		return stipple_checkerboard1;
	else if(!strcmp(a, "checkerboard2"))
		return stipple_checkerboard2;
	else if(!strcmp(a, "checkerboard3"))
		return stipple_checkerboard3;
	else if(!strcmp(a, "checkerboard4"))
		return stipple_checkerboard4;
	else if(!strcmp(a, "tiles"))
		return stipple_tiles;
	else if(!strcmp(a, "zigzag1"))
		return stipple_zigzag1;
	else if(!strcmp(a, "zigzag2"))
		return stipple_zigzag2;
	else if(!strcmp(a, "zigzag3"))
		return stipple_zigzag3;
	else if(!strcmp(a, "shapes"))
		return stipple_shapes;
	else {
		err("polygon_stipple", "invalid pattern name");
		return NULL;
	}
}

GLubyte _stipple[128];

/* *
polygon_stipple(string, bool = false, string = "", int = 0, int = 0)
Set the pattern used for stippling polygons:
"halftone"
"quarttone"
"screen"
"horizontal"
"vertical"
"diagonal1"
"diagonal2"
"diamond1"
"diamond2"
"diamond3"
"diamond4"
"bricks1"
"bricks2"
"pattern1"
"pattern2"
"pattern3"
"pattern4"
"pattern5"
"checkerboard1"
"checkerboard2"
"checkerboard3"
"checkerboard4"
"tiles"
"zigzag1"
"zigzag2"
"zigzag3"
"shapes"

The bool inverts it.

The second string sets the interpolation to use, in which case the existing stipple will not be overwritten:
"mix"
"add"
"subtract"

The ints move the stipple.

C++
polygon_stipple("halftone");

Python
polygon_stipple('halftone')

see:enable
* */
/*void polygon_stipple(const char* a, bool inverse, const char* interpolation, int ox, int oy) {
	GLubyte* s;
	
	s = polygon_stipple_from_name(a);
	
	if(s) {
		if(!interpolation) {
			if(inverse)
				for(int i = 0; i < 128; i++)
					_stipple[i] = ~s[i];
			else
				memcpy(_stipple, s, 128);
		} else {
			if(!strcmp(interpolation, "mix"))
				for(int i = 0; i < 128; i++)
					_stipple[i] ^= !inverse ? s[i] : ~s[i];
			else if(!strcmp(interpolation, "add"))
				for(int i = 0; i < 128; i++)
					_stipple[i] |= !inverse ? s[i] : ~s[i];
			else if(!strcmp(interpolation, "subtract"))
				for(int i = 0; i < 128; i++)
					_stipple[i] &= !inverse ? ~s[i] : s[i];
			else
				err("polygon_stipple", "invalid interpolation");
		}
	}
	
	// Move
	
	ox=ox%32;if(ox<0)ox=32+ox;
	oy=oy%32;if(oy<0)oy=32+oy;
	
	// Move X
	
	if(ox != 0) {
		GLubyte temp[4];
		
		// Move the bytes
		if(ox/8 != 0)
			for(int i = 0; i < 128; i+=4) {
				memcpy(temp, _stipple+i, 4);
				for(int j = 0; j < 4; j++) {
					int t = (j-ox/8)%4;if(t<0)t=4+t;
					_stipple[i+j] = temp[t];
				}
			}
		
		if(ox%8 != 0) {
			// Shift the bits
			for(int i = 0; i < 128; i++)
				_stipple[i] = (_stipple[i] >> (ox%8)) | (_stipple[i] << (8-ox%8));
		
			// Put the remaining bits where they belong
		
			// Mask
			temp[1] = 0xFF >> ox%8;
		
			for(int i = 0; i < 128; i+=4) {
				temp[0] = _stipple[i+3];
				for(int j = 3; j > 0; j--) {
					int t = (j-1)%4;if(t<0)t=4+t;
					_stipple[i+j] &= temp[1];
					_stipple[i+j] |= _stipple[i+t] & ~temp[1]; 
				}
				_stipple[i] &= temp[1];
				_stipple[i] |= temp[0] & ~temp[1];
			}
		}
	}
	
	// Move Y
	
	if (oy != 0) {
		GLubyte temp[128];
		memcpy(temp, _stipple, 128);
		
		for(int i = 0; i < 32; i++) {
			int t = (i-oy)%32;if(t<0)t=32+t;
			memcpy(_stipple+i*4, temp+t*4, 4);
		}
	}
	
	// Update
	
	glPolygonStipple(_stipple);
}*/

/* *
polygon_depth_offset(float = 0.0, float = 0.0)
Set a offset in the depth buffer to be used for polygons.

C++
polygon_depth_offset()

Python
polygon_depth_offset()

see: depth_test
* */
void polygon_depth_offset(float a, float b) {
	glPolygonOffset(a, b);
}

/* *
blend_color(float, float, float, float = 1.0)
Set the blending color.
!The only use for this is with "alpha" texture environment.

C++
blend_color(1.0, 1.0, 1.0);

Python
blend_color(1.0, 1.0, 1.0)

see:blend_mode
* */
void blend_color(float r, float g, float b, float a) {
	glBlendColor(r, g, b, a);
}

/* *
color_mask(bool r, bool g, bool b, bool a)
Disable or enable colors.

C++
color_mask(false); // Disable color rendering

Python
color_mask(False) # Disable color rendering

see:blend_mode
* */
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

/* *
use_vbo(vbo)
Use a vbo.

C++
use_vbo(); //use no vbo

Python
use_vbo() #use no vbo
* */
void use_vbo(vbo* a) {
	glBindBuffer(GL_ARRAY_BUFFER, a->id);
}

void use_vbo() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* *
use_vao(vao)
Use a vao.

C++
use_vao(); //use no vao

Python
use_vao() #use no vao
* */
void use_vao(vao* a) {
	glBindVertexArray(a->id);
}

void use_vao() {
	glBindVertexArray(0);
}

/* *
use_ibo(ibo)
Use an ibo.

C++
use_ibo(); //use no ibo

Python
use_ibo() #use no ibo
* */
void use_ibo(ibo* a) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a->id);
}

void use_ibo() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/* *
scissor(int, int, int, int)
Crop drawing to a portion of the window, or not.

C++
scissor(0, 0, 10, 10);

Python
scissor(0, 0, 10, 10)
* */
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

/* *
blend_mode(string)
Set how you want to blend the images onto the window.
"color" - color blending without alpha
"mix" - normal alpha blending
"multipy" - multiplicative blending
"add" - additive blending
"subtract" - subtractive blending
"saturate"

C++
blend_mode("mix");

Python
blend_mode('mix')

see:blend_color
* */
void blend_mode(const char* a) {
	display_set_blend_mode_from_string(a);
}

/* *
use_program(program)
Draw using a program, or not.
When using a program, color and texturing and some other factors do not apply as they are definable in the shader.

C++
use_program(); //use no program

Python
use_program() #use no program
* */
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
		return NULL;
}

/* *
pixel(int x, int y, string = "")
Returns a vec4 containing the pixel information on the screen or as specified by the string.
"depth"
"stencil"
"color"

C++
glm::vec4 a();
a = pixel(0, 0);

Python
a = pixel(0, 0)
* */
glm::vec4 pixel(int x, int y, const char* w) {
	if(x < 0 || x >= width || y < 0 || y >= height)
		return glm::vec4(0.0, 0.0, 0.0, 1.0);
	
	GLenum fmt = GL_RGB;
	if(w) {
		if(!strcmp(w, "depth"))
			fmt = GL_DEPTH_COMPONENT;
		else if(!strcmp(w, "stencil"))
			fmt = GL_STENCIL_INDEX;
	}
	
	GLfloat c[3];
	glReadPixels(x, y, 1, 1, fmt, GL_FLOAT, c);
	return glm::vec4(c[0], c[1], c[2], 1.0);
}

/* *
stencil_clear(int)
Clears the stencil buffer.

C++
stencil_clear();

Python
stencil_clear()
* */
void stencil_clear(int a) {
	glClearStencil(a);
	glClear(GL_STENCIL_BUFFER_BIT);
}

/* *
stencil_test(string = "==", int = 1)
How to test if the stencil test passes or fails in relation to the reference value.
"=="
"!="
"<"
">"
"<="
">="
"always"
"never"

C++
stencil_test();

Python
stencil_test()
* */

void stencil_test(const char* a, int b) {
	glStencilFunc(comparison_string_to_gl(a), b, 1);
}

/* *
stencil_op(string = "keep")
How to change the stencil buffer during draw operations.
"keep"
"zero"
"replace"
"increment"
"decrement"

C++
stencil_op();

Python
stencil_op()
* */
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

/* *
mouse_position(bool topdown = false)
Returns a vec2 with the mouse position.

C++
glm::vec2 a();
a = mouse_position();

Python
a = mouse_position()

see:move_mouse
* */
glm::vec2 mouse_position(bool topdown) {
	int x, y;
	glm::vec2 r;
	glfwGetMousePos(glfw_window, &x, &y);
	if(!topdown)
		y = height - y;
	r = glm::vec2(x, y);
	return r;
}

/* *
mouse(string = "show")
Set the state of the mouse.
"show"
"hide"
"capture"

C++
mouse("hide"); //hide the pointer

Python
mouse('hide') #hide the pointer
* */
void mouse(const char* a) {
	if(!strcmp(a, "show"))
		glfwSetCursorMode(glfw_window, GLFW_CURSOR_NORMAL);
	else if(!strcmp(a, "hide"))
		glfwSetCursorMode(glfw_window, GLFW_CURSOR_HIDDEN);
	else if(!strcmp(a, "capture"))
		glfwSetCursorMode(glfw_window, GLFW_CURSOR_CAPTURED);
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

/* *
button(string); button(short)
Check if a mouse button is pressed.
1-8
"left"
"middle"
"right"


C++
button("left");

Python
button('left')
* */
bool button(const char* a) {
	return button(mouse_button_string_to_int(a));
}

bool button(short a) {
	if(a < 1 || a > 8)
		return false;
	
	return glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_1 + a - 1) == GLFW_PRESS;
}

/* *
wheel()
Get the wheel offset.

C++
int a = wheel();

Python
a = wheel()
* */
int wheel() {
	int x, y;
	glfwGetScrollOffset(glfw_window, &x, &y);
	return y;
}

/* *
move_mouse(int x, int y, bool topdown = false)
Move the mouse to some position.

C++
move_mouse(160, 120);

Python
move_mouse(160, 120)
* */
void move_mouse(int a, int b, bool topdown) {
	if(!topdown)
		b = height - b;
	glfwSetMousePos(glfw_window, a, b);
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

/* *
key(string)
Check if a key is pressed.

C++
bool a = key("left shift");

Python
a = key('left shift')
* */
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

/* *
key_state(string)
Check if a key is locked.

C++
bool a = key_state("caps lock");

Python
a = key_state('caps lock')
* */
bool key_state(const char* a) {
	//if(glfwGetKeyState(glfw_window, keyboard_key_string_to_int(a)))
	//	return true;
	return false;
}

/* * Resources
pixelcache
A cache of pixels.

	width int
		the width of the cache
	height int
		the height of the cache
	pixel(int x, int y)
		get a pixel as a vec4
	set_pixel(int, int, vec4)
		set a pixel in the cache
	save(string filename, [string type])
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
	data = new GLubyte[height*width*3];
}

pixelcache::~pixelcache() {
	if(data)
		delete data;
}

glm::vec4 pixelcache::pixel(int x, int y) {
	int h;
	
	if(x < 0 || x >= width || y < 0 || y >= height)
		return glm::vec4(0.0, 0.0, 0.0, 0.0);
	
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
	
	FIBITMAP* c = FreeImage_Allocate(width, height, 24);
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
		caches the image's current state
	discard_pixel_cache()
		discards the image's pixelcache
	pixel(int x, int y)
		returns a vec4 containing the pixel color of x and y
	save(string name, string format)
		save the images pixelcache to disk.  If format is omitted it is guessed from the filename.
		"png"
		"jpg"
		"bmp"

C++
image a("tux.png");
a.set("linear");
glm::vec4 b();
b = a.pixel(0, 0);

image c(64, 64, false); //creates a blank image sized 64 by 64, and not high quality

Python
a = image('tux.png')
a.set('linear')
b = a.pixel(0, 0)

c = image(64, 64, False); #creates a blank image sized 64 by 64, and not high quality

see:use_image
* */
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
	
	if(m)
		glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(bm), FreeImage_GetHeight(bm), 0, (has_alpha ? GL_BGRA : GL_BGR), GL_UNSIGNED_BYTE, bits);
	
	FreeImage_Unload(bm);
	
	//id = SOIL_load_OGL_texture(a, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | (m ? SOIL_FLAG_MIPMAPS : 0)); //NULL
	
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

/* *
fontface
A font face.

C++
fontface f("sans.ttf");

Python
f = fontface('sans.ttf')
* */
fontface::fontface(const char* a) {
	if(font_library == NULL)
		setup_font();
	
	data = new Shikoba::Face(font_library, a);
}

fontface::~fontface() {
	delete data;
}

/* *
font
A font.

	height()
		get the line height
	ascent()
		get the ascent of the font
	descent()
		get the decent of the font
	advance(string)
		get the advance of the string, where the next character would be placed

C++
fontface f("sans.ttf");
font b(&f, 16.0);

Python
f = fontface('sans.ttf')
a = font(f, 16)

* */
font::font(fontface* a, float b) {
	if(glfw_state == 0)
		graphics();
	
	data = new Shikoba::Font(a->data, b);
}

font::~font() {
	delete data;
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

float font::advance(const char* a) {
	return data->advance((const unsigned char*)a);
}

/* *
text
A string of text.

	lines()
		get number of lines
	advance()
		returns a float representing maximum advance of the lines
	draw(int, int)
		write the text using the given attributes for vertex positions and texture coordinates.

C++
// Write "Hello World." with 1.0 character spacing, 1.5 line spacing, centered, wrap at 100.0, and tabs will be 4 spaces.
text a(&font, "Hello World.", 1.0, 1.5, "center", 100.0, 4);

Python
// Write 'Hello World.' with 1 character spacing, 1.5 line spacing, centered, wrap at 100, and tabs will be 4 spaces.
a = text(font, 'Hello World.', 1, 1.5, 'center', 100, 4);

see: font
* */
int text_align_from_string(const char* a) {
	if(!strcmp("left", a))
		return Shikoba::LEFT;
	if(!strcmp("center", a))
		return Shikoba::CENTER;
	if(!strcmp("right", a))
		return Shikoba::RIGHT;
	if(!strcmp("justify", a))
		return Shikoba::JUSTIFY;
	if(!strcmp("justify left", a))
		return Shikoba::LEFT | Shikoba::JUSTIFY;
	if(!strcmp("justify center", a))
		return Shikoba::CENTER | Shikoba::JUSTIFY;
	if(!strcmp("justify right", a))
		return Shikoba::RIGHT | Shikoba::JUSTIFY;
	
	err("text", "align", "unknown method");
	return Shikoba::LEFT;
}

text::text(font* f, const char * s, float cw, float lh, const char* a, float mw, int tw) {
	data = new Shikoba::Text(f->data, (const unsigned char*)s, cw, lh, text_align_from_string(a), mw, tw);
}

text::~text() {
	delete data;
}

int text::lines() {
	return data->lines();
}

float text::advance() {
	data->advance();
}

void text::draw(int a, int b) {
	data->draw(a, b);
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
* */
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
}

shader::~shader() {
	if(id)
		glDeleteShader(id);
}

void shader::compile() {
	if(!id)
		return;
	
	glCompileShader(id);
	
	int compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	
	if(compiled != GL_TRUE) {
		GLchar infoLog[2056];
		int length;
	
		glGetShaderInfoLog(id, 2056, &length, infoLog);
	
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
	uniform(string, int|float|vec|mat|image)
		set a uniform on the program
	attribute(string, int)
		set a name for an attribute

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

void program::uniform(const char* a, glm::vec2 m) {
	glUniform2fv(glGetUniformLocation(id, a), 1, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::vec3 m) {
	glUniform3fv(glGetUniformLocation(id, a), 1, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::vec4 m) {
	glUniform4fv(glGetUniformLocation(id, a), 1, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat2 m) {
	glUniformMatrix2fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat3 m) {
	glUniformMatrix3fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat4 m) {
	glUniformMatrix4fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat2x3 m) {
	glUniformMatrix2x3fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat3x2 m) {
	glUniformMatrix3x2fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat2x4 m) {
	glUniformMatrix2x4fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat4x2 m) {
	glUniformMatrix4x2fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat3x4 m) {
	glUniformMatrix3x4fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, glm::mat4x3 m) {
	glUniformMatrix4x3fv(glGetUniformLocation(id, a), 1, GL_FALSE, glm::value_ptr(m));
}

void program::uniform(const char* a, image* c, int b) {
	GLint act;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_2D, c->id);
	
	uniform(a, b);
	
	// Reset to the original texture
	if(act != GL_TEXTURE0 + b) glActiveTexture(act);
}

void program::attribute(int b, const char* a) {
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

/* *
fbo
A frame buffer object.

C++
fbo a(320, 240);

fbo b(320, 240, true, true, true); //with alpha, high quality, and with depth and stencil testing

Python
a = fbo(320, 240)

b = fbo(320, 240, true, true, true) #with alpha, high quality, and with depth and stencil testing

see: use_fbo
* */
fbo::fbo(image* a) {
	if(glfw_state == 0)
		graphics();
	
	GLint last;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last);
	
	glGenFramebuffers(1, &id);
	
	buffer = a;
	buffer_is_mine = false;
	depth_stencil = false;
	
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->id, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindFramebuffer(GL_FRAMEBUFFER, last);
}

fbo::fbo(int b, int c, bool alpha, bool quality, bool ds, int multisample) {
	if(glfw_state == 0)
		graphics();
	
	glGenFramebuffers(1, &id);
	
	GLint last, lastr;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last);
	
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	buffer = new image(b, c, alpha, quality);
	buffer_is_mine = true;
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->id, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	if(ds) {
		depth_stencil = true;
		glGenRenderbuffers(1, &depth_stencil_id);
		
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &lastr);
		
		glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_id);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample, GL_DEPTH24_STENCIL8, b, c);
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_id);
		
		glBindRenderbuffer(GL_RENDERBUFFER, lastr);
	}
	else
		depth_stencil = false;
	
	glBindFramebuffer(GL_FRAMEBUFFER, last);
}

fbo::~fbo() {
	if(buffer_is_mine)
		delete buffer;
	if(depth_stencil_id)
		glDeleteRenderbuffers(1, &depth_stencil_id);
	glDeleteFramebuffers(1, &id);
}

/* *
vbo
A buffer on the graphics.

C++
vbo a(256, "static draw");

Python
a = vbo(256, 'static draw')
* */
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
}

vbo::vbo(int l, const char* t) {
	glGenBuffers(1, &id);
	
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, l, NULL, buffer_usage_from_string(t));
	
	glBindBuffer(GL_ARRAY_BUFFER, last);
}

void vbo::data(int offset, int size, const void* data) {
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
	glGenBuffers(1, &id);
	
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)), buffer_usage_from_string(t));
	
	glBindBuffer(GL_ARRAY_BUFFER, last);
}

void vbo::data(int offset, PyObject * o) {
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ARRAY_BUFFER, offset, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)));
	
	if(last != id)
		glBindBuffer(GL_ARRAY_BUFFER, last);
}
#endif

vbo::~vbo() {
	glDeleteBuffers(1, &id);
}

void draw(const char* a, unsigned int count, unsigned int first) {
	glDrawArrays(primitive_from_string(a), first, count);
}

/* *
ibo
An index buffer.
	draw(string, first, count)
		string can be anything that begin can be
C++
ibo a(256, "static draw");

Python
a = ibo(256, 'static draw')
* */
ibo::ibo(int l, const char* s, const char* t) {
	glGenBuffers(1, &id);
	
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, l, NULL, buffer_usage_from_string(t));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
	
	storage = common_type_from_string(s);
}

void ibo::data(int offset, int size, const void* data) {
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
	glGenBuffers(1, &id);
	
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)), buffer_usage_from_string(t));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
	
	storage = common_type_from_string(s);
}

void ibo::data(int offset, PyObject * o) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)));
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
}
#endif

void ibo::draw(const char* a, unsigned int count, unsigned int first) {
	glDrawElements(primitive_from_string(a), count, storage, (const GLvoid*) first);
}

/* *
vao
A vao.
	.attribute(int, vbo*, string type, int size, int offset, int stride, bool normalized, bool integers)
		size is the number of components per attribute.
		normalized specifies whether values should be normalized.
		
		stride is the byte offset between adjacent attributes.
		offset is the offset to the first attribute.
		integers specifies if the data is read as integers or not.
		
		type can be one of:
		"float"
		"byte"
		"unsigned byte"
		"short"
		"unsigned short"
		"int"
		"unsigned int"
		"double"
	.enable(int, bool = true)
		enables an attribute
	.divisor(int[, step])
		sets the number of instances the attribute advances over.

C++
vao a();

Python
a = vao()
* */
vao::vao() {
	glGenVertexArrays(1, &id);
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
		glBindVertexArray(v->id);
	if(lastb != id)
		glBindBuffer(GL_ARRAY_BUFFER, v->id);
	
	if(integers)
		glVertexAttribIPointer(index, size, common_type_from_string(type), stride, (const GLvoid*) offset);
	else
		glVertexAttribPointer(index, size, common_type_from_string(type), normalized, stride, (const GLvoid*) offset);
	
	if(lastb != id)
		glBindBuffer(GL_ARRAY_BUFFER, last);
	if(last != id)
		glBindVertexArray(last);
}
