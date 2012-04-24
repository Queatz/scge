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

void viewport(int x, int y, int w, int h) {
	glViewport(x, y, w, h);
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

void scissor(int a, int b, int c, int d) {
	glScissor(a, b, c, d);
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
