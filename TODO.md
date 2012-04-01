There should probably be a bug and feature tracker set up, but for now this will do.

The end-all goal is to create an engine that is extremely easy to make games with.
scge should be the only library one has to install to make a complete game.


shared.ui ENHANCEMENTS
===============

* (ext.) Make a complete set of ui elements
	* The core is done but there needs to be many more standard elements
	  and things like padding and margins need to be standardized
	* Many Layouts, tables, buttons, dropdown, list, and so on...

PHYSICS
=======

* (core) Figure out the best way to implement Bullet Physics.
	* As with everything in scge, it should try to closely
	  follow Bullet's own api.


STABILITY
=========

* Verify best practices everywhere
	* With no testers there will obviously be things that could be done better.
	  Let's find 'em and fix 'em.


ERROR HANDLING
==============

* Right now errors are printed and life goes on.
  Unfortunately life is then destined to end in a segfault.
  Error checking needs to be uniformly implemented everywhere
  and follow a standard.
  This may mean images and things need some "validness."


POPULARITY
==========

* Make a flagship game!
	* scge's been quite stable lately,
	  time to show off what it can do in one enthralling game!


DOCUMENTATION
==================

The current documentation is generated from the source, however it's just one big page.  Great documentation is needed asap.


3D MODELS
=========

* (ext.) Complete 3D model export / loading (NetBlend)
	Rather than having built-in support for hundreds of model formats,
	I decided to make a game-spcific model format.
	
	NetBlends are compact bytes that can store any type of data.
	What needs to be done is complete the feature set which is
	basically writing exporters for everything in Blender.
	
	They should support everything useful, including animation.
	Right now there are exporters for scenes, objects, and partial meshes.


3D GAME
=======

* (ext.) Advanced 3D specific subengine
	scge doesn't advocate one type of game over another,
	that's where extensions come in.  This 3D game kit should make it
	dead simple to make a character walk around in a 3D world.
	The goal is to handle these:

		* Scene graph
		* Camera
		* Lights
		* Materials
			* Textures
			* Normals
			* Specular
			* Tangent
		* Physics
		* Deffered lighting
		* Frustum and Occlusion Culling
		* Shadows
		* Transparency
		* Motion blur
		* Reflections
		* Particles and Hair
		* Effectors (wind)
		* Distant objects and LOD
		* Animation blending
		* Particles
		* Composite effects
		* Plenty more
	
	All these should be able to be subclassed customly.
	It will build shaders and take a trmendous load off making 3D games.


2D RPG
======

* (ext.) 2D MMORPG specific subengine
	* Everything to make a 2D MMORPG


VBO VERIFYING
============
VBOs need to have some checking for 0 size.  This has caused many hideous segfaults.  Perhaps this needs to be part of an error checking modal.

IBO CHECKUP
===========
*	Figure out if ibo's need to be different from vbo's
*	Fix how they're used and drawn

OF RANDOM LURE
==================

* There is a random segfault that has something to do with networking.
	* It seems to be when a client times out
* Exiting while audio is playing is not completely smooth.
* Fix multiple windows opening when calling window(), it should close the current one.
* allow Python bytes() to be sent over the network.
* FBOs and multisample needs to be verified
* host.disconnect() to disconnect from all peers
	* peers aren't kept track of (yet)
* pixel() needs to be able to get from the front or back buffer


EVENTUALLY
==================

* Rich text fonts
	Possibly through callbacks per character
	Only font face and size is dealt with,
	user deals with color and anything else 
* Complete implementation of GLM Python
	* I think we can use Clang to write a function wrapper

*	Midi on windows needs to be verified.
*	aubio/pitch needs to be verified on windows.
*	Joystick support

SUPPORT FOR OLDER HARDWARE
==================

scge only works with OpenGL 3+ at the moment.  Support for OpenGL 2.1 with extensions should be considered.











