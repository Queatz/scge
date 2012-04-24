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

void pixelcache::pixel(int x, int y, glm::vec4 c) {
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
