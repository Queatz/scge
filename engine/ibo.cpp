ibo::ibo(int l, const char * s, const char * t) {
	glGenBuffers(1, &id);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, l, NULL, buffer_usage_from_string(t));
	
	storage = common_type_from_string(s);
	size = l;
}

void ibo::data(const void * data, int offset, int size) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

ibo::~ibo() {
	glDeleteBuffers(1, &id);
}

#ifdef WITH_PYTHON
ibo::ibo(PyObject * o, const char * s, const char * t) {
	glGenBuffers(1, &id);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PyBytes_Size(o), (const GLvoid *)const_cast<const char *>(PyBytes_AsString(o)), buffer_usage_from_string(t));
	
	storage = common_type_from_string(s);
	size = PyBytes_Size(o);
}

void ibo::data(PyObject * o, int offset) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, PyBytes_Size(o), (const GLvoid *)const_cast<const char *>(PyBytes_AsString(o)));
}
#endif

void ibo::data(ibo * o, int offset, int copy_offset, int l) {
	glBindBuffer(GL_COPY_READ_BUFFER, o->id);
	glBindBuffer(GL_COPY_WRITE_BUFFER, id);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset, copy_offset, l >= 0 ? l : o->size);
}

void ibo::draw(const char * a, unsigned int count, unsigned int first) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_BINDING, id);
	glDrawElements(primitive_from_string(a), (GLsizei)count, storage, reinterpret_cast<const GLvoid *>(first));
}
