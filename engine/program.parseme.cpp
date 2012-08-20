shader::shader(const char * a, const char * b, bool isfile) {
	if(glfw_state < 2) {
		id = 0;
		
		err("shader", "window needs to be open");
		return;
	}
	
	if(!strcmp(a, "vertex"))
		id = glCreateShader(GL_VERTEX_SHADER);
	else if(!strcmp(a, "fragment"))
		id = glCreateShader(GL_FRAGMENT_SHADER);
	else {
		id = 0;
		
		err("shader", "invalid type");
		return;
	}
	if(isfile) {
		const char * c = read_file(b);
		
		if(c) {
			glShaderSource(id, 1, &c, NULL);
			
			delete c;
		}
		else
			err("shader", "could not load file");
	}
	else
		glShaderSource(id, 1, &b, NULL);
	
	glCompileShader(id);
	
	GLint compiled;
	
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	
	if(compiled != GL_TRUE) {
		GLchar * infoLog;
		GLint length;
		
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		
		infoLog = new GLchar[length];
		
		glGetShaderInfoLog(id, length, &length, infoLog);
		
		err("shader", "compile", "error");
		std::cout << infoLog << std::endl;
		
		delete infoLog;
	}
}

shader::~shader() {
	if(id)
		glDeleteShader(id);
}

program::program() {
	if(glfw_state < 2) {
		id = 0;
		
		err("program", "window needs to be open");
		return;
	}
	
	id = glCreateProgram();
}

program::~program() {
	glDeleteProgram(id);
}

void program::uniform(const char * a, int b) {
	glUniform1i(glGetUniformLocation(id, a), b);
}

void program::uniform(const char * a, float b) {
	glUniform1f(glGetUniformLocation(id, a), b);
}
/*$ PROGRAM::UNIFORM $*/

void program::uniform(const char * a, glm::${p}${type}${n} m) {
	glUniform${'Matrix' if type == 'mat' else ''}${n}${p if p else 'f'}v(glGetUniformLocation(id, a), 1${', GL_FALSE' if type == 'mat' else ''}, glm::value_ptr(m));
}
/*$ $*/

void program::uniform(const char * a, image * c, int b) {
	GLint act;
	
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_2D, c->id);
	
	uniform(a, b);
}

void program::font(const char * a, int b) {
	if(!font_library)
		setup_font();

	GLint act;
	
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_RECTANGLE, font_library->texture());
	
	uniform(a, b);
}

void program::attribute(int b, const char * a) {
	if(id)
		glBindAttribLocation(id, b, a);
}

void program::attach(shader * a) {
	if(id)
		glAttachShader(id, a->id);
}

void program::link() {
	if(id)
		glLinkProgram(id);
}
