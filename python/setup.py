#!/usr/bin/env python3

import os, fnmatch

import sys; sys.path.append('/usr/lib/python3/dist-packages/')
from setuptools import setup

data = ['_scge.so']

for d in os.walk('scge'):
	data += [os.path.join(d[0], x)[5:] for x in fnmatch.filter(d[2], '*.py')]

setup(
	install_requires=['setuptools'],
	name='scge',
	version='0.1',
	license='Public Domain / CC0',
	description='All Purpose General Engine',
	long_description = 'Visit http://queatz.org/scge/',
	platforms='All',
	keywords='game engine light simple c++ microphone audio sound graphics OpenGL OpenAL GLFW enet network networking',
	author='Jacob Ferrero',
	author_email='jacbaferrero@gmail.com',
	url='http://www.queatz.org/scge/',
	packages = ['scge'],
	package_data = {'scge': data}
)
