soundbyte::soundbyte() : data(NULL), length(0) {}

soundbyte::soundbyte(ALshort* d, unsigned int l) : data(d), length(l) {}

soundbyte::~soundbyte() {}

float soundbyte::pitch(unsigned int max_samples, unsigned int nblocks, const char* method) {
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
}

float soundbyte::get(unsigned int i) {
	if(i >= length)
		return 0.0;
	return (float) data[i] / 32768;
}
