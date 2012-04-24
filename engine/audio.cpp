FLUIDSYNTH_API void dumberror(int level, char *message, void *data) {
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
	
	alDistanceModel(AL_NONE);
	
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
		for(std::vector<ALuint>::iterator i = loaded_sounds.begin(); i != loaded_sounds.end();i++) {
			alSourcei(*i, AL_BUFFER, 0);
			alDeleteSources(1, &(*i));
		}
	
	if(!loaded_buffers.empty())
		for(std::vector<ALuint>::iterator i = loaded_buffers.begin(); i != loaded_buffers.end();i++)
			alDeleteBuffers(1, &(*i));
	
	alureShutdownDevice();
}

void audio(float a) {
	alListenerf(AL_GAIN, a);
}

void audio(glm::vec3 a) {
	alListener3f(AL_POSITION, a.x, a.y, a.z);
}

void audio(const char* a) {
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

void audio_soundfont(const char* a) {
	setenv("FLUID_SOUNDFONT", a, 1);
}

/* Microphone */

#define CAPTURE_FREQ 44100
#define CAPTURE_BUF_SIZE 22050

void microphone_on() {
	if(alure_state == 0)
		audio_on();
	
	if(!capture_device) {
		alGetError();
		
		capture_device = alcCaptureOpenDevice(NULL, CAPTURE_FREQ, AL_FORMAT_MONO16, CAPTURE_BUF_SIZE);
	
		if(!capture_device) {
			err("microphone", alErrorString(alGetError()));
			return;
		}
		
		ALenum e;
		e = alcGetError(capture_device);
		if(e != AL_NO_ERROR)
			err("microphone", alcErrorString(e));
	}

	capture_soundbyte.data = NULL;
	capture_soundbyte.frequency = CAPTURE_FREQ;
	
	alcCaptureStart(capture_device);
}

void microphone_off() {
	if(!capture_device)
		return;
	
	alcCaptureStop(capture_device);
	alcCaptureCloseDevice(capture_device);
	
	capture_device = NULL;
}

void microphone(bool a) {
	if(a)
		microphone_on();
	else
		microphone_off();
}

soundbyte* microphone() {
	if(!capture_device)
		return NULL;
	
	ALCint samps;
	alcGetIntegerv(capture_device, ALC_CAPTURE_SAMPLES, 1, &samps);
	
	ALenum e = alcGetError(capture_device);
	if(e) {
		err("microphone", alcErrorString(e));
		return NULL;
	}
	
	// Resize if needed
	if(samps > capture_samples_available) {
		if(capture_soundbyte.data)
			delete capture_soundbyte.data;
		capture_soundbyte.data = new ALshort[samps];
		capture_samples_available = samps;
	}
	
	capture_samples_length = samps;
	
	alcCaptureSamples(capture_device, capture_soundbyte.data, samps);
	
	capture_soundbyte.length = capture_samples_length;
	capture_soundbyte.frequency = CAPTURE_FREQ;
	return &capture_soundbyte;
}
