#ifdef WITH_PYTHON
void _size_callback_wrap(GLFWwindow * w, int x, int y) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_size_callback)
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

void _close_callback_wrap(GLFWwindow * w) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_close_callback)
		return;
	
	PyObject* result;
	int ret;
	result = PyEval_CallObject(z->_close_callback, NULL);
	ret = PyObject_IsTrue(result) ? 1 : 0;
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _dirty_callback_wrap(GLFWwindow * w) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_dirty_callback)
		return;
	
	PyEval_CallObject(z->_dirty_callback, NULL);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _enter_callback_wrap(GLFWwindow * w, int x) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_enter_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(O)", x ? Py_True : Py_False);
	PyEval_CallObject(z->_enter_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _focus_callback_wrap(GLFWwindow * w, int x) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_focus_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(O)", x ? Py_True : Py_False);
	PyEval_CallObject(z->_focus_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _iconify_callback_wrap(GLFWwindow * w, int x) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_iconify_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(O)", x ? Py_True : Py_False);
	PyEval_CallObject(z->_iconify_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _button_callback_wrap(GLFWwindow * w, int x, int y, int mod) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_button_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(sO)", int_to_button_name(x), y ? Py_True : Py_False);
	PyEval_CallObject(z->_button_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _key_callback_wrap(GLFWwindow * w, int x, int scancode, int y, int mod) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_key_callback)
		return;
	
	PyObject* args;
	args = Py_BuildValue("(sO)", int_to_key_name(x), y ? Py_True : Py_False);
	PyEval_CallObject(z->_key_callback, args);
	Py_DECREF(args);
	
	if(PyErr_Occurred())
		PyErr_Print();
}

void _scroll_callback_wrap(GLFWwindow * w, double x, double y) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_scroll_callback)
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

void _mousemove_callback_wrap(GLFWwindow * w, double _x, double _y) {
	int x = (int) _x, y = (int) _y;
	
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_mousemove_callback)
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

void _char_callback_wrap(GLFWwindow * w, unsigned int x) {
	window* z = (window*) glfwGetWindowUserPointer(w);
	
	if(!z || !z->_char_callback)
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
	
	std::ostringstream s;
	std::string l;
	int i, c;
	
	const GLFWvidmode * m = glfwGetVideoModes(glfwGetPrimaryMonitor(), &c);
	
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
	
	const GLFWvidmode * a = glfwGetVideoMode(glfwGetPrimaryMonitor());
	
	return glm::ivec2(a->width, a->height);
}

void use(window * w) {
	if(w->win)
		glfwMakeContextCurrent(w->win);
}

void poll() {
	glfwPollEvents();
}
