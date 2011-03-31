std::vector<sound*> loaded_sounds;
int alure_state = 0;

/* * Audio Functions
audio()
Turn on audio dealings and returns true if it could.

C++
audio();

Python
audio()
* */
bool audio() {
	if(!alureInitDevice(NULL, NULL)) {
		err("audio", "could not initiate");
		return false;
	}
	
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
void audio_off() {
	if(!alureUpdateInterval(0.0))
		err("audio_off", "could not stop");
	
	for(std::vector<sound*>::iterator i = loaded_sounds.begin(); i != loaded_sounds.end(); i++)
		if(*i)
			(*i)->unload();
	
	alureShutdownDevice();
}

void play_ended(void* userdata, ALuint source) {
	sound* a = (sound*)userdata;
	if(a->pending > 0) {
		a->pending--;
		alurePlaySource(a->source, play_ended, a);
	}
}

/* *
play(sound, unsigned int repeats = 0, bool wait = false, bool dont = false)
Play a sound.  Optionally wait until it's finished, or just don't play it if it's already playing.

C++
sound a("tux.ogg");
play(&a);
play(&a, 2, false, true); //play 2 times if it's currently not being played

Python
a = sound('tux.ogg')
play(a)
play(a, 2, False, True); #play 2 times if it's currently not being played


see:sound
* */
void play(sound* a, unsigned int repeats, bool b, bool c) {
	if(b) {
		if(a->looping)
			return;
		
		ALint d;
		alGetSourcei(a->source, AL_SOURCE_STATE, &d);
		if(d == AL_PLAYING) {
			if(!c)
				a->pending += repeats;
			return;
		}
	}
	
	if(a->is_stream) {
		alureRewindStream(a->stream);
		alurePlaySourceStream(a->source, a->stream, NUM_BUFS, (a->looping ? -1 : repeats), NULL, NULL);
	} else {
		alurePlaySource(a->source, play_ended, a);
		a->pending = repeats;
	}	
}

void delete_multiplay(void* userdata, ALuint source) {
	alDeleteSources(1, &source);
}

/* *
multiplay(sound)
Play multiple sources of one sound.

C++
sound a("hello.ogg");
multiplay(&a);

Python
a = sound('hello.ogg')
multiplay(a)
* */
void multiplay(sound* a) {
	if(a->is_stream)
		return;
	ALuint b;
	alGenSources(1, &b);
	alSourcei(b, AL_BUFFER, a->buffer[0]);

	ALfloat f[3];

	alGetSourcefv(a->source, AL_POSITION, f);
	alSourcefv(b, AL_POSITION, f);
	
	alGetSourcef(a->source, AL_PITCH, &f[0]);
	alSourcef(b, AL_PITCH, f[0]);
	
	alGetSourcef(a->source, AL_GAIN, &f[0]);
	alSourcef(b, AL_GAIN, f[0]);
	
	alurePlaySource(b, delete_multiplay, NULL);
}

/* *
stop(sound)
Stop a sound.

C++
sound a("sayhi.ogg");
play(&a);
stop(&a);

Python
a = sound('sayhi.ogg')
play(a)
stop(a)
* */
void stop(sound* a) {
	alureStopSource(a->source, AL_FALSE);
	a->pending = 0;
}

/* *
pause(sound)
Pause a sound.

C++
sound a("titlemusic.ogg", true);
play(&a);
pause(&a);

Python
a = sound('titlemusic.ogg', True)
play(a)
pause(a)
* */
void pause(sound* a) {
	alurePauseSource(a->source);
}

/* *
resume(sound)
Resume a paused sound.

C++
sound a("titlemusic.ogg", true);
play(&a);
pause(&a);
resume(&a);

Python
a = sound('titlemusic.ogg', True)
play(a)
pause(a)
resume(a)
* */
void resume(sound* a) {
	alureResumeSource(a->source);
}

/* Microphone */
