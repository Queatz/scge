std::vector<sound*> active_streams;
std::vector<sound*> queued_sounds;

bool audio() {
	if(!alureInitDevice(NULL, NULL)) {
		err("audio", "could not initiate");
		return false;
	}
	alDistanceModel(AL_NONE);
	active_streams.reserve(1);
	queued_sounds.reserve(1);
	
	return true;
}

void audio_off() {
	alureShutdownDevice();
}

void play(sound* a, bool b, bool c) {
	if(b) {
		if(a->looping)
			return;
		
		ALint d;
		alGetSourcei(a->source, AL_SOURCE_STATE, &d);
		if(d == AL_PLAYING) {
			if(!c)
				queued_sounds.push_back(a);
			return;
		}
	}
	
	if(a->is_stream) {
		if(a->is_playing)
			return;
		a->is_playing = true;
		a->update();
		active_streams.push_back(a);
	}
	
	alSourcePlay(a->source);
}

void stop(sound* a) {
	if(a->is_stream) {
		a->is_playing = false;
		alureRewindStream(a->stream);
	}
	alSourceStop(a->source);
}

void pause(sound* a) {
	if(a->is_stream)
		a->is_playing = false;
}

void clear_audio() {
	active_streams.clear();
	queued_sounds.clear();
}

void update_audio() {
	if(!active_streams.empty()) {
		for(std::vector<sound*>::iterator i = active_streams.begin(); i != active_streams.end();) {
			if(*i == NULL || !(*i)->is_playing) {
				i = active_streams.erase(i);
			} else {
				(*i)->update();
				i++;
			}
		}
	}
	
	if(!queued_sounds.empty()) {
		int state;
		for(std::vector<sound*>::iterator i = queued_sounds.begin(); i != queued_sounds.end();) {
			if(*i == NULL)
				i = queued_sounds.erase(i);
			else if(state != AL_PLAYING) {
				alGetSourcei((*i)->source, AL_SOURCE_STATE, &state);
				alSourcePlay((*i)->source);
				i = queued_sounds.erase(i);
			}
			else
				i++;
		}
	}
}

/* Microphone */
