PyObject* _size_callback = NULL;
PyObject* _close_callback = NULL;
PyObject* _dirty_callback = NULL;
PyObject* _focus_callback = NULL;
PyObject* _iconify_callback = NULL;
PyObject* _button_callback = NULL;
PyObject* _key_callback = NULL;
PyObject* _scroll_callback = NULL;
PyObject* _mousemove_callback = NULL;
PyObject* _string_callback = NULL;

void _size_callback_wrap(GLFWwindow w, int x, int y) {
	PyObject* args;
	args = Py_BuildValue("(Oii)", (window*) glfwGetWindowUserPointer(w), x, y);
	PyEval_CallObject(_size_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

int _close_callback_wrap(GLFWwindow w) {
	PyObject* result;
	int ret;
	PyObject* args = Py_BuildValue("(O)", (window*) glfwGetWindowUserPointer(w));
	result = PyEval_CallObject(_close_callback, args);
	Py_DECREF(args);
	ret = PyObject_IsTrue(result) ? 1 : 0;
	if(PyErr_Occurred())
		PyErr_Print();
	return ret;
}

void _dirty_callback_wrap(GLFWwindow w) {
	PyObject* args = Py_BuildValue("(O)", (window*) glfwGetWindowUserPointer(w));
	PyEval_CallObject(_dirty_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

void _focus_callback_wrap(GLFWwindow w, int x) {
	PyObject* args;
	args = Py_BuildValue("(Oi)", (window*) glfwGetWindowUserPointer(w), x);
	PyEval_CallObject(_focus_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

void _iconify_callback_wrap(GLFWwindow w, int x) {
	PyObject* args;
	args = Py_BuildValue("(Oi)", (window*) glfwGetWindowUserPointer(w), x);
	PyEval_CallObject(_iconify_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

void _button_callback_wrap(GLFWwindow w, int x, int y) {
	PyObject* args;
	args = Py_BuildValue("(Osi)", (window*) glfwGetWindowUserPointer(w), int_to_button_name(x), y);
	PyEval_CallObject(_button_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

void _key_callback_wrap(GLFWwindow w, int x, int y) {
	PyObject* args;
	args = Py_BuildValue("(Osi)", (window*) glfwGetWindowUserPointer(w), int_to_key_name(x), y);
	PyEval_CallObject(_key_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

void _scroll_callback_wrap(GLFWwindow w, double x, double y) {
	PyObject* args;
	args = Py_BuildValue("(Oii)", (window*) glfwGetWindowUserPointer(w), (float) x, (float) y);
	PyEval_CallObject(_scroll_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

void _mousemove_callback_wrap(GLFWwindow w, int x, int y) {
	int u, h;
	glfwGetWindowSize(w, &u, &h);
	
	PyObject* args;
	args = Py_BuildValue("(Oii)", (window*) glfwGetWindowUserPointer(w), x, h - y);
	PyEval_CallObject(_mousemove_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

void _string_callback_wrap(GLFWwindow w, int x) {
	PyObject* args;
	args = Py_BuildValue("(OC)", (window*) glfwGetWindowUserPointer(w), x);
	PyEval_CallObject(_string_callback, args);
	Py_DECREF(args);
	if(PyErr_Occurred())
		PyErr_Print();
}

// set callback

void callback(const char* e, PyObject* o) {
	if(!strcmp(e, "size")) {
		if(_size_callback)
			Py_DECREF(_size_callback);
		_size_callback = o;
		Py_INCREF(o);
		glfwSetWindowSizeCallback(o == Py_None ? NULL : _size_callback_wrap);
	}
	else if(!strcmp(e, "close")) {
		if(_close_callback)
			Py_DECREF(_close_callback);
		_close_callback = o;
		Py_INCREF(o);
		glfwSetWindowCloseCallback(o == Py_None ? NULL : _close_callback_wrap);
	}
	else if(!strcmp(e, "dirty")) {
		if(_dirty_callback)
			Py_DECREF(_dirty_callback);
		_dirty_callback = o;
		Py_INCREF(o);
		glfwSetWindowRefreshCallback(o == Py_None ? NULL : _dirty_callback_wrap);
	}
	else if(!strcmp(e, "focus")) {
		if(_focus_callback)
			Py_DECREF(_focus_callback);
		_focus_callback = o;
		Py_INCREF(o);
		glfwSetWindowFocusCallback(o == Py_None ? NULL : _focus_callback_wrap);
	}
	else if(!strcmp(e, "iconify")) {
		if(_iconify_callback)
			Py_DECREF(_iconify_callback);
		_iconify_callback = o;
		Py_INCREF(o);
		glfwSetWindowIconifyCallback(o == Py_None ? NULL : _iconify_callback_wrap);
	}
	else if(!strcmp(e, "button")) {
		if(_button_callback)
			Py_DECREF(_button_callback);
		_button_callback = o;
		Py_INCREF(o);
		glfwSetMouseButtonCallback(o == Py_None ? NULL : _button_callback_wrap);
	}
	else if(!strcmp(e, "key")) {
		if(_key_callback)
			Py_DECREF(_key_callback);
		_key_callback = o;
		Py_INCREF(o);
		glfwSetKeyCallback(o == Py_None ? NULL : _key_callback_wrap);
	}
	else if(!strcmp(e, "scroll")) {
		if(_scroll_callback)
			Py_DECREF(_scroll_callback);
		_scroll_callback = o;
		Py_INCREF(o);
		glfwSetScrollCallback(o == Py_None ? NULL : _scroll_callback_wrap);
	}
	else if(!strcmp(e, "string")) {
		if(_string_callback)
			Py_DECREF(_string_callback);
		_string_callback = o;
		Py_INCREF(o);
		
		glfwSetCharCallback(o == Py_None ? NULL : _string_callback_wrap);
	}
	else if(!strcmp(e, "mousemove")) {
		if(_mousemove_callback)
			Py_DECREF(_mousemove_callback);
		_mousemove_callback = o;
		Py_INCREF(o);
		glfwSetMousePosCallback(o == Py_None ? NULL : _mousemove_callback_wrap);
	}
	else
		err("set_callback", "not understood");
}
