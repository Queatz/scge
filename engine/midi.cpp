void midi_on() {
	midi_settings = new_fluid_settings();
	fluid_settings_setstr(midi_settings, "audio.driver", "pulseaudio");
	fluid_settings_setnum(midi_settings, "synth.gain", 0.5);
	fluid_settings_setstr(midi_settings, "synth.reverb.active", "yes");
	fluid_settings_setstr(midi_settings, "synth.chorus.active", "yes");
	midi_synth = new_fluid_synth(midi_settings);
	midi_device = new_fluid_audio_driver(midi_settings, midi_synth);
	
	midi_status = 1;
	
	atexit(midi_off);
}

void midi_off() {
	if (midi_device)
		delete_fluid_audio_driver(midi_device);
	
	if (midi_synth)
		delete_fluid_synth(midi_synth);
	
	if (midi_settings)
		delete_fluid_settings(midi_settings);
	
	midi_status = 0;
}

soundfont::soundfont(const char * a) {
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
	fluid_sfont_t * f = fluid_synth_get_sfont_by_id(midi_synth, id);
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

midi::midi(int c) : _channel(c) {}

void midi::font(soundfont * f) {
	fluid_synth_sfont_select(midi_synth, _channel, (unsigned int)f->id);
}

void midi::preset(soundfont * sf, unsigned int bank, unsigned int preset) {
	fluid_synth_program_select(midi_synth, _channel, sf->id, bank, preset);
}

void midi::bank(unsigned int bank) {
	fluid_synth_bank_select(midi_synth, _channel, bank);
}

void midi::play(int key, int velocity) {
	fluid_synth_noteon(midi_synth, _channel, key, velocity);
}

void midi::stop(int key) {
	fluid_synth_noteoff(midi_synth, _channel, key);
}

void midi::pan(int a) {
	fluid_synth_cc(midi_synth, _channel, 10, a);
}
