std::vector<ALuint> loaded_sounds;
std::vector<ALuint> loaded_buffers;
int alure_state = 0;

ALCdevice* capture_device = NULL;
unsigned int capture_samples_available = 0; // Samples in cache
unsigned int capture_samples_length = 0; // Last number of captured samples
soundbyte capture_soundbyte;

#ifndef _WIN32
FLUIDSYNTH_API void dumberror(int level, char *message, void *data) {
	;
}
#endif

bool audio() {
	if(!alureInitDevice(NULL, NULL)) {
		err("audio", "could not initiate");
		return false;
	}
#ifndef _WIN32
	fluid_set_log_function(FLUID_WARN, dumberror, NULL);
	fluid_set_log_function(FLUID_ERR, dumberror, NULL);
	fluid_set_log_function(FLUID_INFO, dumberror, NULL);
	fluid_set_log_function(FLUID_DBG, dumberror, NULL);
#endif
	
	alureUpdateInterval(0.05);
	
	atexit(audio_off);
	//alDistanceModel(AL_NONE);
	//TODO: audio_model(const char* s)
	
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

void audio_gain(float a) {
	alListenerf(AL_GAIN, a);
}

void audio_pan(float a) {
	alListener3f(AL_POSITION, -a, 0.0, 0.0);
}

void audio_soundfont(const char* a) {
#ifndef _WIN32
	setenv("FLUID_SOUNDFONT", a, 1);
#endif
}

/* Microphone */

const char * alErrorString(ALenum code) {
	if(code == AL_NO_ERROR)
		return "no error";
	if(code == AL_INVALID_NAME)
		return "invalid name";
	if(code == AL_INVALID_ENUM)
		return "invalid enum";
	if(code == AL_INVALID_VALUE)
		return "invalid value";
	if(code == AL_INVALID_OPERATION)
		return "invalid operation";
	if(code == AL_OUT_OF_MEMORY)
		return "out of memory";
	return "unknown error";
}

const char * alcErrorString(ALenum code) {
	if(code == AL_NO_ERROR)
		return "not an error";
	if(code == ALC_INVALID_VALUE)
		return "invalid value";
	if(code == ALC_OUT_OF_MEMORY)
		return "out of memory";
	return "unknown error";
}

#define CAPTURE_FREQ 44100
#define CAPTURE_BUF_SIZE 22050

void microphone_on() {
	if(alure_state == 0)
		audio();
	
	if(!capture_device) {
		alGetError();
		
		capture_device = alcCaptureOpenDevice(NULL, CAPTURE_FREQ, AL_FORMAT_MONO16, CAPTURE_BUF_SIZE);
	
		if(!capture_device) {
			err("microphone_on", alErrorString(alGetError()));
			return;
		}
		
		ALenum e;
		e = alcGetError(capture_device);
		if(e != AL_NO_ERROR)
			err("microphone_on", alcErrorString(e));
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

soundbyte* microphone_update() {
	if(!capture_device)
		return NULL;
	
	ALCint samps;
	alcGetIntegerv(capture_device, ALC_CAPTURE_SAMPLES, 1, &samps);
	
	ALenum e = alcGetError(capture_device);
	if(e) {
		err("microphone_update", alcErrorString(e));
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

soundbyte::soundbyte() : data(NULL), length(0) {
}

soundbyte::soundbyte(ALshort* d, unsigned int l) : data(d), length(l) {
}

soundbyte::~soundbyte() {
}

float soundbyte::calculate_frequency(unsigned int max_samples, unsigned int nblocks, const char* method) {
#ifdef _WIN32
	return 0.0;
#else
	uint_t len = length;
	uint_t hop;
	uint_t offset;
	fvec_t* smpls;
	fvec_t* freq;
	float ret;
	
	if(!nblocks)
		nblocks = 8;
	
	
	if(max_samples && len > max_samples) len = max_samples;
	// Use the samples at the end of the soundbyte
	offset = length - len;
	
	hop = len / nblocks;
	
	smpls = new_fvec(hop);
	freq = new_fvec(1);
	
	aubio_pitch_t* p = new_aubio_pitch(const_cast<char*>(method), len, hop, frequency);
	aubio_pitch_set_unit(p, const_cast<char*>("freq"));
	
	int n, i;
	for(n = 0; n < nblocks; n++) {
		for(i = 0; i < hop; i++)
			fvec_write_sample(smpls, (smpl_t) data[offset + n * hop + i] / 32768, i);
		
		aubio_pitch_do(p, smpls, freq);
	}
	
	ret = freq->data[0];
	
	del_fvec(freq);
	del_fvec(smpls);
	del_aubio_pitch(p);
	
	return ret;
#endif
}

float soundbyte::get(unsigned int i) {
	if(i >= length)
		return 0.0;
	return (float) data[i] / 32768;
}

buffer::buffer() {
	alGenBuffers(1, &buf);
	buffer_loaded(buf);
}

buffer::buffer(const char* a) {
	if(alure_state == 0)
		audio();
	
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

ALint al_format_from_string(const char* a) {
	if(!strcmp(a, "mono 8"))
		return AL_FORMAT_MONO8;
	else if(!strcmp(a, "mono 16"))
		return AL_FORMAT_MONO16;
	else if(!strcmp(a, "stereo 8"))
		return AL_FORMAT_STEREO8;
	else if(!strcmp(a, "stereo 16"))
		return AL_FORMAT_STEREO16;
	
	return AL_FORMAT_MONO8;
}

void buffer::data(const void* bufdata, const char* fmt, unsigned int bytes, unsigned int freq) {
	if(!buf) {
		err("buffer", "data", "no buffer");
		return;
	}
	
	alBufferData(buf, al_format_from_string(fmt), bufdata, bytes, freq);
}

inline void reset_sound(sound* a) {
	a->is_stream = false;
	a->looping = false;
	a->pending = 0;
	a->data = NULL;
	a->source = 0;
	
	int i;
	for(i = 0; i < NUM_BUFS; i++)
		a->bufs[i] = 0;
}

sound::sound() {
	if(alure_state == 0)
		audio();
	
	reset_sound(this);
	
	alGenSources(1, &source);
	sound_loaded(source);
}

sound::sound(buffer* a) {
	if(alure_state == 0)
		audio();
	
	reset_sound(this);
	
	alGenSources(1, &source);
	sound_loaded(source);
	
	data = a;
	alSourcei(source, AL_BUFFER, data->buf);
}

sound::sound(const char* a, bool b) {
	if(alure_state == 0)
		audio();
	
	reset_sound(this);
	
	is_stream = b;

	if(is_stream) {
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
	
	//alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
	// TODO is3D (really just positioned)
	
	if(!is_stream)
		alSourcei(source, AL_BUFFER, bufs[0]);
}

sound::~sound() {
	clear();
}

void sound::clear() {
	alureStopSource(source, AL_FALSE);
	
	if(is_stream)
		alureDestroyStream(stream, NUM_BUFS, bufs);
	
	if(source) {
		alSourcei(source, AL_BUFFER, 0);
		alDeleteSources(1, &source);
		sound_unloaded(source);
	}
	
	reset_sound(this);
	
	alDeleteBuffers(NUM_BUFS, bufs);
}

void play_ended(void* userdata, ALuint source) {
	sound* a = (sound*)userdata;
	
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
	
	if(is_stream) {
		alureRewindStream(stream);
		alurePlaySourceStream(source, stream, NUM_BUFS, (looping ? -1 : repeats), NULL, NULL);
	} else {
		pending = repeats;
		if(d == AL_PLAYING)
			alureStopSource(source, AL_FALSE);
			alSourceStop(source);
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

void sound::pan(float a) {
	alSource3f(source, AL_POSITION, a, 0.0, 0.0);
}

void sound::repeat(bool a) {
	looping = a;
	if(!is_stream)
		alSourcei(source, AL_LOOPING, (a ? AL_TRUE : AL_FALSE));
}

void sound::seek(float a, const char* b) {
	if(a) {
		if(strcmp(b, "second"))
			alSourcef(source, AL_SEC_OFFSET, a);
		else if(strcmp(b, "sample"))
			alSourcef(source, AL_SAMPLE_OFFSET, a);
		else if(strcmp(b, "byte"))
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

int sound::get(const char* b) {
	ALint a;
	if(strcmp(b, "bits"))
		alGetSourcei(bufs[0], AL_BITS, &a);
	else if(strcmp(b, "channels"))
		alGetSourcei(bufs[0], AL_CHANNELS, &a);
	else if(strcmp(b, "frequency"))
		alGetSourcei(bufs[0], AL_FREQUENCY, &a);
	else if(strcmp(b, "bytes"))
		alGetSourcei(bufs[0], AL_SIZE, &a);
	else {
		err("sound", "get", "invalid request");
		return 0;
	}
	return a;
}

float sound::get_offset(const char* b) {
	ALfloat a;
	if(strcmp(b, "second"))
		alGetSourcef(source, AL_SEC_OFFSET, &a);
	else if(strcmp(b, "sample"))
		alGetSourcef(source, AL_SAMPLE_OFFSET, &a);
	else if(strcmp(b, "byte"))
		alGetSourcef(source, AL_BYTE_OFFSET, &a);
	else {
		err("sound", "get", "invalid request");
		return 0.0;
	}
	return a;
}

void sound::font(const char* a) {
	if(stream)
		alureSetStreamPatchset(stream, a);
}
