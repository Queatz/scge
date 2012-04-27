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
	description='All Purpose General Engine',
	author='Jacob Ferrero',
	author_email='queatz@gmail.com',
	url='http://www.queatz.com',
	packages = ['scge'],
	package_data = {'scge': data}
)
