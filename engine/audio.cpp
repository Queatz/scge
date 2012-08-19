FLUIDSYNTH_API void dumberror(int level, char * message, void * data) {
	;
}

bool audio_on() {
	if(!alureInitDevice(NULL, NULL)) {
		err("audio", "could not initiate");
		return false;
	}
	
	fluid_set_log_function(FLUID_WARN, dumberror, NULL);
	fluid_set_log_function(FLUID_ERR, dumberror, NULL);
	fluid_set_log_function(FLUID_INFO, dumberror, NULL);
	fluid_set_log_function(FLUID_DBG, dumberror, NULL);
	
	alureUpdateInterval(0.05);
	
	atexit(audio_off);
	
	alure_state = 1;
	return true;
}

void buffer_loaded(ALuint a) {
	loaded_buffers.push_back(a);
}

void buffer_unloaded(ALuint a) {
	for(std::vector<ALuint>::iterator i = loaded_buffers.begin(); i != loaded_buffers.end();)
		if(*i == a) {
			i = loaded_buffers.erase(i);
			break;
		}
		else
			i++;
}
void sound_loaded(ALuint a) {
	loaded_sounds.push_back(a);
}

void sound_unloaded(ALuint a) {
	for(std::vector<ALuint>::iterator i = loaded_sounds.begin(); i != loaded_sounds.end();)
		if(*i == a) {
			i = loaded_sounds.erase(i);
			break;
		}
		else
			i++;
}

void audio_off() {
	if(!alureUpdateInterval(0.0))
		err("audio_off", "could not stop");
	
	if(!loaded_sounds.empty())
		for(std::vector<ALuint>::iterator i = loaded_sounds.begin(); i != loaded_sounds.end(); i++) {
			alSourcei(*i, AL_BUFFER, 0);
			alDeleteSources(1, &(*i));
		}

	if(!loaded_buffers.empty())
		for(std::vector<ALuint>::iterator i = loaded_buffers.begin(); i != loaded_buffers.end(); i++)
			alDeleteBuffers(1, &(*i));

	alureShutdownDevice();
}

void audio_soundfont(const char * a) {
	setenv("FLUID_SOUNDFONT", a, 1);
}

void listener::gain(float a) {
	alListenerf(AL_GAIN, a);
}

void listener::position(glm::vec3 a) {
	alListener3f(AL_POSITION, a.x, a.y, a.z);
}

void listener::attenuation(const char * a) {
	ALenum b;
	
	if(!strcmp(a, "none"))
		b = AL_NONE;
	else if(!strcmp(a, "inverse"))
		b = AL_INVERSE_DISTANCE;
	else if(!strcmp(a, "inverse clamped"))
		b = AL_INVERSE_DISTANCE_CLAMPED;
	else if(!strcmp(a, "linear"))
		b = AL_LINEAR_DISTANCE;
	else if(!strcmp(a, "linear clamped"))
		b = AL_LINEAR_DISTANCE_CLAMPED;
	else if(!strcmp(a, "exponent"))
		b = AL_EXPONENT_DISTANCE;
	else if(!strcmp(a, "exponent clamped"))
		b = AL_EXPONENT_DISTANCE_CLAMPED;
	else {
		err("audio", "invalid");
		return;
	}
	
	alDistanceModel(b);
}
