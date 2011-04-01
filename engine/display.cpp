std::stack<fbo*, std::list<fbo*> > fbo_stack;
std::stack<image*, std::list<image*> > image_stack;
std::stack<font*, std::list<font*> > font_stack;
std::stack<program*, std::list<program*> > program_stack;
std::stack<rgba, std::list<rgba> > color_stack;
std::stack<crop, std::list<crop> > image_crop_stack;
std::stack<icrop, std::list<icrop> > scissor_stack;
std::stack<std::string, std::list<std::string> > blend_mode_stack;
	
int width = 0, height = 0, glfw_state = 0;
bool fullscreened;

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
	if(glfwInit() != GL_TRUE) {
		err("graphics", "could not");
		return false;
	}
		
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
	glfwTerminate();
}

void GLFWCALL window_resize_callback(int w, int h) {
	glViewport(0, 0, w, h);
}

std::string key_pressed_list = "";
std::string char_string = "";

const char* int_to_key_name(int a) {
	if(a == 'A') return "a";
	if(a == 'B') return "b";
	if(a == 'C') return "c";
	if(a == 'D') return "d";
	if(a == 'E') return "e";
	if(a == 'F') return "f";
	if(a == 'G') return "g";
	if(a == 'H') return "h";
	if(a == 'I') return "i";
	if(a == 'J') return "j";
	if(a == 'K') return "k";
	if(a == 'L') return "l";
	if(a == 'M') return "m";
	if(a == 'N') return "n";
	if(a == 'O') return "o";
	if(a == 'P') return "p";
	if(a == 'Q') return "q";
	if(a == 'R') return "r";
	if(a == 'S') return "s";
	if(a == 'T') return "t";
	if(a == 'U') return "u";
	if(a == 'V') return "v";
	if(a == 'W') return "w";
	if(a == 'X') return "x";
	if(a == 'Y') return "y";
	if(a == 'Z') return "z";
	if(a == '1') return "1";
	if(a == '2') return "2";
	if(a == '3') return "3";
	if(a == '4') return "4";
	if(a == '5') return "5";
	if(a == '6') return "6";
	if(a == '7') return "7";
	if(a == '8') return "8";
	if(a == '9') return "9";
	if(a == '0') return "0";
	if(a == '[') return "[";
	if(a == ']') return "]";
	if(a == '\\') return "\\";
	if(a == ';') return ";";
	if(a == '\'') return "'";
	if(a == ',') return ",";
	if(a == '.') return ".";
	if(a == '/') return "/";
	if(a == '`') return "`";
	
	if(a == GLFW_KEY_SPACE) return "space";
	if(a == GLFW_KEY_ESC) return "esc";
	if(a == GLFW_KEY_F1) return "f1";
	if(a == GLFW_KEY_F2) return "f2";
	if(a == GLFW_KEY_F3) return "f3";
	if(a == GLFW_KEY_F4) return "f4";
	if(a == GLFW_KEY_F5) return "f5";
	if(a == GLFW_KEY_F6) return "f6";
	if(a == GLFW_KEY_F7) return "f7";
	if(a == GLFW_KEY_F8) return "f8";
	if(a == GLFW_KEY_F9) return "f9";
	if(a == GLFW_KEY_F10) return "f10";
	if(a == GLFW_KEY_F11) return "f11";
	if(a == GLFW_KEY_F12) return "f12";
	if(a == GLFW_KEY_UP) return "up";
	if(a == GLFW_KEY_DOWN) return "down";
	if(a == GLFW_KEY_LEFT) return "left";
	if(a == GLFW_KEY_RIGHT) return "right";
	if(a == GLFW_KEY_LSHIFT) return "left shift";
	if(a == GLFW_KEY_RSHIFT) return "right shift";
	if(a == GLFW_KEY_LCTRL) return "left ctrl";
	if(a == GLFW_KEY_RCTRL) return "right ctrl";
	if(a == GLFW_KEY_LALT) return "left alt";
	if(a == GLFW_KEY_RALT) return "right alt";
	if(a == GLFW_KEY_TAB) return "tab";
	if(a == GLFW_KEY_ENTER) return "enter";
	if(a == GLFW_KEY_BACKSPACE) return "backspace";
	if(a == GLFW_KEY_INSERT) return "insert";
	if(a == GLFW_KEY_DEL) return "delete";
	if(a == GLFW_KEY_PAGEUP) return "page up";
	if(a == GLFW_KEY_PAGEDOWN) return "page down";
	if(a == GLFW_KEY_HOME) return "home";
	if(a == GLFW_KEY_END) return "end";
	if(a == GLFW_KEY_KP_1) return "kp 1";
	if(a == GLFW_KEY_KP_2) return "kp 2";
	if(a == GLFW_KEY_KP_3) return "kp 3";
	if(a == GLFW_KEY_KP_4) return "kp 4";
	if(a == GLFW_KEY_KP_5) return "kp 5";
	if(a == GLFW_KEY_KP_6) return "kp 6";
	if(a == GLFW_KEY_KP_7) return "kp 7";
	if(a == GLFW_KEY_KP_8) return "kp 8";
	if(a == GLFW_KEY_KP_9) return "kp 9";
	if(a == GLFW_KEY_KP_0) return "kp 0";
	if(a == GLFW_KEY_KP_DIVIDE) return "kp divide";
	if(a == GLFW_KEY_KP_MULTIPLY) return "kp multiply";
	if(a == GLFW_KEY_KP_SUBTRACT) return "kp subtract";
	if(a == GLFW_KEY_KP_ADD) return "kp add";
	if(a == GLFW_KEY_KP_DECIMAL) return "kp decimal";
	if(a == GLFW_KEY_KP_EQUAL) return "kp equal";
	if(a == GLFW_KEY_KP_ENTER) return "kp enter";
	return "";
}

char key_int_to_char(int a) {
	if(a < 256) return (char) a;
	return NULL;
}

void GLFWCALL window_key_callback(int key, int action) {
	if(action == GLFW_PRESS) {
		key_pressed_list += ":";
		key_pressed_list += int_to_key_name(key);
	}
}

void GLFWCALL window_char_callback(int key, int action) {
	if(action == GLFW_PRESS)
		char_string += key_int_to_char(key);
}

/* *
keyboard_string()
Returns typed characters since last called.

C++
const char* a = keyboard_string();

Python
a = keyboard_string()
* */
const char* keyboard_string() {
	std::string a = char_string;
	char_string = "";
	return a.c_str();
}

/* *
keyboard_presses()
Returns keys that have been pressed since last called as a string.
Example: ":left:right:space:a"

C++
const char* a = keyboard_presses();

Python
a = keyboard_presses()
* */
const char* keyboard_presses() {
	std::string a = key_pressed_list;
	key_pressed_list = "";
	return a.c_str();
}

/* *
window(int w, int h, bool fullscreen = false)
Open a window, optionally fullscreen. Returns true if the window was able to be opened, else false.
#In order to be able to draw into the window you need to set up the display matrix. For regular 2D this is done with a call to orthographic(). Afterwards, you will call swap() in your main loop.

C++
window(320, 240);

window(320, 240, true); //fullscreen

Python
window(320, 240)

window(320, 240, True) //fullscreen

see:swap, poll
* */
bool window(int x, int y, bool fullscreen, bool resizeable) {
	if(glfw_state == 1 && glfwGetWindowParam(GLFW_OPENED)) {
		if(!fullscreen && !fullscreened) {
			glfwSetWindowSize(x, y);
			glViewport(0, 0, x, y);
			glfwGetWindowSize(&width, &height);
			return true;
		}
		
		glfwCloseWindow();
		
		if(!resizeable)
			glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
		
		if(glfwOpenWindow(x, y, 0, 0, 0, 0, 0, 0, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW)) != GL_TRUE) {
			err("window", "could alter window");
			return false;
		}
	}
	else {
		if(glfw_state == 0)
			graphics();
	
		if(!resizeable)
			glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	
		if(glfwOpenWindow(x, y, 0, 0, 0, 0, 0, 0, (fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW)) != GL_TRUE) {
			err("window", "could not initiate window");
			return false;
		}
	}
	
	GLenum err = glewInit();
	if(err != GLEW_OK)
		note("window", "extentions unsupported");
	
	glfwSetWindowSizeCallback(window_resize_callback);
	glfwSetKeyCallback(window_key_callback);
	glfwSetCharCallback(window_char_callback);
	
	glfwEnable(GLFW_KEY_REPEAT);
	
	glfwSwapInterval(0);
	
	glfwGetWindowSize(&width, &height);
	
	// ...should these be here...
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1.0, 1.0);
	
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
	glfwCloseWindow();
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
	glfwSetWindowTitle(a);
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
	glfwGetWindowSize(&w, &h);
	if(w != width || h != height) {
		glfwGetWindowSize(&width, &height);
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
	if(glfwGetWindowParam(GLFW_OPENED))
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
	if(glfwGetWindowParam(GLFW_ACTIVE))
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
	glfwSleep(a);
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
	GLFWvidmode a;
	glfwGetDesktopMode(&a);
	
	return ibox(a.Width, a.Height);
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
	glfwGetWindowSize(&w, &h);
	
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
	glfwSetWindowPos(x, y);
}

/* *
vsync(bool)
Sync the display refresh to the monitor refresh rate, which is usually around 60, or not.

C++
vsync(false); //framerate unlimited
vsync(true); //framerate 60

Python
vsync(False) #framerate unlimited
vsync(True); #framerate 60
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
screenshot(string)
Save a screenshot to a TGA file.

C++
screenshot("outcome.tga");

Python
screenshot('outcome.tga')
* */
void screenshot(const char* a) {
	if(!SOIL_save_screenshot(a, SOIL_SAVE_TYPE_TGA, 0, 0, width, height))
		err("screenshot", "could not save");
}

/* *
reset_matrix()
Resets the matrix to it's identity.
Usually used if you want to use a different projection besides the one set up by window.

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
}

/* *
perspective(float left, float right, float bottom, float top, float near = 0.1, float far = 1000.0)
Set up a perspective display matrix.

C++
perspective(0.0, 320.0, 0.0, 240.0);

Python
perspective(0.0, 320.0, 0.0, 240.0)

see:push pop get
* */
void perspective(float a, float b, float c, float d, float e, float f) {
	glFrustum(a, b, c, d, e, f);
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

/* *
enable(string, bool = true)
Enable or disable different things:
"texture" - use texure when drawing
"point smooth" - draw points smoothly
"polygon smooth" - draw polygons smoothly
"line smooth" - draw lines smoothly
"blend" - blending, such as with alpha
"depth" - depth testing

C++
enable("texture");

enable("texture", false);

Python
enable('texture')

enable('texture', False)
* */
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
line_stipple(int, string)
Set the stipple pattern of drawn lines multiplied by an integer value.
"dashed"
"dotted"

C++
line_stipple(1, "dashed");

Python
line_stipple(1, 'dashed')
* */
void line_stipple(int a, const char* b) {
	if(b != NULL) {
		if(!strcmp(b, "dotted"))
			glLineStipple(a, 0x0101);
		else if(!strcmp(b, "dashed"))
			glLineStipple(a, 0x00FF);
	}
	else
		glLineStipple(a, 0x1111);
}


/* *
blend_color(float, float, float, float = 1.0)
Set the blending color.
!There is no way to make use of this function.

C++
blend_color(1.0, 1.0, 1.0);

Python
blend_color(1.0, 1.0, 1.0)
* */
void blend_color(float r, float g, float b, float a) {
	glBlendColor(r, g, b, a);
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

C++
color(0.5, 0.5, 0.5);

Python
color(.5, .5, .5)
* */
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

/* *
blend_mode(string)
Set how you want to blend the images onto the window.
"none" - do not blend
"mix" - normal alpha blending
"multipy" - multiplicative blending
"add" - additive blending
"subtract" - subtractive blending

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

see:use_font, paper.write
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
	glPushAttrib(GL_TEXTURE_BIT);
	font_stack.top()->data->Render(b, -1, FTPoint(x, invert_y ? -y : y));
	glPopAttrib();
	if(invert_y)
		glPopMatrix();
}

/* *
background_image(float x = 0.0, float y = 0.0, float s = 1.0)
Draw a background on the window.

C++:
background_image();

Python:
background_image()

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
inline draw functions
Draw functions that are substanually faster when drawing many of the same type, place between calls to begin() and end():
idraw
iquad
imquad
iline
ipoint

C++
begin("point");
ipoint(160.0, 120.0);
end();

Python
begin('point')
ipoint(160.0, 120.0)
end()

See also: begin/end, shapes
* */
void ipoint(float x, float y) {
	glVertex2i(x, y);
}

void iline(float a, float b, float c, float d) {
	glVertex2i(a, b);
	glVertex2i(c, d);
}

/* *
portion(int x, int y, int x2, int y2)
Draw a portion of an image, keeping it's place.
!This function is a coverup waiting for better multitexture support in programs.

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
pixel(int x, int y)
Returns and rgba containing the pixel color on the screen.

C++
rgba a();
a = pixel(0, 0);

Python
a = pixel(0, 0)
* */
rgba pixel(int x, int y) {
	GLfloat c[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, c);
	return rgba(c[0], c[1], c[2]);
}

/* *
mouse_position()
Returns an offset with the mouse position.

C++:
offset a();
a = mouse_position();

Python:
a = mouse_position()

see:move_mouse
* */
offset mouse_position() {
	int x, y;
	glfwGetMousePos(&x, &y);
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
	
	return glfwGetMouseButton(GLFW_MOUSE_BUTTON_1 + a - 1) == GLFW_PRESS;
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
	return glfwGetMouseWheel();
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
