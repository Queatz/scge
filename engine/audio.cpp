std::vector<ALuint> loaded_sounds;
std::vector<ALuint> loaded_buffers;
int alure_state = 0;

/* * Audio Functions
audio()
Turn on audio dealings and returns true if it could.

C++
audio();

Python
audio()
* */

FLUIDSYNTH_API void dumberror(int level, char *message, void *data) {
	;
}

bool audio() {
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
	//alDistanceModel(AL_NONE);
	
	alure_state = 1;
	return true;
}

/* *
audio_off()
Turn off audio dealings.

C++
audio_off();

Python
audio_off()
* */
void buffer_loaded(ALuint a) {
	loaded_buffers.push_back(a);
}

void buffer_unloaded(ALuint a) {
/*	for(std::vector<ALuint>::iterator i = loaded_buffers.begin(); i != loaded_buffers.end();)
		if(*i == a) {
			i = loaded_buffers.erase(i);
			break;
		}
		else
			i++;*/
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
	
	for(std::vector<ALuint>::iterator i = loaded_sounds.begin(); i != loaded_sounds.end();i++) {
		alSourcei(*i, AL_BUFFER, NULL);
		alDeleteSources(1, &(*i));
	}
	
	for(std::vector<ALuint>::iterator i = loaded_buffers.begin(); i != loaded_buffers.end();i++)
		alDeleteBuffers(1, &(*i));
	
	alureShutdownDevice();
}

/* *
audio_gain(float)
Set the overall volume of sound.

C++
audio_gain(1.0);

Python
audio_gain(0)
* */
void audio_gain(float a) {
	alListenerf(AL_GAIN, a);
}

/* *
audio_pan(float)
Set the overall pan of sound.

C++
audio_pan();

Python
audio_pan()
* */
void audio_pan(float a) {
	alListener3f(AL_POSITION, -a, 0.0, 0.0);
}

/* *
audio_soundfont(string)
Set the soundfont to be used for midi streams.

C++
audio_soundfont();

Python
audio_soundfont()
* */
void audio_soundfont(const char* a) {
	setenv("FLUID_SOUNDFONT", a, 1);
}

/* Microphone */
