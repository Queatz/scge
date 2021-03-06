image::image(const char * a, bool m) {
	GLint last;
	
	if(glfw_state == 0)
		graphics();

	FIBITMAP * bm = FreeImage_Load(fif_from_string(a), a, 0);
	
	if(!bm) {
		err("image", "could not load");
		return;
	}
	
	bool has_alpha = FreeImage_IsTransparent(bm);
	
	FIBITMAP * tmp = bm;
	
	if(has_alpha)
		bm = FreeImage_ConvertTo32Bits(bm);
	else
		bm = FreeImage_ConvertTo24Bits(bm);
	
	FreeImage_Unload(tmp);
	
	
	BYTE * bits = new BYTE[FreeImage_GetHeight(bm) * FreeImage_GetPitch(bm)];
	
	FreeImage_ConvertToRawBits(bits, bm, FreeImage_GetPitch(bm), (has_alpha ? 32 : 24), FI_RGBA_BLUE_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_RED_MASK, FALSE);
	
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
	
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &size.x);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &size.y);
	
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

image::image(glm::ivec2 a, bool alpha, bool quality) {
	GLint last;
	
	if(glfw_state == 0)
		graphics();
	
	cache = NULL;
	mipmaps = false;
	external_cache = false;
	
	size = a;
	
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last);
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? (quality ? GL_RGBA12 : GL_RGBA8) : (quality ? GL_RGB12 : GL_RGB8), size.x, size.y, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, last);
}

image::image(glm::ivec2 a, const char * f) {
	if(glfw_state == 0)
		graphics();
	
	cache = NULL;
	mipmaps = false;
	external_cache = false;
	
	size = a;
	
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
	
	glTexImage2D(GL_TEXTURE_2D, 0, ifmt, size.x, size.y, 0, fmt, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, last);
}

image::image(pixelcache * p) {
	if(glfw_state == 0)
		graphics();
	
	mipmaps = false;
	external_cache = true;
	cache = p;
	
	size = p->size;
	colors = p->colors;
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, p->colors == 4 ? GL_RGBA8 : GL_RGB8, size.x, size.y, 0, p->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	from_pixelcache();
}

image::~image() {
	if(!external_cache && cache)
		delete cache;
}

void image::set(const char * a) {
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
	
	// Reset to the original texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(pixelcache * a) {
	GLint bind;
	
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, a->size.x, a->size.y, a->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, a->data);
	
	// Reset to the original texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(pixelcache * a, glm::ivec2 pos, glm::ivec2 dim) {
	GLint bind;
	
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, pos.x, pos.y, dim.x, dim.y, a->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, a->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache() {
	GLint bind;
	
	if(!cache) {
		err("image", "from_pixelcache", "no cache");
		return;
	}
	
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y, cache->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
	// Reset to the origional texture
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, bind);
}

void image::from_pixelcache(glm::ivec2 pos, glm::ivec2 dim) {
	if(!cache) {
		err("image", "from_pixelcache", "no cache");
		return;
	}
	
	GLint bind;
	
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bind);
	
	if(bind != id)
		glBindTexture(GL_TEXTURE_2D, id);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, pos.x, pos.y, dim.x, dim.y, cache->colors == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cache->data);
	
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
		cache = new pixelcache(size);
	
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

glm::vec4 image::pixel(glm::ivec2 a) {
	if(!cache)
		refresh_pixel_cache();
	
	return cache->pixel(a);
}

bool image::save(const char * a, const char * b) {
	if(!cache)
		refresh_pixel_cache();
	
	return cache->save(a, b);
}
