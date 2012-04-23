GLFWwindow glfw_window = NULL;
int glfw_state = 0;
Shikoba::Library * font_library = NULL;

void setdown_font() {
	delete font_library;
}

void setup_font() {
	if(glfw_state == 0)
		graphics();
	font_library = new Shikoba::Library();
	atexit(setdown_font);
}

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

FREE_IMAGE_FORMAT fif_from_string(const char* a = NULL, const char* b = NULL) {
	if(b) {
		if(!strcmp(b, "png"))
			return FIF_PNG;
		else if(!strcmp(b, "jpg"))
			return FIF_JPEG;
		else if(!strcmp(b, "bmp"))
			return FIF_BMP;
	}
	
	if(a) {
		FREE_IMAGE_FORMAT t = FreeImage_GetFileType(a, 0);
	
		if(t == FIF_UNKNOWN)
			t = FreeImage_GetFIFFromFilename(a);
		return t;
	}
	
	return FIF_UNKNOWN;
}

int default_from_fif(FREE_IMAGE_FORMAT a) {
	if(a == FIF_PNG)
		return PNG_DEFAULT;
	else if(a == FIF_JPEG)
		return JPEG_QUALITYSUPERB;
	else if(a == FIF_BMP)
		return BMP_DEFAULT;
	else
		return 0;
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
	if(w->win) {
		glfw_window = w->win;
		glfwMakeContextCurrent(w->win);
	}
}

void vsync(bool a) {	
	glfwSwapInterval(a ? 1 : 0);
}

void swap() {
	glfwSwapBuffers();
}

void poll() {
	glfwPollEvents();
}

bool screenshot(const char* a, const char* b) {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);

	
	pixelcache p(w, h);
	
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, p.data);
	
	if (p.save(a, b))
		return true;
	err("screenshot", "could not save");
	return false;

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

void clear_color(float a, float b, float c, float d) {
	glClearColor(a, b, c, d);
}

void clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void depth_clear() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

GLenum comparison_string_to_gl(const char* a) {
	if(!strcmp(a, "=="))
		return GL_EQUAL;
	else if(!strcmp(a, "!="))
		return GL_NOTEQUAL;
	else if(!strcmp(a, "<"))
		return GL_LESS;
	else if(!strcmp(a, ">"))
		return GL_GREATER;
	else if(!strcmp(a, "<="))
		return GL_LEQUAL;
	else if(!strcmp(a, ">="))
		return GL_GEQUAL;
	else if(!strcmp(a, "always"))
		return GL_ALWAYS;
	else if(!strcmp(a, "never"))
		return GL_NEVER;
	
	return 0;
}

void depth_test(const char* a) {
	glDepthFunc(comparison_string_to_gl(a));
}

void depth_op(bool a) {
	glDepthMask(a ? GL_TRUE : GL_FALSE);
}

void  point_size(float a) {
	glPointSize(a);
}

void  line_width(float a) {
	glLineWidth(a);
}

void polygon_depth_offset(float a, float b) {
	glPolygonOffset(a, b);
}

void color_mask(bool r, bool g, bool b, bool a) {
	glColorMask(r, g, b, a);
}

void color_mask(bool a) {
	glColorMask(a, a, a, a);
}

void use_fbo(fbo* a) {
	glBindFramebuffer(GL_FRAMEBUFFER, a->id);
}

void use_fbo() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void use_vbo(vbo* a) {
	glBindBuffer(GL_ARRAY_BUFFER, a->id);
}

void use_vbo() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void use_vao(vao* a) {
	glBindVertexArray(a->id);
}

void use_vao() {
	glBindVertexArray(0);
}

void use_ibo(ibo* a) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a->id);
}

void use_ibo() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void scissor(int a, int b, int c, int d) {
	glScissor(a, b, c, d);
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

void blend_mode(const char* a) {
	display_set_blend_mode_from_string(a);
}

void use_program(program* a) {
	glUseProgram(a->id);
}

void use_program() {
	glUseProgram(0);
}

GLenum primitive_from_string(const char* a) {
		if (!strcmp(a, "triangle"))
			return GL_TRIANGLES;
		else if (!strcmp(a, "triangle strip"))
			return GL_TRIANGLE_STRIP;
		else if (!strcmp(a, "triangle fan"))
			return GL_TRIANGLE_FAN;
		else if (!strcmp(a, "line"))
			return GL_LINES;
		else if (!strcmp(a, "line strip"))
			return GL_LINE_STRIP;
		else if (!strcmp(a, "line loop"))
			return GL_LINE_LOOP;
		else if (!strcmp(a, "point"))
			return GL_POINTS;
		else
			err("primitive", "unknown type");
		return 0;
}

glm::vec4 pixel(int x, int y, const char* f) {
	int w, h;
	glfwGetWindowSize(glfw_window, &w, &h);
	
	if(x < 0 || x >= w || y < 0 || y >= h)
		return glm::vec4(0.0, 0.0, 0.0, 1.0);
	
	GLenum fmt = GL_RGB;
	if(w) {
		if(!strcmp(f, "depth"))
			fmt = GL_DEPTH_COMPONENT;
		else if(!strcmp(f, "stencil"))
			fmt = GL_STENCIL_INDEX;
	}
	
	GLfloat c[3];
	glReadPixels(x, y, 1, 1, fmt, GL_FLOAT, c);
	return glm::vec4(c[0], c[1], c[2], 1.0);
}

void stencil_clear(int a) {
	glClearStencil(a);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void stencil_test(const char* a, int b) {
	glStencilFunc(comparison_string_to_gl(a), b, 1);
}

void stencil_op(const char* a) {
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
	
	glStencilOp(f, f, f);
}

pixelcache::pixelcache(const char* a) {
	FIBITMAP* bm = FreeImage_Load(fif_from_string(a), a, 0);
	
	if(!bm) {
		err("pixelcache", "could not load");
		return;
	}
	
	colors = FreeImage_IsTransparent(bm) ? 4 : 3;
	
	BYTE *bits = FreeImage_GetBits(bm);
	int bpp = FreeImage_GetBPP(bm);
	
	width = FreeImage_GetWidth(bm);
	height = FreeImage_GetHeight(bm);
	
	int i, j, s = height*width;
	RGBQUAD pix;
	data = new GLubyte[s*colors];
	for(i = 0; i < width; i++)
	for(j = 0; j < height; j++) {
		FreeImage_GetPixelColor(bm, i, j, &pix);
		data[(j*width+i)*colors] = pix.rgbRed;
		data[(j*width+i)*colors+1] = pix.rgbGreen;
		data[(j*width+i)*colors+2] = pix.rgbBlue;
		if(colors == 4)
			data[(j*width+i)*colors+3] = pix.rgbReserved;
	}
	
	FreeImage_Unload(bm);
}

pixelcache::pixelcache(const pixelcache& a) {
	width = a.width;
	height = a.height;
	colors = a.colors;
	
	int s = height*width*colors;
	data = new GLubyte[s];
	memcpy(data, a.data, sizeof(GLubyte)*s);
}

pixelcache::pixelcache(int w, int h, bool a) {
	width = w;
	height = h;
	colors = a ? 4 : 3;
	data = new GLubyte[height*width*colors];
}

pixelcache::~pixelcache() {
	if(data)
		delete data;
}

glm::vec4 pixelcache::pixel(int x, int y) {
	int h;
	
	if(x < 0 || x >= width || y < 0 || y >= height)
		return glm::vec4(0.0, 0.0, 0.0, colors == 4 ? 0.0 : 1.0);
	
	h = y * width * colors + x * colors;
	return glm::vec4(static_cast<float>(data[h + 0]) / 255.0, static_cast<float>(data[h + 1]) / 255.0, static_cast<float>(data[h + 2]) / 255.0, colors == 4 ? static_cast<float>(data[h + 3]) / 255.0 : 1.0);
}

void pixelcache::set_pixel(int x, int y, glm::vec4 c) {
	int h;
	if(x < 0 || x >= width || y < 0 || y >= height) {
		err("pixelcache", "set_pixel", "out of bounds");
		return;
	}
	
	h = y * width * colors + x * colors;
	data[h + 0] = static_cast<GLubyte>(c.r * 255.0);
	data[h + 1] = static_cast<GLubyte>(c.g * 255.0);
	data[h + 2] = static_cast<GLubyte>(c.b * 255.0);
	if(colors == 4)
		data[h + 3] = static_cast<GLubyte>(c.a * 255.0);
}

bool pixelcache::save(const char* a, const char* b) {
	if(!data)
		return false;
	
	FIBITMAP* c = FreeImage_Allocate(width, height, colors == 4 ? 32 : 24);
	RGBQUAD color;
	
	for(unsigned int x = 0; x < width; x++)
		for(unsigned int y = 0; y < height; y++) {
			int h = y * width * colors + x * colors;
			color.rgbRed = data[h + 0];
			color.rgbGreen = data[h + 1];
			color.rgbBlue = data[h + 2];
			if(colors == 4)
				color.rgbReserved = data[h + 3];
			FreeImage_SetPixelColor(c, x, y, &color);
		}
	
	if (FreeImage_Save(fif_from_string(a, b), c, a, default_from_fif(fif_from_string(a, b))))
		return true;
	return false;
	
	FreeImage_Unload(c);
}

image::image(const char* a, bool m) {
	if(glfw_state == 0)
		graphics();

	FIBITMAP *bm = FreeImage_Load(fif_from_string(a), a, 0);
	
	if(!bm) {
		err("image", "could not load");
		return;
	}
	
	bool has_alpha = FreeImage_IsTransparent(bm);
	
	FIBITMAP *tmp = bm;
	if(has_alpha)
		bm = FreeImage_ConvertTo32Bits(bm);
	else
		bm = FreeImage_ConvertTo24Bits(bm);
	FreeImage_Unload(tmp);
	
	
	BYTE *bits = new BYTE[FreeImage_GetHeight(bm) * FreeImage_GetPitch(bm)];
	
	FreeImage_ConvertToRawBits(bits, bm, FreeImage_GetPitch(bm), (has_alpha ? 32 : 24), FI_RGBA_BLUE_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_RED_MASK, FALSE);
	
	GLint last;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last);
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(bm), FreeImage_GetHeight(bm), 0, (has_alpha ? GL_BGRA : GL_BGR), GL_UNSIGNED_BYTE, bits);
	
	if(m)
		glGenerateMipmap(GL_TEXTURE_2D);
	
	FreeImage_Unload(bm);
	
	external_cache = false;
	cache = NULL;
	mipmaps = m;
	
	if(id == 0) {
		err("image", "could not load");
		return;
	}
	
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	
	if(m) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	
	glBindTexture(GL_TEXTURE_2D, last);
}

image::image(int a, int b, bool alpha, bool quality) {
	if(glfw_state == 0)
		graphics();
	
	cache = NULL;
	mipmaps = false;
	external_cache = false;
	
	width = a;
	height = b;
	
	GLint last;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last);
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? (quality ? GL_RGBA12 : GL_RGBA8) : (quality ? GL_RGB12 : GL_RGB8), a, b, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, last);
}

image::image(int a, int b, const char* f) {
	if(glfw_state == 0)
		graphics();
	
	cache = NULL;
	mipmaps = false;
	external_cache = false;
	
	width = a;
	height = b;
	
	GLint last;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last);
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	GLenum fmt, ifmt;
	if(!strcmp(f, "r")) {
		fmt = GL_RED;
		ifmt = GL_R8;
	}
	else if(!strcmp(f, "rg")) {
		fmt = GL_RG;
		ifmt = GL_RG8;
	}
	else if(!strcmp(f, "rgb")) {
		fmt = GL_RGB;
		ifmt = GL_RGB8;
	}
	else if(!strcmp(f, "rgba")) {
		fmt = GL_RGBA;
		ifmt = GL_RGBA8;
	}
	else if(!strcmp(f, "depth")) {
		fmt = GL_DEPTH_COMPONENT;
		ifmt = GL_DEPTH_COMPONENT24;
	}
	else if(!strcmp(f, "depth stencil")) {
		fmt = GL_DEPTH_STENCIL;
		ifmt = GL_DEPTH24_STENCIL8;
	}
	else {
		err("image", "invalid format");
		fmt = GL_RED;
		ifmt = GL_R8;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, ifmt, a, b, 0, fmt, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, last);
}

image::image(pixelcache* p) {
	if(glfw_state == 0)
		graphics();
	
	mipmaps = false;
	external_cache = true;
	cache = p;
	
	width = p->width;
	height = p->height;
	colors = p->colors;
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, p->colors == 4 ? GL_RGBA8 : GL_RGB8, width, height, 0, p->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	from_pixelcache();
}

image::~image() {
	if(!external_cache && cache)
		delete cache;
}

void image::set(const char* a) {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	if (!strcmp(a, "linear")) {
		if(mipmaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else if (!strcmp(a, "nearest")) {
		if(mipmaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}  else if (!strcmp(a, "nearest mipmap")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else if (!strcmp(a, "mirrored repeat")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	} else if (!strcmp(a, "repeat")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else if (!strcmp(a, "clamp to edge")) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
		err("image", "set", "invalid value");
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(pixelcache* a) {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, a->data);
	
	// Reset to the original texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(pixelcache* a, int x, int y, int w, int h) {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, a->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache() {
	if(!cache) {
		err("image", "from_pixelcache", "no cache");
		return;
	}
	
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, cache->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(int x, int y, int w, int h) {
	if(!cache) {
		err("image", "from_pixelcache", "no cache");
		return;
	}
	
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, cache->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::refresh_pixel_cache() {
	GLint bind;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	if(!cache)
		cache = new pixelcache(width, height);
	
	glGetTexImage(GL_TEXTURE_2D, 0, cache->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::discard_pixel_cache() {
	if(!external_cache && cache)
		delete cache;
	cache = NULL;
}

glm::vec4 image::pixel(int x, int y) {
	if(!cache)
		refresh_pixel_cache();
	
	return cache->pixel(x, y);
}

bool image::save(const char* a, const char* b) {
	if(!cache)
		refresh_pixel_cache();
	
	return cache->save(a, b);
}

fontface::fontface(const char* a) {
	if(font_library == NULL)
		setup_font();
	
	data = new Shikoba::Face(font_library, a);
}

fontface::~fontface() {
	delete data;
}

void font_face(fontface* f) {
	if(font_library == NULL)
		setup_font();

	font_library->setFace(f->data);
}

void font_size(unsigned int s) {
	if(font_library == NULL)
		setup_font();

	font_library->setSize(s);
}

float line_height() {
	if(font_library == NULL)
		setup_font();

	return font_library->height();
}

float ascent() {
	if(font_library == NULL)
		setup_font();

	return font_library->ascender();
}

float descent() {
	if(font_library == NULL)
		setup_font();

	return font_library->descender();
}

float advance(const char* a, const char* b) {
	if(font_library == NULL)
		setup_font();

	return font_library->advance(utf8::unchecked::next(a), utf8::unchecked::next(b));
}

glyphmetrics glyph(const char* a) {
	if(font_library == NULL)
		setup_font();

	glyphmetrics g;
	const Shikoba::Glyph * s = font_library->glyph(utf8::unchecked::next(a));

	g.vertices.x1 = s->vertices.x1;
	g.vertices.x2 = s->vertices.x2;
	g.vertices.y1 = s->vertices.y1;
	g.vertices.y2 = s->vertices.y2;
	g.texcoords.x1 = s->texcoords.x1;
	g.texcoords.x2 = s->texcoords.x2;
	g.texcoords.y1 = s->texcoords.y1;
	g.texcoords.y2 = s->texcoords.y2;

	return g;
}

shader::shader(const char* a, const char* b, bool isfile) {
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
		const char* c = read_file(b);
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
	
	int compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	
	if(compiled != GL_TRUE) {
		GLchar* infoLog;
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

void program::uniform(const char* a, int b) {
	glUniform1i(glGetUniformLocation(id, a), b);
}

void program::uniform(const char* a, float b) {
	glUniform1f(glGetUniformLocation(id, a), b);
}
/*$ PROGRAM::UNIFORM $*/

void program::uniform(const char* a, glm::${p}${type}${n} m) {
	glUniform${'Matrix' if type == 'mat' else ''}${n}${p if p else 'f'}v(glGetUniformLocation(id, a), 1${', GL_FALSE' if type == 'mat' else ''}, glm::value_ptr(m));
}
/*$ $*/

void program::uniform(const char* a, image* c, int b) {
	GLint act;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_2D, c->id);
	
	uniform(a, b);
	
	// Reset to the original texture XXX FIXME don't have to?
	if(act != GL_TEXTURE0 + b) glActiveTexture(act);
}

void program::bind_font(const char* a, int b) {
	if(!font_library)
		setup_font();

	GLint act;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
	glActiveTexture(GL_TEXTURE0 + b);
	glBindTexture(GL_TEXTURE_2D, font_library->texture());
	
	uniform(a, b);
	
	// Reset to the original texture XXX FIXME don't have to?
	if(act != GL_TEXTURE0 + b) glActiveTexture(act);
}

void program::attribute(int b, const char* a) {
	if(id)
		glBindAttribLocation(id, b, a);
}

void program::attach(shader* a) {
	if(id)
		glAttachShader(id, a->id);
}

void program::link() {
	if(id)
		glLinkProgram(id);
}

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

fbo::fbo(int b, int c, bool alpha, bool quality, bool ds, int multisample) {
	if(glfw_state == 0)
		graphics();
	
	glGenFramebuffers(1, &id);
	
	GLint last, lastt;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastt);
	
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	
	buffer_is_mine = true;
	buffer = new image(b, c, alpha, quality);
	
	glBindTexture(GL_TEXTURE_2D, buffer->id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->id, 0);
	
	if(ds) {
		depth = new image(b, c, "depth");
	
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

GLenum buffer_usage_from_string(const char* t) {
	if(!strcmp(t, "static draw"))
		return GL_STATIC_DRAW;
	else if(!strcmp(t, "static read"))
		return GL_STATIC_READ;
	else if(!strcmp(t, "static copy"))
		return GL_STATIC_COPY;
	else if(!strcmp(t, "stream draw"))
		return GL_STREAM_DRAW;
	else if(!strcmp(t, "stream read"))
		return GL_STREAM_READ;
	else if(!strcmp(t, "stream copy"))
		return GL_STREAM_COPY;
	else if(!strcmp(t, "dynamic draw"))
		return GL_DYNAMIC_DRAW;
	else if(!strcmp(t, "dynamic read"))
		return GL_DYNAMIC_READ;
	else if(!strcmp(t, "dynamic copy"))
		return GL_DYNAMIC_COPY;
	return GL_STATIC_DRAW;
}

GLenum common_type_from_string(const char* a) {
	if(!strcmp(a, "float"))
		return GL_FLOAT;
	else if(!strcmp(a, "byte"))
		return GL_BYTE;
	else if(!strcmp(a, "unsigned byte"))
		return GL_UNSIGNED_BYTE;
	else if(!strcmp(a, "short"))
		return GL_SHORT;
	else if(!strcmp(a, "unsigned short"))
		return GL_UNSIGNED_SHORT;
	else if(!strcmp(a, "int"))
		return GL_INT;
	else if(!strcmp(a, "unsigned int"))
		return GL_UNSIGNED_INT;
	else if(!strcmp(a, "double"))
		return GL_DOUBLE;
	else {
		err("type", "invalid");
		return -1;
	}
}

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

void draw(const char* a, unsigned int count, unsigned int first) {
	glDrawArrays(primitive_from_string(a), first, count);
}

ibo::ibo(int l, const char* s, const char* t) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	glGenBuffers(1, &id);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, l, NULL, buffer_usage_from_string(t));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
	
	storage = common_type_from_string(s);
	
	size = l;
}

void ibo::data(const void* data, int offset, int size) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
}

ibo::~ibo() {
	glDeleteBuffers(1, &id);
}

#ifdef WITH_PYTHON
ibo::ibo(PyObject* o, const char* s, const char* t) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	glGenBuffers(1, &id);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)), buffer_usage_from_string(t));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
	
	storage = common_type_from_string(s);
	size = PyBytes_Size(o);
}

void ibo::data(PyObject * o, int offset) {
	GLint last;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last);
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, PyBytes_Size(o), (const GLvoid *)const_cast<const char*>(PyBytes_AsString(o)));
	
	if(last != id)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last);
}
#endif

void ibo::data(ibo* o, int offset, int copy_offset, int l) {
	glBindBuffer(GL_COPY_READ_BUFFER, o->id);
	glBindBuffer(GL_COPY_WRITE_BUFFER, id);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset, copy_offset, l >= 0 ? l : o->size);
}

void ibo::draw(const char* a, unsigned int count, unsigned int first) {
	glDrawElements(primitive_from_string(a), (GLsizei)count, storage, reinterpret_cast<const GLvoid*>(first));
}

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

void vao::attribute(unsigned int index, vbo* v, const char* type, int size, int offset, int stride, bool normalized, bool integers) {
	GLint last, lastb;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastb);
	
	if(last != id)
		glBindVertexArray(id);
	if(lastb != v->id)
		glBindBuffer(GL_ARRAY_BUFFER, v->id);
	
	if(integers)
		glVertexAttribIPointer(index, size, common_type_from_string(type), (GLsizei)stride, reinterpret_cast<const GLvoid*>(offset));
	else
		glVertexAttribPointer(index, size, common_type_from_string(type), normalized, (GLsizei)stride, reinterpret_cast<const GLvoid*>(offset));
	
	if(lastb != v->id)
		glBindBuffer(GL_ARRAY_BUFFER, lastb);
	if(last != id)
		glBindVertexArray(last);
}
