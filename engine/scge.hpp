#include <cstdlib>
#include <cstring>
#include <vector>
#include <stack>
#include <list>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <ctype.h> // toupper()
#include <unistd.h> // usleep()

#include "conf.h"

#ifdef WITH_GRAPHICS
#include <GL/glew.h> // OpenGL extentions
#include <GL/glfw3.h> // Graphics

#include "FreeImagePlus.h"

#include <FTGL/ftgl.h> // Font rendering
#endif

#ifdef WITH_SOUND
#include <AL/alure.h> // Sound
#include <fluidsynth.h>
#include <aubio/aubio.h> // Pitch, etc
#endif

#ifdef WITH_NETWORK
#include <enet/enet.h> // Networking
#endif

#ifdef WITH_MATH
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#endif

struct box {
	box(float = 0.0, float = 0.0);
	
	float w, h;
};

struct ibox {
	ibox(int = 0, int = 0);
	
	int w, h;
};

struct offset {
	offset(float = 0.0, float = 0.0);
	
	offset operator+(const offset&) const;
	offset operator-(const offset&) const;
	bool operator==(const offset&) const;
		
	float x, y;
};

struct rect {
	rect(float = 0.0, float = 0.0, float = 0.0, float = 0.0);
	
	float x, y, w, h;
};

struct irect {
	irect(int = 0, int = 0, int = 0, int = 0);
	
	int x, y, w, h;
};

struct crop {
	crop(bool = true, float = 0.0, float = 0.0, float = 0.0, float = 0.0);
	
	bool use;
	float x, y, w, h;
};

struct icrop {
	icrop(bool = true, int = 0, int = 0, int = 0, int = 0);
	
	bool use;
	int x, y, w, h;
};

struct rgba {
	rgba(float, float, float, float = 1.0);
	rgba(float = 0.0, float = 1.0);
	
	float r, g, b, a;
};

#ifdef WITH_SOUND

#define NUM_BUFS 3

struct soundbyte {
	soundbyte();
	soundbyte(ALshort*, unsigned int);
	~soundbyte();
	
	float calculate_frequency(unsigned int = 0, unsigned int = 0, const char* = "schmitt");
	float get(unsigned int);
	
	ALshort* data;
	unsigned int length;
	unsigned int frequency;
};

struct buffer {
	buffer(const char*);
	buffer();
	~buffer();
	
	void clear();
	void data(const void*, const char*, unsigned int, unsigned int);
	
	ALuint buf;
};

struct sound {
	sound(buffer*);
	sound(const char*, bool = false);
	sound();
	~sound();
	
	void clear();
	
	void play(unsigned int = 0, bool = false, bool = false);
	void stop();
	void pause();
	void resume();
	
	void gain(float);
	void maximum_gain(float);
	void minimum_gain(float);
	void pitch(float);
	void pan(float);
	void repeat(bool = true);
	void seek(float = NULL, const char* = "second");
	bool playing();
	int get(const char*);
	float get_offset(const char* = "second");
	void font(const char*);
	
	buffer* data;
	ALuint bufs[NUM_BUFS];
	ALuint source;
	bool is_stream, looping;
	unsigned int pending;
	alureStream* stream;//x
};

#endif

#ifdef WITH_GRAPHICS

struct pixelcache {
	pixelcache(const char*);
	pixelcache(const pixelcache&);
	pixelcache(int, int, bool = false);
	~pixelcache();
	void set_pixel(int, int, rgba);
	rgba pixel(int, int);
	bool save(const char*, const char* = NULL);
	
	int width, height;
	char colors;
	GLubyte *data;//x
};

struct image {
	image(const char*, bool = false);
	image(int, int, bool = false, bool = false);
	image(pixelcache*);
	~image();
	
	int width, height;
	char colors;
	
	void set(const char*);
	
	void from_pixelcache(pixelcache*);
	void from_pixelcache(pixelcache*, int, int, int, int);
	void from_pixelcache();
	void from_pixelcache(int, int, int, int);
	void refresh_pixel_cache();
	void discard_pixel_cache();
	rgba pixel(int, int);
	bool save(const char*, const char* = NULL);
	
	bool mipmaps;
	
	GLuint id;
	pixelcache* cache;
	bool external_cache;
};

struct font {
	font(const char*, float = 12, const char* = "texture");
	~font();
	
	void depth(float);
	void outset(float);
	void outset(float, float);
	
	void size(float);
	void push_size();
	void pop_size();
	float get_size();
	
	float line_height();
	float ascent();
	float descent();
	float advance(const char*);
	
	float width_of(const char*);
	float height_of(const char*);
	rect bbox(const char*);
	
	FTFont* data;//x
	float size_default;
	
	std::stack<float, std::list<float> > size_stack;//x
};

struct paper {
	paper();
	paper(font*, float = 100.0, const char* = "left");
	~paper();
	
	void align(const char*);
	const char* get_align();
	
	void pen(font*);
	font* get_pen();
	
	void line_spacing(float);
	float get_line_spacing();
	
	void width(float);
	float get_width();
	
	float width_of(const char*);
	float height_of(const char*);
	
	void write(const char*, float = 0.0, float = 0.0);
	
	font* data_font;
	FTSimpleLayout* data;//x
};

struct shader {
	shader(const char*, const char*);
	~shader();
	
	void compile();
	
	GLint id;
};

struct program {
	program();
	~program();
	
	void attach(shader*);
	void link();
	
	void uniform_int(const char*, int);
	void uniform_float(const char*, float);
	void uniform_float(const char*, float, float);
	void uniform_float(const char*, float, float, float);
	void uniform_float(const char*, float, float, float, float);
	
	void uniform_image(const char*, int, image*);
	
	GLhandleARB id;
};

struct fbo {
	image* buffer;
	GLuint depth_stencil_id;

	fbo(int, int, bool = false, bool = false, bool = false);
	fbo(image*);
	~fbo();
	
	GLuint id;
	bool buffer_is_mine, depth_stencil;
};

bool graphics();
void graphics_off();

typedef void (* _resize_callback_function)(int, int);
typedef int (* _close_callback_function)();
typedef void (* _focus_callback_function)(int);
typedef void (* _iconify_callback_function)(int);
typedef void (* _button_callback_function)(const char*, int);
typedef void (* _mousemove_callback_function)(int, int);
typedef void (* _scroll_callback_function)(int);
typedef void (* _key_callback_function)(const char*, int);
typedef void (* _string_callback_function)(const char*);

void key_repeat(bool = true);

bool window(const char* = "scge test", int = 320, int = 240, bool = false, bool = false, int = 0);
void close_window();

void window_title(const char*);
bool window_opened();
bool window_active();

std::string display_modes();
ibox display_dimensions();
ibox window_dimensions();
void position_window(int, int);
ibox window_position();

void vsync(bool = true);
void swap();
void poll();
bool screenshot(const char*, const char* = NULL);

void matrix(const char* = NULL);
void reset_matrix();
void viewport(float, float, float, float);
void orthographic(float, float, float, float, float = -1.0, float = 1.0);
void perspective(float = 60.0, float = NULL, float = 0.1, float = 1000.0);
void frustum(float, float, float, float, float = 0.1, float = 1000.0);

rgba pixel(int, int, const char* = NULL);
rgba pixel_coordinates(float, float, float = 0);

void polygon_mode(const char*);
void enable(const char*, bool = true);

void cull(const char* a);

void light_ambient(float = 0.0, float = 0.0, float = 0.0, float = 0.0);
void light_two_side(bool = true);
void light_set(int, const char*, float = 0.0, float = 0.0, float = 0.0, float = 1.0);
void light_enable(int = 0, bool = true);

void material(const char* = "both", const char* = NULL, float = 0.0, float = 0.0, float = 0.0, float = 1.0);

void clear_color(float, float, float, float = 1.0);
void clear();
void depth_clear();
void depth_test(const char*);
void depth_op(bool);
void point_size(float);
void line_width(float);
void line_stipple(const char* = NULL, int = 1);
void polygon_stipple(const char* = NULL, bool = false, const char* = NULL, int = 0, int = 0);
void polygon_depth_offset(float = 0.0, float = 0.0);

void blend_color(float, float, float, float = 1.0);

void color_mask(bool, bool, bool, bool = true);
void color_mask(bool);

void color(rgba);
void color(float, float, float, float = 1.0);
void color(float = 0.0, float = 1.0);
void push_color();
void pop_color();
rgba get_color();

void use_fbo(fbo*);
void use_fbo();
void push_fbo();
void pop_fbo();
fbo* get_fbo();

void use_image(image*);
void push_image();
void pop_image();
image* get_image();

void use_font(font*);
void push_font();
void pop_font();
font* get_font();

void image_crop(float, float, float, float);
void image_crop();
void push_image_crop();
void pop_image_crop();
crop get_image_crop();

void scissor(int, int, int, int);
void scissor();
void push_scissor();
void pop_scissor();
icrop get_scissor();

void blend_mode(const char*);
void blend_mode();
void push_blend_mode();
void pop_blend_mode();
const char* get_blend_mode();

void use_program(program*);
void use_program();
void push_program();
void pop_program();
program* get_program();

void push_state(const char* = NULL);
void pop_state();

void push_matrix();
void pop_matrix();

void texture_environment();
void texture_environment(const char*);

void translate(float, float);
void translate(float, float, float);
void rotate(float);
void rotate(float, float, float, float = 1.0);
void scale(float);
void scale(float, float);
void scale(float, float, float);


void begin(const char*);
void end();

void draw(float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0);
void idraw(float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0);
void write(const char*, float = 0.0, float = 0.0);

void background_image(float, float, float, float, float = 1.0, float = NULL, float = NULL, float = 0.0, float = 0.0);

void quad(float, float, float, float);
void quad();
void mquad(float, float, float, float);
void mquad();

void iquad(float, float, float, float);
void imquad(float, float, float, float);
void imquad(float, float, float, float, float);

void point(float, float);
void line(float, float, float, float);

void ipoint(float, float);
void ipoint(float, float, float);
void impoint(float, float);
void iline(float, float, float, float);

void sprite(float, float, float = 0.0, float = 1.0, float = NULL, float = NULL);
void save_matrix();

void portion(int, int, int, int);

void texture_coordinates(float, float, unsigned int = 0);
void normal(float = 0.0, float = 0.0, float = 1.0);

void stencil_clear(int = 0);
void stencil_test(const char* = "always", int = 0);
void stencil_op(const char* = "keep");

void alpha_test(const char* = "always", float = 0.0);

offset mouse_position(bool = false);

void mouse(const char* = "show");
bool button(short);
bool button(const char*);
int wheel();
void move_mouse(int, int, bool = false);

bool key(const char*);
bool key_state(const char*);

#endif

#ifdef WITH_SOUND

bool audio();
void audio_off();
void audio_gain(float = 1.0);
void audio_pan(float = 0.0);
void audio_soundfont(const char*);

void midi_on();
void midi_off();

struct soundfont {
	soundfont(const char* = NULL);
	
	std::string get_presets();
	
	unsigned int id;
};

void midi_preset(soundfont*, int = 0, unsigned int = 0, unsigned int = 0);
void midi_soundfont(soundfont*, int = 0);
void midi_bank(int = 0, unsigned int = 0);
void midi_play(int = 0, int = 60, int = 100);
void midi_stop(int = 0, int = 60);
void midi_pan(int = 0, int = 63);

void microphone_on();
void microphone_off();
void microphone_update();
soundbyte* microphone_buffer();


#endif

#ifdef WITH_NETWORK

// Server

bool connection();
void connection_off();

struct peer {
	ENetPeer* who;
	
	int id;
};

struct event {
	int channel();
	const char* type();
	const char* data();
	peer* who();
	void resolve();
	
	ENetEvent evt;//x
};

struct host {
	// Create a new host on [port], with max connections of, with max numbr of channels, with limited downstream, with limited upstream
	host(int = NULL, int = 32, int = 1, int = 0, int = 0);
	// Close the server
	~host();
	
	event service(int = 0);
	void commune();
	
	void send(peer*, const char* = "", int = 0, bool = true, bool = true);
	void broadcast(const char* = "", int = 0, bool = true, bool = true);
	
	peer* connect(const char* = "localhost", int = 2000, int = 1, unsigned int = 0);
	void disconnect(peer*, unsigned int = 0);
	
	ENetAddress address;//x
	ENetHost* me;//x
};

#endif

// Extra

struct noise {
	noise(const char* = "simple", float = 0, float = 1);
	float sample(float x, float y, float s = NULL);
	float scale;
	int seed;
	int type;//x
};

// Other

// Seed random
void seed_rnd(int);

// Random 0-1
float rnd();
