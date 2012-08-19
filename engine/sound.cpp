inline void reset_sound(sound * a) {
	a->streaming = false;
	a->looping = false;
	a->pending = 0;
	a->data = NULL;
	a->source = 0;
	a->stream = NULL;
	
	int i;
	
	for(i = 0; i < NUM_BUFS; i++)
		a->bufs[i] = 0;
}

sound::sound() {
	if(alure_state == 0)
		audio_on();
	
	reset_sound(this);
	
	alGenSources(1, &source);
	sound_loaded(source);
}

sound::sound(buffer * a) {
	if(alure_state == 0)
		audio_on();
	
	reset_sound(this);
	
	alGenSources(1, &source);
	sound_loaded(source);
	
	data = a;
	
	alSourcei(source, AL_BUFFER, data->buf);
}

sound::sound(const char * a, bool b) {
	if(alure_state == 0)
		audio_on();
	
	reset_sound(this);
	
	streaming = b;

	if(streaming) {
		stream = alureCreateStreamFromFile(a, 19200, NUM_BUFS, bufs);
		
		if(!stream)
			err("sound", "could not load");
	} else {
		bufs[0] = alureCreateBufferFromFile(a);
		
		if(bufs[0] == AL_NONE)
			err("sound", "could not load");
	}
	
	alGenSources(1, &source);
	sound_loaded(source);
	
	if(!streaming && bufs[0] != AL_NONE)
		alSourcei(source, AL_BUFFER, bufs[0]);
}

sound::~sound() {
	clear();
}

void sound::clear() {
	if(source) {
		alureStopSource(source, AL_FALSE);
		alSourcei(source, AL_BUFFER, 0);
		alDeleteSources(1, &source);
		sound_unloaded(source);
	}
	
	if(stream)
		alureDestroyStream(stream, NUM_BUFS, bufs);
	else if(bufs[0])
		alDeleteBuffers(1, &bufs[0]);
	
	reset_sound(this);
}

void play_ended(void * userdata, ALuint source) {
	sound * a = (sound *)userdata;
	
	if(a->pending > 0) {
		a->pending--;
		
		alurePlaySource(a->source, play_ended, a);
	}
}

void sound::play(unsigned int repeats, bool b, bool c) {
	ALint d;
	
	alGetSourcei(source, AL_SOURCE_STATE, &d);
	
	if(b) {
		if(looping)
			return;
		
		if(d == AL_PLAYING || pending > 0) {
			if(!c)
				pending += repeats + 1;
			
			return;
		}
	}
	
	if(streaming) {
		alureRewindStream(stream);
		alurePlaySourceStream(source, stream, NUM_BUFS, (looping ? -1 : repeats), NULL, NULL);
	} else {
		pending = repeats;
		
		if(d == AL_PLAYING)
			alureStopSource(source, AL_FALSE);
		
		alurePlaySource(source, play_ended, this);
	}	
}

void sound::stop() {
	alureStopSource(source, AL_FALSE);
	pending = 0;
}

void sound::pause() {
	alurePauseSource(source);
}

void sound::resume() {
	alureResumeSource(source);
}

void sound::gain(float a) {
	alSourcef(source, AL_GAIN, a);
}

void sound::maximum_gain(float a) {
	alSourcef(source, AL_MAX_GAIN, a);
}

void sound::minimum_gain(float a) {
	alSourcef(source, AL_MIN_GAIN, a);
}

void sound::pitch(float a) {
	alSourcef(source, AL_PITCH, a);
}

void sound::position(glm::vec3 a) {
	alSource3f(source, AL_POSITION, a.x, a.y, a.z);
}

void sound::repeat(bool a) {
	looping = a;
	if(!streaming)
		alSourcei(source, AL_LOOPING, (a ? AL_TRUE : AL_FALSE));
}

void sound::seek(float a, const char * b) {
	if(a) {
		if(!strcmp(b, "second"))
			alSourcef(source, AL_SEC_OFFSET, a);
		else if(!strcmp(b, "sample"))
			alSourcef(source, AL_SAMPLE_OFFSET, a);
		else if(!strcmp(b, "byte"))
			alSourcef(source, AL_BYTE_OFFSET, a);
		else
			err("sound", "seek", "invalid specifier");
	}
	else
		alSourceRewind(source);
}

bool sound::playing() {
	ALint a;
	
	alGetSourcei(source, AL_SOURCE_STATE, &a);
	
	return a == AL_PLAYING;
}

int sound::get(const char * b) {
	ALint a;
	
	if(!strcmp(b, "bits"))
		alGetSourcei(bufs[0], AL_BITS, &a);
	else if(!strcmp(b, "channels"))
		alGetSourcei(bufs[0], AL_CHANNELS, &a);
	else if(!strcmp(b, "frequency"))
		alGetSourcei(bufs[0], AL_FREQUENCY, &a);
	else if(!strcmp(b, "bytes"))
		alGetSourcei(bufs[0], AL_SIZE, &a);
	else {
		err("sound", "get", "invalid request");
		return 0;
	}
	
	return a;
}

float sound::offset(const char * b) {
	ALfloat a;
	
	if(!strcmp(b, "second"))
		alGetSourcef(source, AL_SEC_OFFSET, &a);
	else if(!strcmp(b, "sample"))
		alGetSourcef(source, AL_SAMPLE_OFFSET, &a);
	else if(!strcmp(b, "byte"))
		alGetSourcef(source, AL_BYTE_OFFSET, &a);
	else {
		err("sound", "get", "invalid request");
		return 0.0;
	}
	
	return a;
}

void sound::font(const char * a) {
	if(stream)
		alureSetStreamPatchset(stream, a);
}
