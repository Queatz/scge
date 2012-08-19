pixelcache::pixelcache(const char * a) {
	FIBITMAP * bm = FreeImage_Load(fif_from_string(a), a, 0);
	
	if(!bm) {
		err("pixelcache", "could not load");
		return;
	}
	
	colors = FreeImage_IsTransparent(bm) ? 4 : 3;
	
	BYTE * bits = FreeImage_GetBits(bm);
	int bpp = FreeImage_GetBPP(bm);
	
	size.x = FreeImage_GetWidth(bm);
	size.y = FreeImage_GetHeight(bm);
	
	int i, j, s = size.y * size.x;
	RGBQUAD pix;
	
	data = new GLubyte[s*colors];
	
	for(i = 0; i < size.x; i++)
	for(j = 0; j < size.y; j++) {
		FreeImage_GetPixelColor(bm, i, j, &pix);
		data[(j*size.x+i)*colors] = pix.rgbRed;
		data[(j*size.x+i)*colors+1] = pix.rgbGreen;
		data[(j*size.x+i)*colors+2] = pix.rgbBlue;
		if(colors == 4)
			data[(j*size.x+i)*colors+3] = pix.rgbReserved;
	}
	
	FreeImage_Unload(bm);
}

pixelcache::pixelcache(const pixelcache& a) {
	size = a.size;
	colors = a.colors;
	
	int s = size.y * size.x * colors;
	
	data = new GLubyte[s];
	
	memcpy(data, a.data, sizeof(GLubyte) * s);
}

pixelcache::pixelcache(glm::ivec2 s, bool a) {
	size = s;
	colors = a ? 4 : 3;
	data = new GLubyte[size.x*size.y*colors];
}

pixelcache::~pixelcache() {
	if(data)
		delete data;
}

glm::vec4 pixelcache::pixel(glm::ivec2 a) {
	int h;
	
	if(a.x < 0 || a.x >= size.x || a.y < 0 || a.y >= size.y)
		return glm::vec4(0.0, 0.0, 0.0, colors == 4 ? 0.0 : 1.0);
	
	h = a.y * size.x * colors + a.x * colors;
	return glm::vec4(static_cast<float>(data[h]) / 255.0, static_cast<float>(data[h+1]) / 255.0, static_cast<float>(data[h+2]) / 255.0, colors == 4 ? static_cast<float>(data[h + 3]) / 255.0 : 1.0);
}

void pixelcache::pixel(glm::ivec2 a, glm::vec4 c) {
	int h;
	if(a.x < 0 || a.x >= size.x || a.y < 0 || a.y >= size.y) {
		err("pixelcache", "set_pixel", "out of bounds");
		return;
	}
	
	h = a.y * size.x * colors + a.x * colors;
	data[h+0] = static_cast<GLubyte>(c.r * 255.0);
	data[h+1] = static_cast<GLubyte>(c.g * 255.0);
	data[h+2] = static_cast<GLubyte>(c.b * 255.0);
	if(colors == 4)
		data[h+3] = static_cast<GLubyte>(c.a * 255.0);
}

bool pixelcache::save(const char * a, const char * b) {
	if(!data)
		return false;
	
	FIBITMAP * c = FreeImage_Allocate(size.x, size.y, colors == 4 ? 32 : 24);
	RGBQUAD color;
	
	for(unsigned int x = 0; x < size.x; x++)
		for(unsigned int y = 0; y < size.y; y++) {
			int h = y * size.x * colors + x * colors;
			color.rgbRed = data[h+0];
			color.rgbGreen = data[h+1];
			color.rgbBlue = data[h+2];
			if(colors == 4)
				color.rgbReserved = data[h+3];
			FreeImage_SetPixelColor(c, x, y, &color);
		}
	
	if (FreeImage_Save(fif_from_string(a, b), c, a, default_from_fif(fif_from_string(a, b)))) {
		FreeImage_Unload(c);
		
		return true;
	}
	
	FreeImage_Unload(c);
	
	return false;
}
