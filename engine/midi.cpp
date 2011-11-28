#ifndef _WIN32
/* * Midi
midi_on()
Enable midi.

C++
midi_on();

Python
midi_on()
* */

int midi_status = 0;

fluid_settings_t* midi_settings = NULL;
fluid_synth_t* midi_synth = NULL;
fluid_audio_driver_t* midi_device = NULL;

void midi_on() {
	midi_settings = new_fluid_settings();
	fluid_settings_setstr(midi_settings, "audio.driver", "alsa");
	fluid_settings_setnum(midi_settings, "synth.gain", 0.5);
	fluid_settings_setstr(midi_settings, "synth.reverb.active", "yes");
	fluid_settings_setstr(midi_settings, "synth.chorus.active", "yes");
	midi_synth = new_fluid_synth(midi_settings);
	midi_device = new_fluid_audio_driver(midi_settings, midi_synth);
	
	midi_status = 1;
	
	atexit(midi_off);
}

/* *
midi_off()
Disable midi.

C++
midi_off();

Python
midi_off()
* */
void midi_off() {
	if (midi_device)
		delete_fluid_audio_driver(midi_device);
	if (midi_synth)
		delete_fluid_synth(midi_synth);
	if (midi_settings)
		delete_fluid_settings(midi_settings);
	
	midi_status = 0;
}

/* *
soundfont
A soundfont.

	get_presets()
		Returns a semicolon-seperated list of presets.

C++
soundfont a("soundfont.sf2");

Python
a = soundfont('soundfont.sf2')
* */
soundfont::soundfont(const char* a) {
	if(midi_status < 1)
		midi_on();
	
	if(a) {
		id = fluid_synth_sfload(midi_synth, a, 1);
		
		if (id == FLUID_FAILED)
			err("soundfont", "could not load");
	}
	else
		id = -1;
}

std::string soundfont::get_presets() {
	fluid_sfont_t *f = fluid_synth_get_sfont_by_id(midi_synth, id);
	fluid_preset_t p;
	std::ostringstream s;
	std::string t;
	
	if(!f) {
		err("soundfont", "get_presets", "error");
		return "";
	}
	
	f->iteration_start(f);
	while(f->iteration_next(f, &p)) {
		s << p.get_banknum(&p);
		s << ":";
		s << p.get_num(&p);
		s << ":";
		s << p.get_name(&p);
		s << ";";
	}
	
	t = s.str();
	return t;
}

/*const char* soundfont::instrument(unsigned int bank, unsigned int preset) {
	fluid_preset_t* p;
	fluid_sfont_t* f;
	f = fluid_synth_get_sfont(midi_synth, id); 
	p = f->get_preset(bank, preset);
	
	if(p)
		return p->get_name(p);
	return NULL;
}*/

/** *
instrument
An instrument.

C++
instrument a();

Python
a = instrument()
***/
#define RED
/* *
midi_soundfont(soundfont, int channel = 0)
Set the soundfont to use on a channel.

C++
soundfont sf("piano.sf2");
midi_soundfont(&sf, 0);

Python
sf = soundfont('piano.sf2')
midi_soundfont(sf, 0)
* */
void midi_soundfont(soundfont* f, int channel) {
	fluid_synth_sfont_select(midi_synth, channel, (unsigned int)f->id);
}

/* *
midi_preset(soundfont, int channel = 0, int bank = 0, int preset = 0)
Set the preset to use on a channel.

C++
soundfont sf("piano.sf2");
midi_preset(&sf, 0, 0, 0);

Python
sf = soundfont('piano.sf2')
midi_preset(sf, 0, 0, 0)
* */
void midi_preset(soundfont* sf, int channel, unsigned int bank, unsigned int preset) {
	fluid_synth_program_select(midi_synth, channel, sf->id, bank, preset);
}

/* *
midi_bank(int channel = 0, int bank = 0)
Set the bank to use on a channel.

C++
midi_bank(0, 0);

Python
midi_bank(0, 0)
* */
void midi_bank(int channel, unsigned int bank) {
	fluid_synth_bank_select(midi_synth, channel, bank);
}

/* *
midi_play(int channel = 0, int key = 60, int volume = 100)
Start playing a note on a channel, with midi pitch 0-127 and volume 0-127.

C++
midi_play();

Python
midi_play()
* */
void midi_play(int channel, int key, int velocity) {
	fluid_synth_noteon(midi_synth, channel, key, velocity);
}

/* *
midi_stop(int channel = 0, int key = 60)
Stop playing a note on a channel, at midi pitch 0-127.

C++
midi_stop();

Python
midi_stop()
* */
void midi_stop(int channel, int key) {
	fluid_synth_noteoff(midi_synth, channel, key);
}

/* *
midi_pan(int channel = 0, int pan = 63)
Pan a channel between left and right, 0 and 127.

C++
midi_pan();

Python
midi_pan()
* */
void midi_pan(int channel, int a) {
	fluid_synth_cc(midi_synth, channel, 10, a);
}
#endif
