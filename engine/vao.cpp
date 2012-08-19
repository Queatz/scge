vao::vao() {
	GLint last;
	
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	
	glGenVertexArrays(1, &id);
	
	glBindVertexArray(last);
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

void vao::attribute(unsigned int index, vbo * v, const char * type, int size, int offset, int stride, bool normalized, bool integers) {
	GLint last, lastb;
	
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastb);
	
	if(last != id)
		glBindVertexArray(id);
	
	if(lastb != v->id)
		glBindBuffer(GL_ARRAY_BUFFER, v->id);
	
	if(integers)
		glVertexAttribIPointer(index, size, common_type_from_string(type), (GLsizei)stride, reinterpret_cast<const GLvoid *>(offset));
	else
		glVertexAttribPointer(index, size, common_type_from_string(type), normalized, (GLsizei)stride, reinterpret_cast<const GLvoid *>(offset));
	
	if(lastb != v->id)
		glBindBuffer(GL_ARRAY_BUFFER, lastb);
	
	if(last != id)
		glBindVertexArray(last);
}
