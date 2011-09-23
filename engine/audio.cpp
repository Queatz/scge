std::vector<ALuint> loaded_sounds;
std::vector<ALuint> loaded_buffers;
int alure_state = 0;

ALCdevice* capture_device = NULL;
unsigned int capture_samples_available = 0;
unsigned int capture_samples_length = 0;
ALshort* capture_samples = NULL;
soundbyte capture_soundbyte;

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
	//TODO: audio_model(const char* s)
	
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
}

const char * alcErrorString(ALenum code) {
	if(code == AL_NO_ERROR)
		return "not an error";
	if(code == ALC_INVALID_VALUE)
		return "invalid value";
	if(code == ALC_OUT_OF_MEMORY)
		return "out of memory";
}

#define CAPTURE_FREQ 44100
#define CAPTURE_BUF_SIZE 22050


/* * Microphone
microphone_on()
Turn on microphone capturing.

C++
microphone_on();

Python
microphone_on()
* */
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
	
	alcCaptureStart(capture_device);
}

/* *
microphone_off()
Turn off microphone capturing.

C++
microphone_off();

Python
microphone_off()
* */
void microphone_off() {
	if(!capture_device)
		return;
	
	alcCaptureStop(capture_device);
	alcCaptureCloseDevice(capture_device);
	
	capture_device = NULL;
}

/* *
microphone_update()
Refresh the microphone soundbyte with the latest captured samples.

C++
microphone_update();

Python
microphone_update()
* */
void microphone_update() {
	if(!capture_device)
		return;
	
	ALCint samps;
	alcGetIntegerv(capture_device, ALC_CAPTURE_SAMPLES, 1, &samps);
	
	ALenum e = alcGetError(capture_device);
	if(e) {
		err("microphone_update", alcErrorString(e));
		return;
	}
	
	// Resize if needed
	if(samps > capture_samples_available) {
		if(capture_samples)
			delete capture_samples;
		capture_samples = new ALshort[samps];
		capture_samples_available = samps;
	}
	
	capture_samples_length = samps;
	
	alcCaptureSamples(capture_device, capture_samples, samps);
}

/* *
microphone_buffer()
Get the soundbyte associated with the microphone.

C++
microphone_buffer();

Python
microphone_buffer()
* */
soundbyte* microphone_buffer() {
	capture_soundbyte.data = capture_samples;
	capture_soundbyte.length = capture_samples_length;
	capture_soundbyte.frequency = CAPTURE_FREQ;
	return &capture_soundbyte;
}

/* * Resources
soundbyte
Bytes of sound.

	get(sample)
		Get a sample as a float
	calculate_pitch(max_samples = 0, method = "schmitt")
		tries to find a pitch in this soundbyte and returns the frequency
		"mcomb"
		"fcomb"
		"schmitt"
		"yin"
		"yinfft"
C++
soundbyte a();

Python
a = soundbyte()
* */
soundbyte::soundbyte() : data(NULL), length(0) {
}

soundbyte::soundbyte(ALshort* d, unsigned int l) : data(d), length(l) {
}

soundbyte::~soundbyte() {
}

float soundbyte::calculate_frequency(unsigned int max_samples, const char* method) {
	uint_t len = length;
	uint_t nblocks = 4;
	uint_t hop;
	uint_t offset;
	fvec_t* smpls;
	fvec_t* freq;
	float ret;
	
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
}

float soundbyte::get(unsigned int i) {
	if(i >= length)
		return 0.0;
	return (float) data[i] / 32768;
}

/* *
buffer
A sound buffer.
	data(bytes data, string, unsigned int bytes, unsigned int frequency)
		set the data of the buffer, as a certain type, with a length in bytes, and a frequency
		"mono 8"
		"mono 16"
		"stereo 8"
		"stereo 16"
		!the buffer must not be in use
C++
buffer a("powerup.ogg");

Python
a = buffer('powerup.ogg')

see:sound
* */

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

/* *
sound
A sound.

	play(sound, unsigned int repeats = 0, bool wait = false, bool dont = false)
		play the sound, optionally waiting until it's finished, or just don't play it if it's already playing
	stop()
		stop it
	pause()
		pause the sound
	resume()
		resume the sound
	gain(float)
		set the gain of the sound
	maximum_gain(float)
		set the maximum gain allowed for the sound
	minimum_gain(float)
		set the minimum gain allowed for the sound
	pitch(float)
		set the pitch of the sound
	pan(float)
		set the pan of the sound
	repeat(bool)
		turn on and off repeating of the sound
	seek(float = NULL, string specifier = "")
		set the offset at which to play the sound. The specifier can be:
		"second" (default)
		"byte"
		"sample"
	playing()
		returns true if the sound is currently being played
	get(string)
		returns an int relating to:
		"frequency"
		"channels"
		"bytes"
	get_offset(string) as:
		returns a float of:
		"second"
		"byte"
		"sample"
	font(string)
		set the soundfont used for this sound

C++
sound a("click.ogg");
a.pan(-1.0); //pan fully to the left

sound b("music.ogg", true);
b.repeat(true); //set the music to repeat itself

Python
a = sound('click.ogg')
a.pan(-1.0) #pan fully to the left

b = sound('music.ogg', True)
b.repeat(True) #set the music to repeat itself

see:buffer
* */

inline void reset_sound(sound* a) {
	a->is_stream = false;
	a->looping = false;
	a->pending = 0;
	a->data = NULL;
	a->source = 0;
	
	int i;
	for(i = 0; i < NUM_BUFS; i++)
		a->bufs[i] = NULL;
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
	// TODO is3D
	
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
		alSourcei(source, AL_BUFFER, NULL);
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
