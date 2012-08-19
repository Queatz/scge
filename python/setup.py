#!/usr/bin/env python3

import os, fnmatch

from setuptools import setup

data = ['_scge.so']

for d in os.walk('scge'):
	data += [os.path.join(d[0], x)[5:] for x in fnmatch.filter(d[2], '*.py')]

setup(
	install_requires=['distribute'],
	name='scge',
	version='0.1',
	license='Public Domain / CC0',
	description='All Purpose General Engine',
	long_description = 'Visit http://queatz.com/scge/',
	platforms='All',
	keywords='game engine light simple c++ microphone audio sound graphics OpenGL OpenAL GLFW enet network networking',
	author='Jacob Ferrero',
	author_email='queatz@gmail.com',
	url='http://www.queatz.com',
	packages = ['scge'],
	package_data = {'scge': data}
)
