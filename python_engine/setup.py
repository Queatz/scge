#!/usr/bin/env python3

import os

from distutils.core import setup
from distutils.extension import Extension

shared = ['_scge.so']
for r, d, f in os.walk('shared'):
	for s in f:
		shared.append(os.path.join(r, s))

setup(name='scge',
      version='0.1',
      description='Simple C++ Game Engine',
      author='JacobF',
      author_email='queatz@gmail.com',
      url='http://www.queatz.com',
      packages=['scge'],
      package_data={'scge': shared},
)
