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

#include <utf8.h>

#ifdef WITH_NETWORK
#include <enet/enet.h> // Networking
#endif

#ifdef WITH_GRAPHICS
#define GLFW_NO_GLU 1
#ifdef _WIN32
#  undef GL3_PROTOTYPES
#  include <GL/glew.h>
#else
#  define GLFW_INCLUDE_GL3 1
#  ifndef GL3_PROTOTYPES
#    define GL3_PROTOTYPES 1
#  endif
#endif
#include <GL/glfw3.h> // Graphics

#include "FreeImagePlus.h" // Images

#include <Shikoba.hpp> // Font rendering
#endif

#ifdef WITH_SOUND
#include <AL/alure.h> // Sound

#include <fluidsynth.h>
#include <aubio/aubio.h> // Pitch, etc
#endif

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scge {

#ifdef WITH_SOUND

#define NUM_BUFS 3

struct soundbyte {
	soundbyte();
	soundbyte(ALshort*, unsigned int);
	~soundbyte();
	
	float pitch(unsigned int = 0, unsigned int = 0, const char* = "schmitt");
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
	void position(glm::vec3);
	void repeat(bool = true);
	void seek(float = 0.0, const char* = "second");
	bool playing();
	int get(const char*);
	float offset(const char* = "second");
	void font(const char*);
	
	buffer* data;
	bool is_stream, looping;
	unsigned int pending;
	ALuint source;//x
	ALuint bufs[NUM_BUFS];//x
	alureStream* stream;//x
};

#endif

#ifdef WITH_GRAPHICS

struct pixelcache {
	pixelcache(const char*);
	pixelcache(const pixelcache&);
	pixelcache(glm::ivec2, bool = false);
	~pixelcache();
	void pixel(glm::ivec2, glm::vec4);
	glm::vec4 pixel(glm::ivec2);
	bool save(const char*, const char* = NULL);
	
	glm::ivec2 size;
	char colors;
	GLubyte *data;//x
};

struct image {
	image(const char*, bool = false);
	image(glm::ivec2, bool = false, bool = false);
	image(glm::ivec2, const char*);
	image(pixelcache*);
	~image();
	
	glm::ivec2 size;
	char colors;
	
	void set(const char*);
	
	void from_pixelcache(pixelcache*);
	void from_pixelcache(pixelcache*, glm::ivec2, glm::ivec2);
	void from_pixelcache();
	void from_pixelcache(glm::ivec2, glm::ivec2);
	void refresh_pixel_cache();
	void discard_pixel_cache();
	glm::vec4 pixel(glm::ivec2);
	bool save(const char*, const char* = NULL);
	
	bool mipmaps;
	
	GLuint id;
	pixelcache* cache;
	bool external_cache;
};

struct boxy {
	float x1, y1, x2, y2;
};

struct glyphmetrics {
	boxy vertices;
	boxy texcoords;
};

struct font {
	font(const char*, unsigned int = 0);
	~font();
	
	void size(unsigned int);
	glyphmetrics glyph(const char*);
	float height();
	float ascent();
	float descent();
	float advance(const char*, const char* = NULL);
	
	unsigned int _size;//x
	Shikoba::Face* data;//x
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
	void font(const char*, int = 0);
	
	void attribute(int, const char*);
	
	GLhandleARB id;
};

struct fbo {
	image* buffer;
	image* depth;

	fbo(glm::ivec2, bool = false, bool = false, bool = false, int = 0);
	fbo(image*);
	~fbo();
	
	GLuint id;
	bool buffer_is_mine;//x
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

struct window {
	window(const char* = "scge test", glm::ivec2 = glm::ivec2(), bool = false, bool = false, int = 0);
	~window();
	
	void close();
	void set(const char*, bool = true);

	void title(const char*);
	void iconify(bool);
	
	bool opened();
	bool active();
	bool iconified();

	void size(glm::ivec2);
	glm::ivec2 size();
	void position(glm::ivec2);
	glm::ivec2 position();

	glm::vec2 mouse();
	void mouse(const char*);
	void mouse(glm::ivec2);
	bool button(short);
	bool button(const char*);
	glm::vec2 scroll();

	bool key(const char*);
	bool key_state(const char*);

	bool screenshot(const char*, const char* = NULL);
	glm::vec4 pixel(glm::ivec2, const char* = NULL);
	
	void swap();

	void polygon_mode(const char*);
	void enable(const char*, bool = true);

	void cull(const char* a);

	void point_size(float);
	void line_width(float);

	void use(fbo*);
	void use(vao*);
	void use(program*);
	void use();

	void viewport(glm::ivec2, glm::ivec2);
	void viewport(glm::ivec4);
	void scissor(glm::ivec2, glm::ivec2);
	void scissor(glm::ivec4);

	void draw(const char*, unsigned int, unsigned int = 0);

	void color(glm::vec4);
	void color();
	void color(const char*);
	void color(bool, bool, bool, bool = true);
	void color(bool);

	void depth();
	void depth(const char*);
	void depth(bool);
	void depth_offset(float, float = 0.0);

	void stencil();
	void stencil(int);
	void stencil(const char*, int = 0);
	
#ifdef WITH_PYTHON
	void callback(const char*, PyObject*);
#endif

#ifdef WITH_PYTHON
/*$ CALLBACK $*/
	PyObject* _${n}_callback;
/*$ $*/
#endif
	
	GLFWwindow win;//x
};

bool graphics();
void graphics_off();

void use(window*);

std::string display_modes();
glm::ivec2 display_dimensions();

void poll();

#endif

#ifdef WITH_SOUND

bool audio_on();
void audio_off();
void audio_soundfont(const char*);

struct listener {
	void gain(float);
	void position(glm::vec3);
	void attenuation(const char*);
};

void midi_on();
void midi_off();

struct soundfont {
	soundfont(const char* = NULL);
	
	std::string get_presets();
	
	unsigned int id;
};

struct midi {
	midi(int = 0);
	
	void preset(soundfont*, unsigned int = 0, unsigned int = 0);
	void font(soundfont*);
	void bank(unsigned int = 0);
	void play(int = 60, int = 100);
	void stop(int = 60);
	void pan(int = 63);
	
	int _channel;//x
};

struct microphone {
	microphone();
	~microphone();
	void enable(bool = true);
	soundbyte* step();
	
	ALCdevice* _device;//x
	unsigned int _samples_available;//x
	unsigned int _samples_length;//x
	soundbyte _soundbyte;//x
};

#endif

#ifdef WITH_NETWORK

// Server

bool connection();
void connection_off();

struct peer {
	int id;
	const char* state();
	
	ENetPeer* who;//x
};

struct event {
	event();
	~event();	
	int channel();
	const char* type();
#ifdef WITH_PYTHON
	PyObject* data();
#else
	const void* data();
#endif
	unsigned int length();
	peer* who();
	
	ENetEvent evt;//x
};

struct host {
	// Create a new host on [port], with max connections of, with max numbr of channels, with limited downstream, with limited upstream
	host(int = 0, int = 32, int = 1, int = 0, int = 0);
	// Close the server
	~host();
	
	event* service(float = 0.0);
	void commune();
	
#ifdef WITH_PYTHON
	void send(peer*, PyObject*, int = 0, bool = true, bool = true);
	void broadcast(PyObject*, int = 0, bool = true, bool = true);
#else
	void send(peer*, const void* = NULL, unsigned int = 0, int = 0, bool = true, bool = true);
	void broadcast(const void* = NULL, unsigned int = 0, int = 0, bool = true, bool = true);
#endif

	peer* connect(const char* = "localhost", int = 2000, int = 1, unsigned int = 0);
	void disconnect(peer*, unsigned int = 0);
	
	ENetAddress address;//x
	ENetHost* me;//x
};

#endif

} // namespace scge
