#ifdef WITH_PYTHON
#include <Python.h>
#include <glm/python.hpp>
#endif

#include <cstdlib>
#include <cstring>
#include <vector>
#include <stack>
#include <list>
#include <cmath>
#ifndef M_PI
#define M_PI 3.141592653589793
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <ctype.h> // toupper()

#ifdef WITH_NETWORK
#include <enet/enet.h> // Networking
#endif

#ifdef WITH_GRAPHICS
#define GLFW_NO_GLU 1
#ifdef _WIN32
#undef GL3_PROTOTYPES
#include <GL/glew.h>
#else
#define GLFW_INCLUDE_GL3 1
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif
#endif
#include <GL/glfw3.h> // Graphics

#include "FreeImagePlus.h" // Images

#include <Shikoba.hpp> // Font rendering
#endif

#ifdef WITH_SOUND
#include <AL/alure.h> // Sound

#ifndef _WIN32
#include <fluidsynth.h>
#include <aubio/aubio.h> // Pitch, etc
#endif

#endif

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace scge {

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
	void set_pixel(int, int, glm::vec4);
	glm::vec4 pixel(int, int);
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
	glm::vec4 pixel(int, int);
	bool save(const char*, const char* = NULL);
	
	bool mipmaps;
	
	GLuint id;
	pixelcache* cache;
	bool external_cache;
};

struct fontface {
	fontface(const char*);
	~fontface();
	
	Shikoba::Face* data;//x
};

struct font {
	font(fontface*, float = 12.0);
	~font();
	
	float height();
	float ascent();
	float descent();
	float advance(const char*);
	
	//float width_of(const char*);
	//float height_of(const char*);
	
	Shikoba::Font* data;//x
};

struct text {
	text(font*, const char *, float = 1.0, float = 1.0, const char* = "left", float = 0.0, int = 4);
	~text();
	
	int lines();
	float advance();
	
	//float width_of(const char*);
	//float height_of(const char*);
	
	void draw(int, int);
	
	Shikoba::Text* data;//x
};

struct shader {
	shader(const char*, const char*, bool = false);
	~shader();
	
	GLint id;
};

struct program {
	program();
	~program();
	
	void attach(shader*);
	void link();

	void uniform(const char*, int);
	void uniform(const char*, float);
/*$ PROGRAM::UNIFORM $*/
	void uniform(const char*, glm::${p}${type}${n});
/*$ $*/
	void uniform(const char*, image*, int = 0);
	
	void attribute(int, const char*);
	
	GLhandleARB id;
};

struct fbo {
	image* buffer;
	GLuint depth_stencil_id;

	fbo(int, int, bool = false, bool = false, bool = false, int = 0);
	fbo(image*);
	~fbo();
	
	GLuint id;
	bool buffer_is_mine, depth_stencil;
};

struct vbo {
	vbo(int, const char* = "static draw");
	void data(const void*, int, int);
#ifdef WITH_PYTHON
	vbo(PyObject*, const char* = "static draw");
	void data(PyObject*, int = 0);
#endif
	void data(vbo*, int = 0, int = 0, int = -1);
	~vbo();
	
	GLuint id;
	int size;//x
};

struct ibo {
	ibo(int, const char* = "int", const char* = "static draw");
	void data(const void*, int, int);
#ifdef WITH_PYTHON
	ibo(PyObject*, const char* = "int", const char* = "static draw");
	void data(PyObject*, int = 0);
#endif
	void data(ibo*, int = 0, int = 0, int = -1);
	~ibo();
	
	void draw(const char*, unsigned int, unsigned int = 0);
	
	GLenum storage;
	GLuint id;
	int size;
};

struct vao {
	vao();
	~vao();
	
	void attribute(unsigned int, vbo*, const char*, int, int = 0, int = 0, bool = false, bool = false);
	void enable(unsigned int, bool = true);
	void divisor(unsigned int, unsigned int = 0);
	
	GLuint id;
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
glm::ivec2 display_dimensions();
glm::ivec2 window_dimensions();
void position_window(int, int);
glm::ivec2 window_position();

void vsync(bool = true);
void swap();
void poll();
bool screenshot(const char*, const char* = NULL);

glm::vec4 pixel(int, int, const char* = NULL);

void polygon_mode(const char*);
void enable(const char*, bool = true);

void cull(const char* a);

void clear_color(float, float, float, float = 1.0);
void clear();
void depth_clear();
void depth_test(const char*);
void depth_op(bool);
void point_size(float);
void line_width(float);
void polygon_depth_offset(float = 0.0, float = 0.0);

void blend_color(float, float, float, float = 1.0);

void color_mask(bool, bool, bool, bool = true);
void color_mask(bool);

void use_fbo(fbo*);
void use_fbo();

void use_vbo(vbo*);
void use_vbo();

void use_ibo(ibo*);
void use_ibo();

void use_vao(vao*);
void use_vao();

void viewport(int, int, int, int);
void scissor(int, int, int, int);

void blend_mode(const char*);

void use_program(program*);
void use_program();

void draw(const char*, unsigned int, unsigned int = 0);

void stencil_clear(int = 0);
void stencil_test(const char* = "always", int = 0);
void stencil_op(const char* = "keep");

glm::vec2 mouse_position(bool = false);

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

#ifndef _WIN32
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
#endif

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

// Python
#ifdef WITH_PYTHON
void set_callback(const char*, PyObject*);
#endif

} // namespace scge
