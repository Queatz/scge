std::stack<fbo*, std::list<fbo*> > fbo_stack;
std::stack<image*, std::list<image*> > image_stack;
std::stack<font*, std::list<font*> > font_stack;
std::stack<program*, std::list<program*> > program_stack;
std::stack<rgba, std::list<rgba> > color_stack;
std::stack<crop, std::list<crop> > image_crop_stack;
std::stack<icrop, std::list<icrop> > scissor_stack;
std::stack<std::string, std::list<std::string> > blend_mode_stack;

GLFWwindow glfw_window = NULL;

int width = 0, height = 0, glfw_state = 0;
bool fullscreened;

float clipNear, clipFar, FOV, aspect;
float saved_clipNear, saved_clipFar, saved_FOV, saved_aspect;

glm::mat4 saved_projection_matrix, saved_modelview_matrix;
glm::vec3 saved_up, saved_right, saved_z;

GLfloat mv[16];
GLfloat pr[16];

std::string key_pressed_list = "";
std::string char_string = "";

void glfw_error(int code, const char* string) {
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
		
	fbo_stack.push(NULL);
	image_stack.push(NULL);
	font_stack.push(NULL);
	program_stack.push(NULL);
	color_stack.push(rgba(1, 1, 1, 1));
	image_crop_stack.push(crop(false));
	scissor_stack.push(icrop(false));
	blend_mode_stack.push("mix");
	
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
		if(strcmp(b, "png"))
			return FIF_PNG;
		else if(strcmp(b, "jpg"))
			return FIF_JPEG;
		else if(strcmp(b, "bmp"))
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
		return JPEG_DEFAULT;
	else if(a == FIF_BMP)
		return BMP_DEFAULT;
	else
		return 0;
}

void window_resize_callback(GLFWwindow g, int w, int h) {
//	glViewport(0, 0, w, h);
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
	case GLFW_KEY_ESC: return "esc";
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

char key_int_to_char(int a) {
	if(a < 256) return (char) a;
	return NULL;
}

void window_key_callback(GLFWwindow w, int key, int action) {
	if(action == GLFW_PRESS) {
		key_pressed_list += ":";
		key_pressed_list += int_to_key_name(key);
	}
}

void window_char_callback(GLFWwindow w, int key) {
	char_string += key_int_to_char(key);
}

/* *
keyboard_enable(string, bool = true)
Enable keyboard features.
"string" - see keyboard_string
"presses" - see keyboard_presses

C++
keyboard_enable("string");

Python
keyboard_enable('string')
* */
void keyboard_enable(const char* a, bool b) {
	if(!strcmp(a, "string"))
		glfwSetCharCallback(b ? window_char_callback : NULL);
	else if(!strcmp(a, "presses"))
		glfwSetKeyCallback(b ? window_key_callback : NULL);
	else
		err("keyboard_enable", "invalid value");
		
}

/* *
keyboard_string()
Returns typed characters since last called.

C++
const char* a = keyboard_string();

Python
a = keyboard_string()

see:keyboard_enable
* */
std::string keyboard_string() {
	std::string a = char_string;
	char_string = "";
	return a;
}

/* *
keyboard_presses()
Returns keys that have been pressed since last called as a string.
Example: ":left:right:space:a"

C++
const char* a = keyboard_presses();

Python
a = keyboard_presses()

see:keyboard_enable
* */
std::string keyboard_presses() {
	std::string a = key_pressed_list;
	key_pressed_list = "";
	return a;
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
	
	if(!resizeable)
		glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	glfw_window = glfwOpenWindow(x, y, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOWED), title, NULL);
	if(!glfw_window) {
		err("window", "could not initiate window");
		return false;
	}
	
	glfw_state = 2;
	
	GLenum err = glewInit();
	if(err != GLEW_OK)
		note("window", "extentions unsupported");
	
	glfwSetWindowSizeCallback(window_resize_callback);
	
	glfwSwapInterval(0);
	
	//glfwGetWindowSize(glfw_window, &width, &height);
	width = x;
	height = y;
	
	// ...should these be here...
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1.0, 1.0);
	clipNear = -1; clipFar = 1;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
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
window_resized()
Returns true if the window was resized since the last call.

C++
bool a = window_resized();

Python
a = window_resized()
* */
bool window_resized() {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	if(w != width || h != height) {
		glfwGetWindowSize(glfw_window, &width, &height);
		return true;
	}
	return false;
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
rest(float)
Rest for some seconds.

C++
rest(2.0);

Python
rest(2.0)
* */
void rest(float a) {
	usleep(a*1000);
}

/* *
display_dimensions()
Returns an ibox filled out with the resolution of the display.

C++
ibox a = display_dimentions();

Python
a = display_dimentions()
* */
ibox display_dimensions() {
	if(glfw_state == 0)
		graphics();
	
	GLFWvidmode a;
	glfwGetDesktopMode(&a);
	
	return ibox(a.width, a.height);
}

/* *
window_dimensions()
Returns an ibox filled out with the resolution of the window.

C++
ibox a = window_dimentions();

Python
a = window_dimentions()
* */
ibox window_dimensions() {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	
	return ibox(w, h);
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
Show all the drawn things on the screen and update other states, such as the window, keyboard, and mouse.

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
screenshot("outcome.tga");

Python
screenshot('outcome.tga')
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
matrix(string = "view")
Choose the matrix to alter.
"projection" - use only for setting the projection, i.e. with orthographic or perspective
"view" - use for moving the camera around and other uses

C++
matrix();

Python
matrix()
* */
void matrix(const char* a) {
	if(a && !strcmp(a, "projection"))
		glMatrixMode(GL_PROJECTION);
	else
		glMatrixMode(GL_MODELVIEW);
}

/* *
reset_matrix()
Resets the matrix to its identity.

C++
reset_matrix();

Python
reset_matrix()
* */
void reset_matrix() {
	glLoadIdentity();
}

/* *
viewport(float x, float y, float w, float h)
Set the screen viewport size.

C++
viewport(0.0, 0.0, 320.0, 240.0);

Python
viewport(0.0, 0.0, 320.0, 240.0)
* */
void viewport(float x, float y, float w, float h) {
	glViewport(x, y, w, h);
}

/* *
orthographic(float left, float right, float bottom, float top, float near = -1.0, float far = 1.0)
Set up an orthographic display matrix.

C++
orthographic(0.0, 320.0, 0.0, 240.0); //0,0 is in the bottom-left
orthographic(0.0, 320.0, 240.0, 0.0); //0,0 is in the top-left

Python
orthographic(0.0, 320.0, 0.0, 240.0) #0,0 is in the bottom-left
orthographic(0.0, 320.0, 240.0, 0.0) #0,0 is in the top-left

see:push pop get
* */
void orthographic(float a, float b, float c, float d, float e, float f) {
	glOrtho(a, b, c, d, e, f);
	clipNear = e; clipFar = f;
}

/* *
perspective(float fov = 60, float aspect = NULL, float near = 0.1, float far = 1000.0)
Set up a perspective display matrix.

C++
perspective();

Python
perspective()

see:push pop get
* */
void perspective(float fovY, float aspect, float zNear, float zFar) {
	if(!aspect)
		aspect = (float)width/(float)height;
	
	float fH = tan(fovY / 360.0 * M_PI) * zNear;
	float fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
	clipNear = zNear; clipFar = zFar;
	aspect = aspect; FOV = tan(fovY / 360.0 * M_PI);
} 

/* *
frustum(float left, float right, float bottom, float top, float near = 0.1, float far = 1000.0)
Set up a perspective display matrix.

C++
frustum(0.0, 320.0, 0.0, 240.0);

Python
frustum(0.0, 320.0, 0.0, 240.0)

see:push pop get
* */
void frustum(float a, float b, float c, float d, float e, float f) {
	glFrustum(a, b, c, d, e, f);
	clipNear = e; clipFar = f;
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
"point smooth" - draw points smoothly
"polygon smooth" - draw polygons smoothly
"line smooth" - draw lines smoothly
"blend" - blending, such as with alpha
"depth" - depth testing
"cull" - cull
"stencil" - stencil testing
"alpha" - alpha testing
"polygon stipple" - stippling of the polygons
"polygon depth offset" - offsetting the depth values of polygons
"line depth offset" - offsetting the depth values of lines
"point depth offset" - offsetting the depth values of points
"line stipple" - stipple on the lines
"light" - shade with lights
"smooth" - smooth shading
"normalize" - normalize normals

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
	else if (!strcmp(a, "stencil"))
		c = GL_STENCIL_TEST;
	else if (!strcmp(a, "alpha"))
		c = GL_ALPHA_TEST;
	else if (!strcmp(a, "polygon stipple"))
		c = GL_POLYGON_STIPPLE;
	else if (!strcmp(a, "polygon depth offset"))
		c = GL_POLYGON_OFFSET_FILL;
	else if (!strcmp(a, "line depth offset"))
		c = GL_POLYGON_OFFSET_LINE;
	else if (!strcmp(a, "point depth offset"))
		c = GL_POLYGON_OFFSET_POINT;
	else if (!strcmp(a, "line stipple"))
		c = GL_LINE_STIPPLE;
	else if (!strcmp(a, "cull"))
		c = GL_CULL_FACE;
	else if (!strcmp(a, "light"))
		c = GL_LIGHTING;
	else if (!strcmp(a, "smooth")) {
		glShadeModel(b ? GL_SMOOTH : GL_FLAT);
		return;
	}
	else if (!strcmp(a, "normalize"))
		c = GL_NORMALIZE;
	
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
light_ambient(float = 0.0, float = 0.0, float = 0.0, float = 0.0)
Set the ambient lighting when "light" is enable'd.

C++
light_ambient();

Python
light_ambient()
* */
void light_ambient(float r, float g, float b, float a) {
	GLfloat f[4] = {r, g, b, a};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, f);
}

/* *
light_two_side(bool = true)
Enable lighting on both sides of polygons.

C++
light_two_side();

Python
light_two_side()
* */
void light_two_side(bool a) {
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, a?1.0:0.0);
}

/* *
light_set(int, string, float = 0.0, float = 0.0, float = 0.0, float = 0.0)
Set parameters of a light with id.
These use only the first float:
"spot exponent"
"spot cutoff"
"constant attenuation"
"linear attenuation"
"quadraic attenuation"

These use all four:
"ambient"
"diffuse"
"specular"
"position"
"spot direction"

C++
light_set(0, "spot exponent", 0.5);

Python
light_set(0, 'spot exponent', 0.5)
* */
void light_set(int i, const char* w, float r, float g, float b, float a) {
	GLfloat f[4] = {r, g, b, a};
	
	if(!strcmp(w, "spot exponent"))
		glLightf(GL_LIGHT0+i, GL_SPOT_EXPONENT, r);
	else if(!strcmp(w, "spot cutoff"))
		glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, r);
	else if(!strcmp(w, "constant attenuation"))
		glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, r);
	else if(!strcmp(w, "linear attenuation"))
		glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, r);
	else if(!strcmp(w, "quadraic attenuation"))
		glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, r);
	
	else if(!strcmp(w, "ambient"))
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, f);
	else if(!strcmp(w, "diffuse"))
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, f);
	else if(!strcmp(w, "specular"))
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, f);
	else if(!strcmp(w, "position"))
		glLightfv(GL_LIGHT0+i, GL_POSITION, f);
	else if(!strcmp(w, "spot direction"))
		glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, f);
	
}

/* *
light_enable(int = 0, bool = true)
Turn on and off the light.

C++
light_enable();

Python
light_enable()

see:enable
* */
void light_enable(int i, bool e) {
	if(e)
		glEnable(GL_LIGHT0+i);
	else
		glDisable(GL_LIGHT0+i);
}

/* *
material(string = "both", string = "", float = 0.0, float = 0.0, float = 0.0, float = 1.0)
Set the material to be used, for use when "light" is enable'd.

First string can be:
"front"
"back"
"both"

And the seccond is the attribute to set:
"shininess"
"ambient"
"diffuse"
"specular"
"emission"

C++
material("front", "shininess", 0.9);

Python
material('front', 'shininess', 0.9)

see:light_enable
* */
void material(const char* face, const char* w, float r, float g, float b, float a) {
	GLfloat f[4] = {r, g, b, a};
	
	GLenum fce;
	if(!strcmp(w, "front"))
		fce = GL_FRONT;
	else if(!strcmp(w, "back"))
		fce = GL_BACK;
	else
		fce = GL_FRONT_AND_BACK;
	
	if(!strcmp(w, "shininess"))
		glMaterialf(fce, GL_SHININESS, r);
	else if(!strcmp(w, "ambient"))
		glMaterialfv(fce, GL_AMBIENT, f);
	else if(!strcmp(w, "diffuse"))
		glMaterialfv(fce, GL_DIFFUSE, f);
	else if(!strcmp(w, "specular"))
		glMaterialfv(fce, GL_SPECULAR, f);
	else if(!strcmp(w, "emission"))
		glMaterialfv(fce, GL_EMISSION, f);
	else
		err("material", "invalid value");
		
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

/* *
line_stipple(string, int)
Set the stipple pattern of drawn lines multiplied by an integer value.
"dashed"
"long dashed"
"dotted"
"dotted dashed"
"long dotted dashed"
"dots and dashes"

C++
line_stipple("dashed");

Python
line_stipple('dashed')

see:enable
* */
void line_stipple(const char* b, int a) {
	if(!strcmp(b, "dotted"))
		glLineStipple(a, 0xAAAA);
	else if(!strcmp(b, "dashed"))
		glLineStipple(a, 0xEEEE);
	else if(!strcmp(b, "long dashed"))
		glLineStipple(a, 0x00FF);
	else if(!strcmp(b, "dotted dashed"))
		glLineStipple(a, 0xD6B5);
	else if(!strcmp(b, "long dotted dashed"))
		glLineStipple(a, 0x0EBAE);
	else if(!strcmp(b, "dots and dashes"))
		glLineStipple(a, 0xEABA);
	else
		err("line_stipple", "invalid pattern");
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
void polygon_stipple(const char* a, bool inverse, const char* interpolation, int ox, int oy) {
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
}

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
!The only use for this is with "alpha" blending.

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


/* *
*
push pop get
Each push function stores the current state; each pop function restores it; each get function returns it.

C++
push_color();
push_fbo();
push_image();
push_font();
push_image_crop();
push_scissor();
push_blend_mode();
push_program();

pop_color();
pop_fbo();
pop_image();
pop_font();
pop_image_crop();
pop_scissor();
pop_blend_mode();
pop_program();

get_color();
get_fbo();
get_image();
get_font();
get_image_crop();
get_scissor();
get_blend_mode();
get_program();

push_matrix();
pop_matrix();

Python
push_color()
push_fbo()
push_image()
push_font()
push_image_crop()
push_scissor()
push_blend_mode()
push_program()

pop_color()
pop_fbo()
pop_image()
pop_font()
pop_image_crop()
pop_scissor()
pop_blend_mode()
pop_program()

get_color()
get_fbo()
get_image()
get_font()
get_image_crop()
get_scissor()
get_blend_mode()
get_program()

push_matrix()
pop_matrix()
* */

/* *
color(rgba)
color(float, float, float, float = 1.0)
Set the color to draw with.
If you supply 1 value, the color will be gray.
If you supply 2 values the color will be gray with alpha.

C++
color(0.5, 0.5, 0.5);

Python
color(.5, .5, .5)
* */
void color(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void color(float v, float a) {
	glColor4f(v, v, v, a);
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


/* *
use_fbo(fbo)
Set the fbo to draw to, or none.

C++
use_fbo(); //draw to the screen

Python
use_fbo() #draw to the screen
* */
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


/* *
use_image(image)
Set the image to draw with.
#Texturing must be enabled to draw with images. Calling enable("texture") does a fine job here.

C++
image a("penny.png");
use_image(&a);

Python
a = image('penny.png')
use_image(a)
* */
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

/* *
use_font(font)
Set the font to write with.

C++
font a("sans.ttf");
use_font(&a);

Python
a = font('sans.ttf')
use_font(a)
* */
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


/* *
image_crop(float x, float y, float w, float h)
Crop drawing to a section of the image, or not.

C++
image_crop(); //use full image

Python
image_crop() #use full image

see:draw
* */
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


/* *
scissor(int, int, int, int)
Crop drawing to a portion of the window, or not.

C++
scissor(); //draw on all the window

Python
scissor() #draw on all the window
* */
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

/* *
texture_environment(string)
Set up how to draw textures.  An omitted string resets it.
"alpha"

C++
texture_environment();

Python
texture_environment()
* */
void texture_environment() {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
}

void texture_environment(const char* a) {
	if(!strcmp(a, "alpha")) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PREVIOUS);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	}
	else if(!strcmp(a, "s")) {
		float envColor[] = {1,0,0,1};
		glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, envColor );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
		glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE );
		glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_CONSTANT );
		glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS );
	}
	else
		err("texture_environment", "invalid parameter");
}

/* *
*
transform
These functions manipulate drawing.

C++
translate(12.0, 12.0);
rotate(90.0);
rotate(90.0, 0.0, 1.0, 0.0); //around Y
scale(2.0, 2.0);

Python
translate(12.0, 12.0)
rotate(90.0)
rotate(90.0, 0.0, 1.0, 0.0) #around Y
scale(2.0, 2.0)
* */
void translate(float a, float b) {
	glTranslatef(a, b, 0.0);
}

void translate(float a, float b, float c) {
	glTranslatef(a, b, c);
}

void rotate(float a) {
	glRotatef(a, 0.0, 0.0, 1.0);
}

void rotate(float a, float b, float c, float d) {
	glRotatef(a, b, c, d);
}

void scale(float a) {
	glScalef(a, a, 1.0);
}

void scale(float a, float b) {
	glScalef(a, b, 1.0);
}

void scale(float a, float b, float c) {
	glScalef(a, b, c);
}

/* *
*
begin end
Begin a sequence of drawing, and then end that.
"point"
"line"
"line strip"
"line loop"
"triangle"
"triangle strip"
"triangle fan"
"quad"
"quad strip"
"image"


C++
begin("point");
end();

Python
begin('point')
end()

see: inline draw functions
* */
void begin(const char* a) {
	if (!strcmp(a, "image") || !strcmp(a, "quad"))
		glBegin(GL_QUADS);
	else if (!strcmp(a, "quad strip"))
		glBegin(GL_QUAD_STRIP);
	else if (!strcmp(a, "triangle"))
		glBegin(GL_TRIANGLES);
	else if (!strcmp(a, "triangle strip"))
		glBegin(GL_TRIANGLE_STRIP);
	else if (!strcmp(a, "triangle fan"))
		glBegin(GL_TRIANGLE_FAN);
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

/* *
draw(float x = 0.0, float y = 0.0, float sx = 0.0, float sy = 0.0, float r = 0.0, float ox = 0.0, float oy = 0.0)
Draw the current image.

C++
draw(); //draw current image at 0,0 with no scale, rotation, or offset

Python
draw() #draw current image at 0,0 with no scale, rotation, or offset

see:use_image
* */
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

/* *
write(string, float x = 0.0, float y = 0.0, bool flip = false)
Write some text down.

C++
write("Hello World");

Python
write('Hello World')

see:use_font, paper
* */
void write(const char* b, float x, float y, bool invert_y) {
	if(!font_stack.top()) {
		note("write", "no font");
		return;
	}
	
	if(invert_y) {
		glPushMatrix();
		glScalef(1, -1, 1);
	}
	
	GLint bind, act;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	
	font_stack.top()->data->Render(b, -1, FTPoint(x, invert_y ? -y : y));
	
	// Reset to the origional texture
	glBindTexture(GL_TEXTURE_2D, bind);
	glActiveTexture(act);
	
	if(invert_y)
		glPopMatrix();
}

/* *
background_image(float x, float y, float x2, float y2, float sx, float sy, float r, float xo, float yo)
Draw a background on the window.

C++
background_image(0.0, 0.0, 320.0, 240.0);

Python
background_image(0, 0, 320, 240)

see:use_image
* */
void background_image(float x, float y, float x2, float y2, float sx, float sy, float r, float xo, float yo) {
	if(!sy)
		sy = sx;
	
	float cx = std::abs(x - x2) / image_stack.top()->width / sx, cy = std::abs(y - y2) / image_stack.top()->height / sx;
	
	xo /= image_stack.top()->width;
	yo /= image_stack.top()->height;
	
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo); glVertex2i(x, y);
	glTexCoord2f(xo + cx, yo); glVertex2i(x2, y);
	glTexCoord2f(xo + cx, yo + cy); glVertex2i(x2, y2);
	glTexCoord2f(xo, yo + cy); glVertex2i(x, y2);
	glEnd();
}

/* *
*
shapes
Various draw functions.

C++
quad(0.0, 0.0, 12.0, 12.0); //faster than mquad
mquad(0.0, 0.0, 320.0, 240.0); //can be used appropriately with textures
line(0.0, 120.0, 320.0, 120.0);
point(160.0, 120.0);

Python
quad(0.0, 0.0, 12.0, 12.0) #faster than mquad
mquad(0.0, 0.0, 320.0, 240.0) #can be used appropriately with textures and programs
line(0.0, 120.0, 320.0, 120.0)
point(160.0, 120.0)

see:inline draw functions
* */
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

void imquad(float x, float y, float x2, float y2, float z) {
	glTexCoord2f(0, 0); glVertex3f(x, y, z);
	glTexCoord2f(1, 0); glVertex3f(x2, y, z);
	glTexCoord2f(1, 1); glVertex3f(x2, y2, z);
	glTexCoord2f(0, 1); glVertex3f(x, y2, z);
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

/* *
*
inline draw functions
Draw functions that are substanually faster when drawing many of the same type, place between calls to begin() and end():
idraw
iquad
imquad
iline
ipoint
impoint

C++
begin("point");
ipoint(160.0, 120.0);
end();

Python
begin('point')
ipoint(160.0, 120.0)
end()

see:begin end, shapes
* */
void ipoint(float x, float y) {
	glVertex2i(x, y);
}

void ipoint(float x, float y, float z) {
	glVertex3f(x, y, z);
}

void impoint(float x, float y) {
	glTexCoord2f(x/image_stack.top()->width, y/image_stack.top()->height); glVertex2i(x, y);
}

void iline(float a, float b, float c, float d) {
	glVertex2i(a, b);
	glVertex2i(c, d);
}

/* *
sprite(float x, float y, float z, float scaleX, float scaleY)
Draw a mapped quad at a position according to the last saved matrix.

C++
sprite(0.0, 0.0, 0.0);

Python
sprite(0.0, 0.0, 0.0)

see:save_matrix
* */

void sprite(float x, float y, float z, float sx, float sy, float r) {
	if(!sy)
		sy = sx;
	
	glm::vec3 pos(x, y, z);
	
	if(r) {
		float c = cos(r), s = sin(r);
		glm::vec3 bl, tl, tr, br;
		
		bl = glm::gtx::rotate_vector::rotate((-saved_right*sx - saved_up*sy), r, saved_z);
		tl = glm::gtx::rotate_vector::rotate((saved_right*sx - saved_up*sy), r, saved_z);
		tr = glm::gtx::rotate_vector::rotate((saved_right*sx + saved_up*sy), r, saved_z);
		br = glm::gtx::rotate_vector::rotate((-saved_right*sx + saved_up*sy), r, saved_z);
		
	/*	
		glTexCoord2f(co_x, co_y); glVertex2i(x + (-xo * rc - -yo * rs) * sx, y + (-yo * rc + -xo * rs) * sy);
		glTexCoord2f(co_x2, co_y); glVertex2i(x + ((w - xo) * rc - -yo * rs) * sx, y + (-yo * rc + (w - xo) * rs) * sy);
		glTexCoord2f(co_x2, co_y2); glVertex2i(x + ((w - xo) * rc - (h - yo) * rs) * sx, y + ((h - yo) * rc + (w - xo) * rs) * sy);
		glTexCoord2f(co_x, co_y2); glVertex2i(x + (-xo * rc - (h - yo) * rs) * sx, y + ((h - yo) * rc + -xo * rs) * sy);
	*/	
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3fv(glm::value_ptr(pos + bl));
		glTexCoord2f(1, 0); glVertex3fv(glm::value_ptr(pos + tl));
		glTexCoord2f(1, 1); glVertex3fv(glm::value_ptr(pos + tr));
		glTexCoord2f(0, 1); glVertex3fv(glm::value_ptr(pos + br));
		glEnd();
		return;
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3fv(glm::value_ptr(pos + (-saved_right*sx - saved_up*sy)));
	glTexCoord2f(1, 0); glVertex3fv(glm::value_ptr(pos + (saved_right*sx - saved_up*sy)));
	glTexCoord2f(1, 1); glVertex3fv(glm::value_ptr(pos + (saved_right*sx + saved_up*sy)));
	glTexCoord2f(0, 1); glVertex3fv(glm::value_ptr(pos + (-saved_right*sx + saved_up*sy)));
	glEnd();
}


void save_matrix() {
	saved_clipNear = clipNear;
	saved_clipFar = clipFar;
	saved_FOV = FOV;
	saved_aspect = aspect;
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	glGetFloatv(GL_PROJECTION_MATRIX, pr);
	
	for(int column = 0; column < 4; column++) for(int row = 0; row < 4; row++)
		saved_modelview_matrix[column][row] = mv[column + 4*row];
	
	for(int column = 0; column < 4; column++) for(int row = 0; row < 4; row++)
		saved_projection_matrix[column][row] = pr[column + 4*row];
	
	saved_right[0] = saved_modelview_matrix[0][0];
	saved_right[1] = saved_modelview_matrix[0][1];
	saved_right[2] = saved_modelview_matrix[0][2];
	
	saved_up[0] = saved_modelview_matrix[1][0];
	saved_up[1] = saved_modelview_matrix[1][1];
	saved_up[2] = saved_modelview_matrix[1][2];
	
	saved_z[0] = saved_modelview_matrix[2][0];
	saved_z[1] = saved_modelview_matrix[2][1];
	saved_z[2] = saved_modelview_matrix[2][2];
}

/* *
portion(int x, int y, int x2, int y2)
Draw a portion of an image, keeping it's place.

C++
portion(24.0, 24.0, 48.0, 48.0);

Python
portion(24.0, 24.0, 48.0, 48.0)
* */
void portion(int x, int y, int x2, int y2) {
	image* ci = image_stack.top();
	
	glBegin(GL_QUADS);
	glTexCoord2f(x / ci->width, y / ci->height); glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0); glVertex2i(x, y);
	glTexCoord2f(x2 / ci->width, y / ci->height); glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0); glVertex2i(x2, y);
	glTexCoord2f(x2 / ci->width, y2 / ci->height); glMultiTexCoord2fARB(GL_TEXTURE1, 1, 1); glVertex2i(x2, y2);
	glTexCoord2f(x / ci->width, y2 / ci->height); glMultiTexCoord2fARB(GL_TEXTURE1, 0, 1); glVertex2i(x, y2);
	glEnd();
}

/* *
texture_coordinates(float x, float y, int texture = 0)
Change the texture cooridinates of the folliwing vertices to be drawn, optionally specifying the multitexture slot.

C++
texture_coordinates(0.0, 0.0)

Python
texture_coordinates(0, 0)
* */
void texture_coordinates(float x, float y, unsigned int i) {
	glMultiTexCoord2fARB(GL_TEXTURE0+i, x, y);
}

/* *
normal(float x, float y, float z)
Change the normal of polygons.

C++
normal();

Python
normal()
* */
void normal(float x, float y, float z) {
	glNormal3f(x, y, z);
}

/* *
pixel(int x, int y, string = "")
Returns and rgba containing the pixel information on the screen or as specified by the string.
"depth"
"stencil"
"luminance"
"color"

C++
rgba a();
a = pixel(0, 0);

Python
a = pixel(0, 0)
* */
rgba pixel(int x, int y, const char* w) {
	if(x < 0 || x >= width || y < 0 || y >= height)
		return rgba(0.0, 0.0, 0.0);
	
	GLenum fmt = GL_RGB;
	if(w) {
		if(!strcmp(w, "depth"))
			fmt = GL_DEPTH_COMPONENT;
		else if(!strcmp(w, "stencil"))
			fmt = GL_STENCIL_INDEX;
		else if(!strcmp(w, "luminance"))
			fmt = GL_LUMINANCE;
	}
	
	GLfloat c[3];
	glReadPixels(x, y, 1, 1, fmt, GL_FLOAT, c);
	return rgba(c[0], c[1], c[2]);
}

/* *
pixel_coordinates(float x, float y, float z = 0)
Get the coordinates of a screen pixel in world space as an rgba.

C++
pixel_coordinates(160.0, 120.0);

Python
pixel_coordinates(160.0, 120.0)
* */
rgba pixel_coordinates(float x, float y, float z) {
	return rgba(0.0, 0.0, 0.0, 0.0);
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
alpha_test(string = "always", float = 0.0)
How to test if the alpha test passes or fails in relation to the reference value.
"=="
"!="
"<"
">"
"<="
">="
"always"
"never"

C++
alpha_test();

Python
alpha_test()
* */

void alpha_test(const char* a, float b) {
	glAlphaFunc(comparison_string_to_gl(a), b);
}

/* *
mouse_position()
Returns an offset with the mouse position.

C++
offset a();
a = mouse_position();

Python
a = mouse_position()

see:move_mouse
* */
offset mouse_position(bool topdown) {
	int x, y;
	glfwGetMousePos(glfw_window, &x, &y);
	if(!topdown)
		y = height - y;
	return offset(x, y);
}

/* *
mouse(bool = false)
Show or hide the mouse.

C++
mouse(); //hide the pointer

Python
mouse() #hide the pointer
* */
void mouse(bool a) {
	if(a)
		glfwEnable(glfw_window, GLFW_MOUSE_CURSOR);
	else
		glfwDisable(glfw_window, GLFW_MOUSE_CURSOR);
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
Get the wheel offset since the window was opened.

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
move_mouse(int x, int y)
Move the mouse to some position.

C++
move_mouse(160, 120);

Python
move_mouse(160, 120)
* */
void move_mouse(int a, int b) {
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

bool pixelcache::save(const char* a, const char* b) {
	if(!data)
		return false;
	
	FIBITMAP* c = FreeImage_Allocate(width, height, 24);
	RGBQUAD color;
	
	for(unsigned int x = 0; x < width; x++)
		for(unsigned int y = 0; y < height; y++) {
			int h = y * width * 3 + x * 3;
			color.rgbRed = data[h + 0];
			color.rgbGreen = data[h + 1];
			color.rgbBlue = data[h + 2];
			FreeImage_SetPixelColor(c, x, y, &color);
		}
	
	if (FreeImage_Save(fif_from_string(a, b), c, a, 0))
		return true;
	return false;

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
	save(string name, string format)
		save the images pixelcache to disk.  If format is omitted it is guessed from the filename.
		#Alpha is not currently saved.
		"png"
		"jpg"
		"bmp"

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
image::image(const char* a, bool m) {
	if(glfw_state == 0)
		graphics();

	FIBITMAP *bm = FreeImage_Load(fif_from_string(a), a, 0);
	
	if(!bm) {
		err("image", "could not load");
		return;
	}
	
	bool has_alpha = FreeImage_IsTransparent(bm);
	
	if(has_alpha)
		bm = FreeImage_ConvertTo32Bits(bm);
	else
		bm = FreeImage_ConvertTo24Bits(bm);
	
	BYTE *bits = new BYTE[FreeImage_GetHeight(bm) * FreeImage_GetPitch(bm)];
	
	FreeImage_ConvertToRawBits(bits, bm, FreeImage_GetPitch(bm), (has_alpha ? 32 : 24), FI_RGBA_BLUE_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_RED_MASK, FALSE);
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(bm), FreeImage_GetHeight(bm), 0, (has_alpha ? GL_BGRA : GL_BGR), GL_UNSIGNED_BYTE, bits);
	
	if(m)
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	
	if(bm)
		FreeImage_Unload(bm);
	
	//id = SOIL_load_OGL_texture(a, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | (m ? SOIL_FLAG_MIPMAPS : 0)); //NULL
	
	cache = NULL;
	mipmaps = m;
	
	if(id == 0) {
		err("image", "could not load");
		return;
	}
	
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	
	if(m) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}

image::image(int a, int b, bool quality) {
	if(glfw_state == 0)
		graphics();
	
	cache = NULL;
	mipmaps = false;
	
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

bool image::save(const char* a, const char* b) {
	if(!cache)
		refresh_pixel_cache();
	
	return cache->save(a, b);
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
	bbox(string) rect
		get the bounding box of a string

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

rect font::bbox(const char* a) {
	FTBBox b = data->BBox(a);
	return rect(b.Lower().Xf(), b.Lower().Yf(), b.Upper().Xf(), b.Upper().Yf());
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
