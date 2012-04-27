#define CAPTURE_FREQ 44100
#define CAPTURE_BUF_SIZE 22050

microphone::microphone() : _device(NULL), _samples_available(0), _samples_length(0) {
	if(alure_state == 0)
		audio_on();
	
	if(!_device) {
		alGetError();
		
		_device = alcCaptureOpenDevice(NULL, CAPTURE_FREQ, AL_FORMAT_MONO16, CAPTURE_BUF_SIZE);
	
		if(!_device) {
			err("microphone", alErrorString(alGetError()));
			return;
		}
		
		ALenum e;
		e = alcGetError(_device);
		if(e != AL_NO_ERROR)
			err("microphone", alcErrorString(e));
	}

	_soundbyte.data = NULL;
	_soundbyte.frequency = CAPTURE_FREQ;
}

microphone::~microphone() {
	if(!_device)
		return;
	
	alcCaptureStop(_device);
	alcCaptureCloseDevice(_device);
	
	_device = NULL;
}

void microphone::enable(bool a) {
	if(a)
		alcCaptureStart(_device);
	else
		alcCaptureStop(_device);
}

soundbyte* microphone::step() {
	if(!_device)
		return NULL;
	
	ALCint samps;
	alcGetIntegerv(_device, ALC_CAPTURE_SAMPLES, 1, &samps);
	
	ALenum e = alcGetError(_device);
	if(e) {
		err("microphone", alcErrorString(e));
		return NULL;
	}
	
	// Resize if needed
	if(samps > _samples_available) {
		if(_soundbyte.data)
			delete _soundbyte.data;
		_soundbyte.data = new ALshort[samps];
		_samples_available = samps;
	}
	
	_samples_length = samps;
	
	alcCaptureSamples(_device, _soundbyte.data, samps);
	
	_soundbyte.length = _samples_length;
	_soundbyte.frequency = CAPTURE_FREQ;
	return &_soundbyte;
}
