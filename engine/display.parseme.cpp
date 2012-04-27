#ifdef WITH_PYTHON
void _size_callback_wrap(GLFWwindow w, int x, int y) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_size_callback)
		return;
	
	PyObject* args;
	PyObject* p;
	PyObject* GLMivec2Type = PyObject_GetAttrString(PyImport_ImportModule("glm"), "ivec2");
	
	args = Py_BuildValue("(ii)", x, y);
	p = PyObject_CallObject(GLMivec2Type, args);
	Py_DECREF(args);
	
	args = Py_BuildValue("(O)", p);
	PyEval_CallObject(z->_size_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

int _close_callback_wrap(GLFWwindow w) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_close_callback)
		return 1;
	
	PyObject* result;
	int ret;
	result = PyEval_CallObject(z->_close_callback, NULL);
	ret = PyObject_IsTrue(result) ? 1 : 0;
	
	if(PyErr_Occurred())
		PyErr_Print();
	
	return ret;
}

void _dirty_callback_wrap(GLFWwindow w) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_dirty_callback)
		return;
	
	PyEval_CallObject(z->_dirty_callback, NULL);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _enter_callback_wrap(GLFWwindow w, int x) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_enter_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(O)", x ? Py_True : Py_False);
	PyEval_CallObject(z->_enter_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _focus_callback_wrap(GLFWwindow w, int x) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_focus_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(O)", x ? Py_True : Py_False);
	PyEval_CallObject(z->_focus_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _iconify_callback_wrap(GLFWwindow w, int x) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_iconify_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(O)", x ? Py_True : Py_False);
	PyEval_CallObject(z->_iconify_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _button_callback_wrap(GLFWwindow w, int x, int y) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_button_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(sO)", int_to_button_name(x), y ? Py_True : Py_False);
	PyEval_CallObject(z->_button_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _key_callback_wrap(GLFWwindow w, int x, int y) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_key_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(sO)", int_to_key_name(x), y ? Py_True : Py_False);
	PyEval_CallObject(z->_key_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _scroll_callback_wrap(GLFWwindow w, double x, double y) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_scroll_callback)
		return;
	
	PyObject* args;
	PyObject* p;
	PyObject* GLMvec2Type = PyObject_GetAttrString(PyImport_ImportModule("glm"), "vec2");
	
	args = Py_BuildValue("(ff)", (float) x, (float) y);
	p = PyObject_CallObject(GLMvec2Type, args);
	Py_DECREF(args);
	
	args = Py_BuildValue("(O)", p);
	PyEval_CallObject(z->_scroll_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _mousemove_callback_wrap(GLFWwindow w, int x, int y) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_mousemove_callback)
		return;
	
	int u, h;
	glfwGetWindowSize(w, &u, &h);
	y = h - y;
	
	PyObject* args;
	PyObject* p;
	
	PyObject* GLMivec2Type = PyObject_GetAttrString(PyImport_ImportModule("glm"), "ivec2");
	
	args = Py_BuildValue("(ii)", x, y);
	p = PyObject_CallObject(GLMivec2Type, args);
	Py_DECREF(args);
	
	args = Py_BuildValue("(O)", p);
	PyEval_CallObject(z->_mousemove_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _char_callback_wrap(GLFWwindow w, int x) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z->_char_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(C)", x);
	PyEval_CallObject(z->_char_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}
#endif

void glfw_error(int code, const char* string) {
}

bool graphics() {
	glfwSetErrorCallback(glfw_error);
	
	if(glfwInit() != GL_TRUE) {
		err("graphics", "could not");
		return false;
	}
	
/*$ CALLBACK $*/
	glfwSet${g}Callback(_${n}_callback_wrap);
/*$ $*/
	
	FreeImage_Initialise();
		
	glfw_state = 1;
	return true;
}

void graphics_off() {
	FreeImage_DeInitialise();
	glfwTerminate();
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

void use(window* w) {
	if(w->win)
		glfwMakeContextCurrent(w->win);
}

void swap() {
	glfwSwapBuffers();
}

void poll() {
	glfwPollEvents();
}

void viewport(glm::ivec2 a, glm::ivec2 b) {
	glViewport(a.x, a.y, b.x, b.y);
}

void viewport(glm::ivec4 a) {
	glViewport(a.x, a.y, a.z, a.w);
}

void scissor(glm::ivec2 a, glm::ivec2 b) {
	glScissor(a.x, a.y, b.x, b.y);
}

void scissor(glm::ivec4 a) {
	glScissor(a.x, a.y, a.z, a.w);
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
	if(!strcmp(a, "vsync")) {
		glfwSwapInterval(b ? 1 : 0);
		return;
	}
	
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

void color() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void color(glm::vec4 a) {
	glClearColor(a.r, a.g, a.b, a.a);
}

void color(const char* a) {
	display_set_blend_mode_from_string(a);
}

void color(bool r, bool g, bool b, bool a) {
	glColorMask(r, g, b, a);
}

void color(bool a) {
	glColorMask(a, a, a, a);
}

void depth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void depth(const char* a) {
	glDepthFunc(comparison_string_to_gl(a));
}

void depth(bool a) {
	glDepthMask(a ? GL_TRUE : GL_FALSE);
}

void depth_offset(float a, float b) {
	glPolygonOffset(a, b);
}

void  point_size(float a) {
	glPointSize(a);
}

void  line_width(float a) {
	glLineWidth(a);
}

void use(fbo* a) {
	glBindFramebuffer(GL_FRAMEBUFFER, a->id);
}

void use(vao* a) {
	glBindVertexArray(a->id);
}

void use(program* a) {
	glUseProgram(a->id);
}

void use() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void stencil() {
	glClear(GL_STENCIL_BUFFER_BIT);
}

void stencil(int a) {
	glClearStencil(a);
}

void stencil(const char* a, int b) {
	GLenum e = comparison_string_to_gl(a);

	if(e)
		glStencilFunc(e, b, 1);
	else {
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
		else if(!strcmp(a, "invert"))
			f = GL_INVERT;
		else if(!strcmp(a, "increment wrap"))
			f = GL_INCR_WRAP;
		else if(!strcmp(a, "decrement wrap"))
			f = GL_DECR_WRAP;
		else {
			err("stencil", "unknown");
			return;
		}
	
		glStencilOp(f, f, f);
	}
}

void draw(const char* a, unsigned int count, unsigned int first) {
	glDrawArrays(primitive_from_string(a), first, count);
}
