void err(const char* a, const char* b, const char* c) {
	std::cout << "\033[1;34m" << a << " \033[1;31m" << b << "\033[0m " << c << std::endl;
}

void err(const char* b, const char* c) {
	std::cout << " \033[1;31m" << b << "\033[0m " << c << std::endl;
}

void note(const char* a, const char* b, const char* c) {
	std::cout << "\033[1;34m" << a << " \033[1;33m" << b << "\033[0m " << c << std::endl;
}

void note(const char* b, const char* c) {
	std::cout << " \033[1;33m" << b << "\033[0m " << c << std::endl;
}

const char* read_file(const char* a) {
	FILE *fp;
	long len;
	char *buf;
	
	fp = fopen(a, "rb");
	if(!fp)
		return NULL;
	fseek(fp, 0, SEEK_END); // go to end
	len = ftell(fp); // get position at end (length)
	fseek(fp, 0, SEEK_SET); // go to beg.
	buf = (char *) malloc(len + 1); // malloc buffer
	buf[len] = 0; // append a null char
	size_t t = fread(buf, 1, len, fp); // read into buffer
	fclose(fp);
	
	return buf;
}

/* * Other
seed_rnd(int)
Seed rnd.

C++
seed_rnd(0);

Python
seed_rnd(0)
* */
void seed_rnd(int a) {
	srand(a);
}

/* *
rnd()
Returns a float between 0 and 1.

C++
float a = rnd();

Python
a = rnd()
* */
float rnd() {
	return rand() / (float) RAND_MAX;
}
