fbo::fbo(image* a) {
	if(glfw_state == 0)
		graphics();
	
	GLint last, lastt;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastt);
	
	glGenFramebuffers(1, &id);
	
	depth = NULL;
	buffer = a;
	buffer_is_mine = false;
	
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	
	glBindTexture(GL_TEXTURE_2D, buffer->id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->id, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, lastt);
	glBindFramebuffer(GL_FRAMEBUFFER, last);
}

fbo::fbo(glm::ivec2 s, bool alpha, bool quality, bool ds, int multisample) {
	if(glfw_state == 0)
		graphics();
	
	glGenFramebuffers(1, &id);
	
	GLint last, lastt;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastt);
	
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	
	buffer_is_mine = true;
	buffer = new image(s, alpha, quality);
	
	glBindTexture(GL_TEXTURE_2D, buffer->id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->id, 0);
	
	if(ds) {
		depth = new image(s, "depth");
	
		glBindTexture(GL_TEXTURE_2D, depth->id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->id, 0);
	}
	else {
		depth = NULL;
	}
	
	glBindTexture(GL_TEXTURE_2D, lastt);
	glBindFramebuffer(GL_FRAMEBUFFER, last);
}

fbo::~fbo() {
	if(buffer_is_mine)
		delete buffer;
	if(depth)
		delete depth;
	glDeleteFramebuffers(1, &id);
}
