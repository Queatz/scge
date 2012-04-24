vbo::vbo(int l, const char* t) {
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	glGenBuffers(1, &id);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, l, NULL, buffer_usage_from_string(t));
	
	glBindBuffer(GL_ARRAY_BUFFER, last);
	
	size = l;
}

void vbo::data(const void* data, int offset, int size) {
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
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	glGenBuffers(1, &id);
	
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)), buffer_usage_from_string(t));
	
	glBindBuffer(GL_ARRAY_BUFFER, last);
	
	size = PyBytes_Size(o);
}

void vbo::data(PyObject* o, int offset) {
	GLint last;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ARRAY_BUFFER, offset, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)));
	
	if(last != id)
		glBindBuffer(GL_ARRAY_BUFFER, last);
}
#endif

void vbo::data(vbo* o, int offset, int copy_offset, int l) {
	glBindBuffer(GL_COPY_READ_BUFFER, o->id);
	glBindBuffer(GL_COPY_WRITE_BUFFER, id);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset, copy_offset, l >= 0 ? l : o->size);
}

vbo::~vbo() {
	glDeleteBuffers(1, &id);
}
