buffer::buffer() {
	alGenBuffers(1, &buf);
	buffer_loaded(buf);
}

buffer::buffer(const char* a) {
	if(alure_state == 0)
		audio_on();
	
	buf = alureCreateBufferFromFile(a);
	if(buf == AL_NONE) {
		buf = 0;
		err("buffer", "could not load");
	}
	else
		buffer_loaded(buf);
}

buffer::~buffer() {
	clear();
}

void buffer::clear() {
	if(buf) {
		alDeleteBuffers(1, &buf);
		buffer_unloaded(buf);
	}
	buf = 0;
}

void buffer::data(const void* bufdata, const char* fmt, unsigned int bytes, unsigned int freq) {
	if(!buf) {
		err("buffer", "data", "no buffer");
		return;
	}
	
	alBufferData(buf, al_format_from_string(fmt), bufdata, bytes, freq);
}
