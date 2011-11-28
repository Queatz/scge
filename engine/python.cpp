#include "../engine/internal.cpp"

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
	args = Py_BuildValue("(ii)", x, y);
	PyEval_CallObject(_size_callback, args);
	Py_DECREF(args);
}

int _close_callback_wrap(GLFWwindow w) {
	PyObject* result;
	int ret;
	result = PyEval_CallObject(_close_callback, (PyObject *)NULL);
	ret = PyObject_IsTrue(result) ? 1 : 0;
	return ret;
}

void _dirty_callback_wrap(GLFWwindow w) {
	PyEval_CallObject(_focus_callback, (PyObject *)NULL);
}

void _focus_callback_wrap(GLFWwindow w, int x) {
	PyObject* args;
	args = Py_BuildValue("(i)", x);
	PyEval_CallObject(_focus_callback, args);
	Py_DECREF(args);
}

void _iconify_callback_wrap(GLFWwindow w, int x) {
	PyObject* args;
	args = Py_BuildValue("(i)", x);
	PyEval_CallObject(_iconify_callback, args);
	Py_DECREF(args);
}

void _button_callback_wrap(GLFWwindow w, int x, int y) {
	PyObject* args;
	args = Py_BuildValue("(si)", int_to_button_name(x), y);
	PyEval_CallObject(_button_callback, args);
	Py_DECREF(args);
}

void _key_callback_wrap(GLFWwindow w, int x, int y) {
	PyObject* args;
	args = Py_BuildValue("(si)", int_to_key_name(x), y);
	PyEval_CallObject(_key_callback, args);
	Py_DECREF(args);
}

void _scroll_callback_wrap(GLFWwindow w, int x, int y) {
	PyObject* args;
	args = Py_BuildValue("(i)", x);
	PyEval_CallObject(_scroll_callback, args);
	Py_DECREF(args);
}

void _mousemove_callback_wrap(GLFWwindow w, int x, int y) {
	PyObject* args;
	args = Py_BuildValue("(ii)", x, y);
	PyEval_CallObject(_mousemove_callback, args);
	Py_DECREF(args);
}

void _string_callback_wrap(GLFWwindow w, int x) {
	PyObject* args;
	args = Py_BuildValue("(s)", x);
	PyEval_CallObject(_string_callback, args);
	Py_DECREF(args);
}

// set_callback

/* * Python
set_callback(string, function)
Registers a python object as a callback.
'size' (int, int)
'close' () - return True to allow close, False to disallow
'dirty' ()
'focus' (int)
'iconify' (int)
'button' (string, int)
'key' (string, int)
'string' (string)
'mousemove' (int, int)

Python
def dirty(): pass
	
set_callback('dirty', dirty)

* */

void set_callback(const char* e, PyObject* o) {
	if(!strcmp(e, "size")) {
		_size_callback = o;
		glfwSetWindowSizeCallback(o == Py_None ? NULL : _size_callback_wrap);
	}
	else if(!strcmp(e, "close")) {
		_close_callback = o;
		glfwSetWindowCloseCallback(o == Py_None ? NULL : _close_callback_wrap);
	}
	else if(!strcmp(e, "dirty")) {
		_dirty_callback = o;
		glfwSetWindowRefreshCallback(o == Py_None ? NULL : _dirty_callback_wrap);
	}
	else if(!strcmp(e, "focus")) {
		_focus_callback = o;
		glfwSetWindowFocusCallback(o == Py_None ? NULL : _focus_callback_wrap);
	}
	else if(!strcmp(e, "iconify")) {
		_iconify_callback = o;
		glfwSetWindowIconifyCallback(o == Py_None ? NULL : _iconify_callback_wrap);
	}
	else if(!strcmp(e, "button")) {
		_button_callback = o;
		glfwSetMouseButtonCallback(o == Py_None ? NULL : _button_callback_wrap);
	}
	else if(!strcmp(e, "key")) {
		_key_callback = o;
		glfwSetKeyCallback(o == Py_None ? NULL : _key_callback_wrap);
	}
	else if(!strcmp(e, "scroll")) {
		_scroll_callback = o;
		glfwSetScrollCallback(o == Py_None ? NULL : _scroll_callback_wrap);
	}
	else if(!strcmp(e, "string")) {
		_string_callback = o;
		glfwSetCharCallback(o == Py_None ? NULL : _string_callback_wrap);
	}
	else if(!strcmp(e, "mousemove")) {
		_mousemove_callback = o;
		glfwSetMousePosCallback(o == Py_None ? NULL : _mousemove_callback_wrap);
	}
	else
		err("set_callback", "not understood");
}
